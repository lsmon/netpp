#include "http/WebSocketFrame.hpp"
#include <cstdint>

WebSocketFrame::WebSocketFrame(/* args */)
{
}

WebSocketFrame::~WebSocketFrame()
{
}

bool WebSocketFrame::isFin() const {
    return fin;
}

void WebSocketFrame::setFin(bool fin) {
    WebSocketFrame::fin = fin;
}

bool WebSocketFrame::isMasked() const {
    return masked;
}

void WebSocketFrame::setMasked(bool masked) {
    WebSocketFrame::masked = masked;
}

uint8_t WebSocketFrame::getOpcode() const {
    return opcode;
}

void WebSocketFrame::setOpcode(uint8_t opcode) {
    WebSocketFrame::opcode = opcode;
}

const std::string &WebSocketFrame::getPayload() const {
    return payload;
}

void WebSocketFrame::setPayload(const std::string &payload) {
    WebSocketFrame::payload = payload;
}
