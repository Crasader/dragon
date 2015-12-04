//
//  SportLayer.h
//  DragonBattle
//
//  Created by zeng xiaolong on 13-10-23.
//
//

#ifndef __DragonBattle__SportLayer__
#define __DragonBattle__SportLayer__
#include "BattleLayerBase.h"

class SportLayer : public BattleLayerBase
{
public:
    SportLayer();
    ~SportLayer();
    
    CREATE_FUNC(SportLayer);
    void loadLayer(UnitBaseConf Conf,uint32_t *skills,int targetid,uint8_t *session);
    // create UI button
    void createUI();
    void createUnitHpBar();
    void createRoleSprite();
    void createEmenySprite();
    void createbattle();
    void loadResource();
    void onReturnCmtLayer(CCNode *node);
    void LoadcreateBattle(CCNode* pNode);
    void updatelastTime(float time);
    void update(float dt);
    void onBattleEnd(CCObject *obj);
    void onWinBgChange(CCNode*node);
    void showUnitHp(CCObject *obj);
    void battleLose(CCObject *obj);
    void battleWin(CCObject *obj);
    void battleResultEffect(CCObject *obj);
    void onStart();
    void onGetBattleResultComplete(CCObject *obj);
    void onGetKeapResultComplete(CCObject *p);
    void onPause(CCNode *node);
    void onBack(CCNode *node);
    void onContune(CCNode *node);
    void onReConnect(CCObject *obj);
    void beforeSendBattleResult(int result);
    void sendBattleResult(int result);
    XYScaleButton *m_pBtnPause;
    HpBarLayer *m_pEnemyHeroHpBar;
    UnitBaseConf emenyHeroConf;
    BattleUnitSprite *m_pEnemyRole;
    uint32_t m_nEnemykill[4];
    int    m_targetID;
    
    virtual void onBodyguardCompleteLayerGetBonus();
};

#endif /* defined(__DragonBattle__SportLayer__) */
