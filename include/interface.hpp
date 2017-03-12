#ifndef __INTERFACE_HPP__
#define __INTERFACE_HPP__

#include <vector>
#include <cstdint>

namespace Primal
{

using Response = std::vector<uint8_t>;
using Request = std::vector<uint64_t>;

// template <typename T>
// class Request
// {
// public:
//    Request() : data(1) {}
//    Request(T data) : data(1,num) {}
//    Request(const std::vector<T>& num) : data(num) {}
//    Request(std::vector<T>&& num) : data(num) {}
//    operator const std::vector<T>&() {return data;}
//    operator std::vector<T>&() {return data;}
// private:
//    std::vector<T> data;
// };

// class Response
// {
// public:
//    Response() : data(1) {}
//    Response(bool response) : data(1, response ? 1:0) {}
//    Response(const std::vector<uint8_t>& responses) : data(responses) {}
//    Response(std::vector<uint8_t>&& responses) : data(responses) {}
//    operator const std::vector<uint8_t>&() {return data;} // decltype?
//    operator std::vector<uint8_t>&() {return data;}
// private:
//    std::vector<uint8_t> data;
// };

} //namespace

#endif