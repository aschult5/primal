#include <array>
#include <iostream>
#include <boost/asio.hpp>

#include "server.hpp"
#include "miller_rabin.hpp"

static bool primalTest(big n)
{
   return custom::miller_rabin_test(n);
}


bool startServer(uint16_t port)
{
   using namespace boost::asio;
   using namespace std;

   io_service io_serv;
   ip::tcp::acceptor acceptor(io_serv, ip::tcp::endpoint(ip::tcp::v4(), port));

   while(1)
   {
      ip::tcp::socket sock(io_serv);

      cout << "Listening on port " << port << endl;
      acceptor.accept(sock);

      while (sock.is_open())
      {
         // Received connection
         boost::system::error_code error;
         array<big, 1> input;
         read(sock, buffer(input), error);
         if (error == boost::asio::error::eof)
         {
            sock.close();
            break; //Connection closed
         }
         else if (error)
         {
            throw boost::system::system_error(error);
         }

         // Determine if the input is prime
         array<uint8_t, 1> result{0};
         if (primalTest(input.front()))
            result.at(0) = 1;
         // Inform the client
         write(sock, buffer(result));
      }
   }
   return true;
}