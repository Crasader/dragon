//
//  AiFunction.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-5-9.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "AiManager.h"
#include "GlobalDefine.h"
#include "BattleUnitSprite.h"
#include "BattleMapUnits.h"

USING_NS_CC;

bool AiManager::conditionJudge(AIJudgeTreeNode_t* pTreeNode, BattleUnitSprite* pUnit, BattleUnitSprite* pEnemy, int nAttackDistance, bool& bLastAI, int& nRunFrames)
{
    switch (pTreeNode->nJudgeType)
    {
        case kAIJudgeAttackRect:        //bool值判断
        {
            //judgeNorm默认值为100
            float judgeNorm = pTreeNode->fJudgeNorm == 0 ? nAttackDistance : pTreeNode->fJudgeNorm;
            return fabs(pUnit->getCenterPosition().x -pEnemy->getCenterPosition().x) - pEnemy->getUnitSize().width/2 < judgeNorm;
            break;
        }
        case kAIJudgeBeHit:             //是否被攻击
        {
            if (pUnit->getCurAction() == kBeHit)
            {
                bLastAI = true;
                return true;
            }
            else
            {
                return false;
            }
            break;
        }
        case kAIJudgePixelRange:     //像素距离是否少于X
        {
            //judgeNorm默认值为300
            float judgeNorm = pTreeNode->fJudgeNorm == 0 ? 300 : pTreeNode->fJudgeNorm;
            float value = fabs(pUnit->getPosition().x - pEnemy->getPosition().x);
            return value < judgeNorm;
            break;
        }
        case kAIJudgeHp:             //血量少于X
        {
            //judgeNorm默认值为100
            float judgeNorm = pTreeNode->fJudgeNorm == 0 ? 100 : pTreeNode->fJudgeNorm;
            return pUnit->getHp() < judgeNorm;
            break;
        }
        case kAIjudgeScreenMidPixel://距离屏幕中心点的距离是否小于%x
        {
            //judgeNorm默认值为100
            float judgeNorm = pTreeNode->fJudgeNorm == 0 ? 1: pTreeNode->fJudgeNorm;
            return fabs(pUnit->getScreenPosition().x - SCREEN_WIDTH/2)/(SCREEN_WIDTH/2) < judgeNorm;
            break;
        }
        case kAIJudgeFaceToTarget:         //是否面向目标
        {
            return pUnit->isFlipX() == (pEnemy->getPosition().x < pUnit->getPosition().x);
            break;
        }
        case kAIJudgeInScreen://kAIJudgeInScreen
        {
            float x = pUnit->getScreenPosition().x;
            bool isture = x > 0 && x < SCREEN_WIDTH;
            return isture;
            break;
        }
        default:
        {
            return false;
        }
    }
}

kAIState AiManager::aiPolicyJudge(AIJudgeTreeNode_t* pTreeNode, int& nRunFrames)
{
    if (!pTreeNode->policyVec.empty())
    {
        int nPolicyNum = rand() % 100;
        // 根据几率随机
        for (int i = 0; i < pTreeNode->policyVec.size(); i++)
        {
            if (nPolicyNum >= pTreeNode->policyVec[i]->rangeStart && nPolicyNum < pTreeNode->policyVec[i]->rangeEnd)
            {
                nRunFrames = pTreeNode->policyVec[i]->nRunFrames;
                return kAIState(pTreeNode->policyVec[i]->nAiAction);
            }
        }
    }
    return kAIStandBy;
}

