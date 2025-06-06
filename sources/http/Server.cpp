#include "http/Server.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <utility>
#ifdef __linux__
#include <cstring>
#endif
#include "http/Status.hpp"
#include "http/Request.hpp"
#include "http/Response.hpp"
#include "util/String.hpp"
#include "Exception.hpp"
#include "http/Path.hpp"

HttpServer::HttpServer(std::string host, std::string port, size_t maxConnections, size_t numThreads)
    : host(std::move(host)), port(std::move(port)), maxConnections(maxConnections),
      numThreads(numThreads), working(true) {}

void HttpServer::run()
{
    auto serverFd = std::make_shared<Socket>(socket(AF_INET, SOCK_STREAM, 0));
    if (serverFd->get() < 0)
    {
        throw netpp::Exception("Failed to create socket", netpp::ERR_CODE::SOCKET_CREATION);
    }

    struct sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(std::stoi(port));

    if (bind(serverFd->get(), reinterpret_cast<struct sockaddr *>(&address), sizeof(address)) < 0)
    {
        throw netpp::Exception("Failed to bind socket", netpp::ERR_CODE::SOCKET_BINDING);
    }

    if (listen(serverFd->get(), static_cast<int>(maxConnections)) < 0)
    {
        throw netpp::Exception("Failed to listen on socket", netpp::ERR_CODE::SOCKET_LISTENING);
    }

    std::vector<std::thread> workers;
    for (size_t i = 0; i < numThreads; ++i)
    {
        int socketFd = serverFd->get();
        workers.emplace_back([this, serverFd]()
                             { runWorker(serverFd->get()); });
    }

    for (auto &worker : workers)
    {
        worker.join();
    }
}

void HttpServer::setHttpHandler(const std::string &method, const std::string &path, HttpHandler handler)
{
    auto endpoint = std::make_shared<HttpEndpoint>(method, path);
    httpHandlers[endpoint] = std::move(handler);
}

void HttpServer::setWebSocketHandler(WebSocketHandler handler)
{
    webSocketHandler = std::move(handler);
}

void HttpServer::runWorker(int serverFd)
{
    if (serverFd < 0) {
        std::cerr << "Invalid server socket" << std::endl;
        return;
    }
    while (working)
    {

        int clientFd = accept(serverFd, nullptr, nullptr);
        if (clientFd < 0)
        {
            std::cerr << "Failed to accept connection: " << std::strerror(errno) << std::endl;
            continue;
        }

        if (!tryAcquireConnection())
        {
            std::cerr << "Connection limit reached, rejecting new connection." << std::endl;
            close(clientFd);
            continue;
        }

        // Ensure connection release
        std::thread([this, clientFd]() {
            try
            {
                handleConnection(clientFd);
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error handling connection: " << e.what() << std::endl;
            }
            releaseConnection();
        }).detach();
    }
}

struct ConnectionGuard {
    std::function<void()> release;
    ~ConnectionGuard() { release(); }
};

