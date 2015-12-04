//
//  NetworkManager.cpp
//  MyGame
//
//  Created by 永翔 何 on 12-8-1.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "NetworkManager.h"
#include "UserData.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "NetUtil_C.h"
#include "IOSUtility.h"
#else
#include "IOSUtility.h"
#endif
#include "CMDProcessor.h"
#include "LogicManager.h"

NetworkManager* NetworkManager::pNetworkManager = NULL;
volatile uint8_t g_iConnected = 0;

NetworkManager::NetworkManager()
:m_csNetwork(NULL)
,m_thNetworkThread(NULL)
,m_recvLen(0)
,m_hOnlinePort(0)
{
    m_bStartThread = false;
    memset(m_recvBuffer, 0, MAX_RECV_BUFFER);
}

NetworkManager::~NetworkManager()
{
    if (m_csNetwork)
    {
        m_csNetwork->CancelSocket();
        delete m_csNetwork;
        m_csNetwork = NULL;
    }
    
    if (m_thNetworkThread)
    {
        delete m_thNetworkThread;
        m_thNetworkThread = NULL;
    }
    
    memset(m_sOnlineIp, 0, sizeof(m_sOnlineIp));
    m_hOnlinePort = 0;
}

NetworkManager* NetworkManager::shareNetworkManager()
{
    if (pNetworkManager == NULL)
    {
        pNetworkManager = new NetworkManager;
    }
    return pNetworkManager;
}

void NetworkManager::purgeNetworkManager()
{
    if (pNetworkManager)
    {
        delete pNetworkManager;
        pNetworkManager = NULL;
    }
}

bool NetworkManager::init()
{
    CMDProcessor::sharedInstance()->clearBuffer();
    g_iConnected = E_UNCONNECTED;
    m_hOnlinePort = 0;
    return true;
}

