#include "processing.h"

// circular queue
float error_queue[ERROR_QUEUE_SIZE] = { 0 };
uint16_t error_queue_top = 0;
float error_queue_sum = 0;

void pid_reset(void) {
	for (uint16_t i = 0; i < ERROR_QUEUE_SIZE; ++i) {
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
	error_queue_top = QUEUE_NEXT(error_queue_top);
	error_queue_sum -= error_queue[error_queue_top];
	error_queue[error_queue_top] = error;
	error_queue_sum += error;

	ti = ki * error_queue_sum;

	// Derivative
	derivate = error_queue[QUEUE_PREV(error_queue_top)] - error;

	td = kd * derivate;

	return tp + ti + td;
}
