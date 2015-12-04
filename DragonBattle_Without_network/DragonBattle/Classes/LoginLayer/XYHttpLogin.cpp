//
//  XYHttpLogin.cpp
//  XY
//
//  Created by long on 13-1-28.
//
//

#include "XYHttpLogin.h"
#include "CRUtils.h"
#include "UserData.h"
#include "LoginController.h"
#include "CCBSelectServerLayer.h"
#include "GameManager.h"
#include "LogicManager.h"
#include "CCBSelectRoleLayer.h"
#include "DictGenerator.h"
#include "XYString.h"

#include "md5.h"
#include "CommonUtility.h"

#define DELAY_SEC (0.5f)

static XYHttpLogin *s_HttpLogin = NULL; //

XYHttpLogin::XYHttpLogin() : m_LoginAction(0)
{
}

XYHttpLogin::~XYHttpLogin()
{
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(XYHttpLogin::reTryWithDelay), this);
}

XYHttpLogin *XYHttpLogin::getInstance()
{
    if (s_HttpLogin == NULL)
    {
        s_HttpLogin = new XYHttpLogin();
    }
    
    return s_HttpLogin;
}

void XYHttpLogin::login91(string name)
{
    m_LoginAction = kLOGIN_ACTION_TYPE_LOGIN_WITH_TOKEN;
    
    XYTopLayer::getInstance()->ShowWaiting();
    
    XYHttpLogin::getInstance()->m_arrName = name;
    
    CCDictionary *dicdata = CCDictionary::create();
    char session[33] = {0};
    int cur_time = time(0);
    
    char text[200] = {0};
    const char* signStyle = "PLatfROm uSEr GeT sVR LisT WitH?nAMe=%s&TIMe=%d";
    //这儿先对密码进行一次md5
    sprintf(text, signStyle, name.c_str(), cur_time);
    memcpy(session, MD5(MD5(text).toString().c_str()).toString().c_str(), 32);
    CCLOG("%s", MD5(MD5(text).toString().c_str()).toString().c_str());
    session[32] = '\0';
    dicdata->setObject(CCString::create("205"), "cmdid");
    dicdata->setObject(CCString::create(name), "username");
    dicdata->setObject(CCString::createWithFormat("%d", cur_time), "time");
    dicdata->setObject(CCString::create(session), "session");
    XYHttpBridge::getInstance()->sendData(dicdata, XYHttpLogin::getInstance(), httpresponse_selector(XYHttpLogin::loginComplete));
}

void XYHttpLogin::login(string name,string password, bool retry/* = false*/)
{
    XYTopLayer::getInstance()->ShowWaiting();
    
    m_LoginAction = kLOGIN_ACTION_TYPE_LOGIN;
    
    XYHttpLogin::getInstance()->m_arrName = name;
    XYHttpLogin::getInstance()->m_arrPassword = password;
    
    
    CCDictionary *dicdata = CCDictionary::create();
    dicdata->setObject(CCString::create("201"), "cmdid");
    dicdata->setObject(CCString::create(name), "username");
    
    if (retry)
    {
        dicdata->setObject(CCString::create("true"), "retry");
    }
    
    char session[33] = {0};
    int cur_time = time(0);
    
    char text[200] = {0};
    const char* signStyle = "User LOgiN Into gaMe WitH?nAMe=%s&pASSwd=%s&tIMe=%d";
    //这儿先对密码进行一次md5
    sprintf(text, signStyle, name.c_str(), MD5(password).toString().c_str(), cur_time);
    memcpy(session, MD5(text).toString().c_str(), 32);
    CCLOG("%s", MD5(text).toString().c_str());
    session[32] = '\0';
    dicdata->setObject(CCString::create(session), "passwd");
    dicdata->setObject(CCString::createWithFormat("%d", cur_time), "time");
    
    XYHttpBridge::getInstance()->sendData(dicdata, XYHttpLogin::getInstance(), httpresponse_selector(XYHttpLogin::loginComplete));
}

