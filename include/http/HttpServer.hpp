#ifndef __SERVER_HPP__
#define __SERVER_HPP__

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
#include "HttpRequest.hpp"
// #include "Structures.hpp"
#include "HttpHandler.hpp"
#include "HttpMethod.hpp"
#include "WebSocketFrame.hpp"

enum WebSocketOpcode {
    ContinuationFrame = 0x0,
    TextFrame = 0x1,
    BinaryFrame = 0x2,
    CloseFrame = 0x8,
    PingFrame = 0x9,
    PongFrame = 0xA
};

// Define named constants for magic numbers
const int MAX_REQUEST_SIZE = 8192;
const int MAX_FRAME_SIZE = 8192;
const int MAX_7BIT_PAYLOAD_LENGTH = 125;
const int EXTENDED_PAYLOAD_16BIT = 126;
const int EXTENDED_PAYLOAD_64BIT = 127;

struct HttpEndpointComparator
{
    bool operator()(const std::shared_ptr<HttpEndpoint> lhs, const std::shared_ptr<HttpEndpoint> rhs) const
    {
        if (lhs->getHttpMethod() == rhs->getHttpMethod())
        {
            return lhs->getPath() < rhs->getPath();
        }
        return lhs->getHttpMethod() < rhs->getHttpMethod();
    }
};

// Implementing Resource Acquisition Is Initialization to manage the socket resource.
class Socket {
private:
    int fd;

public:
    Socket(int fd) : fd(fd) {}

    ~Socket();

    int get() const;
};

class HttpServer
{
private:
    std::mutex handlersMutex;
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
    bool working;

    // std::map<HttpEndpoint*, HttpHandler> httpHandlers;
    std::map<std::shared_ptr<HttpEndpoint>, HttpHandler, HttpEndpointComparator> httpHandlers;
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
     * @brief This function will handle binary frames, which contain arbitrary binary data.
     * 
     * @param clientFd 
     * @param frame 
     */
    void handleBinaryFrame(int clientFd, const WebSocketFrame& frame);

    /**
     * @brief This function will handle text frames, which typically contain UTF-8 encoded text data.
     * 
     * @param clientFd 
     * @param frame 
     */
    void handleTextFrame(int clientFd, const WebSocketFrame& frame);

    /**
     * @brief This function will handle the close frame, which indicates that the client or server wants to close the WebSocket connection.
     * 
     * @param clientFd 
     */
    void handleCloseFrame(int clientFd);

    /**
     * @brief This function will handle ping frames, which are used to check the connection's liveness. Typically, a pong frame is sent in response.
     * 
     * @param clientFd 
     */
    void handlePingFrame(int clientFd);

    /**
     * @brief This function will handle pong frames, which are sent in response to ping frames. The server might not need to do much here beyond acknowledging receipt.
     * 
     * @param clientFd 
     */
    void handlePongFrame(int clientFd);

    /**
     * @brief This function will handle any frames with opcodes that are not recognized. It’s a good idea to have a catch-all handler.
     * 
     * @param clientFd 
     * @param frame 
     */
    void handleUnknownFrame(int clientFd, const WebSocketFrame& frame);
    
    /**
     * @brief Converting strings to raw bytes before sending.
     * 
     * @param clientFd 
     * @param opcode 
     * @param payload 
     */
    void sendWebSocketFrame(int clientFd, WebSocketOpcode opcode, const std::string& payload);

    /**
     * @brief This helper function will send a WebSocket frame back to the client.
     * 
     * @param clientFd 
     * @param opcode 
     * @param payload 
     */
    void sendWebSocketFrame(int clientFd, WebSocketOpcode opcode, const std::vector<uint8_t>& payload);
    
    /**
     * @brief This helper function will serialize the WebSocketFrame structure into the raw bytes that can be sent over the network.
     * 
     * @param frame 
     * @return std::vector<uint8_t> 
     */
    static std::vector<uint8_t> serializeWebSocketFrame(const WebSocketFrame& frame);

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
    // static void sendWebSocketFrame(int clientFd, const WebSocketFrame &frame);

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
    static std::string generateHttpResponse(const HttpResponse &response);

    /**
     * @brief Retrieves the HTTP status message for a given status code.
     * @param statusCode The HTTP status code.
     * @return The corresponding status message.
     */
    static std::string getStatusMessage(int statusCode);

    /**
     * @brief To validate a WebSocket frame, you should check the following:
     * 1. Opcode: The opcode should be one of the following:
     *   -. 0x0: Continuation frame
     *   -. 0x1: Text frame
     *   -. 0x2: Binary frame
     *   -. 0x8: Close frame
     *   -. 0x9: Ping frame
     *   -. 0xA: Pong frame
     * 2. Payload length: The payload length should be valid based on the opcode.
     * 3. Masking: The masking bit should be set for client-to-server frames.
     * 4. Frame size: The frame size should not exceed the maximum allowed size.
     * 
     * @param frame 
     * @return true 
     * @return false 
     */
    bool validateWebSocketFrame(const WebSocketFrame& frame);

    /**
     * @brief HTTP Request/Response Validation:
     * To validate an HTTP request or response, you should check the following:
     * 1. Method: The method should be one of the following:
     *   -. GET
     *   -. POST
     *   -. PUT
     *   -. DELETE
     *   -. HEAD
     *   -. OPTIONS
     *   -. CONNECT
     *   -. PATCH
     * 2. URI: The URI should be valid and properly formatted.
     * 3. HTTP version: The HTTP version should be one of the following:
     *   -. HTTP/1.0
     *   -. HTTP/1.1
     *   -. HTTP/2
     * 4. Headers: The headers should be valid and properly formatted.
     * 5. Body: The body should be valid and properly formatted based on the content type.
     * 
     * @param request 
     * @return true 
     * @return false 
     */
    bool validateHttpRequest(const HttpRequest& request);

public:
    /**
     * @brief Constructs a non-SSL server.
     * @param host The host address to bind to.
     * @param port The port number to listen on.
     * @param maxConnections Maximum number of simultaneous connections.
     * @param numThreads Number of worker threads.
     */
    HttpServer(std::string host, std::string port, size_t maxConnections, size_t numThreads);

    /**
     * @brief Destroys the ssl ctx instance (if exists) when the Server is destroyed.
     */
    ~HttpServer() = default;

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

    void stop();
};

#endif