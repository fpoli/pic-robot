#include "../lib/search.c"
#include <gtest/gtest.h>

#define W 0
#define X 1
#define Y 2
#define Z 3

#define MEASURE_FITNESS(TEST_VAL) ( \
    - 1 \
    + cos(1 - search_get_ ## TEST_VAL(W)) \
    * cos(10 - search_get_ ## TEST_VAL(X)) \
    * cos(100 - search_get_ ## TEST_VAL(Y)) \
    * cos(1000 - search_get_ ## TEST_VAL(Z)) \
    - ( \
        (1 - search_get_ ## TEST_VAL(W)) \
        * (1 - search_get_ ## TEST_VAL(W)) \
        + (10 - search_get_ ## TEST_VAL(X)) \
        * (10 - search_get_ ## TEST_VAL(X)) \
        + (100 - search_get_ ## TEST_VAL(Y)) \
        * (100 - search_get_ ## TEST_VAL(Y)) \
        + (1000 - search_get_ ## TEST_VAL(Z)) \
        * (1000 - search_get_ ## TEST_VAL(Z)) \
    ) \
)

TEST(SearchTest, CheckInitialInternalState) {
    float fitness;

    search_init(4);
    search_init_param_val(W, 0, -1000, 1000);
    search_init_param_val(X, -1, -1000, 1000);
    search_init_param_val(Y, -10, -2000, 2000);
    search_init_param_val(Z, -100, -2000, 1000);

    fitness = MEASURE_FITNESS(param_val);
    ASSERT_LT(fitness, -1);

    // We want a deterministic test
    srand(42);
    for (int i = 0; i < 3000; ++i) {
        fitness = MEASURE_FITNESS(test_val);
        search_update_with_test_fitness(fitness);
    }

    fitness = MEASURE_FITNESS(param_val);
    ASSERT_NEAR(fitness, 0, 0.0001);
    ASSERT_NEAR(search_get_param_val(W), 1, 0.1);
    ASSERT_NEAR(search_get_param_val(X), 10, 0.1);
    ASSERT_NEAR(search_get_param_val(Y), 100, 0.1);
    ASSERT_NEAR(search_get_param_val(Z), 1000, 0.1);
}
