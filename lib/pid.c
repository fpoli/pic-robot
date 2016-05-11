#include "pid.h"

void pid_update_error_history(float new_error, float dt);
void pid_reset_fitness_history(void);
void pid_update_fitness(float new_error, float dt);
Pid_state *pid;

void pid_init(void) {
    static Pid_state pid_state;
    pid = &pid_state;

    pid_reset();
}

void pid_reset(void) {
    pid->target = 0;
    pid->offset = 0;
    pid->kp = 0;
    pid->ki = 0;
    pid->kd = 0;
    pid_reset_error_history();
    pid_reset_fitness_history();
}

void pid_reset_error_history(void) {
    pid->last_error = 0;
    pid->error_integral = 0;
}

void pid_update_error_history(float new_error, float dt) {
    pid->error_integral = constrainf(
        pid->error_integral + new_error * dt,
        -PID_INTEGRAL_LIMIT,
        PID_INTEGRAL_LIMIT
    );
}

void pid_set_parameters(float kp, float ki, float kd) {
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
}

void pid_set_target(float target) {
    pid->target = target;
}

void pid_set_offset(float offset) {
    pid->offset = offset;
}

float pid_update(float current_value, float dt) {
    float error = (pid->target + pid->offset) - current_value;
    float delta_error = error - pid->last_error;
    float error_derivate = delta_error * dt;

    pid->last_error = error;
    pid_update_error_history(error, dt);
    pid_update_fitness(error, dt);

    // Proportional + Integral + Derivative
    return (pid->kp * error) \
         + (pid->ki * pid->error_integral) \
         + (pid->kd * error_derivate);
}

void pid_reset_fitness_history(void) {
    pid->fitness_time_ms = 0;
    pid->fitness_error_sq_sum = 0;
    pid->fitness = 0;
    pid->fitness_ready = false;
}

void pid_update_fitness(float new_error, float dt) {
    float new_sq_error = new_error * new_error;
    pid->fitness_time_ms += dt * 1000;
    pid->fitness_error_sq_sum += new_sq_error * dt;
    if (pid->fitness_time_ms >= PID_FITNESS_MEASUREMENT_MS) {
        // Fitness is the negative of the mean squared error
        pid->fitness = - pid->fitness_error_sq_sum;
        pid->fitness_time_ms = 0;
        pid->fitness_error_sq_sum = 0;
        pid->fitness_ready = true;
    }
}

bool pid_fitness_ready(void) {
    return pid->fitness_ready;
}

float pid_get_fitness(void) {
    // Fitness is the negative of the mean squared error
    pid->fitness_ready = false;
    return pid->fitness;
}
