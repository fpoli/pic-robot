#ifndef MPU6050_H
#define	MPU6050_H

#include <i2c.h>
#include <stdint.h>
#include "config.h"
#include "utils.h"
#include "sound.h"
#include "uart.h"

#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define TEMP_OUT_H 0x41
#define TEMP_OUT_L 0x42
#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48

#define MPU6050_SLAVE_WRITE 0b11010000
#define MPU6050_SLAVE_READ  0b11010001

#define SMPRT_DIV    0x19
#define CONFIG       0x1A
#define GYRO_CONFIG  0x1B
#define ACCEL_CONFIG 0x1C
#define FIFO_EN      0x23
#define PWR_MGMT_1   0x6B
#define PWR_MGMT_2   0x6C
#define WHO_AM_I     0x75

typedef struct {
    uint8_t xout_h;
    uint8_t xout_l;
    uint8_t yout_h;
    uint8_t yout_l;
    uint8_t zout_h;
    uint8_t zout_l;
} Accel_regs;
#define ACCEL_REGS 0x3B
#define ACCEL_REGS_SIZE 6

typedef struct {
    uint8_t out_h;
    uint8_t out_l;
} Temp_regs;
#define TEMP_REGS 0x41
#define TEMP_REGS_SIZE 2

typedef struct {
    uint8_t xout_h;
    uint8_t xout_l;
    uint8_t yout_h;
    uint8_t yout_l;
    uint8_t zout_h;
    uint8_t zout_l;
} Gyro_regs;
#define GYRO_REGS 0x43
#define GYRO_REGS_SIZE 6

typedef struct {
    Accel_regs accel;
    Temp_regs temp;
    Gyro_regs gyro;
} Data_regs;
#define DATA_REGS 0x3B
#define DATA_REGS_SIZE 14

void MPU6050_init_I2C(void);

void MPU6050_configure(void);

void MPU6050_write(uint8_t dst_register, uint8_t *data, uint16_t data_len);
void MPU6050_write_byte(uint8_t dst_register, uint8_t data);

void MPU6050_read(uint8_t dst_register, uint8_t *data, uint16_t data_len);
uint8_t MPU6050_read_byte(uint8_t dst_register);

void I2C_error_handler(int16_t error);

void MPU6050_test_I2C(void);

#endif	/* MPU6050_H */
