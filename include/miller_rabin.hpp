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


namespace custom {

enum class primality : uint8_t
{
   NOT,
   PROBABLY,
   GUARANTEED
};


namespace detail {

template<class I, class J,
   typename = std::enable_if< std::is_unsigned<I>::value &&
                              std::is_unsigned<J>::value &&
                              sizeof(J) >= sizeof(I) > >
static unsigned int numTrials(const I& n, const std::vector<J> & pseudoPrimes)
{
   // Binary search for first pseudoprime larger than n.
   // If we run miller-rabin with the first i primes as bases,
   // we can guarantee that it's prime (or not).
   auto bound = upper_bound(pseudoPrimes.begin(), pseudoPrimes.end(), static_cast<J>(n));
   return bound - pseudoPrimes.begin() + 1;
}

} //detail namespace


// Below is a derivation of boost's miller-rabin primality test.
// Their use of a uniform_int_distribution precluded
// my use of it in its original form.
// I want to specify the bases used.
template< typename I, typename = std::enable_if<std::is_unsigned<I>::value> >
primality miller_rabin_test(const I& n, unsigned trials)
{
   using namespace boost::multiprecision;
   
   static const std::vector<I> primes{
      2, 3, 5,
      7, 11,13,
      17,19,23,
      29,31,37
   };

   if (primes.size() < trials || trials == 0)
      return primality::NOT;

   //
   // The Miller-Rabin algorithm is not described here
   //
   I nm1 = n - 1;
   I q(n-1), y;

   unsigned k = lsb(q);
   q >>= k;


   auto x = primes.begin();
   for(unsigned i = 0; i < trials; ++i, ++x)
   {
      y = powm(*x, q, n);
      I j = 0;
      while(true)
      {
         if(y == nm1)
            break;
         if(y == 1)
         {
            if(j == 0)
               break;
            return primality::NOT;
         }
         if(++j == k)
            return primality::NOT;
         y = powm(y, 2, n);
      }
   }

   // Due to our use of the first 'trials' primes as bases,
   // we can guarantee primality
   return primality::GUARANTEED;
}

template< typename I, typename = std::enable_if<std::is_unsigned<I>::value> >
primality primality_test(const I& n, boost::multiprecision::uint128_t hugePseudoprime=0)
{
   using namespace boost::multiprecision;
 
   // Smallest possible pseudoprime if <=i primes tested as base
   //   in miller-rabin.  i starts at 1
   // See: http://oeis.org/A014233
   static std::vector<boost::multiprecision::uint128_t> pseudoPrimes{
      2047,                1373653,             25326001,
      3215031751,          2152302898747,       3474749660383,
      341550071728321,     341550071728321,     3825123056546413051,
      3825123056546413051, 3825123056546413051 //, 318665857834031151167461, cannot have 128bit literals
   };
   if (hugePseudoprime > pseudoPrimes.back())
   {
      pseudoPrimes.push_back(hugePseudoprime);
   }

   if (n <= 1)
      return primality::NOT;
   if (n == 2)
      return primality::GUARANTEED;  // Trivial special case.
   if(bit_test(n, 0) == 0)
      return primality::NOT;  // n is even

   // Too large to guarantee prime
   if (n >= pseudoPrimes.back())
   {
      if (boost::multiprecision::miller_rabin_test(n, 25))
         return primality::PROBABLY;
      else
         return primality::NOT;
   }
   else
   {
      return custom::miller_rabin_test(n, detail::numTrials(n, pseudoPrimes));
   }


}

} //custom namespace
#endif