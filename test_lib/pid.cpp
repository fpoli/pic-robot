#include "../lib/pid.c"
#include <gtest/gtest.h>

#define PID_FILL_REPEATED(val, num) \
    for (int i = 0; i < (num); ++i) { pid_update(val, 1); }

TEST(PidTest, CheckInitialInternalState) {
    pid_init();
    ASSERT_EQ(pid->offset, 0);
    ASSERT_EQ(pid->kp, 0);
    ASSERT_EQ(pid->ki, 0);
    ASSERT_EQ(pid->kd, 0);
}

TEST(PidTest, ResetErrorHistoryWorks) {
    float a1, a2, b1, b2, c1, c2, d1, d2;

    pid_init();
    pid_set_parameters(0.4, 0.5, 0.6);
    pid_set_target(100);

    // Calculate reference values
    a1 = pid_update(123, 1);
    b1 = pid_update(-987, 1);
    c1 = pid_update(123, 1);
    PID_FILL_REPEATED(123, 100000);
    d1 = pid_update(10, 1);

    // Reset once and compare
    pid_reset_error_history();
    a2 = pid_update(123, 1);
    b2 = pid_update(-987, 1);
    c2 = pid_update(123, 1);
    PID_FILL_REPEATED(123, 100000);
    d2 = pid_update(10, 1);

    ASSERT_EQ(a1, a2);
    ASSERT_EQ(b1, b2);
    ASSERT_EQ(c1, c2);
    ASSERT_EQ(d1, d2);

    // Reset multiple times and compare
    pid_reset_error_history();
    pid_update(789789, 1);
    pid_reset_error_history();
    pid_reset_error_history();
    a2 = pid_update(123, 1);
    b2 = pid_update(-987, 1);
    c2 = pid_update(123, 1);
    PID_FILL_REPEATED(123, 100000);
    d2 = pid_update(10, 1);

    ASSERT_EQ(a1, a2);
    ASSERT_EQ(b1, b2);
    ASSERT_EQ(c1, c2);
    ASSERT_EQ(d1, d2);
}

TEST(PidTest, OutputsZeroWhenOnTarget) {
    pid_init();

    pid_set_parameters(0.5, 0.5, 0.5);
    pid_set_target(0);
    ASSERT_EQ(0, pid_update(0, 1));

    pid_set_parameters(0.5, 0.5, 0.5);
    pid_set_target(0);
    ASSERT_EQ(0, pid_update(0, 1));

    pid_set_parameters(0.5, 0.5, 0.5);
    pid_set_target(1);
    ASSERT_EQ(0, pid_update(1, 1));

    pid_set_parameters(0.5, 0.5, 0.5);
    pid_set_target(-1);
    ASSERT_EQ(0, pid_update(-1, 1));

    pid_set_parameters(0.5, 0.5, 0.5);
    pid_set_target(123);
    ASSERT_EQ(0, pid_update(123, 1));

    pid_set_parameters(0.5, 0.5, 0.5);
    pid_set_target(10.123);
    ASSERT_EQ(0, pid_update(10.123, 1));

    pid_set_parameters(0.5, 0.5, 0.5);
    pid_set_target(10.123);
    ASSERT_EQ(0, pid_update(10.123, 1));

    pid_set_parameters(0.5, 0.5, 0.5);
    pid_set_target(987987);
    ASSERT_EQ(0, pid_update(987987, 1));

    pid_set_parameters(0.5, 0.5, 0.5);
    pid_set_target(-32452);
    ASSERT_EQ(0, pid_update(-32452, 1));

    pid_set_parameters(0.5, 0.5, 0.5);
    pid_set_target(0);
    ASSERT_EQ(0, pid_update(0, 1));
}

