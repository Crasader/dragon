//
//  WebSocketService.h
//  dragon
//
//  Created by xingchong on 1/8/16.
//
//

#ifndef __dragon__WebSocketService__
#define __dragon__WebSocketService__

#include <stdio.h>
#include "cocos2d.h"
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "network/WebSocket.h"

USING_NS_CC;

class WebSocketService : public Ref, public network::WebSocket::Delegate
{
public:
    static WebSocketService *getInstance();
    
    WebSocketService();
    ~WebSocketService();
    
    bool init();
    void sendMessage();
    
public:
    virtual void onOpen(cocos2d::network::WebSocket* ws)override;
    virtual void onMessage(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::Data& data)override;
    virtual void onClose(cocos2d::network::WebSocket* ws)override;
    virtual void onError(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::ErrorCode& error)override;
    
private:
    network::WebSocket* _wsiSendBinary;
    int _sendBinaryTimes;
};

#endif /* defined(__dragon__WebSocketService__) */
