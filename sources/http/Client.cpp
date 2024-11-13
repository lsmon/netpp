#include "http/Client.hpp"
#include <iostream>
#include <sstream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

HttpClient::HttpClient()
{
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();
}

HttpClient::~HttpClient()
{
    EVP_cleanup();
}

HttpResponse HttpClient::httpRequest(const std::string &url, const std::string &method, const std::string &data, const std::map<std::string, std::string> &headers)
{
    std::string host, path;
    int port;
    bool isHttps;
    parseUrl(url, host, path, port, isHttps);

    int sock;
#if OPENSSL_ENABLED
    SSL_CTX *ctx = nullptr;
    SSL *ssl = nullptr;

    if (isHttps)
    {
        sock = createSSLSocket(host, port, ctx, ssl);
    }
    else
    {
        sock = createSocket(host, port);
    }
#else
    sock = createSocket(host, port);
#endif    

    if (sock < 0)
    {
        throw std::runtime_error("Failed to create socket");
    }

    std::stringstream request;

    switch (HttpMethod::getMethodCode(method))
    {
    case GET:
        request << HttpMethod::GET << " " << path << " HTTP/1.1";
        break;
    case POST:
        request << HttpMethod::POST << " " << path << " HTTP/1.1";
        break;
    case PUT:
        request << HttpMethod::PUT << " " << path << " HTTP/1.1";
        break;
    case DELETE:
        request << HttpMethod::DELETE << " " << path << " HTTP/1.1";
        break;
    case HEAD:
        request << HttpMethod::HEAD << " " << path << " HTTP/1.1";
        break;
    case OPTIONS:
        request << HttpMethod::OPTIONS << " " << path << " HTTP/1.1";
        break;
    case CONNECT:
        request << HttpMethod::CONNECT << " " << path << " HTTP/1.1";
        break;
    case PATCH:
        request << HttpMethod::PATCH << " " << path << " HTTP/1.1";
        break;
    case TRACE:
        request << HttpMethod::TRACE << " " << path << " HTTP/1.1";
        break;
    default:
        throw std::runtime_error("Invalid HTTP method");
        break;
    }
    request << "\r\nHost: " << host;

    for (const auto &header : headers)
    {
        request << "\r\n" << header.first << ": " << header.second;
    }

    if (!data.empty())
    {
        request << "\r\nContent-Length: " << data.size();
        request  << "\r\n\r\n";
        request  << data;
    }
    else
    {
        request  << "\r\n\r\n";
    }
    std::string requestStr = request.str();
    request.clear();
    request.flush();

    if (isHttps)
    {
        SSL_write(ssl, requestStr.c_str(), requestStr.length());
    }
    else
    {
        send(sock, requestStr.c_str(), requestStr.length(), 0);
    }

    char buffer[4096];
    std::stringstream response;

    int bytesRead;
    while ((bytesRead = (isHttps ? SSL_read(ssl, buffer, sizeof(buffer)) : recv(sock, buffer, sizeof(buffer), 0))) > 0)
    {
        response.write(buffer, bytesRead);
    }

    close(sock);
    if (isHttps)
    {
        SSL_free(ssl);
        SSL_CTX_free(ctx);
    }
    std::string responseStr = response.str();
    response.clear();
    response.flush();
    return HttpResponse::parse(responseStr);
}

HttpResponse HttpClient::get(const std::string &url, const std::map<std::string, std::string> &headers)
{
    return httpRequest(url, "GET", "", headers);
}

HttpResponse HttpClient::post(const std::string &url, const std::string &data, const std::map<std::string, std::string> &headers)
{
    return httpRequest(url, "POST", data, headers);
}

void HttpClient::parseUrl(const std::string &url, std::string &host, std::string &path, int &port, bool &isHttps)
{
    isHttps = false;
    port = 80;
    if (url.substr(0, 8) == "https://")
    {
        isHttps = true;
        port = 443;
        host = url.substr(8);
    }
    else if (url.substr(0, 7) == "http://")
    {
        host = url.substr(7);
    }
    else
    {
        throw std::runtime_error("Invalid URL format");
    }

    size_t slashPos = host.find('/');
    if (slashPos != std::string::npos)
    {
        path = host.substr(slashPos);
        host = host.substr(0, slashPos);
    }
    else
    {
        path = "/";
    }

    size_t colonPos = host.find(':');
    if (colonPos != std::string::npos)
    {
        port = std::stoi(host.substr(colonPos + 1));
        host = host.substr(0, colonPos);
    }
}

int HttpClient::createSocket(const std::string &host, int port)
{
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    std::string portStr = std::to_string(port);
    if (getaddrinfo(host.c_str(), portStr.c_str(), &hints, &res) != 0)
    {
        throw std::runtime_error("Failed to resolve host");
    }

    int sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock < 0)
    {
        freeaddrinfo(res);
        throw std::runtime_error("Failed to create socket");
    }

    if (connect(sock, res->ai_addr, res->ai_addrlen) < 0)
    {
        close(sock);
        freeaddrinfo(res);
        throw std::runtime_error("Failed to connect to host");
    }

    freeaddrinfo(res);
    return sock;
}

int HttpClient::createSSLSocket(const std::string &host, int port, SSL_CTX *&ctx, SSL *&ssl)
{
    ctx = SSL_CTX_new(SSLv23_client_method());
    if (!ctx)
    {
        throw std::runtime_error("Failed to create SSL context");
    }

    ssl = SSL_new(ctx);
    if (!ssl)
    {
        SSL_CTX_free(ctx);
        throw std::runtime_error("Failed to create SSL object");
    }

    int sock = createSocket(host, port);

    SSL_set_fd(ssl, sock);
    if (SSL_connect(ssl) != 1)
    {
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        throw std::runtime_error("Failed to connect via SSL");
    }

    return sock;
}