TEST(PidTest, ProportionalSignIsOk) {
    pid_init();
    pid_set_parameters(1, 0, 0);

    pid_set_target(0);
    ASSERT_LT(pid_update(555, 1), 0);

    pid_reset_error_history();
    pid_set_target(0);
    ASSERT_GT(pid_update(-555, 1), 0);

    pid_reset_error_history();
    pid_set_target(555);
    ASSERT_GT(pid_update(0, 1), 0);

    pid_reset_error_history();
    pid_set_target(-555);
    ASSERT_LT(pid_update(0, 1), 0);
}

TEST(PidTest, IntegralSignIsOk) {
    pid_init();
    pid_set_parameters(0, 1, 0);

    pid_set_target(0);
    ASSERT_LT(pid_update(555, 1), 0);

    pid_reset_error_history();
    pid_set_target(0);
    ASSERT_GT(pid_update(-555, 1), 0);

    pid_reset_error_history();
    pid_set_target(555);
    ASSERT_GT(pid_update(0, 1), 0);

    pid_reset_error_history();
    pid_set_target(-555);
    ASSERT_LT(pid_update(0, 1), 0);
}

TEST(PidTest, DerivativeSignIsOk) {
    pid_init();
    pid_set_parameters(0, 0, 1);

    pid_set_target(111);
    ASSERT_LT(pid_update(555, 1), 0);

    pid_reset_error_history();
    pid_set_target(0);
    pid_update(999, 1);
    ASSERT_GT(pid_update(555, 1), 0);

    pid_reset_error_history();
    pid_set_target(0);
    pid_update(-111, 1);
    ASSERT_GT(pid_update(-555, 1), 0);

    pid_reset_error_history();
    pid_set_target(0);
    pid_update(-999, 1);
    ASSERT_LT(pid_update(-555, 1), 0);
}

TEST(PidTest, FitnessReadyIsOk) {
    pid_init();

    PID_FILL_REPEATED(123, 100000);
    ASSERT_TRUE(pid_fitness_ready());
    ASSERT_TRUE(pid_fitness_ready());
    pid_get_fitness();
    ASSERT_FALSE(pid_fitness_ready());
    ASSERT_FALSE(pid_fitness_ready());

    PID_FILL_REPEATED(123, 100000);
    ASSERT_TRUE(pid_fitness_ready());
    pid_reset_fitness_history();
    ASSERT_FALSE(pid_fitness_ready());

    pid_update(0, 1);
    ASSERT_FALSE(pid_fitness_ready());

    PID_FILL_REPEATED(123, 100000);
    ASSERT_TRUE(pid_fitness_ready());
    pid_init();
    ASSERT_FALSE(pid_fitness_ready());

    PID_FILL_REPEATED(123, 100000);
    ASSERT_TRUE(pid_fitness_ready());
    pid_get_fitness();
    ASSERT_FALSE(pid_fitness_ready());
}

TEST(PidTest, FitnessSignIsOk) {
    pid_init();

    PID_FILL_REPEATED(0, 100000);
    ASSERT_EQ(pid_get_fitness(), 0);

    for (int i = 0; i < 100000; ++i) {
        pid_update(-123, 1);
        pid_update(123, 1);
    }
    ASSERT_LT(pid_get_fitness(), 0);

    PID_FILL_REPEATED(0, 100000);
    ASSERT_EQ(pid_get_fitness(), 0);

    PID_FILL_REPEATED(42, 100000);
    ASSERT_LT(pid_get_fitness(), 0);

    PID_FILL_REPEATED(0, 100000);
    ASSERT_EQ(pid_get_fitness(), 0);
}

TEST(PidTest, FitnessOrderIsOk) {
    float bad_fitness, good_fitness;
    pid_init();

    for (int i = 0; i < 100000; ++i) {
        pid_update(-12, 1);
        pid_update(300, 1);
    }
    bad_fitness = pid_get_fitness();

    for (int i = 0; i < 100000; ++i) {
        pid_update(-10, 1);
        pid_update(170, 1);
        pid_update(110, 1);
    }
    good_fitness = pid_get_fitness();

    ASSERT_LT(bad_fitness, good_fitness);
}
