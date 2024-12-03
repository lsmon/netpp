#ifndef __API_CLIENT_HPP__
#define __API_CLIENT_HPP__

#include <string>
#include <map>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "config.h"
#include "http/Response.hpp"
#include "http/Method.hpp"

class ApiClient {
public:
    ApiClient();
    ~ApiClient();

    // Synchronous GET
    HttpResponse get(const std::string& url, const std::map<std::string, std::string>& headers = {});

    // Synchronous GET
    HttpResponse get(const std::string& url, const std::string& data, const std::map<std::string, std::string>& headers = {});

    // Synchronous POST
    HttpResponse post(const std::string& url, const std::string& data, const std::map<std::string, std::string>& headers = {});

    HttpResponse put(const std::string& url, const std::string& data, const std::map<std::string, std::string>& headers = {});

    HttpResponse deleteReq(const std::string& url, const std::string& data = "", const std::map<std::string, std::string>& headers = {});

    HttpResponse head(const std::string& url, const std::string& data = "", const std::map<std::string, std::string>& headers = {});

    HttpResponse options(const std::string& url, const std::string& data = "", const std::map<std::string, std::string>& headers = {});

    HttpResponse connect(const std::string& url, const std::map<std::string, std::string>& headers = {});

    HttpResponse patch(const std::string& url, const std::string& data = "", const std::map<std::string, std::string>& headers = {});

    HttpResponse trace(const std::string& url, const std::string& data = "", const std::map<std::string, std::string>& headers = {});

private:
    static void parseUrl(const std::string& url, std::string& host, std::string& path, int& port, bool& isHttps);
    static int createSocket(const std::string& host, int port);
    static SSL* createSSLSocket(int sock, SSL_CTX*& ctx);
    HttpResponse request(const std::string& url, const HTTP_METHOD &httpMethod, const std::string &data, const std::map<std::string, std::string>& headers);

    static std::string sendRequest(int sock, const std::string& request, bool isHttps, SSL* ssl = nullptr);
#ifdef OPENSSL_ENABLED
    static void initializeSSL();
    static void cleanupSSL();
#endif
    std::string response;
};

#endif // __API_CLIENT_HPP__
