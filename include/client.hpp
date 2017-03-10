#ifndef __CLIENT_HPP_
#define __CLIENT_HPP_

#include <string>
#include <vector>

#include "server.hpp" // big

bool startClient(std::string ip, uint16_t port, const std::vector<big>& numbers);

#endif