#ifndef I2C_H
#define	I2C_H

#include <i2c.h>
#include <stdint.h>
#include "error.h"

#define SLAVE_READ(addr)  ((addr) & 0b11111110)
#define SLAVE_WRITE(addr) ((addr) | 0b00000001)

void I2C_init(void);
void I2C_close(void);

void I2C_write(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data, uint16_t data_len);
void I2C_write_byte(uint8_t slave_addr, uint8_t reg_addr, uint8_t data);

void I2C_read(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data, uint16_t data_len);
uint8_t I2C_read_byte(uint8_t slave_addr, uint8_t reg_addr);

void I2C_error_handler(int16_t error);

#endif	/* I2C_H */
