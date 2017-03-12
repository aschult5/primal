#ifndef __PRIMAL_HPP__
#define __PRIMAL_HPP__

#include <vector>
#include <cstdint>
#include <boost/asio.hpp>

namespace primal
{

using big = uint64_t;
using request = std::vector<big>;
using response = std::vector<uint8_t>;
static const big sentinel{0};


inline bool check(const boost::system::error_code& ec)
{
   if (ec && ec != boost::asio::error::eof && ec != boost::asio::error::connection_reset)
      std::cerr << ec.message() << std::endl;
   return ec;
}

} //namespace

#endif