#ifndef __WEB_SOCKET_FRAME_HPP__
#define __WEB_SOCKET_FRAME_HPP__

#include "netppconfig.h"
#include <string>
#include <cstdint>

class WebSocketFrame
{
private:
    bool fin;
    bool masked;
    uint8_t opcode;
    std::string payload;

public:
    WebSocketFrame(/* args */);
    ~WebSocketFrame();

    bool isFin() const;

    void setFin(bool fin);

    bool isMasked() const;

    void setMasked(bool masked);

    uint8_t getOpcode() const;

    void setOpcode(uint8_t opcode);

    const std::string &getPayload() const;

    void setPayload(const std::string &payload);
};


#endif