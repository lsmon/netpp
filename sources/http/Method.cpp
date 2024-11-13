#include "http/Method.hpp"
#include <algorithm>
#include <cctype>

HTTP_METHOD HttpMethod::getMethodCode(const std::string &method)
{
    std::string METHOD = std::string(method.size(), '\0');
    std::transform(method.begin(), method.end(), METHOD.begin(), ::toupper);
    if (METHOD == GET)
        return HTTP_METHOD::GET;
    else if (METHOD == POST)
        return HTTP_METHOD::POST;
    else if (METHOD == PUT)
        return HTTP_METHOD::PUT;
    else if (METHOD == DELETE)
        return HTTP_METHOD::DELETE;
    else if (METHOD == HEAD)
        return HTTP_METHOD::HEAD;
    else if (METHOD == OPTIONS)
        return HTTP_METHOD::OPTIONS;
    else if (METHOD == CONNECT)
        return HTTP_METHOD::CONNECT;
    else if (METHOD == PATCH)
        return HTTP_METHOD::PATCH;
    else
        return HTTP_METHOD::TRACE;
}
std::string HttpMethod::getValue(const HTTP_METHOD &method)
{
    switch (method)
    {
    case HTTP_METHOD::GET:
        return GET;
    case HTTP_METHOD::POST:
        return POST;
    case HTTP_METHOD::PUT:
        return PUT;
    case HTTP_METHOD::DELETE:
        return DELETE;
    case HTTP_METHOD::HEAD:
        return HEAD;
    case HTTP_METHOD::OPTIONS:
        return OPTIONS;
    case HTTP_METHOD::CONNECT:
        return CONNECT;
    case HTTP_METHOD::PATCH:
        return PATCH;
    default:
        return TRACE;
    }
}