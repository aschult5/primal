#ifndef __PRIMAL_HPP__
#define __PRIMAL_HPP__

#include <iostream>
#include <vector>
#include <cstdint>
#include <boost/asio.hpp>

namespace primal
{

using big = uint64_t;
using request = std::vector<big>;
using response = std::vector<uint8_t>;


class connection
{
public:
   connection(uint16_t port) : port(port), sock(io_serv) {}

   void close()
   {
      try
      {
         sock.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
         sock.close(); 
      }
      catch (std::exception& e)
      {
         std::cerr << e.what() << std::endl;
      }
   }
   
   inline bool check(boost::system::error_code ec)
   {
      if (ec && ec != boost::asio::error::eof)
         std::cerr << ec.message() << std::endl;
      return ec;
   }

protected:
   uint16_t port;
   boost::asio::io_service io_serv;
   boost::asio::ip::tcp::socket sock;
};

} //namespace

#endif