void HttpServer::handleConnection(int clientFd)
{
/*    ConnectionGuard guard = { [this]() { releaseConnection(); } };

    std::optional<HttpRequest> request = readHttpRequest(clientFd);
    if (request) {
        HttpResponse response;
        handleHttpRequest(*request, response);
        sendHttpResponse(clientFd, response);
    } else {
        handleWebSocketConnection(clientFd);
    }

    close(clientFd);*/

    // Set a timeout for idle connections
    struct timeval timeout;
    timeout.tv_sec = 10;  // 10 seconds timeout for inactivity
    timeout.tv_usec = 0;

    if (setsockopt(clientFd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0)
    {
        std::cerr << "Failed to set socket timeout: " << std::strerror(errno) << std::endl;
        close(clientFd);
        return;
    }

    ConnectionGuard guard = { [this]() { releaseConnection(); } };

    // Attempt to read the HTTP request
    std::optional<HttpRequest> request = readHttpRequest(clientFd);

    if (request) {
        // Handle HTTP requests
        HttpResponse response;
        handleHttpRequest(*request, response);
        sendHttpResponse(clientFd, response);
    } else {
        // Handle WebSocket or timeout
        char buffer[1];
        ssize_t result = recv(clientFd, buffer, sizeof(buffer), MSG_PEEK);

        if (result < 0 && (errno == EWOULDBLOCK || errno == EAGAIN)) {
            std::cerr << "Connection timed out (idle)." << std::endl;
        } else if (result == 0) {
            std::cerr << "Client disconnected." << std::endl;
        } else {
            // Proceed with WebSocket handling if there's valid input
            handleWebSocketConnection(clientFd);
        }
    }

    // Close the connection when done
    close(clientFd);
}

std::optional<HttpRequest> HttpServer::readHttpRequest(int clientFd)
{
    const int MAX_REQUEST_SIZE = 8192; // Maximum size of the request buffer
    std::vector<char> requestBuffer(MAX_REQUEST_SIZE, 0);
    int totalBytesRead = 0;

    // Read the request line and headers
    while (totalBytesRead < MAX_REQUEST_SIZE) {
        int bytesRead = read(clientFd, &requestBuffer[totalBytesRead], MAX_REQUEST_SIZE - totalBytesRead);
        if (bytesRead <= 0) {
            // Handle read error or connection close
            return std::nullopt;
        }
        totalBytesRead += bytesRead;

        // Check for "\r\n\r\n" indicating end of headers
        const char* headerDelimiter = "\r\n\r\n";
        auto endOfHeaders = std::search(
            requestBuffer.begin(), 
            requestBuffer.begin() + totalBytesRead, 
            headerDelimiter, 
            headerDelimiter + 4);
        if (endOfHeaders != requestBuffer.begin() + totalBytesRead) {
            break;
        }
    }

    std::string requestStr(requestBuffer.data(), totalBytesRead);
    std::istringstream requestStream(requestStr);

    // Parse the request line
    std::string requestLine;
    std::getline(requestStream, requestLine);
    std::istringstream requestLineStream(requestLine);

    HttpRequest request;
    std::string method, path, version;
    requestLineStream >> method >> path >> version;

    request.setMethod(method);
    
    std::regex rx("\\?");
    
    std::vector<std::string> split = String::tokenize(path, rx);
    if (split.size() > 1)
    {
        request.setPath(split[0]);
        request.setQueryString(QueryString::getInstance(split[1]).getMap());
    }
    else
    {
        request.setPath(path);
    }

    // Parse the headers
    std::string header;
    while (std::getline(requestStream, header) && header != "\r")
    {
        std::size_t colonPos = header.find(':');
        if (colonPos != std::string::npos)
        {
            std::string key = header.substr(0, colonPos);
            std::string value = header.substr(colonPos + 2); // Skip the ": "
            request.addHeader(key, value);
        }
    }

    // Read the request body (if present)
    // Find the end of headers ("\r\n\r\n")
    const char* headerDelimiter = "\r\n\r\n";
    auto endOfHeaders = std::search(
        requestBuffer.begin(),
        requestBuffer.begin() + totalBytesRead,
        headerDelimiter,
        headerDelimiter + 4
    );
    std::string body;
    if (endOfHeaders != requestBuffer.begin() + totalBytesRead) {
        // Calculate where the body starts
        size_t bodyOffset = std::distance(requestBuffer.begin(), endOfHeaders) + 4;
        // Check if Content-Length header is present to determine body size
        auto contentLengthHeader = request.getHeaders()["content-length"].empty() ? 
                                           request.getHeaders()["Content-Length"] : 
                                           request.getHeaders()["content-length"];
        size_t contentLength = 0;
        if (!contentLengthHeader.empty()) {
            contentLength = std::stoul(String::trim(contentLengthHeader));
        }

        // Calculate how many bytes of the body have already been read
        size_t alreadyReadBody = bodyOffset;

        // If Content-Length is specified and we haven't read the full body, read the rest
        while (alreadyReadBody < contentLength) {
            int bytesToRead = std::min(static_cast<size_t>(MAX_REQUEST_SIZE) - totalBytesRead, contentLength - alreadyReadBody);
            if (bytesToRead <= 0) break;
            int bytesRead = read(clientFd, &requestBuffer[totalBytesRead], bytesToRead);
            if (bytesRead <= 0) break;
            totalBytesRead += bytesRead;
            alreadyReadBody += bytesRead;
        }
        // Extract the body from the request buffer
        body.assign(requestBuffer.data() + bodyOffset, totalBytesRead - bodyOffset);
    }
    request.setBody(body);
    
    struct sockaddr_storage clientAddr;
    char clientIP[INET6_ADDRSTRLEN];
    if (clientAddr.ss_family == AF_INET)
    {
        struct sockaddr_in *s = (struct sockaddr_in *)&clientAddr;
        inet_ntop(AF_INET, &s->sin_addr, clientIP, sizeof(clientIP));
    }
    else
    {
        struct sockaddr_in6 *s = (struct sockaddr_in6 *)&clientAddr;
        inet_ntop(AF_INET6, &s->sin6_addr, clientIP, sizeof(clientIP));
    }
    std::cout << "Accepted connection from " << clientIP << std::endl;

    return request;
}

void HttpServer::handleHttpRequest(const HttpRequest &request, HttpResponse &response)
{
    std::lock_guard<std::mutex> lock(handlersMutex);
    auto endpoint = std::make_shared<HttpEndpoint>(request.getMethod(), request.getPath());

    std::string requestPath = request.getPath();
    HttpHandler handler = nullptr;
    for (const auto &item: httpHandlers) {
        if (item.first->getHttpMethod() == request.getMethod()) {
            std::string handlerPath = item.first->getPath();
            Path* path = new Path(handlerPath);
            if (path->match(requestPath)) {
                handler = item.second;
                handler(request, response, path);
                break;
            }
            delete path;
        }
    }

    if (handler == nullptr) {
        response.setStatus(404);
        response.setBody("Not Found");
    }

    /*auto it = httpHandlers.find(endpoint);
    if (it != httpHandlers.end())
    {
        it->second(request, response);
    }
    else
    {
        response.setStatus(404);
        response.setBody("Not Found");
    }*/
}

void HttpServer::sendHttpResponse(int clientFd, const HttpResponse &response)
{
    // Generate HTTP response
    std::string httpResponse = generateHttpResponse(response);
    // Send HTTP response over SSL
    size_t bytesSent = write(clientFd, httpResponse.c_str(), httpResponse.length());
    if (bytesSent <= 0)
    {
        std::cerr << "Failed to send HTTP response" << std::endl;
    }
}

void HttpServer::handleWebSocketConnection(int clientFd)
{
    // Implement WebSocket handshake and frame processing
    WebSocketFrame frame;
    while (readWebSocketFrame(clientFd, frame))
    {
        switch (frame.getOpcode()) {
            case WebSocketOpcode::TextFrame:
                handleTextFrame(clientFd, frame);
                break;
            case WebSocketOpcode::BinaryFrame:
                handleBinaryFrame(clientFd, frame);
                break;
            case WebSocketOpcode::CloseFrame:
                handleCloseFrame(clientFd);
                return; // Close the connection after handling close frame
            case WebSocketOpcode::PingFrame:
                handlePingFrame(clientFd);
                break;
            case WebSocketOpcode::PongFrame:
                handlePongFrame(clientFd);
                break;
            default:
                handleUnknownFrame(clientFd, frame);
                break;
        }
    }
}

void HttpServer::handleBinaryFrame(int clientFd, const WebSocketFrame &frame)
{
    // Process the binary data (e.g., save to a file, forward it, etc.)
    std::vector<uint8_t> binaryData(frame.getPayload().begin(), frame.getPayload().end());


    std::cout << "Received Binary Frame of size: " << binaryData.size() << " bytes" << std::endl;

    // Example: Just acknowledge receipt of binary data
    std::string ackMessage = "Binary data received (" + std::to_string(binaryData.size()) + " bytes)";
    sendWebSocketFrame(clientFd, WebSocketOpcode::TextFrame, ackMessage);
}

void HttpServer::handleTextFrame(int clientFd, const WebSocketFrame &frame)
{
    // Assuming that the payload is a string of UTF-8 text
    std::string textData(frame.getPayload().begin(), frame.getPayload().end());

    // Process the text data (e.g., echo it back, log it, etc.)
    std::cout << "Received Text Frame: " << textData << std::endl;

    // Example: Echo the text data back to the client
    sendWebSocketFrame(clientFd, WebSocketOpcode::TextFrame, textData);
}

void HttpServer::handleCloseFrame(int clientFd)
{
    std::cout << "Received Close Frame. Closing connection..." << std::endl;

    // Send a close frame back to acknowledge the closure
    std::vector<uint8_t> closePayload = {0x03, 0xE8}; // Status code 1000 (normal closure)
    sendWebSocketFrame(clientFd, WebSocketOpcode::CloseFrame, closePayload);

    // Close the connection
    close(clientFd);
}

void HttpServer::handlePingFrame(int clientFd)
{
    std::cout << "Received Ping Frame. Sending Pong..." << std::endl;

    // Respond with a pong frame containing the same payload as the ping
    sendWebSocketFrame(clientFd, WebSocketOpcode::PongFrame, std::string{});
}

void HttpServer::handlePongFrame(int clientFd)
{
    std::cout << "Received Pong Frame." << std::endl;

    // Usually, no action is needed in response to a Pong
}

void HttpServer::handleUnknownFrame(int clientFd, const WebSocketFrame &frame)
{
    std::cerr << "Received Unknown or Unsupported Frame with Opcode: " << frame.getOpcode() << std::endl;

    // Optionally, you can send a close frame with a status code indicating protocol error
    std::vector<uint8_t> closePayload = {0x03, 0xF2}; // Status code 1002 (protocol error)
    sendWebSocketFrame(clientFd, WebSocketOpcode::CloseFrame, closePayload);

    // Close the connection after sending the close frame
    close(clientFd);
}


void HttpServer::sendWebSocketFrame(int clientFd, WebSocketOpcode opcode, const std::string &payload)
{
    std::vector<uint8_t> binaryPayload(payload.begin(), payload.end());
    sendWebSocketFrame(clientFd, opcode, binaryPayload);
}

void HttpServer::sendWebSocketFrame(int clientFd, WebSocketOpcode opcode, const std::vector<uint8_t> &payload)
{
    WebSocketFrame frame;
    frame.setOpcode(opcode);
    std::string payloadToString(payload.begin(), payload.end());
    frame.setPayload(payloadToString);

    // Serialize the frame to the WebSocket frame format
    std::vector<uint8_t> rawFrame = serializeWebSocketFrame(frame);

    // Send the raw frame over the network
    write(clientFd, rawFrame.data(), rawFrame.size());
}

std::vector<uint8_t> HttpServer::serializeWebSocketFrame(const WebSocketFrame &frame) 
{
    std::vector<uint8_t> rawFrame;

    // Build the frame header
    uint8_t firstByte = 0x80 | frame.getOpcode(); // FIN bit set, opcode set
    rawFrame.push_back(firstByte);

    // Payload length
    size_t payloadLength = frame.getPayload().size();
    if (payloadLength <= 125) {
        rawFrame.push_back(static_cast<uint8_t>(payloadLength));
    } else if (payloadLength <= 65535) {
        rawFrame.push_back(126);
        rawFrame.push_back((payloadLength >> 8) & 0xFF);
        rawFrame.push_back(payloadLength & 0xFF);
    } else {
        rawFrame.push_back(127);
        for (int i = 7; i >= 0; --i) {
            rawFrame.push_back((payloadLength >> (i * 8)) & 0xFF);
        }
    }

    // Add the payload
    rawFrame.insert(rawFrame.end(), frame.getPayload().begin(), frame.getPayload().end());

    return rawFrame;
}

bool HttpServer::readWebSocketFrame(int clientFd, WebSocketFrame &frame) 
{
    const int MAX_FRAME_SIZE = 8192; // Maximum size of a WebSocket frame
    unsigned char frameBuffer[MAX_FRAME_SIZE];

    // Read WebSocket frame header
    size_t headerSize = read(clientFd, frameBuffer, 2);
    if (headerSize != 2)
    {
        // Failed to read WebSocket frame header
        return false;
    }

    // Parse WebSocket frame opcode and payload length
    frame.setFin((frameBuffer[0] & 0x80) != 0);
    frame.setOpcode(frameBuffer[0] & 0x0F);
    bool masked = (frameBuffer[1] & 0x80) != 0;
    unsigned long long payloadLength = frameBuffer[1] & 0x7F;
    int lengthBytes = 0;
    if (payloadLength == 126)
    {
        // 16-bit payload length
        lengthBytes = 2;
    }
    else if (payloadLength == 127)
    {
        // 64-bit payload length
        lengthBytes = 8;
    }

    // Read extended payload length (if necessary)
    if (lengthBytes > 0)
    {
        headerSize = read(clientFd, frameBuffer + 2, lengthBytes);
        if (headerSize != lengthBytes)
        {
            // Failed to read extended payload length
            return false;
        }
        // Parse extended payload length
        payloadLength = 0;
        for (int i = 0; i < lengthBytes; ++i)
        {
            payloadLength = (payloadLength << 8) | frameBuffer[2 + i];
        }
    }

    // Read masking key (if masked)
    unsigned char maskingKey[4];
    if (masked)
    {
        headerSize = read(clientFd, maskingKey, 4);
        if (headerSize != 4)
        {
            // Failed to read masking key
            return false;
        }
    }

    // Read payload data
    int payloadSize = read(clientFd, frameBuffer + 2 + lengthBytes + (masked ? 4 : 0), payloadLength);

    if (payloadSize != payloadLength)
    {
        // Failed to read payload data
        return false;
    }

    // Unmask payload data (if masked)
    if (masked)
    {
        for (unsigned long long i = 0; i < payloadLength; ++i)
        {
            frameBuffer[2 + lengthBytes + i] ^= maskingKey[i % 4];
        }
    }

    // Populate WebSocketFrame object
    std::string framePayload;
    framePayload.assign(reinterpret_cast<char *>(frameBuffer + 2 + lengthBytes), payloadLength);
    frame.setPayload(framePayload);
    // frame.payload.assign(reinterpret_cast<char *>(frameBuffer + 2 + lengthBytes), payloadLength);
    return true; // Successfully read WebSocket frame
}
 
bool HttpServer::tryAcquireConnection()
{
    std::lock_guard<std::mutex> lock(connectionsMutex);
    if (currentConnections < maxConnections)
    {
        ++currentConnections;
        std::cout << "Connection acquired. Current connections: " << currentConnections << std::endl;
        return true;
    }
    std::cerr << "Connection limit reached. Current connections: " << currentConnections << std::endl;
    return false;
}

void HttpServer::releaseConnection()
{
    std::lock_guard<std::mutex> lock(connectionsMutex);
    if (currentConnections > 0)
        --currentConnections;
}

std::string HttpServer::generateHttpResponse(const HttpResponse &response)
{
    std::ostringstream httpResponse;
    httpResponse << "HTTP/1.1 " << response.getStatus() << " " << getStatusMessage(response.getStatus()) << "\r\n";
    for (const auto &header : response.getHeaders())
    {
        httpResponse << header.first << ": " << header.second << "\r\n";
    }
    httpResponse << "\r\n" << response.getBody();
    return httpResponse.str();
}

std::string HttpServer::getStatusMessage(int statusCode)
{
    return Status(statusCode).ss.str();
}

void HttpServer::stop() {
    working = false;
}

Socket::~Socket()
{
    if (fd >= 0) 
    {
        close(fd);
    }
}

int Socket::get() const
{
    return fd;
}
