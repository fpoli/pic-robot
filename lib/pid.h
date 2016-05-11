#ifndef LIB_PID_H_
#define LIB_PID_H_

#include <stdint.h>
#include <stdbool.h>
#include "config.h"
#include "utils.h"

#define PID_INTEGRAL_LIMIT 500

#define PID_FITNESS_MEASUREMENT_TIME 10


void pid_init(void);
void pid_reset(void);
void pid_reset_error_history(void);
void pid_set_parameters(float kp, float ki, float kd);
void pid_set_target(float target);
void pid_set_offset(float offset);
float pid_update(float current_value, float dt);

void pid_reset_fitness(void);
bool pid_fitness_ready(void);
float pid_get_fitness(void);

typedef struct {
    // PID parameters
    float target;
    float offset;
    float kp;
    float ki;
    float kd;
    float last_error;
    float error_integral;
    // Fitness
    uint16_t fitness_duration;
    float fitness_error_sq_sum;
    float fitness;
    bool fitness_ready;
} Pid_state;

#endif  // LIB_PID_H_
