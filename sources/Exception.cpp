#include "Exception.hpp"
#include <algorithm>

std::string netpp::Exception::getMessage() const
{
    return message;
}

int netpp::Exception::getErrCode() const
{
    return errCode;
}

netpp::ERR_CODE netpp::ErrorCode::getErrCode(const std::string &errStr)
{
    std::string ERROR = std::string(errStr.size(), '\0');
    std::transform(errStr.begin(), errStr.end(), ERROR.begin(), ::toupper);
    if (ERROR == netpp::ErrorCode::SOCKET_CREATION) return netpp::ERR_CODE::SOCKET_CREATION;
    else if (ERROR == netpp::ErrorCode::SOCKET_BINDING) return netpp::ERR_CODE::SOCKET_BINDING;
    else if (ERROR == netpp::ErrorCode::SOCKET_LISTENING) return netpp::ERR_CODE::SOCKET_LISTENING;
    else if (ERROR == netpp::ErrorCode::SOCKET_CONNECTION) return netpp::ERR_CODE::SOCKET_CONNECTION;
    else if (ERROR == netpp::ErrorCode::SOCKET_ACCEPT) return netpp::ERR_CODE::SOCKET_ACCEPT;
    else if (ERROR == netpp::ErrorCode::SOCKET_SEND) return netpp::ERR_CODE::SOCKET_SEND;
    else if (ERROR == netpp::ErrorCode::URL_MALFORMED) return netpp::ERR_CODE::URL_MALFORMED;
    else if (ERROR == netpp::ErrorCode::NO_HOST) return netpp::ERR_CODE::NO_HOST;
    else if (ERROR == netpp::ErrorCode::SSL_CTX_CREATE) return netpp::ERR_CODE::SSL_CTX_CREATE;
    else if (ERROR == netpp::ErrorCode::SSL_CTX_OBJECT) return netpp::ERR_CODE::SSL_CTX_OBJECT;
    else if (ERROR == netpp::ErrorCode::SSL_CTX_CONNECT) return netpp::ERR_CODE::SSL_CTX_CONNECT;
    else return ERR_CODE();
}

std::string netpp::ErrorCode::getValue(const ERR_CODE &errCode)
{
    switch (errCode)
    {
    case netpp::ERR_CODE::SOCKET_CREATION:
        return netpp::ErrorCode::SOCKET_CREATION;
    case netpp::ERR_CODE::SOCKET_BINDING:
        return netpp::ErrorCode::SOCKET_BINDING;
    case netpp::ERR_CODE::SOCKET_LISTENING:
        return netpp::ErrorCode::SOCKET_LISTENING;
    case netpp::ERR_CODE::SOCKET_CONNECTION:
        return netpp::ErrorCode::SOCKET_CONNECTION;
    case netpp::ERR_CODE::SOCKET_ACCEPT:
        return netpp::ErrorCode::SOCKET_ACCEPT;
    case netpp::ERR_CODE::SOCKET_SEND:
        return netpp::ErrorCode::SOCKET_SEND;
    case netpp::ERR_CODE::URL_MALFORMED:
        return netpp::ErrorCode::URL_MALFORMED;    
    case netpp::ERR_CODE::NO_HOST:
        return netpp::ErrorCode::NO_HOST;
    case netpp::ERR_CODE::SSL_CTX_CREATE:
        return netpp::ErrorCode::SSL_CTX_CREATE;
    case netpp::ERR_CODE::SSL_CTX_OBJECT:
        return netpp::ErrorCode::SSL_CTX_OBJECT;
    case netpp::ERR_CODE::SSL_CTX_CONNECT:
        return netpp::ErrorCode::SSL_CTX_CONNECT;
    default:
        return std::string();
    }
    
}
