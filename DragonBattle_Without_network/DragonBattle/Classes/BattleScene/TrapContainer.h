//
//  TrapContainer.h
//  DragonBattle
//
//  Created by 何 永翔 on 13-6-4.
//
//

#ifndef DragonBattle_TrapContainer_h
#define DragonBattle_TrapContainer_h

#include <vector>
#include "BattleMissionConf.h"
#include "TrapSprite.h"

typedef std::vector<TrapSprite*> TrapSpriteVec;

class BattleMapUnits;
class TrapContainer
{
public:
    TrapContainer();
    ~TrapContainer();
    
    void reset(BattleMapUnits* pBattleMapUnits, TrapUnitVec& trapUnitVec);
    
    void update(BattleUnitSprite* pHero);
    bool remove(BattleUnitSprite* pTrap);
    
    void clearAllTrap();
    
    bool isInterSect(cocos2d::CCRect unitRect, float moveX, float moveY, float* pCorrectX, float* pCorrectY);
    TrapSprite* getOverlappingSpr(float poix,CCRect unitRect);//怪物出生点与障碍物重叠
    TrapSpriteVec *getTrapSpriteVec();
private:
    
    TrapSpriteVec m_trapSpriteVec;
};

#endif
