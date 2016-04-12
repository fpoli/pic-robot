#include "pid.h"

// Circular queue
float error_queue[PID_ERROR_HISTORY_SIZE] = { 0 };
uint16_t error_queue_top = 0;
float error_queue_sum = 0;

#define Q_NEXT(i) ((i + PID_ERROR_HISTORY_SIZE + 1) % PID_ERROR_HISTORY_SIZE)
#define Q_PREV(i) ((i + PID_ERROR_HISTORY_SIZE - 1) % PID_ERROR_HISTORY_SIZE)

void pid_reset(void) {
    for (uint16_t i = 0; i < PID_ERROR_HISTORY_SIZE; ++i) {
        error_queue[i] = 0;
    }
    error_queue_top = 0;
    error_queue_sum = 0;
}

float pid(float current, float target, float kp, float ki, float kd) {
    float error, tp, ti, derivate, td;

    // Proportional
    error = target - current;

    tp = kp * error;

    // Integral
    error_queue_top = Q_NEXT(error_queue_top);
    error_queue_sum -= error_queue[error_queue_top];
    error_queue[error_queue_top] = error;
    error_queue_sum += error;

    ti = ki * error_queue_sum;

    // Derivative
    derivate = error_queue[Q_PREV(error_queue_top)] - error;

    td = kd * derivate;

    return tp + ti + td;
}
