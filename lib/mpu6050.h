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

void MPU6050_init(void);

void MPU6050_write_byte(uint8_t dst_register, uint8_t data);

uint8_t MPU6050_read_byte(uint8_t dst_register);

void I2C_error_handler(int16_t error);

#endif	/* MPU6050_H */
