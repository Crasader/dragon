//
//  CellButton.h
//  DragonBattle
//
//  Created by 何 永翔 on 13-5-29.
//
//

#ifndef DragonBattle_CellButton_h
#define DragonBattle_CellButton_h

#include "cocos2d.h"

class CellButton : public cocos2d::CCLayer
{
public:
    CellButton();
    ~CellButton();
    
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) { return; }
    
    bool initWithFile(const char* pNormalFile, CCNode* target, cocos2d::SEL_CallFuncND touchBegan, cocos2d::SEL_CallFuncND touchEnded);

    static CellButton* create(const char* pNormalFile, CCNode* target, cocos2d::SEL_CallFuncND touchBegan, cocos2d::SEL_CallFuncND touchEnded);
    static CellButton* create(uint32_t normalID, CCNode* target, cocos2d::SEL_CallFuncND touchBegan, cocos2d::SEL_CallFuncND touchEnded);
    
    bool getEnabled() { return m_isEnabled; }
    void setEnabled(bool isEnabled) { m_isEnabled = isEnabled; }
    
    bool appendItem(uint32_t nItemID);
    uint32_t swapItem(uint32_t nItemID);
    uint32_t removeItem();
    bool isEmpty() { return m_nItemID == 0; };
    
private:
    void touchBegan();
    void touchEnded();
    
private:
    cocos2d::CCSprite* m_pButtonSprite;
    const char* m_pNomalFile;
    
    CCNode* m_pTarget;
    cocos2d::SEL_CallFuncND   m_pTouchBegan;
    cocos2d::SEL_CallFuncND   m_pTouchEnded;
    
    bool m_isEnabled;
    uint32_t m_nItemID;
};

#endif
