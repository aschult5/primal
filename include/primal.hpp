#ifndef __PRIMAL_HPP__
#define __PRIMAL_HPP__

#include <vector>
#include <cstdint>
#include <boost/asio.hpp>
#include <boost/multiprecision/cpp_int.hpp>

#include "miller_rabin.hpp" //smelly, but need M-R response value type

namespace primal
{

using uint128_t = boost::multiprecision::uint128_t;

// big identifies the max bit width supported
using big = uint128_t;
using request = std::vector<big>;
using response = std::vector<custom::primality>;


// Returns whether ec contains an error
inline bool check(const boost::system::error_code& ec)
{
   //  eof == connection gracefully closed
   if (ec && ec != boost::asio::error::eof)
      std::cerr << ec.message() << std::endl;
   return ec;
}

} //namespace

#endif