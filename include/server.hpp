#ifndef __SERVER_HPP__
#define __SERVER_HPP__ 

#include <boost/asio.hpp>
#include <cstdint>

namespace Primal
{

class Server
{
public:
   Server(uint16_t port) :
      sock(io_serv),
      acceptor(io_serv, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) 
      {}
   bool listen();
   // void stop();

private:
   void handleRequest();
   void respond(bool);

   boost::asio::io_service io_serv;
   boost::asio::ip::tcp::socket sock;
   boost::asio::ip::tcp::acceptor acceptor;
};

} //namespace

#endif