string& replace_all_distinct(string& str,const string& old_value,const   string& new_value)
{
    for(string::size_type pos(0);pos!=string::npos;pos+=new_value.length())   {
        if(   (pos=str.find(old_value,pos))!=string::npos)
            str.replace(pos,old_value.length(),new_value);
        else
            break;
    }
    return   str;
}

CCArray* XYHttpLogin::getServerListArray(CCDictionary* pDict)
{
    CCArray *arrServerList = CCArray::create();
    
    for (int i = 0; i < pDict->valueForKey("Count")->intValue(); i++)
    {
        char str[64] = {0};
        CCDictionary *server = CCDictionary::create();
        sprintf(str, "zone_id_%d",i);
        if (pDict->objectForKey(str)) server->setObject((XYString*)pDict->objectForKey(str), "zone_id");
        
        sprintf(str, "zone_name_%d",i);
        if (pDict->objectForKey(str)) server->setObject((XYString*)pDict->objectForKey(str), "zone_name");
        
        sprintf(str, "zone_ip_%d",i);
        if (pDict->objectForKey(str)) server->setObject((XYString*)pDict->objectForKey(str), "zone_ip");
        
        sprintf(str, "zone_port_%d",i);
        if (pDict->objectForKey(str)) server->setObject((XYString*)pDict->objectForKey(str), "zone_port");
        
        sprintf(str, "zone_state_%d",i);
        if (pDict->objectForKey(str)) server->setObject((XYString*)pDict->objectForKey(str), "zone_state");
        
        sprintf(str, "role_type_%d", i);
        if (pDict->objectForKey(str)) server->setObject((XYString*)pDict->objectForKey(str), "role_type");
        
        sprintf(str, "role_level_%d", i);
        if (pDict->objectForKey(str)) server->setObject((XYString*)pDict->objectForKey(str), "role_level");
        
        arrServerList->addObject(server);
    }
    return arrServerList;
}

void XYHttpLogin::loginComplete(CCNode* pSender,CCHttpResponse* data)
{
    CCHttpResponse* response = (CCHttpResponse*)data;
    
    CCDictionary* pDict = NULL;
    int result = 0;
    pDict = XYHttpBridge::parseRequest(&result, response);
    CC_SAFE_RETAIN(pDict);
    
    if (result == 0)
    {
        if (pDict)
        {
            if (pDict->objectForKey("user_id"))
            {
                UserData::sharedInstance()->userid = pDict->valueForKey("user_id")->intValue();
            }
            
            if (pDict->objectForKey("last_zone_id"))
            {
                LoginController::sharedInstance()->setLastedSver(pDict->valueForKey("last_zone_id")->intValue());
            }
            
            LoginController::sharedInstance()->setSverList(getServerListArray(pDict));
        }
        
        if (LoginController::sharedInstance()->getLastedSver() != -1)
        {
            LoginController::sharedInstance()->pushLayer(GAME_START);
        }
        else
        {
            LoginController::sharedInstance()->pushLayer(GAME_START);
            LoginController::sharedInstance()->pushLayer(SELECTSERVER);
        }
    }
    else if (result == 11900)
    {
        this->reTry();
    }
    
    CC_SAFE_RELEASE_NULL(pDict);
}

void XYHttpLogin::regist(string name,string password,string email, bool retry/* = false*/)
{
    XYTopLayer::getInstance()->ShowWaiting();
    
    m_LoginAction = kLOGIN_ACTION_TYPE_REGIST;
    
    XYHttpLogin::getInstance()->m_arrName = name;
    XYHttpLogin::getInstance()->m_arrPassword = password;
    XYHttpLogin::getInstance()->m_arrEmail = email;
    
    
    CCDictionary *dicdata = CCDictionary::create();
    dicdata->setObject(CCString::create("200"), "cmdid");
    dicdata->setObject(CCString::create(name), "username");
    
    if (retry)
    {
        dicdata->setObject(CCString::create("true"), "retry");
    }
    
    int cur_time = time(0);
    char passwd_sess[33] = {0};
    memcpy(passwd_sess, MD5(password).toString().c_str(), 32);
    //先对密码进行一次md5
    dicdata->setObject(CCString::create(passwd_sess), "passwd");
    dicdata->setObject(CCString::create(email), "email");
    dicdata->setObject(CCString::createWithFormat("%d", cur_time), "time");
    XYHttpBridge::getInstance()->sendData(dicdata,XYHttpLogin::getInstance(),httpresponse_selector(XYHttpLogin::registComplete));
}

