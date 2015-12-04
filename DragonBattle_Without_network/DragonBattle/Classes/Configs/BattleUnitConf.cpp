//
//  BattleUnitConf.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-19.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "BattleUnitConf.h"

#ifdef WONPEE_SERVER
#else
#include "CRUtils.h"
#include "cocos2d.h"
#include "GameData.h"
#include "UserData.h"

USING_NS_CC;

int UnitBaseConf::getLV()
{
    return nLv;
}

int UnitBaseConf::getMaxHP()
{
    return nLife;
}

float UnitBaseConf::getOutAttack(int BuffSkill,int attackSkill)
{
    int attack = 0;
    if (attackSkill != 0)
    {
        const SkillLevel_t *pConf = GameData::getSkillConf(GET_SKILL_ID(attackSkill), GET_SKILL_LEVEL(attackSkill));
        attack = (fOutAttack+ pConf->skillLvBase.nOutAttackValue)*(1 + pConf->skillLvBase.fOutAttackPrecent);
    }
    else
    {
        attack = fOutAttack;
    }
    
    if (BuffSkill != 0)
    {
        const SkillLevel_t *pConf = GameData::getSkillConf(GET_SKILL_ID(BuffSkill), GET_SKILL_LEVEL(BuffSkill));
        attack = attack *(1+pConf->skillLvBase.fOutAttackPrecent);
    }
    else
    {
        attack = attack;
    }
    return attack;
}

float UnitBaseConf::getInAttack(int BuffSkill,int attackSkill)
{
    int attack = 0;
    if (attackSkill != 0)
    {
        const SkillLevel_t *pConf = GameData::getSkillConf(GET_SKILL_ID(attackSkill), GET_SKILL_LEVEL(attackSkill));
        attack = (fInAttack+ pConf->skillLvBase.nInAttackValue)*(1 + pConf->skillLvBase.fInAttackPrecent);
    }
    else
    {
        attack = fInAttack;
    }

    if (BuffSkill != 0)
    {
        const SkillLevel_t *pConf = GameData::getSkillConf(GET_SKILL_ID(BuffSkill), GET_SKILL_LEVEL(BuffSkill));
        attack = attack*(1+pConf->skillLvBase.fInAttackPrecent);
    }
    else
    {
        attack = attack;
    }
    return attack;
}

float UnitBaseConf::getOutDef(int BuffSkill)
{
    if (BuffSkill != 0)
    {
        const SkillLevel_t *pConf = GameData::getSkillConf(GET_SKILL_ID(BuffSkill), GET_SKILL_LEVEL(BuffSkill));
        return fOutDef*(1+pConf->skillLvBase.fOutDefPrecent);
    }
    else
    {
        return fOutDef;
    }
}

float UnitBaseConf::getInDef(int BuffSkill)
{
    if (BuffSkill != 0)
    {
        const SkillLevel_t *pConf = GameData::getSkillConf(GET_SKILL_ID(BuffSkill), GET_SKILL_LEVEL(BuffSkill));
        return fInDef*(1+pConf->skillLvBase.fInDefPrecent);
    }
    else
    {
        return fInDef;
    }
}


float UnitBaseConf::getReduceHurt(int BuffSkill)
{
    if (BuffSkill != 0)
    {
        const SkillLevel_t *pConf = GameData::getSkillConf(GET_SKILL_ID(BuffSkill), GET_SKILL_LEVEL(BuffSkill));
        return (fReducehurt + pConf->skillLvBase.nReducehurtValue)*(1+pConf->skillLvBase.fReducehurtPrecent);
    }
    else
    {
        return fReducehurt;
    }
}

float UnitBaseConf::getAddhurt(int BuffSkill)
{
    if (BuffSkill != 0)
    {
        const SkillLevel_t *pConf = GameData::getSkillConf(GET_SKILL_ID(BuffSkill), GET_SKILL_LEVEL(BuffSkill));
        return (fAddhurt + pConf->skillLvBase.nAddhurtValue)*(1+pConf->skillLvBase.fAddhurtPrecent);
    }
    else
    {
        return fAddhurt;
    }
}

float UnitBaseConf::getRecoverHP()
{
    return fRecoverHp;
}

float UnitBaseConf::getCritChance(int attackBuffSkill,int underAttackBuffSkill,UnitBaseConf* pUnderAttacked)
{
    float fCritChance = getCrit(attackBuffSkill)/(getCrit(attackBuffSkill) + pUnderAttacked->getToughness(attackBuffSkill) + pUnderAttacked->getLV()*100 + 500);
    return fCritChance;
}

float UnitBaseConf::getCrit(int BuffSkill)
{
    if (BuffSkill != 0)
    {
        const SkillLevel_t *pConf = GameData::getSkillConf(GET_SKILL_ID(BuffSkill), GET_SKILL_LEVEL(BuffSkill));
        return fCrit + pConf->skillLvBase.nCrit;
    }
    else
    {
        return fCrit;
    }
}

float UnitBaseConf::getToughness(int BuffSkill)
{
    return fToughness;
}


float UnitBaseConf::getCritTimes()
{
    return 1.5;
}

#endif

