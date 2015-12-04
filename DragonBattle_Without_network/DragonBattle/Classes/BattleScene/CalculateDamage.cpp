//
//  CalculateDamage.cpp
//  DragonBattle
//
//  Created by zeng xiaolong on 13-11-2.
//
//

#include "CalculateDamage.h"
#include "CRUtils.h"
#include "UserData.h"
#include "GameManager.h"
float CalculateDamage::getNormalAttackHurt(int attackBuffSkill,int underAttackBuffSkill,UnitBaseConf*Attacked,UnitBaseConf* pUnderAttacked, float fOutAttackCorrect, float fInAttackCorrect,bool &bCrit)
{
    int hurt = 0;
    if(GameManager::sharedInstance()->getBattleLayer()->getBattleType() ==  kArena || GameManager::sharedInstance()->getBattleLayer()->getBattleType() == kKeap)
    {
        hurt = getOutAttackHurt(attackBuffSkill,underAttackBuffSkill,Attacked,pUnderAttacked,fOutAttackCorrect) + getInAttackHurt(attackBuffSkill,underAttackBuffSkill,Attacked,pUnderAttacked,fInAttackCorrect);
        if (CCRANDOM_0_1()>Attacked->getCritChance(attackBuffSkill,underAttackBuffSkill,pUnderAttacked))
        {
            bCrit = false;
        }
        else
        {
            bCrit = true;
            hurt = hurt*Attacked->getCritTimes();
        }
    }
    else
    {
        if (Attacked->nUnitType == kHero)//英雄攻击
        {
            
            hurt = getOutAttackHurt(attackBuffSkill,0,Attacked,pUnderAttacked,fOutAttackCorrect) + getInAttackHurt(attackBuffSkill,0,Attacked,pUnderAttacked,fInAttackCorrect);
            if (CCRANDOM_0_1()>Attacked->getCritChance(attackBuffSkill,0,pUnderAttacked))
            {
                bCrit = false;
            }
            else
            {
                bCrit = true;
                hurt = hurt*Attacked->getCritTimes();
            }
            int present = gUserData->getParnterTalentByID(10)*1000;
            if(present>0)
            {
                int random = CRUtils::getRandomNum(0, 1000);
                if (random<present)
                {
                    hurt = pUnderAttacked->getMaxHP();
                }
            }
        }
        
        if (Attacked->nUnitType == kNormalMonster ||
            Attacked->nUnitType == kFollowMonster||
            Attacked->nUnitType == kBoss||
            Attacked->nUnitType == kPartnerType)//怪物攻击
        {
            hurt = getOutAttackHurt(0,underAttackBuffSkill,Attacked,pUnderAttacked,fOutAttackCorrect) + getInAttackHurt(0,underAttackBuffSkill,Attacked,pUnderAttacked,fInAttackCorrect);
            if (CCRANDOM_0_1()>Attacked->getCritChance(0,underAttackBuffSkill,pUnderAttacked))
            {
                bCrit = false;
            }
            else
            {
                bCrit = true;
                hurt = hurt*Attacked->getCritTimes();
            }
        }
    }
    if (hurt<1)
    {
        hurt = 1;
    }
    return hurt;
}

