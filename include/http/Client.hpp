#ifndef __HTTP_CLIENT_HPP__
#define __HTTP_CLIENT_HPP__

#include <string>
#include <map>
#include "config.h"
#if OPENSSL_ENABLED
#include <openssl/ssl.h>
#include <openssl/err.h>
#endif
#include "http/Method.hpp"
#include "http/Response.hpp"

class HttpClient {
public:
    HttpClient();
    ~HttpClient();

    HttpResponse get(const std::string& url, const std::map<std::string, std::string>& headers = {});
    HttpResponse post(const std::string& url, const std::string& data, const std::map<std::string, std::string>& headers = {});
    HttpResponse put(const std::string& url, const std::string& data, const std::map<std::string, std::string>& headers = {});
    HttpResponse deleteMethod(const std::string& url, const std::map<std::string, std::string>& headers = {});
    HttpResponse head(const std::string& url, const std::map<std::string, std::string>& headers = {});
    HttpResponse Options(const std::string& url, const std::map<std::string, std::string>& headers = {});
    HttpResponse Connect(const std::string& url, const std::string& data, const std::map<std::string, std::string>& headers = {});
    HttpResponse Patch(const std::string& url, const std::string& data, const std::map<std::string, std::string>& headers = {});
    HttpResponse Trace(const std::string& url, const std::map<std::string, std::string>& headers = {});
private:
    HttpResponse httpRequest(const std::string& url, const std::string& method, const std::string& data, const std::map<std::string, std::string>& headers);
    void parseUrl(const std::string& url, std::string& host, std::string& path, int& port, bool& isHttps);
    int createSocket(const std::string& host, int port);
    int createSSLSocket(const std::string& host, int port, SSL_CTX*& ctx, SSL*& ssl);
};

#endif // HTTPCLIENT_HPP
