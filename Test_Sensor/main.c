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

    // Initialize I2C
    MPU6050_init();
    
    // Initialize UART module
    uart_init();
}

void main(void) {
    init();
    printf("\n");

    play_ok();

    // MPU6050 configuration
    MPU6050_write_byte(PWR_MGMT_1, 0b00101000);   // CYCLE & TEMP_DIS
    MPU6050_write_byte(PWR_MGMT_2, 0b00000000);   // enable all axis
    MPU6050_write_byte(FIFO_EN, 0b00000000);      // disable fifo buffer
    MPU6050_write_byte(CONFIG, 0b00000101);       // configure low pass filter
    MPU6050_write_byte(GYRO_CONFIG, 0b00000000);  //
    MPU6050_write_byte(ACCEL_CONFIG, 0b00000000); //

    int16_t accel_x, accel_y, accel_z;
    int16_t gyro_x, gyro_y, gyro_z;
    int16_t temp;

    // It is recommended that the main() function does not end
    // (XC8 manual, section 5.10, page 209)
    while (1) {
        accel_x = (MPU6050_read_byte(ACCEL_XOUT_H) << 8) + MPU6050_read_byte(ACCEL_XOUT_L);
        accel_y = (MPU6050_read_byte(ACCEL_YOUT_H) << 8) + MPU6050_read_byte(ACCEL_YOUT_L);
        accel_z = (MPU6050_read_byte(ACCEL_ZOUT_H) << 8) + MPU6050_read_byte(ACCEL_ZOUT_L);
        gyro_x = (MPU6050_read_byte(GYRO_XOUT_H) << 8) + MPU6050_read_byte(GYRO_XOUT_L);
        gyro_y = (MPU6050_read_byte(GYRO_YOUT_H) << 8) + MPU6050_read_byte(GYRO_YOUT_L);
        gyro_z = (MPU6050_read_byte(GYRO_ZOUT_H) << 8) + MPU6050_read_byte(GYRO_ZOUT_L);
        temp = (MPU6050_read_byte(TEMP_OUT_H) << 8) + MPU6050_read_byte(TEMP_OUT_L);

        printf(
            "accel x: %+6d, y: %+6d, z: %+6d  gyro x: %+6d, y: %+6d, z: %+6d  temp: %+6d\n",
            accel_x, accel_y, accel_z,
            gyro_x, gyro_y, gyro_z,
            temp
        );

        delay_ms(1);
    }
}