float CalculateDamage::getSkillAttackHurt(int attackskill,int attackBuffSkill,int underAttackBuffSkill,UnitBaseConf*Attacked,UnitBaseConf* pUnderAttacked, float fOutAttackCorrect, float fInAttackCorrect,bool &bCrit)
{
    if (Attacked->nUnitType == kPartnerType)
    {
        return getThrowAttackHurt(attackskill, attackBuffSkill, underAttackBuffSkill,Attacked,pUnderAttacked,fOutAttackCorrect,fInAttackCorrect,bCrit);
    }
    const SkillLevel_t *pConf = GameData::getSkillConf(GET_SKILL_ID(attackskill), GET_SKILL_LEVEL(attackskill));
    
    int intattack = fInAttackCorrect * MAX(Attacked->getInAttack(attackBuffSkill,attackskill)*(1+pConf->skillLvBase.fInAttackPrecent) - pUnderAttacked->getInDef(underAttackBuffSkill),0);
    intattack += CRUtils::getRandomNum(1, pUnderAttacked->getLV());
    
    int outtattack =fOutAttackCorrect * MAX(Attacked->getOutAttack(attackBuffSkill,attackskill)*(1+pConf->skillLvBase.fOutAttackPrecent) - pUnderAttacked->getOutDef(underAttackBuffSkill), 0);
    outtattack += CRUtils::getRandomNum(1,pUnderAttacked->getLV());
    
    int hurt = outtattack + intattack;
    int addhurt = Attacked->getAddhurt(attackBuffSkill)*(1+pConf->skillLvBase.fAddhurtPrecent) + pConf->skillLvBase.nAddhurtValue - pUnderAttacked->getReduceHurt(underAttackBuffSkill);
    addhurt = addhurt>0?addhurt:0;
    hurt += addhurt;
    if (CCRANDOM_0_1()>Attacked->getCritChance(attackBuffSkill,underAttackBuffSkill,pUnderAttacked))
    {
        bCrit = false;
    }
    else
    {
        bCrit = true;
        hurt = hurt*Attacked->getCritTimes();
    }
    if (hurt<1)
    {
        hurt = 1;
    }
    
    if (Attacked->nUnitType == kHero)
    {
        int present = gUserData->getParnterTalentByID(10)*1000;
        if(present>0)
        {
            int random = CRUtils::getRandomNum(0, 1000);
            if (random<present)
            {
                hurt = pUnderAttacked->getMaxHP();
            }
        }
    }
    return hurt;
}

float CalculateDamage::getThrowAttackHurt(int attackskill,int attackBuffSkill,int underAttackBuffSkill,UnitBaseConf*Attacked,UnitBaseConf* pUnderAttacked,float fOutAttackCorrect, float fInAttackCorrect,bool &bCrit)
{
    if (Attacked->nUnitType == kHero || Attacked->nUnitType == kEnemyHero)//英雄攻击
    {
        return CalculateDamage::getSkillAttackHurt(attackskill, attackBuffSkill, 0,Attacked,pUnderAttacked,fOutAttackCorrect,fInAttackCorrect,bCrit);
    }
    else if (Attacked->nUnitType == kNormalMonster||
        Attacked->nUnitType == kFollowMonster||
        Attacked->nUnitType == kBoss)//怪物攻击
    {
        return CalculateDamage::getNormalAttackHurt(0, underAttackBuffSkill, Attacked, pUnderAttacked, fOutAttackCorrect, fInAttackCorrect, bCrit);
    }
    else if(Attacked->nUnitType == kPartnerType)//侠客攻击
    {
        int hurt = getInAttackHurt(0,0,Attacked,pUnderAttacked,fInAttackCorrect) + getOutAttackHurt(0,0,Attacked,pUnderAttacked,fOutAttackCorrect);
        
        hurt = hurt - pUnderAttacked->getReduceHurt(0) + Attacked->getAddhurt(0) + GameData::getPartnerSkillHurt(GET_SKILL_ID(attackskill),GET_SKILL_LEVEL(attackskill));
        ;
        
        if (CCRANDOM_0_1()>Attacked->getCritChance(attackBuffSkill,underAttackBuffSkill,pUnderAttacked))
        {
            bCrit = false;
        }
        else
        {
            bCrit = true;
            hurt = hurt*Attacked->getCritTimes();
        }
        return hurt;
    }
    return 0;
}

float CalculateDamage::getOutAttackHurt(int attackBuffSkill,int underAttackBuffSkill,UnitBaseConf*Attacked,UnitBaseConf* pUnderAttacked,float fOutAttackCorrect)
{
    int hurt = fOutAttackCorrect * MAX(Attacked->getOutAttack(attackBuffSkill) - pUnderAttacked->getOutDef(underAttackBuffSkill), 0);
    hurt += CRUtils::getRandomNum(1,pUnderAttacked->getLV());
    return hurt;
}

float CalculateDamage::getInAttackHurt(int attackBuffSkill,int underAttackBuffSkill,UnitBaseConf*Attacked,UnitBaseConf* pUnderAttacked,float fInAttackCorrect)
{
    int hurt = fInAttackCorrect * MAX(Attacked->getInAttack(attackBuffSkill) - pUnderAttacked->getInDef(underAttackBuffSkill),0);
    hurt += CRUtils::getRandomNum(1,pUnderAttacked->getLV());
    return hurt;
}