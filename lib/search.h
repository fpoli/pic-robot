#ifndef LIB_SEARCH_H_
#define LIB_SEARCH_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <float.h>
#include <math.h>
#include "utils.h"

#define SEARCH_NUM_PARAMETERS 4

void search_init(void);

void search_init_param_val(uint16_t index, float value, float min, float max);
float search_get_param_val(uint16_t index);
float search_get_test_val(uint16_t index);
void search_update_with_test_fitness(float test_fitness);

typedef struct {
    float best_fitness;
    float param_val[SEARCH_NUM_PARAMETERS];
    float test_val[SEARCH_NUM_PARAMETERS];
    float param_min[SEARCH_NUM_PARAMETERS];
    float param_max[SEARCH_NUM_PARAMETERS];
    float temperature;
} Search_state;

#endif  // LIB_SEARCH_H_
