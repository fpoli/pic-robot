#include "../lib/processing.c"
#include <gtest/gtest.h>

TEST(PidTest, ResetWorks) {
	float a1, a2, b1, b2, c1, c2, d1, d2;

	// Calculate reference values
	a1 = pid(123, -234, 0.4, 0.5, 0.6);
	b1 = pid(-987, 0, 0.8, 0.8, 0.8);
	c1 = pid(123, 789, 0.3, 0.3, 0.3);
	for (int i = 0; i < 10000; ++i) {
		pid(-134, 8799, 0.7, 0.7, 0.7);
	}
	d1 = pid(10, 10, 0.8, 0.7, 0.6);

	// Reset once and compare
	pid_reset();
	a2 = pid(123, -234, 0.4, 0.5, 0.6);
	b2 = pid(-987, 0, 0.8, 0.8, 0.8);
	c2 = pid(123, 789, 0.3, 0.3, 0.3);
	for (int i = 0; i < 10000; ++i) {
		pid(-134, 8799, 0.7, 0.7, 0.7);
	}
	d2 = pid(10, 10, 0.8, 0.7, 0.6);

	ASSERT_EQ(a1, a2);
	ASSERT_EQ(b1, b2);
	ASSERT_EQ(c1, c2);
	ASSERT_EQ(d1, d2);

	// Reset multiple times and compare
	pid_reset();
	pid(789789, 1234, 0.8, 0.8, 0.5);
	pid_reset();
	pid_reset();
	pid_reset();
	a2 = pid(123, -234, 0.4, 0.5, 0.6);
	b2 = pid(-987, 0, 0.8, 0.8, 0.8);
	c2 = pid(123, 789, 0.3, 0.3, 0.3);
	for (int i = 0; i < 10000; ++i) {
		pid(-134, 8799, 0.7, 0.7, 0.7);
	}
	d2 = pid(10, 10, 0.8, 0.7, 0.6);

	ASSERT_EQ(a1, a2);
	ASSERT_EQ(b1, b2);
	ASSERT_EQ(c1, c2);
	ASSERT_EQ(d1, d2);

	// clean state
	pid_reset();
}

TEST(PidTest, OutputsZeroWhenOnTarget) {
	ASSERT_EQ(0, pid(0, 0, 0.5, 0.5, 0.5));
	ASSERT_EQ(0, pid(0, 0, 0.5, 0.5, 0.5));
	ASSERT_EQ(0, pid(0, 0, 0.5, 0.5, 0.5));
	ASSERT_EQ(0, pid(1, 1, 0.5, 0.5, 0.5));
	ASSERT_EQ(0, pid(-1, -1, 0.5, 0.5, 0.5));
	ASSERT_EQ(0, pid(-1, -1, 0.5, 0.5, 0.5));
	ASSERT_EQ(0, pid(123, 123, 0.5, 0.5, 0.5));
	ASSERT_EQ(0, pid(10.123, 10.123, 0.5, 0.5, 0.5));
	ASSERT_EQ(0, pid(10.123, 10.123, 0.5, 0.5, 0.5));
	ASSERT_EQ(0, pid(987987, 987987, 0.5, 0.5, 0.5));
	ASSERT_EQ(0, pid(-32452, -32452, 0.5, 0.5, 0.5));
	ASSERT_EQ(0, pid(0, 0, 0.5, 0.5, 0.5));

	// clean state
	pid_reset();
}

TEST(PidTest, OutputsSignIsOk) {
	ASSERT_GT(pid(0, 1, 0.5, 0.5, 0.5), 0);
	pid_reset();

	ASSERT_GT(pid(0, 123, 0.5, 0.5, 0.5), 0);
	pid_reset();

	ASSERT_GT(pid(0, 789789, 0.5, 0.5, 0.5), 0);
	pid_reset();

	ASSERT_LT(pid(0, -1, 0.5, 0.5, 0.5), 0);
	pid_reset();

	ASSERT_LT(pid(0, -123, 0.5, 0.5, 0.5), 0);
	pid_reset();

	ASSERT_LT(pid(0, -789789, 0.5, 0.5, 0.5), 0);
	pid_reset();

	pid(0, -123, 0.5, 0.5, 0.5);
	pid(0, 0, 0.5, 0.5, 0.5);
	pid(0, 0, 0.5, 0.5, 0.5);
	ASSERT_GT(pid(0, 456, 0.5, 0.5, 0.5), 0);
	pid_reset();

	pid(0, 123, 0.5, 0.5, 0.5);
	pid(0, 0, 0.5, 0.5, 0.5);
	pid(0, 0, 0.5, 0.5, 0.5);
	ASSERT_LT(pid(0, -456, 0.5, 0.5, 0.5), 0);
	pid_reset();
}
