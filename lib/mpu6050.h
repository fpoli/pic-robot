#ifndef LIB_MPU6050_H_
#define LIB_MPU6050_H_

#include <stdint.h>
#include <math.h>
#include "i2c.h"
#include "error.h"

#define SMPRT_DIV    0x19
#define CONFIG       0x1A
#define GYRO_CONFIG  0x1B
#define ACCEL_CONFIG 0x1C
#define FIFO_EN      0x23
#define INT_STATUS   0x3A
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define TEMP_OUT_H   0x41
#define TEMP_OUT_L   0x42
#define GYRO_XOUT_H  0x43
#define GYRO_XOUT_L  0x44
#define GYRO_YOUT_H  0x45
#define GYRO_YOUT_L  0x46
#define GYRO_ZOUT_H  0x47
#define GYRO_ZOUT_L  0x48
#define SIGNAL_PATH_RESET 0x68
#define USER_CTRL    0x6A
#define PWR_MGMT_1   0x6B
#define PWR_MGMT_2   0x6C
#define FIFO_COUNT_H 0x72
#define FIFO_COUNT_L 0x73
#define FIFO_R_W     0x74
#define WHO_AM_I     0x75

#define MPU6050_SLAVE_ADDR 0b11010000

#define INT_STATUS_FIFO_OFLOW_INT 0b00010000
#define INT_STATUS_DATA_RDY_INT   0b00000001

typedef struct {
    uint8_t h;
    uint8_t l;
} Data_regs;

#define FIFO_COUNT 0x72
#define FIFO_COUNT_SIZE 2

#define TEMP_REGS 0x41
#define TEMP_REGS_SIZE 2

typedef struct {
    Data_regs xout;
    Data_regs yout;
    Data_regs zout;
} Accel_regs;
#define ACCEL_REGS 0x3B
#define ACCEL_REGS_SIZE 6

typedef struct {
    Data_regs xout;
    Data_regs yout;
    Data_regs zout;
} Gyro_regs;
#define GYRO_REGS 0x43
#define GYRO_REGS_SIZE 6

typedef struct {
    Accel_regs accel;
    Data_regs temp;
    Gyro_regs gyro;
} Sensor_regs;
#define SENSOR_REGS 0x3B
#define SENSOR_REGS_SIZE 14

typedef struct {
    Accel_regs accel;
    Gyro_regs gyro;
} Accel_gyro_regs;
#define ACCEL_GYRO_REGS_SIZE 12

#define SMPRT_DIV_VALUE 15  // 15 for debugging, 2 for production
#define SENSOR_HZ (1000.0 / (1.0 + SMPRT_DIV_VALUE))
#define SENSOR_DT ((1.0 + SMPRT_DIV_VALUE) / 1000.0)
#define ACCEL_SCALE_G (1.0 / 32768.0)
#define GYRO_SCALE_RAD ((250.0 / 32768.0) * (M_PI / 180.0))

void MPU6050_init(void);
void MPU6050_configure(void);

void MPU6050_test_identity(void);

uint16_t byteswap(Data_regs data);

#define MPU6050_write(a, b, c)   I2C_write(MPU6050_SLAVE_ADDR, a, b, c)
#define MPU6050_write_byte(a, b) I2C_write_byte(MPU6050_SLAVE_ADDR, a, b)
#define MPU6050_read(a, b, c)    I2C_read(MPU6050_SLAVE_ADDR, a, b, c)
#define MPU6050_read_byte(a)  I2C_read_byte(MPU6050_SLAVE_ADDR, a)

#endif  // LIB_MPU6050_H_