kAIState AiManager::getUnitAIPolicy(BattleUnitSprite* pUnit, BattleUnitSprite* pEnemy, BattleMapUnits* pMap, int nAIID, int nAttackDistance, bool& bLastAI, int& nRunFrames, int& nLastNodeId, int nLastRunFrames)
{
    const AIJudgeTree* pJudgeTree = GameData::getAiJudgeTree(nAIID);
    if (!pJudgeTree && pEnemy != NULL)
    {
        return kAIStandBy;
    }
    
    kAIState aiState = kAIStandBy;
    int     nJudgeIndex = 0;
    bool     bJudgeVale = false;
    
    while (1)
    {
        AIJudgeTreeNode_t* pTreeNode = pJudgeTree->at(nJudgeIndex);
        
        //通用AI判定
        bJudgeVale = AiManager::conditionJudge(pTreeNode, pUnit, pEnemy, nAttackDistance, bLastAI, nRunFrames);
        
        //怪物AI判定
        switch (pTreeNode->nJudgeType)
        {
            case kAIJudgeObstacleToHero:      //是否往英雄方向是否有障碍
            {
                float correctX = 0;
                float correctY = 0;
                if (pEnemy->getPosition().x - pUnit->getPosition().x > 0)
                {
                    bJudgeVale = (pUnit->getObstacleDirection(10, 0, &correctX, &correctY) & kObstacleRight) == kObstacleRight;
                }
                else
                {
                    bJudgeVale = (pUnit->getObstacleDirection(-10, 0, &correctX, &correctY) & kObstacleLeft) == kObstacleLeft;
                }
                break;
            }
            case kAIJudgeSameHeightAsHero:    //是否在同一高度
            {
                // 都在障碍物上，考虑英雄倒地的情况，所以给个50的差量
                if ((pEnemy->getCurObstacleDirection() & kObstacleDown))
                {
                    bJudgeVale = fabs(pUnit->getUnitRect().origin.y - pEnemy->getUnitRect().origin.y) <= 50;
                }
                else if (IS_JUMPING_ACTION(pEnemy->getCurAction()))
                {
                    //许翔：这儿的逻辑似乎不对，暂时没想到更好的办法，暂不做修改
                    bJudgeVale = pUnit->getUnitRect().origin.y - pEnemy->getUnitRect().origin.y < pUnit->getCenterPosition().x;
                }
                else//如果英雄正在空中被击倒的状态，默认认为其在一条线
                {
                    bJudgeVale = true;
                }
                break;
            }
            case  kAIJudgeToOutScene://是否走出屏幕外（仅怪物、小偷有效）
            {
                bJudgeVale = pUnit->getPosition().x > pMap->getMapLenght() + 100;
                break;
            }
            case  kAIJudgeTimer:
            {
                bJudgeVale = pMap->getFightTime() < pTreeNode->fJudgeNorm;
                break;
            }
            default:
                break;
        }
        
        if (pTreeNode->nJudgeType == kAIPolicy)
        {
            
            if (nLastNodeId != pTreeNode->nNodeID || nLastRunFrames == 0)
            {
                nLastNodeId = pTreeNode->nNodeID;
                aiState = aiPolicyJudge(pTreeNode, nRunFrames);
            }
            else
            {
                aiState = kAIStandBy;
                nRunFrames = 0;
            }
            break;
        }
        else
        {
            for (int i = 0; i < pTreeNode->childVec.size(); i++)
            {
                if (pTreeNode->childVec[i]->bTrue == bJudgeVale)//判定值
                {
                    if (pTreeNode->childVec[i]->nNextTreeID != 0)
                    {
                        return AiManager::getUnitAIPolicy(pUnit, pEnemy, pMap, pTreeNode->childVec[i]->nNextTreeID, nAttackDistance, bLastAI, nRunFrames, nLastNodeId, nLastRunFrames);//进入下一个树
                    }
                    else
                    {
                        nJudgeIndex = pTreeNode->childVec[i]->nChildNodeID - 1;//下一个节点
                        break;
                    }
                }
            }
        }
    }
    //CCLOG(aiJudgeStr.c_str());
    return aiState;
}

