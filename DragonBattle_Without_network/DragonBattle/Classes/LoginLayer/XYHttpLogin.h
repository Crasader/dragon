//
//  XYHttpLogin.h
//  XY
//
//  Created by long on 13-1-28.
//
//

#ifndef __XY__XYHttpLogin__
#define __XY__XYHttpLogin__

#include "XYHttpBridge.h"
#include "XYTopLayer.h"

enum LOGIN_ACTION_TYPE
{
    kLOGIN_ACTION_TYPE_LOGIN = 1,
    kLOGIN_ACTION_TYPE_REGIST = 2,
    kLOGIN_ACTION_TYPE_SELECT_SEVER = 3,
    kLOGIN_ACTION_TYPE_CREATE_ROLE = 4,
    kLOGIN_ACTION_TYPE_SELECT_HERO = 5,
    kLOGIN_ACTION_TYPE_LOGIN_WITH_TOKEN = 6,
};

class XYHttpLogin:public CCObject
{
public:
    XYHttpLogin();
    ~XYHttpLogin();
    
    static XYHttpLogin *getInstance();
    
    void reTry();
    
    void reTryWithDelay(float dt);
    void reTry(LOGIN_ACTION_TYPE type);
    
    void regist(std::string name,std::string password,std::string email, bool retry = false);
    void registComplete(CCNode* pSender,CCHttpResponse* data);
    
    void login(std::string name,std::string password, bool retry = false);
    void loginComplete(CCNode* pSender,CCHttpResponse* data);
    
    void login91(std::string name);
    
    void selectServer(int serverid, const char* pStr, int nPort, bool retry = false);
    void selectComplete(CCNode* pSender,CCHttpResponse* data);
    
    void createRole(std::string nickname, std::string invitecode, int roletype, bool retry = false);
    void createComplete(CCNode* pSender,CCHttpResponse* data);
    
    void selectHero(int herotype, bool retry = false);
    void selectHeroComplete(CCNode* pSender,CCHttpResponse* data);
    
    std::string Ip;
    uint16_t Port;
    int m_Zoneid;
    
private:
        
    CCArray* getServerListArray(CCDictionary* pDict);
    
    int m_LoginAction;
    
    std::string m_arrName;
    std::string m_arrPassword;
    std::string m_arrEmail;
    
    std::string nickName;
    std::string inviteCode;
    int roleType;

};

#endif /* defined(__XY__XYHttpLogin__) */
