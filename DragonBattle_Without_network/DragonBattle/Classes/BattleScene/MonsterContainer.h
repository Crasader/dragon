//
//  MonsterContainer.h
//  DragonBattle
//  战斗场景中，场景中出生的怪物的容器
//  Created by 小敦 王 on 13-5-8.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_MonsterContainer_h
#define DragonBattle_MonsterContainer_h

#include "MonsterSprite.h"
#include "ThrowSprite.h"
typedef std::vector<BattleUnitSprite*> BattleMonsterVec;

class MonsterContainer
{
public:
    MonsterContainer();
    ~MonsterContainer();
    
    bool init();
    
    // 添加monster
    void addMonster(BattleUnitSprite* pMonster);
    
    // 得到某一类monster的剩余的数量
    int  getMonsterCountByType(int monsterType);
    
    // 得到地图中当前非BOSS和随从怪物数量
    int  getMissionMonster();
    
    int  getBornMissionMonster();
    
    int  getBornMonsterSum(int monsterType);
    
    int getKillMonsterSum(int monsterType);
    // remove monster
    bool removeMonster(BattleUnitSprite* pMonster);
    
    void setFollowMonsterRelife(bool brelife);
    // 受到英雄的攻击
    void underAttack(BattleUnitSprite* pHero, AttackInfo_t* pAttack);
    cocos2d::CCRect underThrowAttack(ThrowSprite* ts, bool isRangeAttack);
    void attack(BattleUnitSprite* pMoster,BattleUnitSprite* pHero, AttackInfo_t* pAttack);//怪物攻击
    // 执行一次AI
    void runAIOnce(BattleUnitSprite* pHero);
    
    // 设置攻击受击区域阴影
    void setAttacRectShow(bool bShow);
    
    // 当boss死亡倒地时，设置所有怪物死亡
    void setMonsterDeath();
    //侠客出现，所有怪物击飞
    void setMonsterBehitFly();
    // clear all
    void clearAllMonster();
    
    // 容器当前的大小
    int  getContainerSize() const;

    BattleUnitSprite*   getMonsterByIndex(int index);
    int getBossHurHp();
    
private:
    void unitUnderAttack(BattleUnitSprite* pHero, AttackInfo_t* pAttack);
    cocos2d::CCRect unitUnderThrowAttack(ThrowSprite* ts, bool isRangeAttack);
    bool removeMonster(BattleMonsterVec& monsterVec, BattleUnitSprite* pMonster);
    
    void setMonsterDeath(int monsterType);
    void setMonsterBehitFly(int monsterType);
    BattleMonsterVec getMonsterVecByType(int monsterType);
    
    void SetComboInfo(int* pCombo, long* pComboTime);
private:
    BattleMonsterVec    m_AllMonsterVec;
    
    uint16_t            m_nBornBossSum;         //boss刷新的总量
    uint16_t            m_nBornFollowMonsterSum;//随从怪刷的总量
    uint16_t            m_nBornNormalMonsterSum;//普通怪的总量
    uint16_t            m_nBornNeutralMonsterSum;   //杂碎的总量
    uint16_t            m_nBornEnemyHeroSum;      //敌方英雄刷新总量
    uint16_t            m_nBornMonsterSum;      //刷怪总量
    uint32_t            m_nBossHurtHpValue;
};

#define MONSTER_CONTAINER_FOREACH(__pContainer__, __pMonster__) \
    for (int i = 0; ((__pMonster__) = (__pContainer__)->getMonsterByIndex(i)) && ((__pMonster__) != NULL); i++)

#endif
