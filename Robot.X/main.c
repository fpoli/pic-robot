#include <xc.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../lib/config.h"
#include "../lib/pid.h"
#include "../lib/search.h"
#include "../lib/mpu6050.h"
#include "../lib/error.h"
#include "../lib/uart.h"
#include "../lib/sound.h"
#include "../lib/motor.h"

#define PARAM_OFFSET 0
#define PARAM_KP 1
#define PARAM_KI 2
#define PARAM_KD 3

void init(void) {
    // Initialize LED pin as output
    LED_TRIS = 0;
    LED_PIN = 1;

    // Initialize SPEAKER pin as output
    SPEAKER_TRIS = 0;
    play_ok();

    // Initialize UART module
    uart_init();
    printf("\n");
    printf("[INIT] UART module initialized\n");
    play_ok();

    // Initialize rand()
    srand(42);
    printf("[INIT] rand() initialized\n");
    play_ok();

    // Initial PID values
    float init_offset = 4.5 * DEG_TO_RAD;
    float init_kp = 3000;
    float init_ki = 0;
    float init_kd = 0;

    // Initialize PID
    pid_init();
    pid_set_target(0);
    pid_set_offset(init_offset);
    pid_set_parameters(init_kp, init_ki, init_kd);
    printf("[INIT] PID initialized\n");
    play_ok();

    // Initialize search module
    search_init(2);
    search_init_param_val(PARAM_OFFSET, init_offset, 0, 10 * DEG_TO_RAD);
    search_init_param_val(PARAM_KP, init_kp, 2500, 3500);
    search_init_param_val(PARAM_KI, init_ki, 0, 10);
    search_init_param_val(PARAM_KD, init_kd, 0, 10);

    // Initialize motor driver
    motor_init();
    printf("[INIT] Motor driver initialized\n");
    play_ok();

    // Initialize I2C and MPU6050
    MPU6050_init();
    printf("[INIT] MPU6050 initialized\n");
}

void main(void) {
    init();

    Accel_gyro_regs data;
    Data_regs fifo_count;
    uint16_t fifo_count_value;
    int16_t accel_x, accel_y, accel_z;
    int16_t gyro_x, gyro_y, gyro_z;
    float theta_accel, theta_gyro = 0, rotation_gyro, best_theta = 0;
    bool best_theta_available = false;
    float pid_reaction;
    uint16_t abs_power;
    float fitness;

    printf("[MAIN] Start main loop\n");
    while (1) {
        // Read the size of data in buffer
        MPU6050_read(FIFO_COUNT, (uint8_t*) &fifo_count, FIFO_COUNT_SIZE);
        fifo_count_value = byteswap(fifo_count);

        // If there is not enough data, try again
        if (fifo_count_value < ACCEL_GYRO_REGS_SIZE) continue;

        if (fifo_count_value == 1024) {
            error("[MAIN] MPU6050 FIFO buffer overflow detected!\n");
        }

        // Read and process data from buffer
        for (uint16_t x = 0; x < fifo_count_value / ACCEL_GYRO_REGS_SIZE; ++x) {
            MPU6050_read(FIFO_R_W, (uint8_t*) &data, ACCEL_GYRO_REGS_SIZE);

            // Convert to little-endian
            accel_x = byteswap(data.accel.xout);
            accel_y = byteswap(data.accel.yout);
            accel_z = byteswap(data.accel.zout);
            gyro_x = byteswap(data.gyro.xout);
            gyro_y = byteswap(data.gyro.yout);
            gyro_z = byteswap(data.gyro.zout);

            // Calculate angle (rad) by combining sensory data
            theta_accel = atan2((float)accel_y, (float)accel_z);
            rotation_gyro = (float)gyro_x * GYRO_SCALE_RAD * SENSOR_DT;
            theta_gyro = theta_gyro + rotation_gyro;

            if (best_theta_available) {
                best_theta =
                    0.9 * (best_theta + rotation_gyro) +
                    0.1 * theta_accel;
            } else {
                best_theta = theta_accel;
                best_theta_available = true;
            }

            // PID control
            pid_reaction = pid_update(best_theta, SENSOR_DT);
        }

        // Set motor power and direction
        abs_power = constrain((uint16_t)fabs(round(pid_reaction)), 0, 1023);
        motor_set_pwm(abs_power);
        motor_set_dir(pid_reaction >= 0 ? DIR_FORWARD : DIR_REVERSED);

        #ifdef DEBUG
            #ifdef AUTOTUNE
                printf("fitness: %+6d  ", (int16_t)(1000 * fitness));
                printf("(offset: %+6d  ", (int16_t)(10 * RAD_TO_DEG * search_get_test_val(PARAM_OFFSET)));
                printf("Kp: %+6d  ", (int16_t)(10 * search_get_test_val(PARAM_KP)));
                printf("Ki: %+6d  ", (int16_t)(10 * search_get_test_val(PARAM_KI)));
                printf("Kd: %+6d)  ", (int16_t)(10 * search_get_test_val(PARAM_KD)));
                printf("best_theta: %+6d  ", (int16_t)(10 * RAD_TO_DEG * best_theta));
                printf("pid: %+6d --> %4u\n", (int16_t)(pid_reaction), abs_power);
            #else
                printf("fifo count: %4d   ", fifo_count_value);
                printf("theta_accel: %+6d  ", (int16_t)(theta_accel * RAD_TO_DEG * 10));
                printf("theta_gyro: %+6d  ", (int16_t)(theta_gyro * RAD_TO_DEG * 10));
                printf("best_theta: %+6d  ", (int16_t)(best_theta * RAD_TO_DEG * 10));
                printf("pid_reaction: %+6d --> %4u\n", (int16_t)(pid_reaction), abs_power);
            #endif
        #endif

        #ifdef AUTOTUNE
            if (pid_fitness_ready()) {
                fitness = pid_get_fitness();
                search_update_with_test_fitness(fitness);
                pid_set_offset(search_get_test_val(PARAM_OFFSET));
                pid_set_parameters(
                    search_get_test_val(PARAM_KP),
                    search_get_test_val(PARAM_KI),
                    search_get_test_val(PARAM_KD)
                );
            }
        #endif

        // Blink LED to indicate activity
        LED_PIN ^= 1;
    }

    // It is recommended that the main() function does not end
    error("[MAIN] End of main() function reached\n");
}