kAIState AiManager::getEnemyHeroAIPolicy(BattleUnitSprite* pUnit, BattleUnitSprite* pEnemy, BattleMapUnits* pMap, int nAIID, int nAttackDistance,int nContinueSkill, bool& bLastAI, int& nRunFrames, int& nLastNodeId, int nLastRunFrames)
{
    const AIJudgeTree* pJudgeTree = GameData::getAiJudgeTree(nAIID);
    if (!pJudgeTree && pEnemy != NULL)
    {
        return kAIStandBy;
    }
    
    kAIState aiState = kAIStandBy;
    int     nJudgeIndex = 0;
    bool     bJudgeVale = false;
    
    while (1)
    {
        
        AIJudgeTreeNode_t* pTreeNode = pJudgeTree->at(nJudgeIndex);
        CCLOG("循环%d",pTreeNode->nNodeID);
        //通用AI判定
        bJudgeVale = AiManager::conditionJudge(pTreeNode, pUnit, pEnemy, nAttackDistance, bLastAI, nRunFrames);
        
        //怪物AI判定
        switch (pTreeNode->nJudgeType)
        {
            case kAIJudgeObstacleToHero:      //是否往英雄方向是否有障碍
            {
                float correctX = 0;
                float correctY = 0;
                if (pEnemy->getPosition().x - pUnit->getPosition().x > 0)
                {
                    bJudgeVale = (pUnit->getObstacleDirection(10, 0, &correctX, &correctY) & kObstacleRight) == kObstacleRight;
                }
                else
                {
                    bJudgeVale = (pUnit->getObstacleDirection(-10, 0, &correctX, &correctY) & kObstacleLeft) == kObstacleLeft;
                }
                break;
            }
            case kAIJudgeSameHeightAsHero:    //是否在同一高度
            {
                // 都在障碍物上，考虑英雄倒地的情况，所以给个50的差量
                if ((pEnemy->getCurObstacleDirection() & kObstacleDown))
                {
                    bJudgeVale = fabs(pUnit->getUnitRect().origin.y - pEnemy->getUnitRect().origin.y) <= 50;
                }
                else if (IS_JUMPING_ACTION(pEnemy->getCurAction()))
                {
                    //许翔：这儿的逻辑似乎不对，暂时没想到更好的办法，暂不做修改
                    bJudgeVale = pUnit->getUnitRect().origin.y - pEnemy->getUnitRect().origin.y < pUnit->getCenterPosition().x;
                }
                else//如果英雄正在空中被击倒的状态，默认认为其在一条线
                {
                    bJudgeVale = true;
                }
                break;
            }
            case kAIJudgeHpPresent:     //血量是否少于X%
            {
                float judgeNorm = pTreeNode->fJudgeNorm;
                float value = (((float)pUnit->getHp())/pUnit->getUnitBaseConf()->getMaxHP());
                bJudgeVale =  value< judgeNorm;
                break;
            }
            case kAIJudgeSkillTimes:
                bJudgeVale =  nContinueSkill < pTreeNode->fJudgeNorm;
                break;
            default:
                break;
        }
        
        if (pTreeNode->nJudgeType == kAIPolicy)
        {
            
            if (nLastNodeId != pTreeNode->nNodeID || nLastRunFrames == 0)
            {
                nLastNodeId = pTreeNode->nNodeID;
                aiState = aiPolicyJudge(pTreeNode, nRunFrames);
            }
            else
            {
                aiState = kAIStandBy;
                nRunFrames = 0;
            }
            break;
        }
        else
        {
            for (int i = 0; i < pTreeNode->childVec.size(); i++)
            {
                if (pTreeNode->childVec[i]->bTrue == bJudgeVale)//判定值
                {
                    if (pTreeNode->childVec[i]->nNextTreeID != 0)
                    {
                        return AiManager::getUnitAIPolicy(pUnit, pEnemy, pMap, pTreeNode->childVec[i]->nNextTreeID, nAttackDistance, bLastAI, nRunFrames, nLastNodeId, nLastRunFrames);//进入下一个树
                    }
                    else
                    {
                        nJudgeIndex = pTreeNode->childVec[i]->nChildNodeID - 1;//下一个节点
                        break;
                    }
                }
            }
        }
    }
    //CCLOG(aiJudgeStr.c_str());
    return aiState;
}

