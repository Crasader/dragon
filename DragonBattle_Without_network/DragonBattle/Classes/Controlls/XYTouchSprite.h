//
//  XYTouchSprite.h
//  DragonBattle
//
//  Created by 何 永翔 on 13-12-4.
//
//

#ifndef __DragonBattle__XYTouchSprite__
#define __DragonBattle__XYTouchSprite__

#include "cocos2d.h"

class XYTouchSprite : public cocos2d::CCSprite, public cocos2d::CCTouchDelegate
{
public:
    XYTouchSprite(void);
    virtual ~XYTouchSprite(void);
    
    virtual bool initWithFile(const char * pFile, bool bIsHorizontal, int nPriority = 0);
    static XYTouchSprite * create(const char * pFile, bool bIsHorizontal, int nPriority = 0);
    
    
    CC_SYNTHESIZE(bool, m_isEnabled,Enable);
    void setPriority(int nPriority);//onEnter调用后才能设置
    CC_PROPERTY(int, m_iPriority,Proirity);
    CC_SYNTHESIZE(cocos2d::CCRect, m_TouchRect,TouchRect);
    virtual void touchBegin();
    virtual void touchEnd(bool bIsPositive);
    void setTouchBeganEvent(CCObject *obj, cocos2d::SEL_CallFuncN callfun){ m_pTarget = obj;m_pTouchBegan= callfun;}
    void setTouchEndEvent(CCObject *obj, cocos2d::SEL_CallFuncND callfun)
    {
        m_pTarget = obj;m_pTouchEnded = callfun;
    }
protected:
    cocos2d::CCObject* m_pTarget;
    cocos2d::SEL_CallFuncN  m_pTouchBegan;
    cocos2d::SEL_CallFuncND  m_pTouchEnded;
    std::string m_strPic;
    cocos2d::CCPoint m_moveVec;
    bool m_isHorizontal;
    cocos2d::CCPoint m_startPoi;
    cocos2d::CCLayerColor *m_pTouchRectLayer;
protected:
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
};

#endif /* defined(__DragonBattle__XYTouchSprite__) */
