#include "../lib/utils.c"
#include <gtest/gtest.h>

TEST(UtilsTest, CheckRandInt) {
    int count_zero, count_one, count_two, count_other = 0;

    // We want a deterministic test
    srand(42);
    for (int i = 0; i < 10000; ++i) {
        switch (rand_int(3)) {
            case 0:
                ++count_zero;
                break;
            case 1:
                ++count_one;
                break;
            case 2:
                ++count_two;
                break;
            default:
                ++count_other;
        }
    }

    ASSERT_GT(count_zero, 10);
    ASSERT_GT(count_one, 10);
    ASSERT_GT(count_two, 10);
    ASSERT_EQ(count_other, 0);
}