void NetworkManager::run()
{
    CCLOG("---------------------NetworkManager::begin working--------------------------");
    while (true)
    {
        CCLOG("m_bStartThread=%d g_iConnected == %d", m_bStartThread, g_iConnected);
        while (m_bStartThread && g_iConnected == E_UNCONNECTED && this->isGotOnline())
        {
            int ret = this->connectToOnline();
            if (ret != 0)
            {
                continue;
            }
            else// connect online succ
            {
                g_iConnected = E_ONLINE_CONNECTED;
                //设置非阻塞模式
                m_csNetwork->SetBlockMode(1);
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
                CMDProcessor::sharedInstance()->setConnectStatus(E_CONNECT_ONLINE);
#else
                performFuncOnMainThread(E_CONNECT_ONLINE);
#endif
                
            }
            continue;
        }
        
        CCLOG("---------------------NetworkManager::recv--------------------------");
        while (m_bStartThread && g_iConnected && isDeviceNetConnect())
        {
            if (m_bCancel)
            {
                m_bStartThread = !m_bCancel;
                break;
            }
            
            int protoLen = 0;
            if (m_csNetwork && m_csNetwork->DataArrival(ONLINE_ARRIVAL_TIME) == 1)
            {
                //接受数据
                int len = recv(m_csNetwork->getSocketID(), m_recvBuffer + m_recvLen, MAX_RECV_BUFFER, 0);
                while (len != -1 && len != 0 && g_iConnected)
                {
                    m_recvLen += len;
                    len = recv(m_csNetwork->getSocketID(), m_recvBuffer + m_recvLen, MAX_RECV_BUFFER, 0);
                }

                if (len == -1)//接受完成
                {
                    if (errno != EAGAIN && errno != EINTR)
                    {
                        CCLOG("网络出错!");
                        break;
                    }
                    
                    bool proto_error = false;
                    while (m_recvLen >= sizeof(cli_proto_head_t))
                    {
                        cli_proto_head_t* head = (cli_proto_head_t*)(m_recvBuffer);
                        protoLen = head->len;
                        if (protoLen >= MAX_RECV_BUFFER || protoLen < sizeof(cli_proto_head_t))
                        {
                            proto_error = true;
                            CCLOG("接受数据出错，请检查协议长度");
                            break;
                        }
                        
                        if (m_recvLen >= protoLen && protoLen != 0 && g_iConnected)//够一个协议
                        {
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
                            CMDProcessor::sharedInstance()->setListBuffer(protoLen, m_recvBuffer);//跨平台处理方式
                            CMDProcessor::sharedInstance()->setConnectStatus(E_PROCESS_CMD);
#else
                            CMDProcessor::sharedInstance()->setBuffer(protoLen, m_recvBuffer);
                            performFuncOnMainThread(E_PROCESS_CMD);
#endif
                            if (m_recvLen > protoLen)
                            {
                                memmove(m_recvBuffer, m_recvBuffer + protoLen, m_recvLen - protoLen);
                                m_recvLen = m_recvLen - protoLen;
                            }
                            else
                            {
                                m_recvLen = 0;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                    if (proto_error)
                    {
                        break;
                    }
                }
                else if (len == 0)  //服务器socket关闭
                {
                    CCLOG("server socket close!");
                    break;
                }
            }
        }
        
        //网络断开了 则关闭socket
        CCLOG("---------------close recv-----------------");
        disConnect();
        if (!m_bStartThread)
        {
            break;
        }
    }
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    CMDProcessor::sharedInstance()->setConnectStatus(E_CLOSE_ONLINE);
#else
    performFuncOnMainThread(E_CLOSE_ONLINE);
#endif
    return ;
}

void NetworkManager::startThread()
{
    createOnlineSocket();
    if (m_thNetworkThread == NULL)
    {
        m_thNetworkThread = new Thread(this);
    }
    m_bStartThread = true;
    m_thNetworkThread->start();
    return ;
}

void NetworkManager::createOnlineSocket()
{
    if (g_iConnected)
    {
        return ;
    }
    
    m_csNetwork = new CSocketUtil();
    m_csNetwork->CreateSocket();
    m_recvLen = 0;
    if (m_thNetworkThread)
    {
        delete m_thNetworkThread;
        m_thNetworkThread = NULL;
    }
    
    this->m_bCancel = false;
    
    m_thNetworkThread = new Thread(this);
}

int NetworkManager::connectToOnline()
{
    return connect(m_sOnlineIp, m_hOnlinePort, ONLINE_SOCKET_TIMEOUT, E_ONLINE_CONNECTED);
}

int NetworkManager::connect(char* ip, int port, int timeout, uint8_t type)
{
    if (!isDeviceNetConnect())
    {
        return -1;
    }
    
    if (g_iConnected != E_UNCONNECTED)
    {
        return 0;
    }
    
    if (m_csNetwork == NULL)
    {
        m_csNetwork = new CSocketUtil();
        m_csNetwork->CreateSocket();
    }
    m_recvLen = 0;

    signal(SIGPIPE, SIG_IGN);
    if (m_csNetwork->ConnectTo(ip, port, timeout) == 0)//connect succ
    {
        g_iConnected = type;
        return 0;
    }
    else
    {
        if (m_csNetwork)
        {
            delete m_csNetwork;
            m_csNetwork = NULL;
        }
        g_iConnected = E_UNCONNECTED;
    }

    return -1;
}

void NetworkManager::disConnect()
{
    if (m_csNetwork)
    {
        delete m_csNetwork;
        m_csNetwork = NULL; 
    }
    
    g_iConnected = E_UNCONNECTED;
    m_recvLen = 0;
    m_bStartThread = false; //关闭进程
}

/*
 * @brief close Online socket
 */
void NetworkManager::disConnectOnline()
{
    if (g_iConnected == E_ONLINE_CONNECTED)
    {
        g_iConnected = E_UNCONNECTED;
        sleep(1);
    }
}

void NetworkManager::closeConnect()
{
    g_iConnected = E_UNCONNECTED;
    m_recvLen = 0;
    //m_hOnlinePort = 0;
//    waitForThreadOver();
    m_bStartThread = false;
//    sleep(1);
    
    //等待线程关闭socket
    //while (m_csNetwork)
    {
    }
}

void NetworkManager::close()
{
    m_bStartThread = false;
    g_iConnected = E_UNCONNECTED;
    m_csNetwork->CancelSocket();
}

int NetworkManager::sendDataToOnline(int seqNum, int cmd, Cmessage* pMsg, uint32_t userid)
{         
    //如果没有得到online
    if (!this->isGotOnline())
    {
        return -1;
    }
    
    if (g_iConnected == E_UNCONNECTED)
    {
        //if (this->connectToOnline() != 0)
        {
            CCLOG("连接online失败！...");
            return -1;
        }
    }
    
    return sendData(seqNum, cmd, pMsg, userid);
}

int NetworkManager::sendData(int pSeqNum, int cmd, Cmessage* pMsg, uint32_t userid)
{
    //发送前检查
    if (m_csNetwork == NULL || g_iConnected == E_UNCONNECTED)
    {
        g_iConnected = E_UNCONNECTED;
        return 0;
    }

    cli_proto_head_t head;
    head.len        = sizeof(head);
    head.cmd        = cmd;
    head.user_id    = UserData::sharedInstance()->userid;
    if (userid != 0)
    {
        head.user_id = userid;
    }
    head.seq_num    = pSeqNum;
    head.ret        = 0;
    
    CCLOG("send to Online: userid[%d] cmd[%d]", head.user_id, cmd);
    
    uint32_t headLen = head.len;
    int ret = 0;
        
    if(pMsg == NULL)
    {
        //write(m_csNetwork->getSocketID(), (const void*)(&head), head.len);
        ret = m_csNetwork->SendSocket((char*)&head, head.len, 0);
    }
    else
    {
        // add msg length!
        //head.len += pMsg->get_msg_len();
        
        // initialize byte array
        static byte_array_t ba;
        ba.init_postion();
        
        // write head
        ba.write_buf((const char*)&head, headLen);
        
        // write msg
        pMsg->write_to_buf(ba);
        
        *(uint32_t*)(ba.get_buf()) = ba.get_postion();

        // send
        //write(m_csNetwork->getSocketID(), ba.get_buf(), ba.get_postion());
        //long size = send(m_csOnline->getSocketID(), ba.get_buf(), ba.get_bufsize(),ba.get_postion());
        //CCLOG("send size: %ld", size);
        ret = m_csNetwork->SendSocket((char*)(ba.get_buf()), ba.get_postion(), 0);
    }
    
    // 如果发送不成功，则socket断掉了
    if (ret <= 0)
    {
        g_iConnected = E_UNCONNECTED;
    }
    return ret;
}

/*
 * @brief 得到online的IP和port后 断开Login的连接
 */
void NetworkManager::setOnlineIpAndPort(const char* ip, uint16_t port)
{
    //port = 5201;
    memcpy(this->m_sOnlineIp, ip, sizeof(m_sOnlineIp));
    m_hOnlinePort = port;
}

bool NetworkManager::isOnlineConnected()
{
    return g_iConnected == E_ONLINE_CONNECTED;
}