kAIState AiManager::getPartnerAIPolicy(BattleUnitSprite* pUnit, BattleUnitSprite* pHero, BattleUnitSprite* pEnemy, BattleMapUnits* pMap, int nAIID, int nAttackDistance, bool& bLastAI, int& nRunFrames, int& nLastNodeId, int nLastRunFrames)
{
    const AIJudgeTree* pJudgeTree = GameData::getAiJudgeTree(nAIID);
    if (!pJudgeTree)
    {
        return kAIStandBy;
    }
    
    kAIState aiState = kAIStandBy;
    int     nJudgeIndex = 0;
    bool    bJudgeVale = false;
    
    while (1)
    {
        AIJudgeTreeNode_t* pTreeNode = pJudgeTree->at(nJudgeIndex);
        
        //通用AI判定
        bJudgeVale = AiManager::conditionJudge(pTreeNode, pUnit, pEnemy, nAttackDistance, bLastAI, nRunFrames);

        //伙伴特有AI判定
        switch (pTreeNode->nJudgeType)
        {
            case kAIJudgeSameHeightAsHero:    //是否在同一高度
            {
                // 都在障碍物上，考虑英雄倒地的情况，所以给个50的差量
                if ((pHero->getCurObstacleDirection() & kObstacleDown))
                {
                    bJudgeVale = fabs(pUnit->getUnitRect().origin.y - pHero->getUnitRect().origin.y) <= 50;
                }
                else if (IS_JUMPING_ACTION(pHero->getCurAction()))
                {
                    //许翔：这儿的逻辑似乎不对，暂时没想到更好的办法，暂不做修改
                    bJudgeVale = pUnit->getUnitRect().origin.y - pHero->getUnitRect().origin.y < pUnit->getCenterPosition().x;
                }
                else//如果英雄正在空中被击倒的状态，默认认为其在一条线
                {
                    bJudgeVale = true;
                }
                
                break;
            }
            case kAIJudgeMonsterExist:   //当前屏幕是否有怪
            {
                bJudgeVale = (pEnemy != NULL);
                break;
            }
            case kAIJudgeObstacleToMonster:      //是否往怪物方向是否有障碍
            {
                float correctX = 0;
                float correctY = 0;
                if (pEnemy->getPosition().x - pUnit->getPosition().x > 0)
                {
                    bJudgeVale = (pUnit->getObstacleDirection(10, 0, &correctX, &correctY) & kObstacleRight) == kObstacleRight;
                }
                else
                {
                    bJudgeVale = (pUnit->getObstacleDirection(-10, 0, &correctX, &correctY) & kObstacleLeft) == kObstacleLeft;
                }
                bJudgeVale = false;//障碍物就是怪
                break;
            }
            case kAIJudgeObstacleToHero:      //是否往英雄方向是否有障碍
            {
                float correctX = 0;
                float correctY = 0;
                if (pHero->getPosition().x - pUnit->getPosition().x > 0)
                {
                    bJudgeVale = (pUnit->getObstacleDirection(10, 0, &correctX, &correctY) & kObstacleRight) == kObstacleRight;
                }
                else
                {
                    bJudgeVale = (pUnit->getObstacleDirection(-10, 0, &correctX, &correctY) & kObstacleLeft) == kObstacleLeft;
                }
                break;
            }
            case kAIJudgeAtHeroPoint:       //是否到达英雄身边
            {
                float judgeNorm = pTreeNode->fJudgeNorm == 0 ? 100 : pTreeNode->fJudgeNorm;
                bJudgeVale = ccpDistance(pUnit->getPosition(), pHero->getPosition()) <= judgeNorm;
                break;
            }
            case kAIJudgeSameHeightAsMonster: //是否与怪物在同一高度
            {
                // 都在障碍物上，考虑怪物倒地的情况，所以给个50的差量
                if ((pEnemy->getCurObstacleDirection() & kObstacleDown))
                {
                    bJudgeVale = fabs(pUnit->getUnitRect().origin.y - pEnemy->getUnitRect().origin.y) <= 50;
                }
                else if (IS_JUMPING_ACTION(pEnemy->getCurAction()))
                {
                    //许翔：这儿的逻辑似乎不对，暂时没想到更好的办法，暂不做修改
                    bJudgeVale = pUnit->getUnitRect().origin.y - pEnemy->getUnitRect().origin.y < pUnit->getCenterPosition().x;
                }
                else
                {
                    //如果怪物正在空中被击倒的状态，默认认为其在一条线
                    bJudgeVale = true;
                }
                break;
            }
            default:
            {
                break;
            }
        }
        
        if (pTreeNode->nJudgeType == kAIPolicy)
        {
            if (nLastNodeId != pTreeNode->nNodeID || nLastRunFrames == 0)
            {
                nLastNodeId = pTreeNode->nNodeID;
                aiState = aiPolicyJudge(pTreeNode, nRunFrames);
            }
            else
            {
                aiState = kAIStandBy;
                nRunFrames = 0;
            }
            break;
        }
        else
        {
            
            for (int i = 0; i < pTreeNode->childVec.size(); i++)
            {
                if (pTreeNode->childVec[i]->bTrue == bJudgeVale)//判定值
                {
                    if (pTreeNode->childVec[i]->nNextTreeID != 0)
                    {
                        return AiManager::getPartnerAIPolicy(pUnit, pHero, pEnemy, pMap, pTreeNode->childVec[i]->nNextTreeID, nAttackDistance, bLastAI, nRunFrames, nLastNodeId, nLastRunFrames);//进入下一个树
                    }
                    else
                    {
                        nJudgeIndex = pTreeNode->childVec[i]->nChildNodeID - 1;//下一个节点
                        break;
                    }
                }
            }
        }
    }
    return aiState;
}

