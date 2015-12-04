//
//  XYBaseTouchSprite.h
//  DragonBattle
//
//  Created by zeng xiaolong on 13-6-3.
//
//

#ifndef __DragonBattle__XYBaseTouchSprite__
#define __DragonBattle__XYBaseTouchSprite__
#include "cocos2d.h"
#include "CRUtils.h"
USING_NS_CC;
class BYGraySprite : public CCSprite{
    
public:
    BYGraySprite();
    virtual ~BYGraySprite();
    static BYGraySprite* create(const char* pszFileName);
    bool initWithGrayFile( const char* pszFilename);
    virtual void draw();
    virtual void update(float time);
    int timeUniformLocation;
    float totalTime;

};

class XYBaseTouchSprite : public CCSprite,public CCTouchDelegate
{
public:
    XYBaseTouchSprite(void);
    virtual ~XYBaseTouchSprite(void);
    
    virtual bool initWithFile(const char * pFile,int nPriority = 0);
    static XYBaseTouchSprite * create(const char * pFile,int nPriority = 0);
    
    
    CC_SYNTHESIZE(bool, m_isEnabled,Enable);
    void setPriority(int nPriority);//onEnter调用后才能设置
    CC_PROPERTY(int, m_iPriority,Proirity);
    CC_SYNTHESIZE(CCRect, m_TouchRect,TouchRect);
    virtual void touchBegin();
    virtual void touchEnd();
    void setTouchBeganEvent(CCObject *obj,SEL_CallFuncN callfun){ m_pTarget = obj;m_pTouchBegan= callfun;}
    void setTouchEndEvent(CCObject *obj,SEL_CallFuncN callfun)
    {
        m_pTarget = obj;m_pTouchEnded = callfun;
    }
protected:
    CCObject* m_pTarget;
    cocos2d::SEL_CallFuncN  m_pTouchBegan;
    cocos2d::SEL_CallFuncN  m_pTouchEnded;
    std::string m_strPic;
    bool m_isMove;
    CCPoint m_startPoi;
    CCLayerColor *m_pTouchRectLayer;
protected:    
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
};
#endif
