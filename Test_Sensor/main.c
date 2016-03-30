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

    // Initialize UART module
    uart_init();

    // Initialize I2C
    MPU6050_init();
}

void main(void) {
    init();

    printf("\n");
    play_ok();

    // MPU6050 configuration
    MPU6050_write_byte(PWR_MGMT_1, 0b00100000);   // CYCLE
    MPU6050_write_byte(PWR_MGMT_2, 0b11000000);   // set LP_WAKE_CTRL frequency
    MPU6050_write_byte(SMPRT_DIV, 0b00000000);    // set sample rate divider
    MPU6050_write_byte(FIFO_EN, 0b00000000);      // disable fifo buffer
    MPU6050_write_byte(CONFIG, 0b00000101);       // configure low pass filter
    MPU6050_write_byte(GYRO_CONFIG, 0b00000000);  //
    MPU6050_write_byte(ACCEL_CONFIG, 0b00000000); //

    int16_t accel_x, accel_y, accel_z;
    int16_t gyro_x, gyro_y, gyro_z;
    int16_t temp;
    float temp_celsius, theta;

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

        temp_celsius = ((float)temp) / 340 + 36.53;
        theta = ((float)accel_y) / ((float)accel_z);

        printf("accel: %+6d, %+6d, %+6d  gyro: %+6d, %+6d, %+6d  temp: %+3d cC  tetha: %+6d\n",
            accel_x, accel_y, accel_z,
            gyro_x, gyro_y, gyro_z,
            (int16_t)(temp_celsius * 100),
            (int16_t)(theta * 1000)
        );

        //delay_ms(1);
        play_sound((uint16_t)(theta * 1000), 40);
    }
}
