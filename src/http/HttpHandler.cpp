//
// Created by Salomon Lee on 4/26/24.
//
#include "HttpHandler.hpp"

HttpEndpoint::HttpEndpoint(std::string httpMethod, std::string path) : httpMethod(std::move(httpMethod)), path(std::move(path)) {}

std::string HttpEndpoint::getHttpMethod() const {
    return httpMethod;
}

void HttpEndpoint::setHttpMethod(const std::string &value) {
    httpMethod = value;
}

std::string HttpEndpoint::getPath() const {
    return path;
}

void HttpEndpoint::setPath(const std::string &value) {
    path = value;
}

bool HttpEndpoint::operator==(const std::string& str) {
    std::string key = httpMethod + " " + path;
    return str == key;
}