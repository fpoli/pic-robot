#include "i2c.h"

void I2C_init(void) {
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

void I2C_close(void) {
    CloseI2C1();
}

void I2C_write(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data, uint16_t data_len) {
    // Protocol described in data sheet, section 15.6.6.4, page 246
    StartI2C1();
    IdleI2C1();

    if (WriteI2C1(SLAVE_WRITE(slave_addr)) < 0)
        I2C_error_handler(1);
    IdleI2C1();

    if (WriteI2C1(reg_addr) < 0)
        I2C_error_handler(2);
    IdleI2C1();

    for (uint16_t i = 0; i < data_len; ++i) {
        if (WriteI2C1(data[i]) < 0)
            I2C_error_handler(3);
        IdleI2C1();
    }

    StopI2C1();
}

void I2C_write_byte(uint8_t slave_addr, uint8_t reg_addr, uint8_t data) {
    I2C_write(slave_addr, reg_addr, &data, 1);
}

void I2C_read(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data, uint16_t data_len) {
    StartI2C1();
    IdleI2C1();

    if (WriteI2C1(SLAVE_WRITE(slave_addr)) < 0)
        I2C_error_handler(3);
    IdleI2C1();

    if (WriteI2C1(reg_addr) < 0)
        I2C_error_handler(4);
    IdleI2C1();

    RestartI2C1();
    IdleI2C1();

    if (WriteI2C1(SLAVE_READ(slave_addr)) < 0)
        I2C_error_handler(5);
    IdleI2C1();

    for (uint16_t i = 0; i < data_len; ++i) {
        if (i > 0) AckI2C1();
        data[i] = ReadI2C1();
    }
    NotAckI2C1();

    StopI2C1();
}

uint8_t I2C_read_byte(uint8_t slave_addr, uint8_t reg_addr) {
    uint8_t data;
    I2C_read(slave_addr, reg_addr, &data, 1);
    return data;
}

void I2C_error_handler(int16_t error) {
    error1("[I2C] Error %d\n", error);
}
