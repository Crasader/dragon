//
//  BattleUILayer.h
//  DragonBattle
//
//  Created by zeng xiaolong on 13-10-22.
//
//

#ifndef __DragonBattle__BattleUILayer__
#define __DragonBattle__BattleUILayer__
#include "cocos2d.h"
#include "XYScaleButton.h"
#include "XYToggleButton.h"

USING_NS_CC;

class  BattleOperateDelegate
{
public:
    virtual void directMoveOutside(){}
    virtual void directTouchEnd(){}
    virtual void onAttackTouchBegan(){}
    virtual void onAttackTouchEnded(){}
    virtual void onRollTouchBegan(CCNode *node){}
    virtual void onSkillTouchBegan(CCNode *node){}
    virtual void onAutoFightTouchBegan(CCNode *node){}
};

class  BattleUILayer:public CCLayer
{
public:
    BattleUILayer();
    ~BattleUILayer();
    
    virtual bool init();
    CREATE_FUNC(BattleUILayer);
    
    virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    
    bool getRunLeft()   { return m_bRunLeft;}
    bool getRunRight()  { return m_bRunRight;}
    void setDelegate(BattleOperateDelegate *delegate)
    {m_delegate = delegate;}
    cocos2d::CCSprite*  m_pLeftTouch;       //左
    cocos2d::CCSprite*  m_pRightTouch;      //右
    
    XYBattleButton*     m_pAttackBtn;       //攻击
    XYRollButton*       m_pRollBtn;
    XYRollButton*       m_pSkillBtn[4];        //技能
    XYToggleButton*     m_pBtnAutoFight;
    
protected:
    virtual void onAttackTouchBegan(CCNode *node);
    virtual void onAttackTouchEnded(CCNode *node);
    virtual void onRollTouchBegan(CCNode *node);
    virtual void onSkillTouchBegan(CCNode *node);
    virtual void onAutoFightTouchBegan(CCNode *node);
    BattleOperateDelegate *m_delegate;
    bool                m_bRunLeft;
    bool                m_bRunRight;
};

#endif /* defined(__DragonBattle__BattleUILayer__) */
