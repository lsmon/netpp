#ifndef HTTP_HANDLER_HPP
#define HTTP_HANDLER_HPP

#include <functional>
#include <utility>
#include "HttpMethod.hpp"
#include "Structures.hpp"

/**
     * @brief Defines the type for HTTP request handler functions.
     * @param request The HTTP request object.
     * @param response The HTTP response object to be generated.
     */
using HttpHandler = std::function<void(const HttpRequest&, HttpResponse&)>;

/**
 * @brief Represents an HTTP endpoint with an associated HTTP method, path, and session ID.
 */
class HttpEndpoint {
private:
    std::string httpMethod; /**< The HTTP method associated with the endpoint. */
    std::string path; /**< The path associated with the endpoint. */
    std::string sessionId; /**< The session ID associated with the endpoint. */

public:
    /**
     * @brief Constructs an HttpEndpoint object with the specified HTTP method and path.
     * @param httpMethod The HTTP method associated with the endpoint.
     * @param path The path associated with the endpoint.
     */
    HttpEndpoint(std::string httpMethod, std::string path);

    /**
     * @brief Constructs an HttpEndpoint object with the specified HTTP method, path, and session ID.
     * @param httpMethod The HTTP method associated with the endpoint.
     * @param path The path associated with the endpoint.
     * @param sessionId The session ID associated with the endpoint.
     */
    HttpEndpoint(std::string httpMethod, std::string path, std::string sessionId);

    /**
     * @brief Destructor for the HttpEndpoint object.
     */
    ~HttpEndpoint() = default;

    /**
     * @brief Gets the HTTP method associated with the endpoint.
     * @return The HTTP method.
     */
    std::string getHttpMethod() const;

    /**
     * @brief Sets the HTTP method associated with the endpoint.
     * @param value The HTTP method to set.
     */
    void setHttpMethod(const std::string &value);

    /**
     * @brief Gets the path associated with the endpoint.
     * @return The path.
     */
    std::string getPath() const;

    /**
     * @brief Sets the path associated with the endpoint.
     * @param value The path to set.
     */
    void setPath(const std::string &value);

    /**
     * @brief Gets the session ID associated with the endpoint.
     * @return The session ID.
     */
    std::string getSessionId() const;

    /**
     * @brief Sets the session ID associated with the endpoint.
     * @param value The session ID to set.
     */
    void setSessionId(const std::string &value);

    /**
     * @brief Equality comparison operator for HttpEndpoint objects.
     * @param str The string to compare with.
     * @return True if the HTTP method or path matches the given string, false otherwise.
     */
    bool operator==(const std::string& str);
};
#endif
