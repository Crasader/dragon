//
//  NetworkManager.h
//  MyGame
//
//  Created by 永翔 何 on 12-8-1.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef NetworkManager_h
#define NetworkManager_h

#include "cocos2d.h"
#include "Thread.h"
#include "CSocketUtil.h"
#include "proto_base.h"
#include "proto_encry.h"
#include "ServerConfig.h"

enum ENUM_PERFUNC_TYPE
{
    E_PROCESS_CMD,
    E_CONNECT_ONLINE,
    E_CLOSE_ONLINE,
};

enum
{
    E_UNCONNECTED = 0,
    E_ONLINE_CONNECTED,
};

class NetworkManager : public Runnable
{
public:
    NetworkManager();
    ~NetworkManager();
    
    static NetworkManager* shareNetworkManager();
    static void purgeNetworkManager();
    
    virtual bool init();
    virtual void run();

    //挂起线程
    void waitOnline();
    //开始线程
    void startThread();
    //结束
    void close();
    
    void createOnlineSocket();
    
    int connectToOnline();
    
    void closeConnect();
    
    //关闭Login的连接
    void disConnectOnline();
    
    void disConnect();
    
    //发送数据
    int sendDataToOnline(int pSeqNum, int cmd, Cmessage* pMsg, uint32_t userid = 0);
    
public:
    //是否获得了Online,用online port来判断，如果是0则没有获得
    bool isGotOnline() { return (m_hOnlinePort != 0); }
    
    //判断online是否连接着
    bool isOnlineConnected();
    
    void setOnlineIpAndPort(const char* ip, uint16_t port);
    const char* getOnlineIp() {return m_sOnlineIp;}
    const uint16_t getOnlinePort() {return m_hOnlinePort;}

private:
    int connect(char* ip, int port, int timeout, uint8_t type);
    //如果userid不等于0，则用userid
    int sendData(int pSeqNum, int cmd, Cmessage* pMsg, uint32_t userid = 0);

private:
    static NetworkManager* pNetworkManager;
    
    CSocketUtil*    m_csNetwork;
    Thread*         m_thNetworkThread;
    
    int             m_recvLen;
    char            m_recvBuffer[MAX_RECV_BUFFER];
    
    char            m_sOnlineIp[16];
    uint16_t        m_hOnlinePort;
    bool            m_bStartThread;
};

#endif
