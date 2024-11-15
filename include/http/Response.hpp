#ifndef __HTTP_RESPONSE_HPP__
#define __HTTP_RESPONSE_HPP__

#include <string>
#include <unordered_map>

class HttpResponse
{
private:
    std::string version;
    int status;
    std::string statusMsg;
    std::unordered_map<std::string, std::string> headers;
    std::string body;
    std::string protocol;

public:
    HttpResponse() = default;
    
    static HttpResponse parse(const std::string &msg);

    void setVersion(const std::string &value);

    std::string getVersion() const;

    void setStatus(const int &status);
    
    int getStatus() const;

    void setStatusMsg(const std::string &statusMsg);
    
    std::string getStatusMsg() const;

    void setHeaders(const std::unordered_map<std::string, std::string> header);

    void addHeader(const std::string &key, const std::string &value);
    
    std::unordered_map<std::string, std::string> getHeaders() const;
    
    void setBody(const std::string &value);
    
    std::string getBody() const;
    
    void setProtocol(const std::string &value);
    
    std::string getProtocol() const;
};

#endif