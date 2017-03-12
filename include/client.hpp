#ifndef __CLIENT_HPP_
#define __CLIENT_HPP_

#include <string>
#include <vector>
#include "primal.hpp"

namespace primal
{

class client : public connection
{
public:
   client() = delete;
   client(std::string ip, uint16_t port);

   bool test(const std::vector<big>& numbers) { return sendRequest(request(numbers)); }

private:
   bool sendRequest(request&&);
};

} //namespace

#endif