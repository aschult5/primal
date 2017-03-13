#ifndef __PRIMAL_HPP__
#define __PRIMAL_HPP__

#include <vector>
#include <cstdint>
#include <boost/asio.hpp>

#include "miller_rabin.hpp" //smelly, but need response value type for interface

namespace primal
{

using big = uint64_t;
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