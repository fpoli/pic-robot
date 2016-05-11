#include "search.h"

Search_state *search;

void search_init(uint16_t num_params) {
    static Search_state search_state;
    search = &search_state;

    num_params = constrain(num_params, 0, SEARCH_MAX_PARAMETERS);
    for (uint16_t i = 0; i < num_params; ++i) {
        search->param_val[i] = 0;
        search->test_val[i] = 0;
        search->param_min[i] = -1000;
        search->param_max[i] = +1000;
    }
    search->num_params = num_params;
    search->best_fitness = -FLT_MAX;
    search->temperature = 1;
}

void search_init_param_val(uint16_t index, float value, float min, float max) {
    if (index < search->num_params) {
        search->param_val[index] = value;
        search->test_val[index] = value;
        search->param_min[index] = min;
        search->param_max[index] = max;
    }
}

float search_get_param_val(uint16_t index) {
    if (index < search->num_params) {
        return search->param_val[index];
    } else {
        return 0;
    }
}

float search_get_test_val(uint16_t index) {
    if (index < search->num_params) {
        return search->test_val[index];
    } else {
        return 0;
    }
}

void search_update_with_test_fitness(float test_fitness) {
    // Gradient descent with decreasing temperature
    if (test_fitness >= search->best_fitness) {
        search->best_fitness = test_fitness;
        for (uint16_t i = 0; i < search->num_params; ++i) {
            search->param_val[i] = search->test_val[i];
        }
    }

    // Prepare test values
    for (uint16_t i = 0; i < search->num_params; ++i) {
        search->test_val[i] = search->param_val[i];
    }

    // Mutate one random parameter
    uint16_t index = rand_int(search->num_params);
    float param_min = search->param_min[index];
    float param_max = search->param_max[index];
    float param_val = search->param_val[index];
    float radius = (param_max - param_min) * search->temperature;
    float rand_min = constrainf(param_val - radius, param_min, param_max);
    float rand_max = constrainf(param_val + radius, param_min, param_max);
    search->test_val[index] = rand_float(rand_min, rand_max);

    // Update temperature
    search->temperature = constrainf(0.9 * search->temperature, 0.001, 1);
}
