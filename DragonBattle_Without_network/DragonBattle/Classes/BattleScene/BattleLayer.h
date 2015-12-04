//
//  BattleScene.h
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-16.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_BattleScene_h
#define DragonBattle_BattleScene_h

#include "cocos2d.h"
#include "GameData.h"
#include "CCBPanelCardSelectNew.h"
#include "CCBPanelDialSelect.h"
#include "XYButton.h"
#include "BattleUIComponent.h"
#include "BattleLayerBase.h"
#include "XYToggleButton.h"
#include "TouchLayer.h"
class XYButton;
class BattleUIButton;
class BattleUnitSprite;
class BattleMapUnits;
class HeroSprite;
class PartnerSprite;
class CCBPanelVictoryNew;

class BattleLayer : public BattleLayerBase, public CCBPanelCardSelectNewDelegate, public CCBPanelDialSelectDelegate
{
public:
    BattleLayer();
    ~BattleLayer();
    
    bool init(uint32_t nMissionID);
    static BattleLayer* create(uint32_t nMissionID);
    // create UI button
    void createUI();
    // create bkg 
    void createMapBkg();
    // create unit hp bar
    void createUnitHpBar();
    void battleRestart(CCObject *p);
    void createbattle();
    void loadResource();
    virtual void update(float dt);
    void battleLose(CCObject *obj);
    void battleWin(CCObject *obj);
    void showUnitHp(CCObject *obj);
    void showTutoria(CCObject *obj);
    void setBattleStop(CCObject* obj);
    void onStart();
    void onEnterForground(CCObject* obj);
public:
    void setConfig(uint32_t nStartTime, uint32_t nBattleKey, std::vector<item_unit_t>* pItemVec, std::vector<item_unit_t>* pThiefItemVec);
    
    virtual void onPanelCardSelectNewSlotClick(uint32_t nCostGold, uint32_t nCardState);
    virtual void onPanelCardSelectNewClose();
    virtual void onPanelCardSelectNewReturn();
    
    virtual void onPanelDialSelectDial(uint32_t nCostGold, int nTimes);
    virtual void onPanelDialSelectReturn();
    
    void onKillCount(CCObject *pSender);
    void onUIVisable(CCObject *p);
    CCBPanelVictoryNew* onBattleWin(uint32_t map_type, uint32_t nScore, std::vector<item_unit_t>* pFirstPrize, std::vector<item_unit_t>* pPassPrize, std::vector<item_unit_t>* pCardVec, int nTotalCostGold);//战斗结束回调
    void beforeSendBattleResult(int result);
    void sendBattleResult(int result);
    void exitGame();
    void onBattleLose(uint32_t map_type);
    void onReConnect(CCObject *obj);
    void onBattleFirstWin();
    void onGetCardPrize(item_unit_t* pCard, bool bIsBagFull);
    void flyItemToPortrait(int nItemId, CCPoint worldPosition);
    
    void onWorldBossOverNotice(CCObject*);
    
    void onTowerBattleWin(std::vector<item_unit_t>* pCardVec, int nTotalCostGold);
    void onTowerBattleLose();
    
    void onBattleEnd(CCObject *obj);
    void onWinBgChange(CCNode*node);
    void networkBattleRestart();   //重来一次
    void onPause(CCNode *node);
    void onRenew(CCNode *node);
    void onClose(CCNode *node);
    void onBack(CCNode *node);
    void onContune(CCNode *node);
    void onMedicine(CCNode *node);
    void onUseMedicine(CCObject *obj);
    
    virtual void hpWarning();
    virtual void stopHpWarning();
    void cancelAutoFight(CCNode *node);
    void recoverUIState();
    void createRoleSprite();
    void createWorldBoss();
    uint32_t m_nStartTime;                  //开始时间
    uint32_t m_nBattleKey;                  //加密Key
    std::vector<item_unit_t> m_ItemLimitVec;//掉落物限制向量
    std::vector<item_unit_t> m_ThiefItemVec;//小偷掉落物品向量
private:
    XYScaleButton*     m_pBtnPause;
    XYScaleButton*     m_pBtnRenew;
    XYScaleButton*     m_pBtnClose;
    XYScaleButton*     m_pBtnMed;          //药剂
    
    BossHpBarLayer*         m_pBossHpBar1;
    PartnerHpBarLayer*      m_pPartnerHpBar;
    CCBPanelCardSelectNew*  m_pPanelCardSelect;
    CCBPanelDialSelect*     m_pPanelDialSelect;
    CCBPanelVictoryNew*     m_pPanelVictory;
    int                     m_nCardCostGold;
    int                     m_nUseMedicineCount;
    CCSprite*               m_taskBar;

    bool m_bCollectTask;
public:
    void createUITools();
    void onShowAttackRect(CCObject* pSender);
private:
    void LoadcreateBattle(CCNode* pNode);
    void setMedNum(int nNum);
    
    cocos2d::CCLabelTTF* m_pShowLabel;
    bool m_bShowAttackRect;
    
    std::vector<item_unit_t> m_cardVec;
    int m_nTotalCostGold;
};

#endif
