#ifndef __SERVER_HPP__
#define __SERVER_HPP__ 

#include <cstdint>   //uint16_t
#include "primal.hpp"

namespace primal
{

class server : public connection
{
public:
   server() = delete;
   server(uint16_t port);

   bool listen();

private:
   void handleRequest();
   void respond(bool);
};

} //namespace

#endif