#include "http/HttpResponse.hpp"
#include "util/String.hpp"
#include <iostream>
#include <istream>
#include <string>

HttpResponse HttpResponse::parse(const std::string &msg)
{
    /* HttpResponse response;
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
        else if (i > 0 && line != "\r\n" && flags[1])
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
            if (lines[i+1] == "\n")
                flags[1] = false;
        }
        else if (line != "\n" && flags[2])
        { // Parse body
            line = lines[++i];
            response.setBody(response.getBody() + line);
            if (lines[i+1] == "\n")
                flags[2] = false;
        }
    }
    return response; */
    HttpResponse response;
    std::string str(msg);
    // const std::string& line ;
    bool bodyFlags = true;

    // Parse protocol and status
    size_t pos = str.find("\r\n");
    if (pos != std::string::npos)
    {
        std::string protocol_status = str.substr(0, pos);
        size_t space_pos = protocol_status.find(' ');
        response.setProtocol(protocol_status.substr(0, space_pos));
        response.setStatus(std::stoi(protocol_status.substr(space_pos + 1)));
    }

    // Parse headers
    pos = str.find("\r\n\r\n");
    if (pos != std::string::npos)
    {
        std::string headers = str.substr(pos + 4); // +4 to skip "\r\n\r\n"
        size_t header_end = headers.find("\r\n\r\n");
        if (header_end != std::string::npos)
        {
            headers = headers.substr(0, header_end);
            std::unordered_map<std::string, std::string> header_map;
            size_t start = 0;
            while (true)
            {
                size_t end = headers.find("\r\n", start);
                if (end == std::string::npos)
                    break;
                std::string header = headers.substr(start, end - start);
                size_t colon_pos = header.find(':');
                if (colon_pos != std::string::npos)
                {
                    std::string key = header.substr(0, colon_pos);
                    std::string value = header.substr(colon_pos + 1);
                    header_map[key] = value;
                }
                start = end + 2; // +2 to skip "\r\n"
            }
            response.setHeaders(header_map);
        }
    }

    // Parse body
    if (bodyFlags)
    {
        size_t body_start = str.find("\r\n\r\n");
        if (body_start != std::string::npos)
        {
            response.setBody(str.substr(body_start + 4)); // +4 to skip "\r\n\r\n"
            bodyFlags = false;
        }
    }

    return response;
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
