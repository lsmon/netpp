#include "http/HttpResponse.hpp"
#include "util/String.hpp"
#include <iostream>
#include <istream>
#include <string>
#include <locale>
#include <string>

HttpResponse HttpResponse::parse(const std::string &msg)
{
    HttpResponse response;
    std::string str(msg);
    std::string line;
    bool flags[] = {true, true, true};
    // Parse protocol and status
    std::regex rn("\r");
    std::vector<std::string> lines = String::tokenize(str, rn);

    for (size_t i = 0; i < lines.size(); i++)
    {
        line = lines[i];
        if (i == 0)
        {
            std::regex rx(" ");
            std::vector<std::string> protocol_status = String::tokenize(line, rx);
            response.setProtocol(protocol_status[0]);
            response.setStatus(std::stoi(protocol_status[1]));
            response.setStatusMsg(protocol_status[2]);
            flags[0] = false;
        }
        else if (line != "\r\n" && flags[1])
        {
            // Parse headers
            // while (line != "\r\n")
            {
                std::regex colonRx(":");
                std::vector<std::string> header = String::tokenize(line, colonRx);
                if (header.size() > 1)
                {
                    std::string key = String::trim(header[0]);
                    std::string value = String::trim(header[1]);
                    response.addHeader(key, value);
                }
            }
            if (lines[i + 1] == "\n")
                flags[1] = false;
        }
        else if (line != "\n" && flags[2])
        {
            
            if (!String::isUnicodeEscapeSequence(line))
            {
                response.setBody(response.getBody() + String::trim(line));
            }
            if (lines[i + 1] == "\n")
                flags[2] = false;
        }
    }
    return response;
}

void HttpResponse::setVersion(const std::string &value)
{
    version = value;
}

std::string HttpResponse::getVersion() const
{
    return version;
}

void HttpResponse::setStatus(const int &status)
{
    this->status = status;
}

int HttpResponse::getStatus() const
{
    return status;
}

void HttpResponse::setStatusMsg(const std::string &statusMsg)
{
    this->statusMsg = statusMsg;
}

std::string HttpResponse::getStatusMsg() const
{
    return statusMsg;
}

void HttpResponse::setHeaders(const std::unordered_map<std::string, std::string> header)
{
    headers = header;
}

void HttpResponse::addHeader(const std::string &key, const std::string &value)
{
    headers[key] = value;
}

std::unordered_map<std::string, std::string> HttpResponse::getHeaders() const
{
    return headers;
}

void HttpResponse::setBody(const std::string &value)
{
    body = value;
}

void HttpResponse::setProtocol(const std::string &value)
{
    protocol = value;
}

std::string HttpResponse::getBody() const
{
    return body;
}

std::string HttpResponse::getProtocol() const
{
    return protocol;
}
