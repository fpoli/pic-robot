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

    play_ok();

    // MPU6050 configuration
    MPU6050_write_byte(PWR_MGMT_1, 0b00100000);
    MPU6050_write_byte(CONFIG, 0b00000101); 
    MPU6050_write_byte(GYRO_CONFIG, 0b00000000);
    MPU6050_write_byte(ACCEL_CONFIG, 0b00000000);

    uint16_t accel_x, accel_y, accel_z;
    uint16_t gyro_x, gyro_y, gyro_z;
    uint16_t temp;

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

        printf("accel x: %d, y: %d, z: %d\n", accel_x, accel_y, accel_z);
        printf("gyro x: %d, y: %d, z: %d\n", gyro_x, gyro_y, gyro_z);
        printf("temp: %d\n", temp);

        delay_ms(100);
    }
}
