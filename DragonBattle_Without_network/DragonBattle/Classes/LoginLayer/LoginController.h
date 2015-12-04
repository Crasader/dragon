//
//  LoginManager.h
//  DragonBattle
//
//  Created by zeng xiaolong on 13-6-22.
//
//

#ifndef __DragonBattle__LoginManager__
#define __DragonBattle__LoginManager__
#include "cocos2d.h"
#include "LoginLayer.h"
USING_NS_CC;


#define FLOWER_TAG (100)

typedef enum
{
    NONELAYER = 0,
    UPDATE_RESC,
    REGIST,
    LOGIN,
    SELECTSERVER,
    CREATEROLE,
    SELECTROLE,
    GAME_START,
}LOGINLAYERTYPE;

class LoginController:public CCObject
{
public:
    LoginController();
    ~LoginController();
    static LoginController* sharedInstance();
    LoginLayer* creatLayer();
    void removeLoginLayer();
    CCLayer* pushLayer(LOGINLAYERTYPE type);
    void popLayer();
    CCLayer* getCurrentLayer();
    CCLayer *getLayerByType(LOGINLAYERTYPE type);
    
    CCDictionary* getLastSvrInfo();
private:
    CCLayer     *m_curLayer;
    LoginLayer *m_loginLayer;
    CCArray *m_arrLayers;
    static LoginController* pSharedInstance;
    
protected:
    CC_SYNTHESIZE_RETAIN(CCArray*, m_pSverList, SverList);
    CC_SYNTHESIZE(int, m_iLastedSver, LastedSver);
};

#endif /* defined(__DragonBattle__LoginManager__) */
