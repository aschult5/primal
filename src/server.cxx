//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// Derived from boost::asio examples

#include <iostream>
#include <functional>   //bind
#include <fstream>

#include "server.hpp"
#include "miller_rabin.hpp"

using namespace primal;
using namespace boost::asio;
using boost::system::error_code;
using std::placeholders::_1;
using std::placeholders::_2;

connection::~connection()
{
   error_code ignore;
   sock.shutdown(ip::tcp::socket::shutdown_both, ignore);
   sock.close(ignore);
}

void connection::handleRequest()
{
   // Read the request asynchronously while the server takes more connections
   async_read(sock, buffer(number), std::bind(&connection::readHandler,shared_from_this(),_1,_2));
}

void connection::readHandler(const error_code& ec, size_t bytes)
{
   if (!check(ec))
   {
      // Determine if the input is prime.
      respond(custom::miller_rabin_test(number.front(), server::hugePseudoprime));

      // clients may send multiple requests
      handleRequest(); 
   }
}

void connection::respond(custom::primality result)
{
   error_code ec;
   response res{result};

   write(sock, buffer(res), ec);
   check(ec);
}

uint128_t server::hugePseudoprime=0;

server::server(uint16_t port, uint128_t pseudoprime) :
   acceptor(io_serv, ip::tcp::endpoint(ip::tcp::v4(), port))
{
   hugePseudoprime = pseudoprime;
   listen();
   io_serv.run();
}

void server::listen()
{
   auto conn = std::make_shared<connection>(io_serv);
   acceptor.async_accept(conn->socket(), bind(&server::handleAccept, this, conn, _1));
}

void server::handleAccept(std::shared_ptr<connection> newConnection, const error_code& ec)
{
   if (ec)
      std::cerr << ec.message() << std::endl;
   else
      newConnection->handleRequest();

   listen(); //forever
}
