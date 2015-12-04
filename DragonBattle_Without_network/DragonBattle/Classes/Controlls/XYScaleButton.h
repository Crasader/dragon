//
//  XYScaleButton.h
//  DragonBattle
//
//  Created by 小敦 王 on 13-9-2.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_XYScaleButton_h
#define DragonBattle_XYScaleButton_h

#include "XYButton.h"

class XYScaleButton : public XYButton
{
public:
    XYScaleButton(void);
    ~XYScaleButton(void);
    static XYScaleButton * create(const char * pFile, int nPriority = 0);
    static XYScaleButton * createWithFileAndPriority(const char * pNormalFile, const char *pSelectFile, const char *pDisableFile, int nPriority);
    
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    void touchBegin();
    virtual void setEnabled(bool isEnabled);

    void setScaleValue(float fScale);
protected:
    float m_fScaleValue;
    bool m_bWaitCD;
    float m_fCD;
    float m_fTimerCounter;
private:
    cocos2d::CCProgressTimer* m_pProgressSprite;
    cocos2d::CCLabelAtlas* m_pLblTimerCounter;
    
    void updateTimer(float dt);
public:
    float getCD();
    void setCD(float var, bool showTime = false);
    bool isWaitCD() { return m_bWaitCD; }
    
    void showProgressSprite();
    void removeCDImg();
};

class XYBattleButton:public XYScaleButton
{
public:
    static XYBattleButton * create(const char * pFile, int nPriority = 0);
    static XYBattleButton * createWithFileAndPriority(const char * pNormalFile, const char *pSelectFile, const char *pDisableFile, int nPriority);
    
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
};

class XYRollButton:public XYScaleButton
{
public:
    static XYRollButton * createWithFileAndPriority(const char * pNormalFile, const char *pSelectFile, const char *pDisableFile, int nPriority);
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    void touchBegin();
};
#endif
