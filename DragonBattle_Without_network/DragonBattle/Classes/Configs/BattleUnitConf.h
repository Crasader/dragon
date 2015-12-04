//
//  BattleUnitConf.h
//  DragonBattle
//  关卡中单位的信息
//  Created by 小敦 王 on 13-4-19.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_BattleUnitConf_h
#define DragonBattle_BattleUnitConf_h

#ifdef WONPEE_SERVER
#include "../player.hpp"
#include "cocos2d_diff.h"
#else
#include "cocos2d.h"
#endif

#include "ItemConf.h"

// 关卡中配置的信息
class UnitBaseConf
{
public:
    UnitBaseConf()
    {
        nLife = 0;
        fOutAttack = 0;
        fOutDef = 0;
        fInAttack = 0;
        fInDef = 0;
        nLv = 0;
        
        fCrit = 0;
        fToughness = 0;
        fReducehurt = 0;
        fAddhurt = 0;
        fRecoverHp = 0;
        
        nUnitType = 0;
        nResourceID = 0;
        nAIID = 0;
    }
    virtual ~UnitBaseConf() {}
    
    int      nUnitID;           //战斗单位ID，怪物ID与弟子或主角ID可能一样
	uint32_t nUnitType;         //战斗单位类型 kMonsterType
    char     unitName[64];		//名称
    uint32_t nLv;               //等级
    uint32_t nLife;             //生命值，许翔：这儿记录的是最大生命值
    float    fOutAttack;        //攻击力
    float    fOutDef;           //防御力
    float    fInAttack;         //内攻
    float    fInDef;            //内防
    
    float    fCrit;             //暴击
    float    fToughness;        //韧性
    float    fRecoverHp;        //回血
    float    fReducehurt;       //减伤
    float    fAddhurt;          //伤害加深
    
    uint32_t nResourceID;       //配置的资源ID
    uint16_t nAIID;             //单位AI ID,英雄和bossAI为给定
    
    int getLV();
    int getMaxHP();
    
    float getOutAttack(int BuffSkill,int attackSkill = 0);//外攻
    float getInAttack(int BuffSkill,int attackSkill = 0);//内攻
    float getOutDef(int BuffSkill);//外防
    float getInDef(int BuffSkill);//内防
    float getCrit(int BuffSkill);//暴击
    float getToughness(int BuffSkill);//韧性
    float getReduceHurt(int BuffSkill);//减伤
    float getAddhurt(int BuffSkill);//伤害加深
    float getRecoverHP();//回血
    
    float getCritChance(int attackBuffSkill,int underAttackBuffSkill,UnitBaseConf* pUnderAttacked);//暴击概率
    float getCritTimes();//暴击倍数
    int getFightPoint()
    {
        int attack = int(nLife*0.5 + (fOutAttack+fInAttack)*3 + (fOutDef+fInDef)*2 + fCrit*1.5 + fToughness*1 + fReducehurt*3 + fAddhurt*5 + fRecoverHp*3);
        return attack;
    }
};

class TrapUnit : public UnitBaseConf
{
public:
    TrapUnit()
    {
        pos = cocos2d::CCPoint(0, 0);
        size = cocos2d::CCSize(0, 0);
        fAttackInterval = 0;
    }
    ~TrapUnit() {}
    
    cocos2d::CCPoint    pos;            //陷阱位置
    cocos2d::CCSize     size;           //陷阱尺寸
    float               fAttackInterval;//攻击间隔时间
    uint32_t            nJudgeCollide;  //判断是否产生碰撞
    
    int      nExp;              //杀掉陷阱得到的经验值
    std::vector<itemDrop_t> dropItemVec; //掉落的物品、数量及概率信息
};

class MonsterUnitConf: public UnitBaseConf
{
public:
    MonsterUnitConf()
    {
        nMaxDropGold = 0;
        nMinDropGold = 0;
        fDropGoldRatio = 0;
        isRight = 1;
        dropItemVec.clear();
    }
    ~MonsterUnitConf() {}
    
    int      nMission;          //出现在哪个关卡
    int      isRight;           //是否右边出怪
    int      nMaxDropGold;      //掉落银两最大数
    int      nMinDropGold;      //最小掉落金币数
    float    fDropGoldRatio;    //银两掉落概率
    int      nExp;              //杀掉怪物得到的经验值
    std::vector<itemDrop_t> dropItemVec; //掉落的物品、数量及概率信息
};

#endif
