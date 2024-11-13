#ifndef __HANDLER_HPP__
#define __HANDLER_HPP__

#include <functional>
#include <utility>
#include "http/Method.hpp"
#include "http/Structures.hpp"
#include "http/Request.hpp"
#include "http/Client.hpp"

/**
     * @brief Defines the type for HTTP request handler functions.
     * @param request The HTTP request object.
     * @param response The HTTP response object to be generated.
     */
using HttpHandler = std::function<void(const HttpRequest&, HttpResponse&)>;

class HttpEndpoint {
private:
    std::string httpMethod;
    std::string path;

public:
    HttpEndpoint(std::string httpMethod, std::string path);

    ~HttpEndpoint() = default;

    std::string getHttpMethod() const;

    void setHttpMethod(const std::string &value);

    std::string getPath() const;

    void setPath(const std::string &value);

    bool operator==(const std::string& str);
};

#endif
