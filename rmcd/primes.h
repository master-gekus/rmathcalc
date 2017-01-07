#ifndef RMATHCALC_PRIMES_H_INCLUDED
#define RMATHCALC_PRIMES_H_INCLUDED

#include <limits>

namespace math
{
  /**
   * @brief Calculates floor of square root
   * @param value Value to find floor of square root of
   * @return Floor of square root
   *
   * Finds the integer floor of square root of value, i.e. the maximum integer number
   * satisfying the condition:
   * @code
   * sqrt_floor * sqrt_floor <= value
   * @endcode
   */
  template<typename T>
  T sqrt_floor(T value)
  {
    static_assert(std::numeric_limits<T>::is_integer,
                  "sqrt_floor is usable only with integer types!");
    if (0 >= value)
      return 0;
    if (3 >= value)
      return 1;

    T result, devider = (value / 2);
    do
      {
        result = devider;
        devider = ((value / devider + devider) >> 1);
      }
    while (result > devider);

    return result;
  }
}

#endif // RMATHCALC_PRIMES_H_INCLUDED
