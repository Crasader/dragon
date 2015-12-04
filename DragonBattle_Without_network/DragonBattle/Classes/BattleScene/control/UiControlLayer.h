//
//  UiControlLayer.h
//  DragonBattle
//
//  Created by 小敦 王 on 13-5-23.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_UiControlLayer_h
#define DragonBattle_UiControlLayer_h

#include "cocos2d.h"

class UiControlLayer : public cocos2d::CCLayer
{
public:
    UiControlLayer();
    ~UiControlLayer();
    
    static UiControlLayer* create();
    bool init();
    
    void onShowAttackRect(CCObject* pSender);
    void onResetGame(CCObject* pSender);
    void onVictory(CCObject* pSender);
    void onFailure(CCObject* pSender);
    void onReturn(CCObject* pSender);
    void onClose(CCObject* pSender);
    
    // register touch dispatcher
    virtual void registerWithTouchDispatcher(void);
    
    // default implements are used to call script callback if exist
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
private:
    void createBtns();
    void createClose();
    
private:
    cocos2d::CCLabelTTF*    m_pShowLabel;
    bool                    m_bShowAttackRect;
};

#endif
