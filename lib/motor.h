#ifndef LIB_MOTOR_H_
#define LIB_MOTOR_H_

#include <xc.h>
#include <stdint.h>

#define DIR_FORWARD       0b01
#define DIR_REVERSED      0b10
#define DIR_OFF           0b00
#define DIR_WASTE_BATTERY 0b11

void motor_init(void);

void motor_set_pwm(uint16_t);

void motor_set_dir(uint8_t);

#endif  // LIB_MOTOR_H_
