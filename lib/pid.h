#ifndef LIB_PID_H_
#define LIB_PID_H_

#include <stdint.h>
#include <stdbool.h>

#define PID_ERROR_HISTORY_SIZE 128
#define PID_FITNESS_HISTORY_SIZE 256

void pid_init(void);
void pid_reset_error_history(void);
void pid_update_error_history(float new_error);
void pid_set_sampling_frequency(float freq);
void pid_set_parameters(float kp, float ki, float kd);
void pid_set_target(float target);
void pid_set_offset(float offset);
float pid_update(float current_value);

void pid_reset_fitness_history(void);
void pid_update_fitness_history(float new_error);
bool pid_fitness_ready(void);
float pid_get_fitness(void);

#define NEXT_MOD(i, mod) (((i) + (mod) + 1) % (mod))
#define PREV_MOD(i, mod) (((i) + (mod) - 1) % (mod))

typedef struct {
    // PID parameters
    float sampling_freq;
    float target;
    float offset;
    float kp;
    float ki;
    float kd;
    // Integral
    float error_hist[PID_ERROR_HISTORY_SIZE];
    uint16_t error_hist_top;
    float error_hist_sum;
    // Fitness
    uint16_t fitness_hist_size;
    float fitness_hist_sq_sum;
    float fitness;
    bool fitness_ready;
} Pid_state;

#endif  // LIB_PID_H_
