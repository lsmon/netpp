#ifndef NETPP_STRUCTURES_HPP
#define NETPP_STRUCTURES_HPP

#include <string>
#include <unordered_map>

struct HttpRequest
{
    std::string method;
    std::string path;
    std::unordered_map<std::string, std::string> headers;
    std::string body;
};

struct HttpResponse
{
    int status;
    std::unordered_map<std::string, std::string> headers;
    std::string body;
};

struct WebSocketFrame
{
    bool fin;
    bool masked;
    uint8_t opcode;
    std::string payload;
};

#endif