void XYHttpLogin::registComplete(CCNode* pSender,CCHttpResponse* data)
{
    CCHttpResponse* response = (CCHttpResponse*)data;
    int result = 0;
    CCDictionary* pDict = XYHttpBridge::parseRequest(&result, response);
    if (result == 0)
    {
        //许翔：如果注册成功，默认记住用户名及密码
        CCUserDefault::sharedUserDefault()->setStringForKey("UserName", m_arrName);
        CCUserDefault::sharedUserDefault()->setBoolForKey("IsPasswordSave", true);
        CCUserDefault::sharedUserDefault()->setStringForKey("Password", m_arrPassword);
        
        if (pDict)
        {
            if (pDict->objectForKey("user_id"))
            {
                UserData::sharedInstance()->userid = pDict->valueForKey("user_id")->intValue();
            }
            
            if (pDict->objectForKey("last_zone_id"))
            {
                LoginController::sharedInstance()->setLastedSver(pDict->valueForKey("last_zone_id")->intValue());
            }
            
            LoginController::sharedInstance()->setSverList(getServerListArray(pDict));
        }
        
        LoginController::sharedInstance()->pushLayer(SELECTSERVER);
        
    }
    else if (result == 11900)
    {
        this->reTry();
    }
}

void XYHttpLogin::selectServer(int serverid, const char* pStr, int nPort, bool retry/* = false*/)
{
    m_LoginAction = kLOGIN_ACTION_TYPE_SELECT_SEVER;

    XYHttpLogin::getInstance()->Ip = pStr;
    XYHttpLogin::getInstance()->Port = nPort;
    XYHttpLogin::getInstance()->m_Zoneid = serverid;
    
    UserData::sharedInstance()->server_id = serverid;
    
    XYTopLayer::getInstance()->ShowWaiting();
    char str[128] = {0};
    sprintf(str, "%d", serverid);
    CCDictionary *dicdata = CCDictionary::create();
    dicdata->setObject(CCString::create(str), "zone_id");
    dicdata->setObject(CCString::create("202"), "cmdid");
    dicdata->setObject(CCString::createWithFormat("%d",UserData::sharedInstance()->userid), "user_id");
    
    if (retry)
    {
        dicdata->setObject(CCString::create("true"), "retry");
    }
    
    int cur_time = time(0);
    char session[33] = {0};
    char text[200] = {0};
    const char* signStyle = "USEr_sELecT+ZonE_WiTh?uSEriD=%d&zoNeId=%d&TIMe=%d";
    sprintf(text, signStyle, gUserData->userid, serverid, cur_time);
    memcpy(session, MD5(text).toString().c_str(), 32);
    session[32] = 0;
    dicdata->setObject(CCString::create(session), "session");
    dicdata->setObject(CCString::createWithFormat("%d", cur_time), "time");
    XYHttpBridge::getInstance()->sendData(dicdata, XYHttpLogin::getInstance(), httpresponse_selector(XYHttpLogin::selectComplete));
}

void XYHttpLogin::selectComplete(CCNode* pSender, CCHttpResponse* data)
{
    CCHttpResponse* response = (CCHttpResponse*)data;
    
    int result = 0;
    CCDictionary* pDict = XYHttpBridge::parseRequest(&result, response);
    
    if (result == 0)
    {
        if (pDict)
        {
            if(0 == pDict->valueForKey("role_count")->intValue())
            {
                LoginController::sharedInstance()->pushLayer(CREATEROLE);
            }
            else
            {
                gUserData->userid = pDict->valueForKey("userid_0")->intValue();
                XYTopLayer::getInstance()->ShowWaiting();
                LogicManager::shareLogicManager()->startNetWork(XYHttpLogin::getInstance()->Ip.c_str(), XYHttpLogin::getInstance()->Port);
            }
        }
        
    }
    else if (result == 11900)
    {
        this->reTry();
    }
}

