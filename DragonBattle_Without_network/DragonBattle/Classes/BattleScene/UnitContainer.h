//
//  UnitContainer.h
//  DragonBattle
//
//  Created by zeng xiaolong on 14-2-15.
//
//

#ifndef __DragonBattle__UnitContainer__
#define __DragonBattle__UnitContainer__

#include "BattleUnitSprite.h"
typedef std::vector<BattleUnitSprite*> BattleMonsterVec;

class UnitContainer
{
public:
    UnitContainer();
    ~UnitContainer();
    bool init();
    
    void addUnit(BattleUnitSprite* pUnit);
    
    virtual bool removeUnit(BattleUnitSprite*pMonster);
    
    // 受到攻击
    void underAttack(BattleUnitSprite* pHero, AttackInfo_t* pAttack, int* pCombo, long* pComboTime);
    cocos2d::CCRect underThrowAttack(ThrowSprite* ts, bool isRangeAttack);
    void attack(BattleUnitSprite* pMoster,BattleUnitSprite* pHero, AttackInfo_t* pAttack);//怪物攻击
    BattleUnitSprite*  getMonsterByIndex(int index);
private:
    void unitUnderAttack(BattleUnitSprite* pHero, AttackInfo_t* pAttack, int* pCombo, long* pComboTime);
    cocos2d::CCRect unitUnderThrowAttack(ThrowSprite* ts, bool isRangeAttack);
    bool removeMonster(BattleMonsterVec& monsterVec, BattleUnitSprite* pMonster);
private:
    uint16_t            m_nBornUnitSum;
    BattleMonsterVec    m_AllUnitVec;
};
#define UNIT_CONTAINER_FOREACH(__pContainer__, __pMonster__) \
for (int i = 0; ((__pMonster__) = (__pContainer__)->getMonsterByIndex(i)) && ((__pMonster__) != NULL); i++)
#endif /* defined(__DragonBattle__UnitContainer__) */
