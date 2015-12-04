//
//  XYHttpClient.cpp
//  XY
//
//  Created by long on 12-12-27.
//
//
#include "XYHttpClient.h"
#include "ServerConfig.h"

static XYHttpClient *s_HttpClient = NULL;

XYHttpClient *XYHttpClient::getInstance()
{
    if (s_HttpClient == NULL)
    {
        s_HttpClient = new XYHttpClient();
    }
    return s_HttpClient;
}

XYHttpClient::XYHttpClient(void)
{
    CCHttpClient::getInstance()->setTimeoutForConnect(HTTP_TIME_OUT_SEC);
    CCHttpClient::getInstance()->setTimeoutForRead(HTTP_TIME_OUT_SEC);
}

XYHttpClient::~XYHttpClient(void)
{
}

void XYHttpClient::send(CCHttpRequest* request)
{
    if (request == NULL)
    {
        return;
    }
    CCHttpClient::getInstance()->send(request);
}