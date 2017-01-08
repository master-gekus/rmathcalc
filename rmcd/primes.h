#ifndef RMATHCALC_PRIMES_H_INCLUDED
#define RMATHCALC_PRIMES_H_INCLUDED

#include <cstdint>
#include <limits>
#include <functional>

namespace math
{
  enum
  {
    /** \brief Maximum count of primes numbers
     *
     * Stored prime numbers are stored during checkin number to be a prime and are used for
     * fast checking in next case. Stored only 32-bit prime numbers.
     *
     * Becouse of only 32-bit numbers only a stored, total memory needed to store is
     * sizeof(uint32_t) * MAX_STORED_PRIMES.
     */
    MAX_STORED_PRIMES = 10 * 1024 * 1024,
  };

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

    T result, devider = (value >> 1);
    do
      {
        result = devider;
        devider = ((value / devider + devider) >> 1);
      }
    while (result > devider);

    return result;
  }

  /**
   * @brief Checks for value is a prime number
   * @param value Value to check
   * @param check_interrupt Interrupt check point (see description)
   * @return true, if value is a prime number
   *
   * check_interrupt is regulary called during calculation and can be used to interrupt process
   * by throwing an exception. For example,this can be ::boost::this_thread::interruption_point()
   */
  bool is_prime(uint32_t value, std::function<void()> check_interrupt = nullptr);

  /**
   * @brief Calculates next prime value
   * @param value The value from which to search for the next prime number
   * @param check_interrupt Interrupt check point (see description of \ref is_prime)
   * @return The first prime number greater than specified
   *
   * If the calculated prime number exceeds maximum of uint32_t, 0 returned.
   */
  uint32_t next_prime(uint32_t value, std::function<void()> check_interrupt = nullptr);

  //Factorizing number (decomposes into prime factors)
  /**
   * @brief Factorizes the number (decomposes it into prime factors)
   * @param [out] result Result of factorization
   * @param [in] value Number to factorize
   * @param [in] check_interrupt Interrupt check point (see description of \ref is_prime)
   *
   * If value is prime, result will contents only one element - value.
   */
  void factorize(std::vector<uint64_t>& result, uint64_t value,
                 std::function<void()> check_interrupt = nullptr);

}

#endif // RMATHCALC_PRIMES_H_INCLUDED
