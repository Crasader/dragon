//
//  BattleUIButton.h
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-18.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_BattleUIButton_h
#define DragonBattle_BattleUIButton_h

#include "cocos2d.h"
#include <string>

class BattleUIButton : public cocos2d::CCLayer
{
public:
    BattleUIButton();
    ~BattleUIButton();
    
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) { return; }
    
    bool initWithFile(const char* pNormalFile, const char* pSelFile, CCObject* target, cocos2d::SEL_CallFunc touchBegan, cocos2d::SEL_CallFunc touchEnded);
    
    static BattleUIButton* create(const char* pFile, CCObject* target, cocos2d::SEL_CallFunc touchBegan, cocos2d::SEL_CallFunc touchEnded);
    static BattleUIButton* create(const char* pNormalFile, const char* pSelFile, CCObject* target, cocos2d::SEL_CallFunc touchBegan, cocos2d::SEL_CallFunc touchEnded);
    static BattleUIButton* create(uint32_t normalID, CCObject* target, cocos2d::SEL_CallFunc touchBegan, cocos2d::SEL_CallFunc touchEnded);
    static BattleUIButton* create(uint32_t normalID, uint32_t selectedID, CCObject* target, cocos2d::SEL_CallFunc touchBegan, cocos2d::SEL_CallFunc touchEnded);
    
    bool getEnabled() { return m_isEnabled; }
    void setEnabled(bool isEnabled) { m_isEnabled = isEnabled; }
    
private:
    void touchBegan();
    void touchEnded();
    void changeButtonSprite(bool bSel);
    
private:
    cocos2d::CCSprite* m_pButtonSprite;
    const char* m_pNomalFile;
    const char* m_pSelectedFile;
    
    CCObject* m_pTarget;
    cocos2d::SEL_CallFunc   m_pTouchBegan;
    cocos2d::SEL_CallFunc   m_pTouchEnded;
    
    bool m_isEnabled;
};

#endif
