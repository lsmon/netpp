#ifndef __HTTP_REQUEST__
#define __HTTP_REQUEST__

#include "http/QueryString.hpp"

class HttpRequest
{
private:
    std::string version;
    std::unordered_map<std::string, std::string> headers;
    std::string method;
    std::string path;
    std::unordered_map<std::string, std::string> queryString;
    std::string fragment;
    std::string body;

public:
    void setVersion(const std::string& version);

    std::string getVersion() const;

    void setHeaders(const std::unordered_map<std::string, std::string> value);

    void addHeader(const std::string &key, const std::string &value);

    std::unordered_map<std::string, std::string> getHeaders() const;

    void setMethod(const std::string value);

    std::string getMethod() const;

    void setPath(const std::string &value);

    std::string getPath() const;

    void setQueryString(const std::unordered_map<std::string, std::string> value);

    std::unordered_map<std::string, std::string> getQueryString() const;

    void setFragment(const std::string &value);

    std::string getFragment() const;

    void setBody(const std::string &body);

    std::string getBody() const;
};

#endif