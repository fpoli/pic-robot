#include "mpu6050.h"

void MPU6050_init_I2C(void) {
    // set SCL1 and SDA1 pins as input
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;

    // set I2C master mode with slew rate (suggested at 400 kHz)
    SSP1CON1 = 0b00101000; 
    SSP1STAT = 0b00000000;

    // set I2C clock at 400kHz (Fosc = 32MHz)
    // I2C clock = Fosc / (4 * (SSP1ADD + 1)
    SSP1ADD = 19;

    // reset all I2C conditions
    SSP2CON2 = 0;

    // connect
    OpenI2C1(MASTER, SLEW_ON);
}

void MPU6050_configure(void) {
    MPU6050_write_byte(SMPRT_DIV, 0b00000000);    // set sample rate divider
    MPU6050_write_byte(FIFO_EN, 0b00000000);      // disable fifo buffer
    MPU6050_write_byte(CONFIG, 0b00000101);       // configure low pass filter
    MPU6050_write_byte(GYRO_CONFIG, 0b00000000);  //
    MPU6050_write_byte(ACCEL_CONFIG, 0b00000000); //
    MPU6050_write_byte(PWR_MGMT_2, 0b00000000);   // Enable all axis
    MPU6050_write_byte(PWR_MGMT_1, 0b00000000);   // Disable SLEEP and CYCLE
}

void MPU6050_write_byte(uint8_t dst_register, uint8_t data) {
    // Protocol described in data sheet, section 15.6.6.4, page 246
    IdleI2C1();
    StartI2C1();

    IdleI2C1();
    if (WriteI2C1(MPU6050_SLAVE_WRITE) < 0)
        I2C_error_handler(1);

    IdleI2C1();
    if (WriteI2C1(dst_register) < 0)
        I2C_error_handler(2);

    IdleI2C1();
    if (WriteI2C1(data) < 0)
        I2C_error_handler(3);

    IdleI2C1();
    StopI2C1();
}

uint8_t MPU6050_read_byte(uint8_t dst_register) {
    IdleI2C1();
    StartI2C1();

    IdleI2C1();
    if (WriteI2C1(MPU6050_SLAVE_WRITE) < 0)
        I2C_error_handler(3);

    IdleI2C1();
    if (WriteI2C1(dst_register) < 0)
        I2C_error_handler(4);

    IdleI2C1();
    RestartI2C1();

    IdleI2C1();
    if (WriteI2C1(MPU6050_SLAVE_READ) < 0)
        I2C_error_handler(5);

    IdleI2C1();
    uint8_t data = ReadI2C1();

    IdleI2C1();
    NotAckI2C1();
    StopI2C1();

    return data;
}

void I2C_error_handler(int16_t error) {
    play_error();
    printf("[MPU6050] Error %d in I2C protocol\n");
    while (1);
}

void MPU6050_test_I2C(void) {
    int8_t mpu_address = MPU6050_read_byte(WHO_AM_I);
    if (mpu_address != 0x68) {
        play_error();
        printf("[MPU6050] The sensor provided a wrong address: 0x%X\n", mpu_address);
        while (1);
    }
}
