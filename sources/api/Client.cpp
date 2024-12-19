#include <iostream>
#include <sstream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include "api/Client.hpp"
#include "Exception.hpp"
#include "netppconfig.h"

ApiClient::ApiClient() {
#ifdef OPENSSL_ENABLED
    initializeSSL();
#endif
}

ApiClient::~ApiClient() {
#ifdef OPENSSL_ENABLED
    cleanupSSL();
#endif
}

#ifdef OPENSSL_ENABLED
void ApiClient::initializeSSL() {
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();
}

void ApiClient::cleanupSSL() {
    EVP_cleanup();
}
#endif

void ApiClient::parseUrl(const std::string& url, std::string& host, std::string& path, int& port, bool& isHttps) {
    isHttps = false;
    port = 80;

    if (url.substr(0, 8) == "https://") {
        isHttps = true;
        port = 443;
        host = url.substr(8);
    } else if (url.substr(0, 7) == "http://") {
        host = url.substr(7);
    } else {
        throw std::runtime_error("Invalid URL format");
    }

    size_t slashPos = host.find('/');
    if (slashPos != std::string::npos) {
        path = host.substr(slashPos);
        host = host.substr(0, slashPos);
    } else {
        path = "/";
    }

    size_t colonPos = host.find(':');
    if (colonPos != std::string::npos) {
        port = std::stoi(host.substr(colonPos + 1));
        host = host.substr(0, colonPos);
    }
}

int ApiClient::createSocket(const std::string& host, int port) {
    struct addrinfo hints{}, *res;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    std::string portStr = std::to_string(port);
    if (getaddrinfo(host.c_str(), portStr.c_str(), &hints, &res) != 0) {
        throw std::runtime_error("Failed to resolve host");
    }

    int sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock < 0) {
        freeaddrinfo(res);
        throw std::runtime_error("Failed to create socket");
    }

    if (connect(sock, res->ai_addr, res->ai_addrlen) < 0) {
        close(sock);
        freeaddrinfo(res);
        throw std::runtime_error("Failed to connect to host");
    }

    freeaddrinfo(res);
    return sock;
}

SSL* ApiClient::createSSLSocket(int sock, SSL_CTX*& ctx) {
    ctx = SSL_CTX_new(SSLv23_client_method());
    if (!ctx) {
        throw std::runtime_error("Failed to create SSL context");
    }

    SSL* ssl = SSL_new(ctx);
    if (!ssl) {
        SSL_CTX_free(ctx);
        throw std::runtime_error("Failed to create SSL object");
    }

    SSL_set_fd(ssl, sock);
    if (SSL_connect(ssl) != 1) {
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        throw std::runtime_error("Failed to establish SSL connection");
    }

    return ssl;
}

std::string ApiClient::sendRequest(int sock, const std::string& request, bool isHttps, SSL* ssl) {
    if (isHttps) {
        SSL_write(ssl, request.c_str(), request.size());
    } else {
        send(sock, request.c_str(), request.size(), 0);
    }

    std::string bytesInterpreted;
    char buffer[4096];

    int bytesRead;
    while ((bytesRead = (isHttps ? SSL_read(ssl, buffer, sizeof(buffer)) : recv(sock, buffer, sizeof(buffer), 0))) > 0) {
        bytesInterpreted.append(buffer, bytesRead);
    }

    return bytesInterpreted;
}

HttpResponse ApiClient::request(const std::string &url, const HTTP_METHOD &method, const std::string &data,
                                const std::unordered_map<std::string, std::string> &headers) {
    // Parse the URL into host, path, port, and isHttps
    std::string host, path;
    int port;
    bool isHttps;

    parseUrl(url, host, path, port, isHttps);

    // Create the socket and establish connection
    int sock = createSocket(host, port);
    if (sock < 0) {
        throw netpp::Exception("Failed to create socket", netpp::ERR_CODE::SOCKET_CREATION);
    }

    // SSL setup if HTTPS is required
    SSL_CTX* ctx = nullptr;
    SSL* ssl = nullptr;

    if (isHttps) {
        ssl = createSSLSocket(sock, ctx);
    }

    // Build the HTTP request
    std::ostringstream request;
    request << HttpMethod::getValue(method) << " " << path << " HTTP/1.1\r\n";
    request << "Host: " << host << "\r\n";

    // Add headers
    for (const auto& header : headers) {
        request << header.first << ": " << header.second << "\r\n";
    }

    // If the method supports a body, include Content-Length and the data payload
    if (method == HTTP_METHOD::GET || method == HTTP_METHOD::POST || method == HTTP_METHOD::PUT || method == HTTP_METHOD::PATCH) {
        request << "Content-Length: " << data.size() << "\r\n";
    }

    // Close the connection by default
    request << "Connection: close\r\n\r\n";

    // Append the data payload if needed
    if (!data.empty() && (method == HTTP_METHOD::GET ||method == HTTP_METHOD::POST || method == HTTP_METHOD::PUT || method == HTTP_METHOD::PATCH)) {
        request << data;
    }

    // Send the request and get the response
    response = sendRequest(sock, request.str(), isHttps, ssl);

    // Clean up SSL resources if used
    if (isHttps) {
        SSL_free(ssl);
        SSL_CTX_free(ctx);
    }

    // Close the socket
    close(sock);

    // Parse and return the HttpResponse
    return HttpResponse::parse(response);
}

HttpResponse
ApiClient::get(const std::string &url, const std::string &data, const std::unordered_map<std::string, std::string> &headers) {
    return request(url, HTTP_METHOD::GET, data, headers);
}

HttpResponse
ApiClient::post(const std::string& url, const std::string& data, const std::unordered_map<std::string, std::string>& headers) {
    return request(url, HTTP_METHOD::POST, data, headers);
}

HttpResponse
ApiClient::put(const std::string &url, const std::string &data, const std::unordered_map<std::string, std::string> &headers) {
    return request(url, HTTP_METHOD::PUT, data, headers);
}

HttpResponse ApiClient::deleteReq(const std::string &url, const std::string& data,  const std::unordered_map<std::string, std::string> &headers) {
    return request(url, HTTP_METHOD::DELETE, data, headers);
}

HttpResponse
ApiClient::head(const std::string &url, const std::string &data, const std::unordered_map<std::string, std::string> &headers) {
    return request(url, HTTP_METHOD::HEAD, data, headers);
}

HttpResponse
ApiClient::options(const std::string &url, const std::string &data, const std::unordered_map<std::string, std::string> &headers) {
    return request(url, HTTP_METHOD::OPTIONS, data, headers);
}

HttpResponse ApiClient::connectReq(const std::string &url, const std::unordered_map<std::string, std::string> &headers) {
    return request(url, HTTP_METHOD::CONNECT, "", headers);
}

HttpResponse
ApiClient::patch(const std::string &url, const std::string &data, const std::unordered_map<std::string, std::string> &headers) {
    return request(url, HTTP_METHOD::PATCH, data, headers);
}

HttpResponse
ApiClient::trace(const std::string &url, const std::string &data, const std::unordered_map<std::string, std::string> &headers) {
    return request(url, HTTP_METHOD::TRACE, data, headers);
}
