#include <stdint.h>
#include <xc.h>
#include "../lib/config.h"
#include "../lib/utils.h"
#include "../lib/mpu6050.h"
#include "../lib/uart.h"
#include "../lib/sound.h"

void init(void) {
    // Initialize LED/SOUND pin as output
    TRISBbits.TRISB3 = 0;
    play_ok();

    // Initialize UART module
    uart_init();
    printf("\n");
    printf("[UART] module initialized\n");

    // Initialize I2C
    printf("[MPU6050] Initializing I2C...\n");
    MPU6050_init_I2C();
    printf("[MPU6050] Testing I2C...\n");
    printf("[MPU6050] If it hangs here remove the power supply and retry.\n");
    MPU6050_test_I2C();
    printf("[MPU6050] I2C is ok\n");

    // Configure sensor
    printf("[MPU6050] Configuring sensor...\n");
    MPU6050_configure();
    printf("[MPU6050] Done.\n");
}

void main(void) {
    init();

    int16_t old_accel_x = 0;
    int16_t accel_x, accel_y, accel_z;
    int16_t gyro_x, gyro_y, gyro_z;
    int16_t temp;
    float temp_celsius, theta;

    printf("[MAIN] Start main loop\n");
    while (1) {
        accel_x = (MPU6050_read_byte(ACCEL_XOUT_H) << 8) + MPU6050_read_byte(ACCEL_XOUT_L);
        accel_y = (MPU6050_read_byte(ACCEL_YOUT_H) << 8) + MPU6050_read_byte(ACCEL_YOUT_L);
        accel_z = (MPU6050_read_byte(ACCEL_ZOUT_H) << 8) + MPU6050_read_byte(ACCEL_ZOUT_L);
        gyro_x = (MPU6050_read_byte(GYRO_XOUT_H) << 8) + MPU6050_read_byte(GYRO_XOUT_L);
        gyro_y = (MPU6050_read_byte(GYRO_YOUT_H) << 8) + MPU6050_read_byte(GYRO_YOUT_L);
        gyro_z = (MPU6050_read_byte(GYRO_ZOUT_H) << 8) + MPU6050_read_byte(GYRO_ZOUT_L);
        temp = (MPU6050_read_byte(TEMP_OUT_H) << 8) + MPU6050_read_byte(TEMP_OUT_L);
 
        temp_celsius = ((float)temp) / 340 + 36.53;
        theta = ((float)accel_y) / ((float)accel_z);

        /*printf("accel: %+6d, %+6d, %+6d  gyro: %+6d, %+6d, %+6d  temp: %+3d cC  tetha: %+6d\n",
            accel_x, accel_y, accel_z,
            gyro_x, gyro_y, gyro_z,
            (int16_t)(temp_celsius * 100),
            (int16_t)(theta * 1000)
        );*/

        // Notify that the data changed
        if (accel_x != old_accel_x) {
            LATBbits.LATB3 = !LATBbits.LATB3;
            old_accel_x = accel_x;
        }
    }

    // It is recommended that the main() function does not end
    while (1) play_error();
}
