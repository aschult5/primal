#include <array>
#include <iostream>
#include <boost/asio.hpp>

#include "server.hpp"
#include "miller_rabin.hpp"

using namespace boost::asio;
using namespace std;
using namespace primal;

server::server(uint16_t port) :
   connection(port)
{}


void server::respond(bool result)
{
   boost::system::error_code ec;
   // Form a response and send it
   response resp(1,result);
   write(sock, buffer(resp), ec);
   if (ec)
   {
      if (ec != error::eof) //Connection closed
         cerr << ec.message() << endl;
      return; 
   }
}

void server::handleRequest()
{
   boost::system::error_code ec;
   // Read the request
   request req(1);
   read(sock, buffer(req), ec);
   if (ec)
   {
      if (ec != error::eof) //Connection closed
         cerr << ec.message() << endl;
      sock.shutdown(ip::tcp::socket::shutdown_both);
      sock.close();
      return;
   }

   // Determine if the input is prime.
   // Inform the client.
   respond(custom::miller_rabin_test(req.front()));
}

bool server::listen()
{
   ip::tcp::acceptor acceptor(io_serv, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));
   acceptor.accept(sock);

   while (sock.is_open())
   {
      // Received connection
      handleRequest();
   }
   return true;
}
