#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../lib/config.h"
#include "../lib/pid.h"
#include "../lib/utils.h"
#include "../lib/mpu6050.h"
#include "../lib/error.h"
#include "../lib/uart.h"
#include "../lib/sound.h"
#include "../lib/motor.h"

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
    float reaction;
    uint16_t power;

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
        }

        // PID control
        reaction = pid(best_theta, 0, 0.5, 0.5, 0.5);

        // Set motor power and direction
        power = clamp((uint16_t)fabs(round(reaction)), 0, 1023);
        motor_set_pwm(500);
        motor_set_dir(best_theta >= 0 ? DIR_REVERSED : DIR_FORWARD);

        #ifdef DEBUG
            // Report data
            printf("fifo count: %4d   ", fifo_count_value);
            printf("theta_accel: %+6d  ", (int16_t)(theta_accel * 1800 / M_PI));
            printf("theta_gyro: %+6d  ", (int16_t)(theta_gyro * 1800 / M_PI));
            printf("best_theta: %+6d  ", (int16_t)(best_theta * 1800 / M_PI));
            printf("reaction: %+6d --> %4u\n", (int16_t)(reaction), power);
        #endif

        // Blink LED to indicate activity
        LED_PIN ^= 1;
    }

    // It is recommended that the main() function does not end
    error("[MAIN] End of main() function reached\n");
}
