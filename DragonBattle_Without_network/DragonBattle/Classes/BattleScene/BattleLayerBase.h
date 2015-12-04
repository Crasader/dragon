//
//  BattleLayerBase.h
//  DragonBattle
//
//  Created by zeng xiaolong on 13-10-22.
//
//

#ifndef __DragonBattle__BattleLayerBase__
#define __DragonBattle__BattleLayerBase__
#include "cocos2d.h"
#include "GameData.h"
#include "XYButton.h"
#include "BattleUIComponent.h"
#include "XYScaleButton.h"
#include "HeroSprite.h"
#include "BattleUILayer.h"
#include "BattleCacheManager.h"
USING_NS_CC;
#define TIMELBLTAG 1000
#define BOOLDTAG   1001
#define TASKLBLTAG 1002
enum kBattleLayerZOrder
{
    kMapLayerOrder = 1, //地图场景及地图单位
    kNotice = 7,        //公告
    kUIOrder = 8,       //UI
    kResultPanelOrder,
    kLayerMaxZOrder,
    
};

class BattleUIButton;
class BattleUnitSprite;
class BattleMapUnits;
class BattleLayerBase: public CCLayer,public BattleOperateDelegate
{
public:
    BattleLayerBase();
    ~BattleLayerBase();
    
    static BattleLayerBase* createWithID(uint16_t nMissionID);
    bool initWithID(uint16_t nMissionID);
    CCNode*             m_pMapNode;
    BattleMapUnits*     m_pMapUnitsLayer;
    MissionConf_t*      m_pMissionConf;    //关卡
    int                 m_lasttime;
    CCSprite *          m_timeBar;
    BattleUILayer*      m_pUIContainer;
    HeroSprite*         m_pRole;
    bool                m_bShowAttackRect;
    HpBarLayer*         m_pHeroHpBar;       //英雄
    int                 m_battleResult;     //0失败 1成功 2exit
    CC_SYNTHESIZE(kBattleType, m_kBattleType, BattleType);
    // create UI button
    virtual void createUI();
    
    // create Role
    virtual void createRoleSprite();
    virtual void hpWarning();
    virtual void stopHpWarning();
    // create bkg
    virtual void createMapBkg();
    
    // create unit hp bar
    virtual void createUnitHpBar();
    
    virtual void networkBattleRestart();
    virtual void createbattle();
    virtual void loadResource();
    virtual void LoadcreateBattle(CCNode* pNode);
    
    virtual void battleLose(CCObject *obj){};
    virtual void battleWin(CCObject *obj){};
    virtual void battleRestart(){};
    
    virtual void directMoveOutside();
    virtual void directTouchEnd();
    virtual void onAttackTouchBegan();
    virtual void onAttackTouchEnded();
    virtual void onRollTouchBegan(CCNode *node);
    virtual void onSkillTouchBegan(CCNode *node);
    virtual void onAutoFightTouchBegan(CCNode *node);
    
    virtual void onMpChange(CCObject *pSender);
    virtual void onAIPlaySkill(CCObject *pSender);
    virtual void setSkillBtnStatue(XYRollButton *btn,int skill);
    virtual void onStart();
    virtual void update(float dt);
    virtual void updatelastTime(float time);
    
    bool m_bHasGetAward;
    void setAttackRectVisible(bool isVisible) { m_bShowAttackRect = isVisible; }
    bool isAttackRectVisible() const { return m_bShowAttackRect; }
};
#endif /* defined(__DragonBattle__BattleLayerBase__) */
