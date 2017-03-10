#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <string>

using big = uint64_t;

static bool isNumber(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(), s.end(),
                         [](const char& c) { return !std::isdigit(c); }) == s.end();
}

#endif
