#ifndef __PRIMAL_HPP__
#define __PRIMAL_HPP__

#include <vector>
#include <cstdint>
#include <boost/asio.hpp>
#include <boost/multiprecision/cpp_int.hpp>

#include "miller_rabin.hpp" //smelly, but need M-R response value type

namespace primal
{

using big = boost::multiprecision::uint128_t;
using request = std::vector<big>;
using response = std::vector<custom::primality>;


inline bool check(const boost::system::error_code& ec)
{
   if (ec && ec != boost::asio::error::eof && ec != boost::asio::error::connection_reset)
      std::cerr << ec.message() << std::endl;
   return ec;
}

} //namespace

#endif