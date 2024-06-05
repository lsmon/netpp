#include "http/HttpMethod.hpp"
#include <algorithm>
#include <cctype>

HTTP_METHOD_CODE HttpMethod::getMethodCode(const std::string & method)
{
    std::string METHOD = std::string(method.size(), '\0');
    std::transform(method.begin(), method.end(), METHOD.begin(), ::toupper);
    if (METHOD == GET) return HTTP_METHOD_CODE::GET;
    else if (METHOD == POST) return HTTP_METHOD_CODE::POST;
    else if (METHOD == PUT) return HTTP_METHOD_CODE::PUT;
    else if (METHOD == DELETE) return HTTP_METHOD_CODE::DELETE;
    else if (METHOD == HEAD) return HTTP_METHOD_CODE::HEAD;
    else if (METHOD == OPTIONS) return HTTP_METHOD_CODE::OPTIONS;
    else if (METHOD == CONNECT) return HTTP_METHOD_CODE::CONNECT;
    else if (METHOD == PATCH) return HTTP_METHOD_CODE::PATCH;
    else return HTTP_METHOD_CODE::TRACE;
}