//
//  BattleUIComponent.h
//  DragonBattle
//
//  Created by 小敦 王 on 13-5-22.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_BattleUIComponent_h
#define DragonBattle_BattleUIComponent_h

#include "cocos2d.h"
USING_NS_CC;
class BattleUnitSprite;

class HpBarBase:public CCSprite
{
public:
    HpBarBase();
    ~HpBarBase();
    
    void resetHpBar();
    virtual void setBar(int curhp,int maxhp){}
    virtual void attachUnit(BattleUnitSprite* pUnit);
    void detachUnit();
    BattleUnitSprite* getAttackUnit() { return m_pAttachUnit; }
    void hideSelf(float dt);

    cocos2d::CCSprite*      m_pHeadSprite;      //头像
    cocos2d::CCLabelTTF*    m_pUnitLv;        //名字
    cocos2d::CCLabelTTF*    m_pUnitName;        //名字
    BattleUnitSprite*       m_pAttachUnit;      //单位
    int m_nCurHp;
};

class HpBarLayer:public HpBarBase
{
public:
    HpBarLayer();
    ~HpBarLayer();
    
    static HpBarLayer* create(int unitType1);
    bool init(int unitType);
    static HpBarLayer* createArena(int unitType);
    bool initArena(int unitType);
    void loadLayer();
    void setMp(float mp,int maxMp);
    void setBarFlipX(bool bflipx);
    virtual void attachUnit(BattleUnitSprite* pUnit);
    virtual void setBar(int curhp,int maxhp);
protected:
    cocos2d::CCProgressTimer*   m_pMpBar;       //怒气
    cocos2d::CCProgressTimer*   m_pMpAddBar;       //add怒气
    cocos2d::CCProgressTimer*   m_pHpBar;       //血条
};

class PartnerHpBarLayer:public HpBarBase
{
public:
    static PartnerHpBarLayer* create(int unitType1);
    bool init(int unitType);
    virtual void setBar(int curhp,int maxhp);
    cocos2d::CCProgressTimer*   m_pHpBar;       //血条
    void attachUnit(BattleUnitSprite *pUnit);
};

class BossHpBarLayer:public HpBarBase
{
public:
    BossHpBarLayer();
    ~BossHpBarLayer();
    static BossHpBarLayer* create(int barnum);
    bool init(int barnum);
    void attachUnit(BattleUnitSprite *pUnit);
protected:
    virtual void setBar(int curhp,int maxhp);
    int m_nBarnum;
    CCArray *m_arrBar;
    CCLabelTTF *lbl;
};
#endif
