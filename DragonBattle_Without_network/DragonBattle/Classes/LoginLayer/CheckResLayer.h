//
//  CheckResLayer.h
//  DragonBattle
//
//  Created by 白明江 on 12/11/13.
//
//

#ifndef __DragonBattle__CheckResLayer__
#define __DragonBattle__CheckResLayer__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "TouchLayer.h"
#include "GlobalDefine.h"

#include <dirent.h>
#include <sys/stat.h>

#include "NetResourceManager.h"
#include "ResourceDelegate.h"
#include "LoginController.h"

USING_NS_CC;
USING_NS_CC_EXT;


class CheckResLayer : public TouchLayer, public ResourceDelegate
{
private:
    std::string pathToSave;
    bool m_bCanTouch;
    
private:
    CCLabelTTF* m_pProgressLabel;
    NetResourceManager* m_pResourceManager;
    
    CCProgressTimer* m_pProgressTimer;
    
    
public:
    
    CheckResLayer();
    virtual ~CheckResLayer();
    
    CREATE_FUNC(CheckResLayer);
    
    bool init()
    {
        bool bRet = CCLayer::init();
        if (bRet)
        {
            this->setPosition(SCREEN_MID_POS);
            createDownloadedDir();
            loadView();
        }
        return bRet;
    }
    
    void loadView();
    void createDownloadedDir();
    
    void updateResource();
    
public:
    virtual void onEnter();
    
    virtual void onError(ErrorCode errorCode);
    virtual void onProgress(int percent);
    virtual void onSuccess();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
    {
        if (m_bCanTouch)
        {
            LoginController::sharedInstance()->pushLayer(LOGIN);
            removeFromParent();
            return true;
        }
        else
        {
            return false;
        }
        
    }
    
};

#endif /* defined(__DragonBattle__CheckResLayer__) */