kAIState AiManager::getHeroAIPolicy(BattleUnitSprite* pUnit, BattleUnitSprite* pEnemy, BattleMapUnits* pMap, int nAIID, int nAttackDistance, bool& bLastAI, int& nRunFrames, int& nLastNodeId, int nLastRunFrames)
{
    const AIJudgeTree* pJudgeTree = GameData::getAiJudgeTree(nAIID);
    if (!pJudgeTree)
    {
        return kAIStandBy;
    }
    
    kAIState aiState = kAIStandBy;
    int     nJudgeIndex = 0;
    bool    bJudgeVale = false;
    
    while (1)
    {
        AIJudgeTreeNode_t* pTreeNode = pJudgeTree->at(nJudgeIndex);
        
        //通用AI判定
        bJudgeVale = AiManager::conditionJudge(pTreeNode, pUnit, pEnemy, nAttackDistance, bLastAI, nRunFrames);
        
        //英雄特有AI判定
        switch (pTreeNode->nJudgeType)
        {
            case kAIJudgeMonsterExist:   //当前屏幕是否有怪
            {
                bJudgeVale = (pEnemy != NULL);
                break;
            }
            case kAIJudgeObstacleToMonster:      //是否往怪物方向是否有障碍
            {
                float correctX = 0;
                float correctY = 0;
                if (pEnemy->getPosition().x - pUnit->getPosition().x > 0)
                {
                    bJudgeVale = (pUnit->getObstacleDirection(10, 0, &correctX, &correctY) & kObstacleRight) == kObstacleRight;
                }
                else
                {
                    bJudgeVale = (pUnit->getObstacleDirection(-10, 0, &correctX, &correctY) & kObstacleLeft) == kObstacleLeft;
                }
                break;
            }
            case kAIJudgeSameHeightAsMonster: //是否与怪物在同一高度
            {
                // 都在障碍物上，考虑怪物倒地的情况，所以给个50的差量
                if ((pEnemy->getCurObstacleDirection() & kObstacleDown))
                {
                    bJudgeVale = fabs(pUnit->getUnitRect().origin.y - pEnemy->getUnitRect().origin.y) <= 50;
                }
                else if (IS_JUMPING_ACTION(pEnemy->getCurAction()))
                {
                    //许翔：这儿的逻辑似乎不对，暂时没想到更好的办法，暂不做修改
                    bJudgeVale = pUnit->getUnitRect().origin.y - pEnemy->getUnitRect().origin.y < pUnit->getCenterPosition().x;
                }
                else
                {
                    //如果怪物正在空中被击倒的状态，默认认为其在一条线
                    bJudgeVale = true;
                }
                break;
            }
            default:
            {
                break;
            }
        }
        
        if (pTreeNode->nJudgeType == kAIPolicy)
        {
            if (nLastNodeId != pTreeNode->nNodeID || nLastRunFrames == 0)
            {
                nLastNodeId = pTreeNode->nNodeID;
                aiState = aiPolicyJudge(pTreeNode, nRunFrames);
            }
            else
            {
                aiState = kAIStandBy;
                nRunFrames = 0;
            }
            break;
        }
        else
        {
            for (int i = 0; i < pTreeNode->childVec.size(); i++)
            {
                if (pTreeNode->childVec[i]->bTrue == bJudgeVale)//判定值
                {
                    if (pTreeNode->childVec[i]->nNextTreeID != 0)
                    {
                        return AiManager::getHeroAIPolicy(pUnit, pEnemy, pMap, pTreeNode->childVec[i]->nNextTreeID, nAttackDistance, bLastAI, nRunFrames, nLastNodeId, nLastRunFrames);//进入下一个树
                    }
                    else
                    {
                        nJudgeIndex = pTreeNode->childVec[i]->nChildNodeID - 1;//下一个节点
                        break;
                    }
                }
            }
        }
    }
    return aiState;
}
