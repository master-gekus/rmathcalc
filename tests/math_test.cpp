#include <algorithm>

#include "gtest/gtest.h"

TEST(first_test, primitive_test)
{
  int i = 100;
  int j = 50;
  EXPECT_EQ(i/j, 2);
}
