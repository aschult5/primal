#ifndef __CLIENT_HPP_
#define __CLIENT_HPP_

#include <string>
#include <boost/asio.hpp>
#include "primal.hpp" //request response

namespace primal
{

class client
{
public:
   client() = delete;
   client(std::string ip, uint16_t port);
   ~client();

   // No copying
   client(const client&) = delete;
   client& operator=(const client&) = delete;

   bool sendRequest(const request&, response&);

private:
   boost::asio::io_service io_serv;
   boost::asio::ip::tcp::socket sock;
};

} //namespace

#endif