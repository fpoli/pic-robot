#include "mpu6050.h"

void MPU6050_init(void) {
    // Initialize I2C
    printf("[MPU6050] Initializing I2C...\n");
    I2C_init();
    printf("[MPU6050] Testing I2C and sensor's identity...\n");
    printf("[MPU6050] If it hangs here remove the power supply and retry.\n");
    MPU6050_test_identity();
    printf("[MPU6050] I2C and identity are ok\n");

    // Configure sensor
    printf("[MPU6050] Configuring sensor...\n");
    // In order to enable the FIFO buffer a "power cycle" is required
    MPU6050_configure();
    I2C_close();
    I2C_init();
    MPU6050_configure();
}

void MPU6050_configure(void) {
    // set sample rate divider
    // sample rate = 1kHz / (1 + SMPRT_DIV)
    //MPU6050_write_byte(SMPRT_DIV, 2u); // without reporting data via UART
    MPU6050_write_byte(SMPRT_DIV, 15u);  // reporting data via UART
    // configure low pass filter
    MPU6050_write_byte(CONFIG, 0b00000101);
    // configure gyro (default)
    MPU6050_write_byte(GYRO_CONFIG, 0b00000000);
    // configure accel (default)
    MPU6050_write_byte(ACCEL_CONFIG, 0b00000000);
    // enable fifo buffer for accel, gyro
    MPU6050_write_byte(FIFO_EN, 0b01111000);
    // enable fifo buffer
    MPU6050_write_byte(USER_CTRL, 0b01000000);
    // enable all axis in low power mode (default)
    MPU6050_write_byte(PWR_MGMT_2, 0b00000000);
    // exit from sleep, use gyro_z as clock ("highly recommended")
    MPU6050_write_byte(PWR_MGMT_1, 0b00000011);
}

void MPU6050_test_identity(void) {
    int8_t identiy = MPU6050_read_byte(WHO_AM_I);
    if (identiy != (MPU6050_SLAVE_ADDR >> 1)) {
        error1("[MPU6050] The sensor provided a wrong identity: 0x%X\n", identiy);
    }
}

uint16_t byteswap(Data_regs data) {
    return (((uint16_t)data.h) << 8) | data.l;
}
