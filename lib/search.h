#ifndef LIB_SEARCH_H_
#define LIB_SEARCH_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <float.h>
#include <math.h>
#include "utils.h"

#define SEARCH_MAX_PARAMETERS 10

void search_init(uint16_t num_params);

void search_init_param_val(uint16_t index, float value, float min, float max);
float search_get_param_val(uint16_t index);
float search_get_test_val(uint16_t index);
void search_update_with_test_fitness(float test_fitness);

typedef struct {
    uint16_t num_params;
    float best_fitness;
    float param_val[SEARCH_MAX_PARAMETERS];
    float test_val[SEARCH_MAX_PARAMETERS];
    float param_min[SEARCH_MAX_PARAMETERS];
    float param_max[SEARCH_MAX_PARAMETERS];
    float temperature;
} Search_state;

#endif  // LIB_SEARCH_H_
