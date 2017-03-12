#include <iostream>
#include "client.hpp"

using namespace boost::asio;
using namespace std;
using namespace primal;

client::client(string ip, uint16_t port) :
   sock(io_serv)
{
   ip::tcp::resolver resolver(io_serv);
   ip::tcp::resolver::query query(ip, to_string(port));

   auto endpoint_it = resolver.resolve(query);
   connect(sock, endpoint_it);
}

client::~client()
{
   boost::system::error_code ignore;
   sock.shutdown(ip::tcp::socket::shutdown_both, ignore);
   sock.close(ignore);
}

bool client::sendRequest(const request& req, response& res)
{
   boost::system::error_code ec;
   if (req.size() != res.size())
      return false;

   // Send request
   write(sock, buffer(req), ec);
   if (check(ec))
      return false;

   // Read response
   read(sock, buffer(res), ec);
   if (check(ec))
      return false;

   return true;
}
