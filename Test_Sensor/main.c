#include <stdint.h>
#include <xc.h>
#include <math.h>
#include "../lib/config.h"
#include "../lib/utils.h"
#include "../lib/mpu6050.h"
#include "../lib/error.h"
#include "../lib/uart.h"
#include "../lib/sound.h"

void init(void) {
    // Initialize LED/SOUND pin as output
    TRISBbits.TRISB3 = 0;
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
    double theta_accel, theta_gyro = 0;

    printf("[MAIN] Start main loop\n");
    while (1) {
        // Read buffer count
        MPU6050_read(FIFO_COUNT, (uint8_t*) &fifo_count, FIFO_COUNT_SIZE);
        fifo_count_value = byteswap(fifo_count);

        // If there is no data, try again
        if (fifo_count_value < ACCEL_GYRO_REGS_SIZE) continue;

        if (fifo_count_value == 1024) {
            error("[MAIN] MPU6050 FIFO buffer overflow detected!\n");
        }

        // Read data in buffer
        for (uint16_t x = 0; x < fifo_count_value / ACCEL_GYRO_REGS_SIZE; ++x) {
            MPU6050_read(FIFO_R_W, (uint8_t*) &data, ACCEL_GYRO_REGS_SIZE);

            // Converto data
            accel_x = byteswap(data.accel.xout);
            accel_y = byteswap(data.accel.yout);
            accel_z = byteswap(data.accel.zout);
            gyro_x = byteswap(data.gyro.xout);
            gyro_y = byteswap(data.gyro.yout);
            gyro_z = byteswap(data.gyro.zout);

            // Calculate angles
            theta_accel = atan2((double)accel_x, (double)accel_z);
            theta_gyro += (double)gyro_y;
        }

        // Report new data
        printf("fifo count: %4d  ", fifo_count_value);
        printf("accel: %+6d, %+6d, %+6d  tetha_accel: %+6d°  ",
            accel_x, accel_y, accel_z,
            (int16_t)(theta_accel / M_PI * 180)
        );
        printf("gyro: %+6d, %+6d, %+6d  tetha_gyro: %+6d°\n",
            gyro_x, gyro_y, gyro_z,
            (int16_t)(theta_gyro * 250 / 32767 *  16 / 1000 )
        );

        // Notify loop cycle
        LATBbits.LATB3 = !LATBbits.LATB3;
    }

    // It is recommended that the main() function does not end
    error("[MAIN] end of main() function reached");
}
