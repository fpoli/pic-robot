#include "../lib/pid.c"
#include <gtest/gtest.h>

TEST(PidTest, ResetErrorHistoryWorks) {
    float a1, a2, b1, b2, c1, c2, d1, d2;

    // Initialization
    pid_init();
    pid_set_parameters(0.4, 0.5, 0.6);
    pid_set_target(100);

    // Calculate reference values
    a1 = pid_run(123);
    b1 = pid_run(-987);
    c1 = pid_run(123);
    for (int i = 0; i < 10000; ++i) {
        pid_run(-134);
    }
    d1 = pid_run(10);

    // Reset once and compare
    pid_reset_error_history();
    a2 = pid_run(123);
    b2 = pid_run(-987);
    c2 = pid_run(123);
    for (int i = 0; i < 10000; ++i) {
        pid_run(-134);
    }
    d2 = pid_run(10);

    ASSERT_EQ(a1, a2);
    ASSERT_EQ(b1, b2);
    ASSERT_EQ(c1, c2);
    ASSERT_EQ(d1, d2);

    // Reset multiple times and compare
    pid_reset_error_history();
    pid_run(789789);
    pid_reset_error_history();
    pid_reset_error_history();
    a2 = pid_run(123);
    b2 = pid_run(-987);
    c2 = pid_run(123);
    for (int i = 0; i < 10000; ++i) {
        pid_run(-134);
    }
    d2 = pid_run(10);

    ASSERT_EQ(a1, a2);
    ASSERT_EQ(b1, b2);
    ASSERT_EQ(c1, c2);
    ASSERT_EQ(d1, d2);
}

TEST(PidTest, OutputsZeroWhenOnTarget) {
    pid_init();

    pid_set_parameters(0.5, 0.5, 0.5);
    pid_set_target(0);
    ASSERT_EQ(0, pid_run(0));

    pid_set_parameters(0.5, 0.5, 0.5);
    pid_set_target(0);
    ASSERT_EQ(0, pid_run(0));

    pid_set_parameters(0.5, 0.5, 0.5);
    pid_set_target(1);
    ASSERT_EQ(0, pid_run(1));

    pid_set_parameters(0.5, 0.5, 0.5);
    pid_set_target(-1);
    ASSERT_EQ(0, pid_run(-1));

    pid_set_parameters(0.5, 0.5, 0.5);
    pid_set_target(123);
    ASSERT_EQ(0, pid_run(123));

    pid_set_parameters(0.5, 0.5, 0.5);
    pid_set_target(10.123);
    ASSERT_EQ(0, pid_run(10.123));

    pid_set_parameters(0.5, 0.5, 0.5);
    pid_set_target(10.123);
    ASSERT_EQ(0, pid_run(10.123));

    pid_set_parameters(0.5, 0.5, 0.5);
    pid_set_target(987987);
    ASSERT_EQ(0, pid_run(987987));

    pid_set_parameters(0.5, 0.5, 0.5);
    pid_set_target(-32452);
    ASSERT_EQ(0, pid_run(-32452));

    pid_set_parameters(0.5, 0.5, 0.5);
    pid_set_target(0);
    ASSERT_EQ(0, pid_run(0));
}

TEST(PidTest, ProportionalSignIsOk) {
    pid_init();
    pid_set_parameters(1, 0, 0);

    pid_set_target(0);
    ASSERT_TRUE(pid_run(555) < 0);

    pid_reset_error_history();
    pid_set_target(0);
    ASSERT_TRUE(pid_run(-555) > 0);

    pid_reset_error_history();
    pid_set_target(555);
    ASSERT_TRUE(pid_run(0) > 0);

    pid_reset_error_history();
    pid_set_target(-555);
    ASSERT_TRUE(pid_run(0) < 0);
}

TEST(PidTest, IntegralSignIsOk) {
    pid_init();
    pid_set_parameters(0, 1, 0);

    pid_set_target(0);
    ASSERT_TRUE(pid_run(555) < 0);

    pid_reset_error_history();
    pid_set_target(0);
    ASSERT_TRUE(pid_run(-555) > 0);

    pid_reset_error_history();
    pid_set_target(555);
    ASSERT_TRUE(pid_run(0) > 0);

    pid_reset_error_history();
    pid_set_target(-555);
    ASSERT_TRUE(pid_run(0) < 0);
}

TEST(PidTest, DerivativeSignIsOk) {
    pid_init();
    pid_set_parameters(0, 0, 1);

    pid_set_target(0);
    pid_run(111);
    ASSERT_TRUE(pid_run(555) < 0);

    pid_reset_error_history();
    pid_set_target(0);
    pid_run(999);
    ASSERT_TRUE(pid_run(555) > 0);

    pid_reset_error_history();
    pid_set_target(0);
    pid_run(-111);
    ASSERT_TRUE(pid_run(-555) > 0);

    pid_reset_error_history();
    pid_set_target(0);
    pid_run(-999);
    ASSERT_TRUE(pid_run(-555) < 0);
}
