//
//  CMDProcessor.h
//  MyGame
//
//  Created by 永翔 何 on 12-9-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef MyGame_CMDProcessor_h
#define MyGame_CMDProcessor_h

#include "cocos2d.h"
USING_NS_CC;
#include "NetworkManager.h"
#include "OnlineManager.h"
class BuffData
{
public:   
    int     m_bufLen;
    int     m_index;
};

class CMDProcessor : public cocos2d::CCNode
{
public:
    CMDProcessor();
    virtual ~CMDProcessor();
    
    static CMDProcessor* sharedInstance();
    static void purgeInstance();
    
    // description: set received protocal buffer
    // parameter:   - recvLen   received protocal buffer length
    //              - recvBuf   received protocal buffer pointer
    void    setBuffer(int recvLen, const char* recvBuf);
    void    setEncodeBuffer(BuffData buffdata);
    
    void    setListBuffer(int recvLen, const char* recvBuf);
    void    setConnectStatus(ENUM_PERFUNC_TYPE type);
    // description: clear current buffer
    void    clearBuffer();
    
    // description: receive current cmd
    void    receiveCmd();
    
    void    bufUpdate(float ftime);
private:
    static CMDProcessor* pSharedInstance;
    ENUM_PERFUNC_TYPE m_type;
    bool    isReceive;
    int     m_bufLen;               // buffer length;
    char    m_buf[MAX_RECV_BUFFER]; // current protocal buffer to process
};

#endif
