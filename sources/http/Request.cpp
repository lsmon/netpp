#include "http/Request.hpp"

void HttpRequest::setVersion(const std::string &value)
{
    version = value;
}

std::string HttpRequest::getVersion() const
{
    return version;
}

void HttpRequest::setHeaders(const std::unordered_map<std::string, std::string> value)
{
    headers = value;
}

void HttpRequest::addHeader(const std::string &key, const std::string &value)
{
    headers[key] = value;
}

std::unordered_map<std::string, std::string> HttpRequest::getHeaders() const
{
    return headers;
}

void HttpRequest::setMethod(const std::string value)
{
    method = value;
}

std::string HttpRequest::getMethod() const
{
    return method;
}

void HttpRequest::setPath(const std::string &value)
{
    path = value;
}

std::string HttpRequest::getPath() const
{
    return path;
}

void HttpRequest::setQueryString(const std::unordered_map<std::string, std::string> value)
{
    queryString = value;
}

std::unordered_map<std::string, std::string> HttpRequest::getQueryString() const
{
    return queryString;
}

void HttpRequest::setFragment(const std::string &value)
{
    fragment = value;
}

std::string HttpRequest::getFragment() const
{
    return fragment;
}

void HttpRequest::setBody(const std::string &value)
{
    body = value;
}

std::string HttpRequest::getBody() const
{
    return body;
}
