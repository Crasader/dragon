//
//  BattleCDButton.h
//  DragonBattle
//
//  Created by 何 永翔 on 13-5-28.
//
//

#ifndef DragonBattle_BattleCDButton_h
#define DragonBattle_BattleCDButton_h

#include "cocos2d.h"
#include <string>

typedef bool (cocos2d::CCObject::*SEL_Callback)();

#define callback_selector(_SELECTOR) (SEL_Callback)(&_SELECTOR)

class BattleCDButton : public cocos2d::CCLayer
{
public:
    BattleCDButton();
    ~BattleCDButton();
    
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) { return; }
    
    bool initWithFile(const char* pNormalFile, const char* pSelFile, float fCD, CCObject* target, SEL_Callback touchBegan, SEL_Callback touchEnded);
    
    static BattleCDButton* create(const char* pFile, float fCD, CCObject* target, SEL_Callback touchBegan, SEL_Callback touchEnded);
    static BattleCDButton* create(const char* pNormalFile, const char* pSelFile, float fCD, CCObject* target, SEL_Callback touchBegan, SEL_Callback touchEnded);
    static BattleCDButton* create(uint32_t normalID, float fCD, CCObject* target, SEL_Callback touchBegan, SEL_Callback touchEnded);
    static BattleCDButton* create(uint32_t normalID, uint32_t selectedID, float fCD, CCObject* target, SEL_Callback touchBegan, SEL_Callback touchEnded);
    
    bool getEnabled() { return m_isEnabled; }
    void setEnabled(bool isEnabled) { m_isEnabled = isEnabled; }
    
private:
    void touchBegan();
    void touchEnded();
    void showProgressSprite();
    
private:
    cocos2d::CCSprite* m_pButtonSprite;
    cocos2d::CCProgressTimer* m_pProgressSprite;
    const char* m_pNomalFile;
    const char* m_pSelectedFile;
    
    CCObject* m_pTarget;
    SEL_Callback   m_pTouchBegan;
    SEL_Callback   m_pTouchEnded;
    
    bool m_isEnabled;
    int m_nProgressTag;
    float m_fCD;
};

#endif
