#ifndef NETPP_SERVER_HPP
#define NETPP_SERVER_HPP

#include "http/Structures.hpp"
#include "http/HttpHandler.hpp"
#include "http/HttpMethod.hpp"
#include <mutex>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <optional>
#include <queue>
#include <iostream>
#include <memory>
#include <map>

class Server
{
private:
   /**
     * @brief Defines the type for WebSocket message handler functions.
     * @param frame The WebSocket frame received.
     * @param response The WebSocket frame to be sent in response.
     */
    using WebSocketHandler = std::function<void(const WebSocketFrame &, WebSocketFrame &)>;

    std::string host;
    std::string port;
    size_t maxConnections;
    size_t numThreads;

    std::map<HttpEndpoint*, HttpHandler> httpHandlers;
    std::optional<WebSocketHandler> webSocketHandler;
    std::mutex connectionsMutex;
    size_t currentConnections = 0;

    /**
     * @brief Runs a worker thread to handle incoming connections.
     * @param serverFd The server socket file descriptor.
     */
    void runWorker(int serverFd);

    /**
     * @brief Handles an incoming connection.
     * @param clientFd The client socket file descriptor.
     */
    void handleConnection(int clientFd);

    /**
     * @brief Reads an HTTP request from a regular socket connection.
     * @param clientFd The client socket file descriptor.
     * @return An optional containing the HTTP request, or std::nullopt if failed to read.
     */
    static std::optional<HttpRequest> readHttpRequest(int clientFd);

    /**
     * @brief Handles an HTTP request and generates a response.
     * @param request The HTTP request.
     * @param response The HTTP response to be generated.
     */
    void handleHttpRequest(const HttpRequest &request, HttpResponse &response);


    /**
     * @brief Sends an HTTP response over a regular socket connection.
     * @param clientFd The client socket file descriptor.
     * @param response The HTTP response to send.
     */
    void sendHttpResponse(int clientFd, const HttpResponse &response);

    /**
     * @brief Handles a WebSocket connection and message exchange over a regular socket connection.
     * @param clientFd The client socket file descriptor.
     */
    void handleWebSocketConnection(int clientFd);

    /**
     * @brief Reads a WebSocket frame from a regular socket connection.
     * @param clientFd The client socket file descriptor.
     * @param frame Reference to store the read WebSocket frame.
     * @return True if successfully read the frame, otherwise false.
     */
    static bool readWebSocketFrame(int clientFd, WebSocketFrame &frame);

    /**
     * @brief Sends a WebSocket frame over a regular socket connection.
     * @param clientFd The client socket file descriptor.
     * @param frame The WebSocket frame to send.
     */
    static void sendWebSocketFrame(int clientFd, const WebSocketFrame &frame);

    /**
     * @brief Attempts to acquire a connection slot.
     * @return True if a connection slot is acquired, otherwise false.
     */
    bool tryAcquireConnection();

    /**
     * @brief Releases a connection slot.
     */
    void releaseConnection();

    /**
     * @brief Generates an HTTP response string.
     * @param response The HTTP response object.
     * @return The generated HTTP response string.
     */
    static std::string generateHttpResponse(const HttpResponse& response);

    /**
     * @brief Retrieves the HTTP status message for a given status code.
     * @param statusCode The HTTP status code.
     * @return The corresponding status message.
     */
    static std::string getStatusMessage(int statusCode);

public:
    /**
     * @brief Constructs a non-SSL server.
     * @param host The host address to bind to.
     * @param port The port number to listen on.
     * @param maxConnections Maximum number of simultaneous connections.
     * @param numThreads Number of worker threads.
     */
    Server(std::string host, std::string port, size_t maxConnections, size_t numThreads);

    /**
     * @brief Destroys the ssl ctx instance (if exists) when the Server is destroyed.
     */
    ~Server() = default;

    /**
     * @brief Starts the server.
     */
    void run();

    /**
     * @brief Sets the HTTP request handler for a specific route.
     * @param method The HTTP method (GET, POST, etc.).
     * @param path The URL path.
     * @param handler The handler function.
     */
    void setHttpHandler(const std::string &method, const std::string &path, HttpHandler handler);

    /**
     * @brief Sets the WebSocket message handler.
     * @param handler The handler function.
     */
    void setWebSocketHandler(WebSocketHandler handler);
};

#endif