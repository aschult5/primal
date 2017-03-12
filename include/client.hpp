#ifndef __CLIENT_HPP_
#define __CLIENT_HPP_

#include <string>
#include <vector>

bool startClient(std::string ip, uint16_t port, const std::vector<uint64_t>& numbers);

#endif