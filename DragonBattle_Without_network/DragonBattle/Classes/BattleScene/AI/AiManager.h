//
//  AiFunction.h
//  DragonBattle
//
//  Created by 小敦 王 on 13-5-9.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_AiFunction_h
#define DragonBattle_AiFunction_h

#include "GameData.h"

class BattleUnitSprite;
class BattleMapUnits;
class AiManager
{
public:
    static kAIState getUnitAIPolicy(BattleUnitSprite* pUnit, BattleUnitSprite* pEnemy, BattleMapUnits* pMap, int nAIID, int nAttackDistance, bool& bLastAI, int& nRunFrames, int& nLastNodeId, int nLastRunFrames);
    static kAIState getEnemyHeroAIPolicy(BattleUnitSprite* pUnit, BattleUnitSprite* pEnemy, BattleMapUnits* pMap, int nAIID, int nAttackDistance,int nContinueSkill, bool& bLastAI, int& nRunFrames, int& nLastNodeId, int nLastRunFrames);
    static kAIState getPartnerAIPolicy(BattleUnitSprite* pUnit, BattleUnitSprite* pHero, BattleUnitSprite* pEnemy, BattleMapUnits* pMap, int nAIID, int nAttackDistance, bool& bLastAI, int& nRunFrames, int& nLastNodeId, int nLastRunFrames);
    static kAIState getHeroAIPolicy(BattleUnitSprite* pUnit, BattleUnitSprite* pEnemy, BattleMapUnits* pMap, int nAIID, int nAttackDistance, bool& bLastAI, int& nRunFrames, int& nLastNodeId, int nLastRunFrames);
private:
    static bool conditionJudge(AIJudgeTreeNode_t* pTreeNode, BattleUnitSprite* pUnit, BattleUnitSprite* pEnemy, int nAttackDistance, bool& bLastAI, int& nRunFrames);
    static kAIState aiPolicyJudge(AIJudgeTreeNode_t* pTreeNode, int& nRunFrames);

};

#endif
