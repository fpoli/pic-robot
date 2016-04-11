#ifndef LIB_PROCESSING_H_
#define LIB_PROCESSING_H_

#include <stdint.h>

#define complementary_filter(accel_angle, gyro_angle, gyro_k) \
    ((accel_angle) * (1.0 - (gyro_k)) + (gyro_angle) * (gyro_k))

#define ERROR_QUEUE_SIZE 128
#define QUEUE_NEXT(i) ((i + ERROR_QUEUE_SIZE + 1) % ERROR_QUEUE_SIZE)
#define QUEUE_PREV(i) ((i + ERROR_QUEUE_SIZE - 1) % ERROR_QUEUE_SIZE)

void pid_reset(void);
float pid(float current, float target, float kp, float ki, float kd);

#endif  // LIB_PROCESSING_H_
