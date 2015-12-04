//
//  XYHttpBridge.cpp
//  XY
//
//  Created by long on 12-12-29.
//
//

#include "XYHttpBridge.h"
#include "XYHttpClient.h"
#include "CRUtils.h"
#include "md5.h"
#include "XYTopLayer.h"
#include "ServerConfig.h"
#include "DealErrorCode.h"
#include "DictGenerator.h"

static XYHttpBridge *s_HttpBridge = NULL; //

XYHttpBridge::XYHttpBridge(void)
{

}

XYHttpBridge::~XYHttpBridge(void)
{
}

XYHttpBridge *XYHttpBridge::getInstance()
{
    if (s_HttpBridge == NULL)
    {
        s_HttpBridge = new XYHttpBridge();
    }
    return s_HttpBridge;
}

void XYHttpBridge::checkHaveBulletin(CCObject*pTarget,SEL_HttpResponse callfun)
{
    CCHttpRequest* request = new CCHttpRequest();
    request->setUrl(BULLETIN_URL);
    request->setRequestType(CCHttpRequest::kHttpGet);
    if (pTarget)
    {
        request->setResponseCallback(pTarget,callfun);
    }
    XYHttpClient::getInstance()->send(request);
    request->release();
}

void XYHttpBridge::sendData(CCDictionary *dicdata,CCObject*pTarget,SEL_HttpResponse callfun)
{
    string strurl = SERVER_LOGIN;
    strurl = strurl + getPostFields(dicdata);
    CCLOG("%s", strurl.c_str());
    CCHttpRequest* request = new CCHttpRequest();
    request->setUrl(strurl.c_str());
    request->setRequestType(CCHttpRequest::kHttpGet);
    if (pTarget)
    {
        request->setResponseCallback(pTarget,callfun);
    }
    XYHttpClient::getInstance()->send(request);
    request->release();
}

void XYHttpBridge::sendEncryData(CCDictionary *dicdata,CCObject*pTarget,SEL_HttpResponse callfun)
{
//    XYTopLayer::getInstance()->ShowWaiting();
//    dicdata->setObject(CCString::create(XYUserData::getInstance()->m_hashkey), "hashKey");
//    char str [128] = {0};
//    sprintf(str,"%d",XYUserData::getInstance()->m_clientid);
//    dicdata->setObject(CCString::create(str), "clientId");
//    dicdata->setObject(CCString::create(getMd5str(dicdata)), "sign");
//    sendData(dicdata, pTarget, callfun);
}

string XYHttpBridge::getPostFields(CCDictionary *dicdata)
{
    if (!dicdata)
    {
        return "";
    }
    string ret="";
    CCArray *keys = dicdata->allKeys();
    
    for( int i = 0; i<keys->count(); i++ )
    {
        CCString *key = (CCString*)keys->objectAtIndex(i);
        CCString *val = (CCString*)dicdata->objectForKey(key->m_sString);
        if (ret.length()>0)
        {
            ret.append("&");
        }
        ret.append(key->m_sString + "=" + val->m_sString);
    }
    return ret;
}

string XYHttpBridge::getMd5str(CCDictionary *dicdata)
{
    CCArray *arrkey = dicdata->allKeys();
    if (!arrkey)
    {
//        CRUtils::ShowToast("出错");
    }
    vector<string> strkeys;
    for (int i = 0; i < arrkey->count(); i++)
    {
        CCString *str = (CCString*)arrkey->objectAtIndex(i);
        strkeys.push_back(str->m_sString);
    }
    for (int i = 0; i < strkeys.size() - 1; i++)
    {
        for (int j = 0; j < strkeys.size() - i - 1; j++)
        {
            if(strkeys.at(j).compare(strkeys.at(j + 1)) > 0)
                strkeys.at(j).swap(strkeys.at(j + 1));
        }
    }
    for (int i = 0; i < strkeys.size(); i++)
    {
        CCLOG("%s",strkeys.at(i).c_str());
    }
    std::string s;
    std::string sr;
    for (int j = 0; j < strkeys.size(); j++)
    {
        s = s + strkeys.at(j) + dicdata->valueForKey(strkeys.at(j))->m_sString;
        sr = sr + strkeys.at(j) + "=" + dicdata->valueForKey(strkeys.at(j))->m_sString;
    }
    std::string str = MD5(s.c_str()).toString();
    str = str + MD5KEY;
    sr = sr + str;
    s = MD5(sr.c_str()).toString();
    return s;
}

CCDictionary* XYHttpBridge::parseRequest(int* result, cocos2d::extension::CCHttpResponse* response)
{
    XYTopLayer::getInstance()->RemoveWaiting();
    CCDictionary* pDict = NULL;
    if (response && response->isSucceed())
    {
        CCLog("response succeed!");
        std::string strContents;
        strContents.assign(response->getResponseData()->begin(), response->getResponseData()->end());
        cocos2d::CCLog("%s", strContents.c_str());
        
        CRUtils::writeToReadAbleFile("abcdefg.txt", strContents);
        
        pDict = DictGenerator::dictionaryFromJSON(strContents);
        if (pDict == NULL) //兼容以前
        {
            strContents = strContents.substr(1, strContents.length() - 2);
            pDict = DictGenerator::dictionaryFromJSON(strContents);
        }
        
        CC_SAFE_RETAIN(pDict);
        
        if (pDict)
        {
            *result = pDict->valueForKey("result")->intValue();
        }
        else
        {
            *result = cli_message_in_error;
        }
    }
    else
    {
        *result = SVR_NET_ERR;
    }
    
    if (*result != 0 && *result != 11900)
    {
        DealErrorCode::dealErrorFunc(*result);
    }
    
    return pDict;
}




