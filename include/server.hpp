//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// Derived from boost::asio examples

#ifndef __SERVER_HPP__
#define __SERVER_HPP__ 

#include <memory>
#include <boost/asio.hpp>
#include "primal.hpp"


namespace primal
{

//
// connection
//
class connection : public std::enable_shared_from_this<connection>
{
public:
   connection() = delete;

   explicit connection(boost::asio::io_service& io_serv) : sock(io_serv), number(1) {}
   ~connection();

   // No copying
   connection(const connection&) = delete;
   connection& operator=(const connection&) = delete;
   
   void handleRequest();
   boost::asio::ip::tcp::socket& socket() {return sock;}

private:
   void respond(custom::primality);
   void readHandler(const boost::system::error_code&, size_t);

   boost::asio::ip::tcp::socket sock;
   request number;
};

//
// server
//
class server
{
public:
   server() = delete;

   explicit server(uint16_t port);

   // No copying
   server(const server&) = delete;
   server& operator=(const server&) = delete;

private:
   void listen();
   void handleAccept(std::shared_ptr<connection>, const boost::system::error_code&);

   boost::asio::io_service io_serv;
   boost::asio::ip::tcp::acceptor acceptor;
};

} //namespace

#endif