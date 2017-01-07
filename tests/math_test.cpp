#include <limits>

#include "gtest/gtest.h"

#include "primes.h"

size_t _const_primes_count();
uint32_t _const_prime(size_t index);

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

namespace test_primes
{
  TEST(cons_primes, integrity)
  {
    size_t count = _const_primes_count();
    EXPECT_GT(count, 2);
    EXPECT_EQ(_const_prime(0), 2);
    EXPECT_EQ(_const_prime(1), 3);

    uint32_t last_checked = 3;
    for (size_t i = 2; i < count;)
      {
        last_checked += 2;
        uint32_t root = math::sqrt_floor(last_checked);
        bool is_complex = false;
        for (size_t j = 1; !is_complex; ++j)
          {
            uint32_t prime = _const_prime(j);
            if (prime > root)
              break;
            if (0 == (last_checked % prime))
              is_complex = true;
          }

        if (is_complex)
          continue;

        if (_const_prime(i) != last_checked)
          {
            FAIL() << "Integration of const primes corrupt at index " << i << ": Expected "
                   << last_checked << ", received " << _const_prime(i);
            break;
          }

        ++i;
      }
  }

}

