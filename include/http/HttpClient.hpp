#ifndef HTTPCLIENT_HPP
#define HTTPCLIENT_HPP

#include <string>
#include <map>

class HttpClient {
public:
    HttpClient();
    ~HttpClient();

    std::string get(const std::string& url, const std::map<std::string, std::string>& headers = {});
    std::string post(const std::string& url, const std::string& data, const std::map<std::string, std::string>& headers = {});

private:
    std::string httpRequest(const std::string& url, const std::string& method, const std::string& data, const std::map<std::string, std::string>& headers);
    std::string parseUrl(const std::string& url, std::string& host, std::string& path, int& port, bool& isHttps);
    int createSocket(const std::string& host, int port);
    int createSSLSocket(const std::string& host, int port, SSL_CTX*& ctx, SSL*& ssl);
};

#endif // HTTPCLIENT_HPP
