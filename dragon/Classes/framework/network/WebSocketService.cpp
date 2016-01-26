//
//  WebSocketService.cpp
//  dragon
//
//  Created by xingchong on 1/8/16.
//
//

#include "WebSocketService.h"

static WebSocketService *_webSocketService = nullptr;

WebSocketService *WebSocketService::getInstance()
{
    if(!_webSocketService)
    {
        _webSocketService = new WebSocketService();
        _webSocketService->init();
    }
    return _webSocketService;
}

WebSocketService::WebSocketService()
{
    
}

WebSocketService::~WebSocketService()
{
    
}

bool WebSocketService::init()
{
    _wsiSendBinary = new network::WebSocket();
    
    std::string url = "ws://echo.websocket.org";
    url = "ws://192.168.191.2:7070";
    url = "ws://127.0.0.1:7070";
    
    
    if (!_wsiSendBinary->init(*this, url))
    {
        CC_SAFE_DELETE(_wsiSendBinary);
    }
    
    return true;
}

void WebSocketService::sendMessage()
{
    if (_wsiSendBinary->getReadyState() == network::WebSocket::State::OPEN)
    {
//        char buf[] = "Hello WebSocket,\0 I'm\0 a\0 binary\0 message\0.";
        char buf[] = "0";
        _wsiSendBinary->send((unsigned char*)buf, sizeof(buf));
    }
    else
    {
        CCLOG("WebSocket is not open!!");
    }
}


// Delegate methods
void WebSocketService::onOpen(network::WebSocket* ws)
{
    log("Websocket (%p) opened", ws);
}

void WebSocketService::onMessage(network::WebSocket* ws, const network::WebSocket::Data& data)
{
    if (data.isBinary)
    {
        _sendBinaryTimes++;
        char times[100] = {0};
        sprintf(times, "%d", _sendBinaryTimes);
        
        std::string binaryStr = "response bin msg: ";
        
        for (int i = 0; i < data.len; ++i) {
            if (data.bytes[i] != '\0')
            {
                binaryStr += data.bytes[i];
            }
            else
            {
                binaryStr += "\'\\0\'";
            }
        }
        
        binaryStr += std::string(", ")+times;
        log("%s", binaryStr.c_str());
    }
}

void WebSocketService::onClose(network::WebSocket* ws)
{
    log("websocket instance (%p) closed.", ws);
    // Delete websocket instance.
    CC_SAFE_DELETE(ws);
}

void WebSocketService::onError(network::WebSocket* ws, const network::WebSocket::ErrorCode& error)
{
    log("Error was fired, error code: %d", error);
}

