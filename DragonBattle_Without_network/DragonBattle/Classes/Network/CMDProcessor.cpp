//
//  CMDProcessor.cpp
//  MyGame
//
//  Created by 永翔 何 on 12-9-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "CMDProcessor.h"
#include "btl_head.hpp"
#include "LogicManager.h"
static char  s_m_buf[MAX_RECV_BUFFER] = {0};
static std::vector<BuffData> buffVec;
static pthread_mutex_t  s_requestQueueMutex;
CMDProcessor* CMDProcessor::pSharedInstance = NULL;

CMDProcessor::CMDProcessor()
:m_bufLen(0)
{
    isReceive = false;
    m_type = E_PROCESS_CMD;
    memset(m_buf, 0, MAX_RECV_BUFFER);
}

CMDProcessor::~CMDProcessor()
{
    
}

CMDProcessor* CMDProcessor::sharedInstance()
{
    if(!pSharedInstance)
    {
        pSharedInstance = new CMDProcessor();
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
        schedule_selector(CMDProcessor::bufUpdate), pSharedInstance, 0, false);
        pthread_mutex_init(&s_requestQueueMutex, NULL);
#endif
    }
    return pSharedInstance;
}

void CMDProcessor::purgeInstance()
{
    if(pSharedInstance)
    {
        delete pSharedInstance;
        pSharedInstance = NULL;
    }
}

void CMDProcessor::bufUpdate(float ftime)
{
    pthread_mutex_lock(&s_requestQueueMutex);
    if (m_type == E_CONNECT_ONLINE)
    {
        LogicManager::shareLogicManager()->connectOnlineSuccess();
        m_type = E_PROCESS_CMD;
    }
    else if (m_type == E_CLOSE_ONLINE)
    {
        LogicManager::shareLogicManager()->setStopNetReason(E_SERVER_CLOSE);
        LogicManager::shareLogicManager()->onOnlineClosed();
        m_type = E_PROCESS_CMD;
    }
    else
    {
        if (isReceive)
        {
            for (std::vector< BuffData > ::iterator it = buffVec.begin(); it != buffVec.end(); ++ it)
            {
                BuffData buffdata  = * it;
                setEncodeBuffer(buffdata);
                receiveCmd();
            }
            buffVec.clear();
            memset(s_m_buf, 0, MAX_RECV_BUFFER);
            isReceive = false;
            
        }
    }
    pthread_mutex_unlock(&s_requestQueueMutex);
}

void CMDProcessor::setEncodeBuffer(BuffData buffdata)
{
    memset(m_buf, 0, MAX_RECV_BUFFER);
    memcpy(m_buf, s_m_buf + buffdata.m_index, buffdata.m_bufLen);
    m_bufLen = buffdata.m_bufLen;
}

void CMDProcessor::setListBuffer(int recvLen, const char* recvBuf)
{
    pthread_mutex_lock(&s_requestQueueMutex); //Get request task from queue
    isReceive = true;
    BuffData buffdata;
    if (buffVec.size() == 0)
    {
        buffdata.m_index = 0;
    }
    else
    {
        BuffData lastbuffdata = buffVec.at(buffVec.size() - 1);
        buffdata.m_index = lastbuffdata.m_index + lastbuffdata.m_bufLen;
    }
    buffdata.m_bufLen = recvLen;
    memcpy(s_m_buf+buffdata.m_index, recvBuf, recvLen);
    
    buffVec.push_back(buffdata);
    pthread_mutex_unlock(&s_requestQueueMutex);
}

void CMDProcessor::setConnectStatus(ENUM_PERFUNC_TYPE type)
{
    pthread_mutex_lock(&s_requestQueueMutex);
    if (m_type != E_PROCESS_CMD)
    {
        return;
    }
    m_type = type;
    pthread_mutex_unlock(&s_requestQueueMutex);
}
// description: set received protocal buffer
// parameter:   - recvLen   received protocal buffer length
//              - recvBuf   received protocal buffer pointer
void CMDProcessor::setBuffer(int recvLen, const char* recvBuf)
{
    m_bufLen = recvLen;
    memcpy(m_buf, recvBuf, recvLen);
}

// description: clear current buffer
void CMDProcessor::clearBuffer()
{
    m_bufLen = 0;
    memset(m_buf, 0, MAX_RECV_BUFFER);
}

// description: receive current cmd
void CMDProcessor::receiveCmd()
{
    // extract header
    cli_proto_head_t* head = (cli_proto_head_t*)m_buf;
    int headLen = sizeof(cli_proto_head_t);
    
    // TODO: intergrity check
    
    CCLOG("Received cmd %d", head->cmd);
    switch (head->cmd)
    {
        default:
        {
            OnlineManager::sharedManager()->process_Online_CMD(m_buf, m_bufLen - headLen);
            break;
        }
    }
    
    // clear current buffer
    this->clearBuffer();
}
