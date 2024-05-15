#include "Server.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <utility>
#include "http/Status.hpp"
#include "http/HttpRequest.hpp"
#include "util/String.hpp"

Server::Server(std::string host, std::string port, size_t maxConnections, size_t numThreads)
        : host(std::move(host)), port(std::move(port)), maxConnections(maxConnections),
          numThreads(numThreads) {}

void Server::run() {
    int serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd < 0) {
        throw std::runtime_error("Failed to create socket");
    }

    struct sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(std::stoi(port));

    if (bind(serverFd, reinterpret_cast<struct sockaddr *>(&address), sizeof(address)) < 0) {
        throw std::runtime_error("Failed to bind socket");
    }

    if (listen(serverFd, static_cast<int>(maxConnections)) < 0) {
        throw std::runtime_error("Failed to listen on socket");
    }

    std::cout << "Server listening on " << host << ":" << port << std::endl;

    std::vector<std::thread> workers;
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back([this, serverFd]() { runWorker(serverFd); });
    }

    for (auto &worker: workers) {
        worker.join();
    }
}

void Server::setHttpHandler(const std::string &method, const std::string &path, HttpHandler handler) {
    httpHandlers[new HttpEndpoint(method, path)] = std::move(handler);
}

void Server::setWebSocketHandler(WebSocketHandler handler) {
    webSocketHandler = std::move(handler);
}

void Server::runWorker(int serverFd) {
    while (true) {
        int clientFd = accept(serverFd, nullptr, nullptr);
        if (clientFd < 0) {
            std::cerr << "Failed to accept connection: " << std::strerror(errno) << std::endl;
            continue;
        }

        if (!tryAcquireConnection()) {
            std::cerr << "Connection limit reached, rejecting new connection." << std::endl;
            close(clientFd);
            continue;
        }

        std::thread([this, clientFd]() {
            handleConnection(clientFd);
            releaseConnection();
        })
                .detach();
    }
}

void Server::handleConnection(int clientFd) {
    std::stringstream error;
    // Read HTTP request without SSL
    std::optional<HttpRequest> request = readHttpRequest(clientFd);
    if (request) {
        HttpResponse response;
        handleHttpRequest(*request, response);
        sendHttpResponse(clientFd, response); // Modify sendHttpResponse to use regular socket write
    } else {
        handleWebSocketConnection(clientFd);
    }

    close(clientFd);
}

std::optional<HttpRequest> Server::readHttpRequest(int clientFd) {
    const int MAX_REQUEST_SIZE = 8192; // Maximum size of the request buffer
    char requestBuffer[MAX_REQUEST_SIZE];
    int bytesRead = 0;

    // Read the request line and headers
    while (true) {
        int n = read(clientFd, requestBuffer + bytesRead, MAX_REQUEST_SIZE - bytesRead);
        if (n <= 0) {
            // Error or connection closed
            return std::nullopt;
        }

        bytesRead += n;

        // Check if we've reached   the end of headers
        char *endOfHeaders = strstr(requestBuffer, "\r\n\r\n");
        if (endOfHeaders) {
            // Null-terminate the request buffer
            *endOfHeaders = '\0';
            break;
        }

        // Check if the request buffer is full
        if (bytesRead == MAX_REQUEST_SIZE) {
            // Request is too large, return an error
            return std::nullopt;
        }
    }

    std::string requestStr(requestBuffer);
    std::istringstream requestStream(requestStr);

    // Parse the request line
    std::string requestLine;
    std::getline(requestStream, requestLine);
    std::istringstream requestLineStream(requestLine);

    HttpRequest request;
    std::string method, path, version;
    requestLineStream >> method >> path >> version;

    request.setMethod(method);

    std::regex rx("?");
    
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
    while (std::getline(requestStream, header) && header != "\r") {
        std::size_t colonPos = header.find(':');
        if (colonPos != std::string::npos) {
            std::string key = header.substr(0, colonPos);
            std::string value = header.substr(colonPos + 2); // Skip the ": "
            request.addHeader(key, value);
        }
    }

    // Read the request body (if present)
    std::string body(requestBuffer + strlen(requestBuffer) + 4); // Skip the "\r\n\r\n"
    request.setBody(body);

    return request;
}

void Server::handleHttpRequest(const HttpRequest &request, HttpResponse &response) {
     auto it = httpHandlers.find(new HttpEndpoint(request.getMethod(), request.getPath()));
    if (it != httpHandlers.end()) {
        it->second(request, response);
    } else {
        response.status = 404;
        response.body = "Not Found";
    }
}

void Server::sendHttpResponse(int clientFd, const HttpResponse &response) {
    // Generate HTTP response
    std::string httpResponse = generateHttpResponse(response);
    // Send HTTP response over SSL
    size_t bytesSent = write(clientFd, httpResponse.c_str(), httpResponse.length());
    if (bytesSent <= 0) {
        std::cerr << "Failed to send HTTP response" << std::endl;
    }
}

