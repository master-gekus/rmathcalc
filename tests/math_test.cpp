#include <limits>

#include "gtest/gtest.h"

#include "primes.h"

namespace test_sqrt
{
  struct test_params
  {
    uint64_t value;
    uint32_t sqrt_floor;
  };

  ::std::ostream& operator<<(::std::ostream& os, const test_params& p)
  {
    return os << "{Value: " << p.value << "; Sqrt: " << p.sqrt_floor << ";}";
  }

  class sqrt_floor : public ::testing::TestWithParam<test_params>
  {
  };

  TEST_P(sqrt_floor, simple_test)
  {
    test_params p = GetParam();
    EXPECT_EQ(math::sqrt_floor(p.value), p.sqrt_floor);
  }

  INSTANTIATE_TEST_CASE_P(sqrt_floor, sqrt_floor, ::testing::Values(
    test_params{0, 0},
    test_params{1, 1},
    test_params{2, 1},
    test_params{3, 1},
    test_params{4, 2},
    test_params{5, 2},
    test_params{6, 2},
    test_params{7, 2},
    test_params{8, 2},
    test_params{9, 3},
    test_params{100, 10},
    test_params{101, 10},
    test_params{102, 10},
    test_params{120, 10},
    test_params{121, 11},
    test_params{9999999999UL, 99999},
    test_params{99999999999UL, 316227},
    test_params{999999999999999UL, 31622776},
    test_params{1000000000000000UL, 31622776},
    test_params{10000000000000000UL, 100000000},
    test_params{std::numeric_limits<int32_t>::max(), 46340},
    test_params{std::numeric_limits<uint32_t>::max(), 65535},
    test_params{std::numeric_limits<int64_t>::max(), 3037000499},
    test_params{std::numeric_limits<uint64_t>::max(), 4294967295}
  ));
}

