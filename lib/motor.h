#ifndef MOTOR_H
#define MOTOR_H

#include <xc.h>
#include <stdint.h>

void motor_init();

void motor_set_pwm(uint16_t);
void motor_set_dir(uint8_t);

#define DIR_FORWARD         0b01
#define DIR_REVERSED        0b10
#define DIR_OFF             0b00
#define DIR_WASTE_BATTERY   0b11

#endif

