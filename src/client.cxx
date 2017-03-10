#include <iostream>
#include <boost/asio.hpp>
#include "client.hpp"

bool startClient(std::string ip, uint16_t port, const std::vector<big>& numbers)
{
   using namespace boost::asio;
   using namespace std;

   io_service io_serv;
   ip::tcp::resolver resolver(io_serv);
   ip::tcp::socket sock(io_serv);

   ip::tcp::resolver::query query(ip, to_string(port));
   auto endpoint_it = resolver.resolve(query);

   connect(sock, endpoint_it);
   for (auto const& num: numbers)
   {
      // write server
      boost::system::error_code error;
      array<big, 1> data{num};
      array<uint8_t, 1> result{0};

      write(sock, buffer(data));
      read(sock, buffer(result), error);
      if (error == boost::asio::error::eof)
      {
         //Connection closed
         break;
      }
      else if (error)
      {
         throw boost::system::system_error(error);
      }

      cout << num << " is ";
      if (result.front() == 1)
         cout << "PRIME" << endl;
      else
         cout << "not prime" << endl;
   }
   return true;
}