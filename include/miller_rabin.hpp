//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// Derived from boost::multiprecision::miller_rabin_test

#ifndef __MILLER_RABIN_HPP
#define __MILLER_RABIN_HPP

#include <boost/multiprecision/integer.hpp>
#include <boost/multiprecision/miller_rabin.hpp>
#include <cstdint>
#include <vector>
#include <algorithm>

using uint128_t = boost::multiprecision::uint128_t;


namespace custom {

enum class primality : uint8_t
{
   NOT,
   PROBABLY,
   GUARANTEED
};

namespace detail {

template<class I, class J>
static unsigned int numTrials(I n, const std::vector<J> & pseudoPrimes)
{
   // Binary search for first pseudoprime larger than n.
   // If we run miller-rabin with the first i primes as bases,
   // we can guarantee that it's prime (or not).
   auto bound = upper_bound(pseudoPrimes.begin(), pseudoPrimes.end(), static_cast<J>(n));
   return bound - pseudoPrimes.begin() + 1;
}

} //detail namespace


// Below is a derivation of boost's miller-rabin implementation.
// Their use of a uniform_int_distribution precluded
// my use of it in its original form.
template<typename big>
primality miller_rabin_test(const big& n, uint128_t hugePseudoprime=0)
{
   using namespace boost::multiprecision;
 
   // list of first n primes
   static const std::vector<big> primes{
      2, 3, 5,
      7, 11,13,
      17,19,23,
      29,31,37
   };
   // Smallest possible pseudoprime if <=i primes tested as base
   //   in miller-rabin.  i starts at 1
   // See: http://oeis.org/A014233
   static std::vector<uint128_t> pseudoPrimes{
      2047,                1373653,             25326001,
      3215031751,          2152302898747,       3474749660383,
      341550071728321,     341550071728321,     3825123056546413051,
      3825123056546413051, 3825123056546413051 //, 318665857834031151167461, cannot have 128bit literals
   };
   if (hugePseudoprime > pseudoPrimes.back())
   {
      pseudoPrimes.push_back(hugePseudoprime);
   }

   if (n == 2)
      return primality::GUARANTEED;  // Trivial special case.
   if(bit_test(n, 0) == 0)
      return primality::NOT;  // n is even

   big nm1 = n - 1;
   big q(n-1), y;

   unsigned k = lsb(q);
   q >>= k;

   // Too large to guarantee prime
   if (n >= pseudoPrimes.back())
   {
      if (boost::multiprecision::miller_rabin_test(n, 25))
         return primality::PROBABLY;
      else
         return primality::NOT;
   }

   // We can guarantee prime if we use
   // the first 'trials' primes as bases
   auto x = primes.begin();
   auto trials = detail::numTrials(n, pseudoPrimes);
   for(unsigned i = 0; i < trials; ++i, ++x)
   {
      y = powm(*x, q, n);
      big j = 0;
      while(true)
      {
         if(y == nm1)
            break;
         if(y == 1)
         {
            if(j == 0)
               break;
            return primality::NOT; // test failed
         }
         if(++j == k)
            return primality::NOT; // failed
         y = powm(y, 2, n);
      }
   }
   return primality::GUARANTEED;  // Yeheh! prime.
}

} //custom namespace
#endif