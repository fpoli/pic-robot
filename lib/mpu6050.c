#include "mpu6050.h"

void MPU6050_init(void) {
    // Initialize I2C
    TRISCbits.TRISC3 = 1; // Input SCL1
    TRISCbits.TRISC4 = 1; // Input SDA1
    SSP1CON1 = 0b00101000;
    SSP1STAT = 0b10000000;
    SSP1ADD = 19;
    SSP2CON2 = 0; // reset all conditions

    OpenI2C1(MASTER, SLEW_OFF);
}

void MPU6050_write_byte(uint8_t dst_register, uint8_t data) {
    // If this line is removed, it doesn't work... why? WHY?
    printf("debug: MPU6050_write_byte(%x, %x)\n", dst_register, data);

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
    printf("I2C error %d\n", error);
}
