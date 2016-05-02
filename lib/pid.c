#include "pid.h"

Pid_state pid;

void pid_init(void) {
    pid.target = 0;
    pid.sampling_freq = 1;
    pid.kp = 0;
    pid.ki = 0;
    pid.kd = 0;
    pid_reset_error_history();
    pid_reset_fitness_history();
}

void pid_reset_error_history(void) {
    for (uint16_t i = 0; i < PID_ERROR_HISTORY_SIZE; ++i) {
        pid.error_hist[i] = 0;
    }
    pid.error_hist_top = 0;
    pid.error_hist_sum = 0;
}

void pid_update_error_history(float new_error) {
    pid.error_hist_top = NEXT_MOD(
        pid.error_hist_top,
        PID_ERROR_HISTORY_SIZE
    );
    pid.error_hist_sum -= pid.error_hist[pid.error_hist_top];
    pid.error_hist[pid.error_hist_top] = new_error;
    pid.error_hist_sum += new_error;
}

void pid_set_sampling_frequency(float freq) {
    pid.sampling_freq = freq;
}

void pid_set_parameters(float kp, float ki, float kd) {
    pid.kp = kp;
    pid.ki = ki;
    pid.kd = kd;
}

void pid_set_target(float target) {
    pid.target = target;
}

float pid_run(float current_value) {
    float error = pid.target - current_value;
    pid_update_error_history(error);
    pid_update_fitness_history(error);

    uint16_t prev_error_index = PREV_MOD(
        pid.error_hist_top,
        PID_ERROR_HISTORY_SIZE
    );
    float delta_error = error - pid.error_hist[prev_error_index];
    float error_derivate = delta_error * pid.sampling_freq;

    // Proportional + Integral + Derivative
    return (pid.kp * error) \
         + (pid.ki * pid.error_hist_sum) \
         + (pid.kd * error_derivate);
}

void pid_reset_fitness_history(void) {
    for (uint16_t i = 0; i < PID_FITNESS_HISTORY_SIZE; ++i) {
        pid.fitness_hist[i] = 0;
    }
    pid.fitness_hist_top = 0;
    pid.fitness_hist_sq_sum = 0;
    pid.fitness_ready = false;
}


void pid_update_fitness_history(float new_error) {
    float new_sq_error = new_error * new_error;
    pid.fitness_hist_top = NEXT_MOD(
        pid.fitness_hist_top,
        PID_FITNESS_HISTORY_SIZE
    );
    pid.fitness_hist_sq_sum -= pid.fitness_hist[pid.fitness_hist_top];
    pid.fitness_hist[pid.fitness_hist_top] = new_sq_error;
    pid.fitness_hist_sq_sum += new_sq_error;
    if (pid.fitness_hist_top == PID_FITNESS_HISTORY_SIZE - 1) {
        pid.fitness_ready = true;
    }
}

bool pid_fitness_ready(void) {
    return pid.fitness_ready;
}

float pid_get_fitness(void) {
    // Return the negative of the mean squared error
    return - pid.fitness_hist_sq_sum / PID_FITNESS_HISTORY_SIZE;
}
