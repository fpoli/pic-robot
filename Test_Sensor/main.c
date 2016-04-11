#include <xc.h>
#include <stdint.h>
#include <math.h>
#include "../lib/config.h"
#include "../lib/processing.h"
#include "../lib/utils.h"
#include "../lib/mpu6050.h"
#include "../lib/error.h"
#include "../lib/uart.h"
#include "../lib/sound.h"

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
    double theta_accel, theta_gyro = 0, best_theta;

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

            // Calculate angles (rad)
            theta_accel = atan2((double)accel_x, (double)accel_z);
            gyro_angvel = (double)gyro_y * (250 / 180 * M_PI) / 32767;
            theta_gyro = best_theta + gyro_angvel * (16 / 1000);
            best_theta = complementary_filter(theta_accel, theta_gyro, 0.98);
        }

        // Report new data
        printf("fifo count: %4d  ", fifo_count_value);
        printf("accel: %+6d, %+6d, %+6d  tetha_accel: %+6d°  ",
            accel_x, accel_y, accel_z,
            (int16_t)(theta_accel / M_PI * 180)
        );
        printf("gyro: %+6d, %+6d, %+6d  theta_gyro: %+6d°  ",
            gyro_x, gyro_y, gyro_z,
            (int16_t)(theta_gyro / M_PI * 180)
        );
        printf("best_theta: %+6d°\n",
            (int16_t)(best_theta / M_PI * 180)
        );

        // Blink LED to indicate activity
        LED_PIN ^= 1;
    }

    // It is recommended that the main() function does not end
    error("[MAIN] End of main() function reached\n");
}
