#ifndef NETPP_EXCEPTION_HPP
#define NETPP_EXCEPTION_HPP

#include <stdexcept>
#include <string>

namespace netpp 
{
    enum ERR_CODE 
    {
        SOCKET_CREATION = 4000,
        SOCKET_BINDING = 4001,
        SOCKET_LISTENING = 4002,
        SOCKET_CONNECTION = 4003,
        URL_MALFORMED = 4004,
        NO_HOST = 4005,
        SSL_CTX_CREATE = 4006,
        SSL_CTX_OBJECT = 4007,
        SSL_CTX_CONNECT = 4008
    };

    class ErrorCode 
    {
    public:
        inline static std::string SOCKET_CREATION = "CREATE SOCKET ERROR";
        inline static std::string SOCKET_BINDING = "SOCKET_BINDING";
        inline static std::string SOCKET_LISTENING = "SOCKET_LISTENING";
        inline static std::string SOCKET_CONNECTION = "SOCKET_LISTENING";
        inline static std::string URL_MALFORMED = "URL_MALFORMED";
        inline static std::string NO_HOST = "NO_HOST";
        inline static std::string SSL_CTX_CREATE = "SSL_CTX_CREATE";
        inline static std::string SSL_CTX_OBJECT = "SSL_CTX_OBJECT";
        inline static std::string SSL_CTX_CONNECT = "SSL_CTX_CONNECT";
        
        static ERR_CODE getErrCode(const std::string &errStr);

        static std::string getValue(const ERR_CODE &errCode);
    };

    class Exception : public std::runtime_error 
    {
    private:
        std::string message;
        int errCode;
        
    public:
        explicit Exception(const std::string& message) : std::runtime_error(message), message(message), errCode(-1) {}

        explicit Exception(const std::string& message, const int errCode) : std::runtime_error("Error Code: " + std::to_string(errCode) +"\n" + message), message(message), errCode(errCode) {}

        std::string getMessage() const;

        int getErrCode() const;
    };

} // namespace netpp

#endif // NETPP_EXCEPTION_HPP