void XYHttpLogin::createRole(std::string nickname, std::string invitecode, int roletype, bool retry/* = false*/)
{
    m_LoginAction = kLOGIN_ACTION_TYPE_CREATE_ROLE;
    
    XYTopLayer::getInstance()->ShowWaiting();
    
    CCDictionary *dicdata = CCDictionary::create();
    dicdata->setObject(CCString::create("203"), "cmdid");
    dicdata->setObject(CCString::createWithFormat("%d", XYHttpLogin::getInstance()->m_Zoneid), "zone_id");
    dicdata->setObject(CCString::createWithFormat("%d", UserData::sharedInstance()->userid), "user_id");
    dicdata->setObject(CCString::createWithFormat("%d", roletype), "role_type");
    dicdata->setObject(CCString::create(nickname), "role_nick");
    
    this->nickName = nickname;
    this->inviteCode = invitecode;
    this->roleType = roletype;
    
    
    if (invitecode.length() != 0)
    {
        dicdata->setObject(CCString::create(invitecode), "invite_code");
    }
    
    if (retry)
    {
        dicdata->setObject(CCString::create("true"), "retry");
    }
    
    int cur_time = time(0);
    char session[33] = {0};
    char text[200] = {0};
    const char* signStyle = "usER_CreATe(RolE#wiTH?UsERId=%d&ZonEID=%d&rOLeTyPE=%d&roleNICk=%s&TIMe=%d";
    sprintf(text, signStyle, gUserData->userid, XYHttpLogin::getInstance()->m_Zoneid, roletype, nickname.c_str(), cur_time);
    memcpy(session, MD5(text).toString().c_str(), 32);
    session[32] = 0;
    dicdata->setObject(CCString::create(session), "session");
    dicdata->setObject(CCString::createWithFormat("%d", cur_time), "time");
    XYHttpBridge::getInstance()->sendData(dicdata, XYHttpLogin::getInstance(), httpresponse_selector(XYHttpLogin::createComplete));
}

void XYHttpLogin::createComplete(CCNode* pSender,CCHttpResponse* data)
{
    CCHttpResponse* response = (CCHttpResponse*)data;
    
    int result = 0;
    CCDictionary* pDict = XYHttpBridge::parseRequest(&result, response);
    if(result == 0)
    {
        UserData::sharedInstance()->userid = pDict->valueForKey("user_id")->intValue();
        LogicManager::shareLogicManager()->startNetWork(Ip.c_str(), Port);
    }
    else if (result == 11900)
    {
        this->reTry();
    }
}

void XYHttpLogin::reTry()
{
    XYTopLayer::getInstance()->ShowWaiting();
    
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(XYHttpLogin::reTryWithDelay), this);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(XYHttpLogin::reTryWithDelay), this, 0, 0, DELAY_SEC, false);
}

void XYHttpLogin::reTryWithDelay(float dt)
{
    reTry((LOGIN_ACTION_TYPE)XYHttpLogin::getInstance()->m_LoginAction);
}

void XYHttpLogin::reTry(LOGIN_ACTION_TYPE type)
{
    switch (type) {
        case kLOGIN_ACTION_TYPE_LOGIN:
        {
            login(m_arrName, m_arrPassword, true);
            break;
        }
        case kLOGIN_ACTION_TYPE_SELECT_SEVER:
        {
            selectServer(m_Zoneid, Ip.c_str(), Port, true);
            break;
        }
        case kLOGIN_ACTION_TYPE_REGIST:
        {
            regist(m_arrName, m_arrPassword, m_arrEmail, true);
            break;
        }
        case kLOGIN_ACTION_TYPE_CREATE_ROLE:
        {
            createRole(nickName, inviteCode, roleType);
            break;
        }
        case kLOGIN_ACTION_TYPE_LOGIN_WITH_TOKEN:
        {
            login91(m_arrName);
            break;
        }
        default:
            break;
    }
}