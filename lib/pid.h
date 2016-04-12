#ifndef LIB_PID_H_
#define LIB_PID_H_

#include <stdint.h>

#define ERROR_QUEUE_SIZE 128

void pid_reset(void);

float pid(float current, float target, float kp, float ki, float kd);

#endif  // LIB_PID_H_
