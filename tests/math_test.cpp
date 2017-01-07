#include <algorithm>

#include "gtest/gtest.h"

#include "primes.h"

TEST(sqrt_floor, simple_test)
{
  EXPECT_EQ(math::sqrt_floor(0), 0);
  EXPECT_EQ(math::sqrt_floor(1), 1);
  EXPECT_EQ(math::sqrt_floor(2), 1);
  EXPECT_EQ(math::sqrt_floor(3), 1);
  EXPECT_EQ(math::sqrt_floor(4), 2);
  EXPECT_EQ(math::sqrt_floor(120), 10);
  EXPECT_EQ(math::sqrt_floor(121), 11);
  EXPECT_EQ(math::sqrt_floor(125), 11);
  EXPECT_EQ(math::sqrt_floor(143), 11);
  EXPECT_EQ(math::sqrt_floor(144), 12);
}
