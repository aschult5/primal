#include <array>
#include <iostream>
#include <boost/asio.hpp>

#include "server.hpp"
#include "interface.hpp"
#include "miller_rabin.hpp"

using namespace Primal;
using namespace boost::asio;
using namespace std;

static bool primalTest(uint64_t n)
{
   return custom::miller_rabin_test(n);
}

void Server::respond(bool result)
{
   boost::system::error_code error;
   // Form a response and send it
   Response resp(1,result);
   write(sock, buffer(resp), error);
   if (error)
   {
      if (error != error::eof) //Connection closed
         cerr << error.message() << endl;
      return; 
   }
}

void Server::handleRequest()
{
   boost::system::error_code error;
   // Read the request
   Request req(1);
   read(sock, buffer(req), error);
   if (error)
   {
      if (error != error::eof) //Connection closed
         cerr << error.message() << endl;
      sock.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
      sock.close();
      return;
   }

   // Determine if the input is prime.
   // Inform the client.
   respond(primalTest(req.front()));
}

bool Server::listen()
{
   acceptor.accept(sock);

   while (sock.is_open())
   {
      // Received connection
      handleRequest();
   }
   return true;
}

// void Server::stop()
// {
//    try
//    {
//       sock.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
//       sock.close();
//    }
//    catch (std::exception& e)
//    {
//       std::cerr << "Failed to stop:" << std::endl;
//       std::cerr << e.what() << std::endl;
//    }
// }