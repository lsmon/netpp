#ifndef HTTPCLIENT_HPP
#define HTTPCLIENT_HPP

#include <string>
#include <map>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "http/HttpMethod.hpp"

class HttpClient {
public:
    HttpClient();
    ~HttpClient();

    std::string Get(const std::string& url, const std::map<std::string, std::string>& headers = {});
    std::string Post(const std::string& url, const std::string& data, const std::map<std::string, std::string>& headers = {});
    std::string Put(const std::string& url, const std::string& data, const std::map<std::string, std::string>& headers = {});
    std::string Delete(const std::string& url, const std::map<std::string, std::string>& headers = {});
    std::string Head(const std::string& url, const std::map<std::string, std::string>& headers = {});
    std::string Options(const std::string& url, const std::map<std::string, std::string>& headers = {});
    std::string Connect(const std::string& url, const std::string& data, const std::map<std::string, std::string>& headers = {});
    std::string Patch(const std::string& url, const std::string& data, const std::map<std::string, std::string>& headers = {});
    std::string Trace(const std::string& url, const std::map<std::string, std::string>& headers = {});
private:
    std::string httpRequest(const std::string& url, const std::string& method, const std::string& data, const std::map<std::string, std::string>& headers);
    void parseUrl(const std::string& url, std::string& host, std::string& path, int& port, bool& isHttps);
    int createSocket(const std::string& host, int port);
    int createSSLSocket(const std::string& host, int port, SSL_CTX*& ctx, SSL*& ssl);
};

#endif // HTTPCLIENT_HPP
