//
//  BattleUnitConf.h
//  DragonBattle
//  战斗单位（英雄 小怪 boss）AI 配置文件
//  Created by 小敦 王 on 13-4-19.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_UnitAIConf_h
#define DragonBattle_UnitAIConf_h

#include <vector>
#include <map>
#include "XmlEncrypt.h"

enum kAIState
{
    kAIZero = 0,
    kAIStandBy = 1,         //待机
    kAIMoveToHero = 2,    //靠近英雄
    kAIFarAwayTarget = 3,   //远离目标
    kAISkillAttack1 = 4,     //技能攻击（仅BOSS起作用）
    kAINormalAttack = 5,    //普通攻击
    kAISkillAttack2 = 6,     //技能攻击（仅同伴起作用）
    kAIJumpForwardToMonster = 8,     //向怪物方向跳
    kAIJumpForwardToHero = 9,     //向英雄方向跳(竞技场)
    kAIMoveToMonster = 10,    //靠近怪物
    kAIFlashToHero = 11,       //闪现到英雄身边
    kAIMoveToScreen = 12,       //向屏幕中间走
    kAIFlashToHeroTwice = 13,      //闪现2次
    kAIMoveForward = 14,           //一直向前走（小偷、怪物、英雄有效）
    kAIFlashToOutScene = 15,       //闪现出场景外（仅小偷、怪物有效）
};

enum kAIJudgeConditionState
{
    kAIPolicy = 0,              //决策
    kAIJudgeAttackRect = 1,     //攻击范围X内
    kAIJudgeBeHit = 2,          //是否被攻击
    kAIJudgePixelRange = 3,     //像素距离是否少于X
    kAIJudgeHp = 4,             //血量少于X
    kAIjudgeScreenMidPixel = 5,        //距屏幕中点是否少于x%
    kAIJudgeFaceToTarget = 6,         //是否面向目标
    kAIJudgeSameHeightAsHero = 8,     //是否与攻击对象在同一高度
    kAIJudgeMonsterExist = 9,   //当前屏幕是否有怪
    kAIJudgeObstacleToMonster = 10,      //是否往怪物方向是否有障碍
    kAIJudgeObstacleToHero = 11,      //是否往英雄方向是否有障碍
    kAIJudgeAtHeroPoint = 12,       //是否到达英雄身边
    kAIJudgeSameHeightAsMonster = 13, //是否与怪物在同一高度
    kAIJudgeInScreen = 14, //是否在屏幕内
    kAIJudgeHpPresent = 15,//血量少于X%
    kAIJudgeSkillTimes = 16,//技能连续释放少于x次(竞技场)
    kAIJudgeToOutScene = 17,//是否走出屏幕外（仅小偷、怪物有效）
    kAIJudgeTimer = 18//时间是否小于x
};

struct AIProbabilyState_t
{
    int      nAiAction;     //AI状态
    float    rangeStart;    //开始范围
    float    rangeEnd;      //结束范围
    int      nRunFrames;    //持续帧数
};
typedef std::vector<AIProbabilyState_t*> AIJudgeTreeNodeVec;

struct AIJudgeCondition_t
{
    bool  bTrue;
    int   nChildNodeID;
    int   nNextTreeID;
};
typedef std::vector<AIJudgeCondition_t*> AIJudgeConditionChildVec;

struct AIJudgeTreeNode_t
{
    uint32_t    nNodeID;
    bool        bChildNode;
    uint32_t    nJudgeType; //判定类型
    float       fJudgeNorm; //判定范围
    AIJudgeConditionChildVec   childVec;
    AIJudgeTreeNodeVec         policyVec;
};

typedef std::vector<AIJudgeTreeNode_t*> AIJudgeTree;

typedef std::map<uint16_t, AIJudgeTree*>  AIJudgeTreeMap;

class AIConfig
{
public:
	AIConfig();
	~AIConfig();

	bool loadFromFile(const char* xmlFile, bool bEncrypt = false);
    void unloadAIConfig();
    
    // 校验配置表
    bool checkAIConfigRight();
    
    const AIJudgeTree*    getAIFromID(uint16_t treeID) const;
    
private:
    AIJudgeTreeMap   m_AIJudgeTreeMap;
};

#endif
