#include <iostream>
#include "client.hpp"

using namespace boost::asio;
using namespace std;
using namespace primal;

client::client(string ip, uint16_t port) :
   connection(port)
{
   ip::tcp::resolver resolver(io_serv);
   ip::tcp::resolver::query query(ip, to_string(port));

   auto endpoint_it = resolver.resolve(query);
   connect(sock, endpoint_it);
}


bool client::sendRequest(request&& req)
{
   boost::system::error_code ec;

   // Send request
   write(sock, buffer(req), ec);
   if (check(ec))
   {
      close();
      return false;
   }

   // Read response
   response resp(req.size());
   read(sock, buffer(resp), ec);
   if (check(ec))
   {
      close();
      return false;
   }

   for (unsigned i=0; i<resp.size(); ++i)
   {
      cout << req[i] << " is ";
      if (resp[i] == 1)
         cout << "PRIME" << endl;
      else
         cout << "not prime" << endl;
   }

   return true;;
}