void Server::handleWebSocketConnection(int clientFd) {
    // Implement WebSocket handshake and frame processing
    WebSocketFrame frame;
    while (readWebSocketFrame(clientFd, frame)) {
        WebSocketFrame response;
        webSocketHandler.value_or(
                [](const WebSocketFrame &, WebSocketFrame &response) {
                    response.fin = true;
                    response.opcode = 0x1; // Text frame
                    response.payload = "No WebSocket handler set";
                })(frame, response);
        sendWebSocketFrame(clientFd, response);
    }
}

bool Server::readWebSocketFrame(int clientFd, WebSocketFrame &frame) {
    const int MAX_FRAME_SIZE = 8192; // Maximum size of a WebSocket frame
    unsigned char frameBuffer[MAX_FRAME_SIZE];

    // Read WebSocket frame header
    size_t headerSize = read(clientFd, frameBuffer, 2);
    if (headerSize != 2) {
        // Failed to read WebSocket frame header
        return false;
    }

    // Parse WebSocket frame opcode and payload length
    frame.fin = (frameBuffer[0] & 0x80) != 0;
    frame.opcode = frameBuffer[0] & 0x0F;
    bool masked = (frameBuffer[1] & 0x80) != 0;
    unsigned long long payloadLength = frameBuffer[1] & 0x7F;
    int lengthBytes = 0;
    if (payloadLength == 126) {
        // 16-bit payload length
        lengthBytes = 2;
    } else if (payloadLength == 127) {
        // 64-bit payload length
        lengthBytes = 8;
    }

    // Read extended payload length (if necessary)
    if (lengthBytes > 0) {
        headerSize = read(clientFd, frameBuffer + 2, lengthBytes);
        if (headerSize != lengthBytes) {
            // Failed to read extended payload length
            return false;
        }
        // Parse extended payload length
        payloadLength = 0;
        for (int i = 0; i < lengthBytes; ++i) {
            payloadLength = (payloadLength << 8) | frameBuffer[2 + i];
        }
    }

    // Read masking key (if masked)
    unsigned char maskingKey[4];
    if (masked) {
        headerSize = read(clientFd, maskingKey, 4);
        if (headerSize != 4) {
            // Failed to read masking key
            return false;
        }
    }

    // Read payload data
    int payloadSize = read(clientFd, frameBuffer + 2 + lengthBytes + (masked ? 4 : 0), payloadLength);

    if (payloadSize != payloadLength) {
        // Failed to read payload data
        return false;
    }

    // Unmask payload data (if masked)
    if (masked) {
        for (unsigned long long i = 0; i < payloadLength; ++i) {
            frameBuffer[2 + lengthBytes + i] ^= maskingKey[i % 4];
        }
    }

    // Populate WebSocketFrame object
    frame.payload.assign(reinterpret_cast<char *>(frameBuffer + 2 + lengthBytes), payloadLength);
    return true; // Successfully read WebSocket frame
}

void Server::sendWebSocketFrame(int clientFd, const WebSocketFrame &frame) {
    const int MAX_FRAME_SIZE = 8192; // Maximum size of a WebSocket frame
    unsigned char frameBuffer[MAX_FRAME_SIZE];
    int frameSize = 0;

    // Construct WebSocket frame header
    frameBuffer[frameSize++] = 0x80 | frame.opcode; // FIN + opcode
    unsigned long long payloadLength = frame.payload.length();
    if (payloadLength <= 125) {
        frameBuffer[frameSize++] = payloadLength; // Payload length (7 bits)
    } else if (payloadLength <= 0xFFFF) {
        frameBuffer[frameSize++] = 126; // Extended payload length (16 bits)
        frameBuffer[frameSize++] = (payloadLength >> 8) & 0xFF;
        frameBuffer[frameSize++] = payloadLength & 0xFF;
    } else {
        frameBuffer[frameSize++] = 127; // Extended payload length (64 bits)
        for (int i = 7; i >= 0; --i) {
            frameBuffer[frameSize++] = (payloadLength >> (8 * i)) & 0xFF;
        }
    }

    // Copy payload data into frame buffer
    memcpy(frameBuffer + frameSize, frame.payload.c_str(), payloadLength);
    frameSize += payloadLength;

    // Send WebSocket frame over SSL connection
    size_t bytesSent = write(clientFd, frameBuffer, frameSize);
    if (bytesSent != frameSize) {
        std::cerr << "Failed to send WebSocket frame" << std::endl;
    }
}

bool Server::tryAcquireConnection() {
    std::lock_guard<std::mutex> lock(connectionsMutex);
    if (currentConnections >= maxConnections) {
        return false;
    }
    ++currentConnections;
    return true;
}

void Server::releaseConnection() {
    std::lock_guard<std::mutex> lock(connectionsMutex);
    --currentConnections;
}

std::string Server::generateHttpResponse(const HttpResponse &response) {
    std::ostringstream httpResponse;
    httpResponse << "HTTP/1.1 " << response.status << " " << getStatusMessage(response.status) << "\r\n";
    for (const auto &header: response.headers) {
        httpResponse << header.first << ": " << header.second << "\r\n";
    }
    httpResponse << "\r\n"
                 << response.body;
    return httpResponse.str();
}

std::string Server::getStatusMessage(int statusCode) {
    return Status(statusCode).ss.str();
}

