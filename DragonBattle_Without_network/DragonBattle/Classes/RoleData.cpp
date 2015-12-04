//
//  RoleData.cpp
//  DragonBattle
//
//  Created by zeng xiaolong on 13-8-8.
//
//
#include "lib_formula.h"
#include "RoleData.h"
#include "UserData.h"
#include "OnlineManager.h"
#include "GameManager.h"

using namespace LIB_FORMULA;

// 穿上装备
void RoleData::addEquipItem(package_equipment_t bagItem)
{
    userEquip.erase(GameData::getItem(bagItem.equipment_id)->u.equipment.nItemType);
    userEquip[GameData::getItem(bagItem.equipment_id)->u.equipment.nItemType] = bagItem;//带上人物装备
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EQUIPCHANGE_NOTE);
}

// 脱下装备
void RoleData::removeEquipItem(int Equiptype)
{
    if (userEquip.end() != userEquip.find(Equiptype))
    {
        userEquip.erase(Equiptype);
        CCNotificationCenter::sharedNotificationCenter()->postNotification(EQUIPCHANGE_NOTE);
    }
}

void RoleData::setExp(uint64_t Exp)
{
    nExp = Exp;
}

uint64_t RoleData::getExp()
{
    return nExp;
}

void RoleData::addExp(uint64_t addexp)
{
    setExp(addexp + nExp);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(ATTRBITECHANGE_NOTE);
}

int RoleData::getExtraAttr(EquipAttributeType_t type)
{
    //加入装备效果
    int nAttr = 0;
    if (userEquip.size() != 0)
    {
        std::map<int, package_equipment_t>::iterator it = userEquip.begin();
        for (; it != userEquip.end(); it++)
        {
            package_equipment_t *pEquipment = &(it->second);
            nAttr += get_equipment_attr(pEquipment, (int)type);
        }
    }
    
    //加入占星效果
    for (int i = 0; i < 4; i++)
    {
        if (star_array[i].star_id != 0)
        {
            const item_t* pItem = GameData::getItem(star_array[i].star_id);
            if (!pItem)
            {
                continue;
            }
            if (pItem->u.star.nAddHurt != 0 && type == ADD_HURT)
            {
                nAttr += pItem->u.star.nAddHurt * star_array[i].level;
            }
            if (pItem->u.star.nReduceHurt != 0 && type == REDUCE_HURT)
            {
                nAttr += pItem->u.star.nReduceHurt * star_array[i].level;
            }
            if (pItem->u.star.nBaseDefCrit != 0 && type == TOUGHNESS)
            {
                nAttr += pItem->u.star.nBaseDefCrit * star_array[i].level;
            }
            if (pItem->u.star.nBaseCrit != 0 && type == CRIT)
            {
                nAttr += pItem->u.star.nBaseCrit * star_array[i].level;
            }
            if (pItem->u.star.nBaseHp != 0 && type == HP)
            {
                nAttr += pItem->u.star.nBaseHp * star_array[i].level;
            }
            if (pItem->u.star.nBaseInnerAtk != 0 && type == IN_ATTACK)
            {
                nAttr += pItem->u.star.nBaseInnerAtk * star_array[i].level;
            }
            if (pItem->u.star.nBaseAtk != 0 && type == OUT_ATTACK)
            {
                nAttr += pItem->u.star.nBaseAtk * star_array[i].level;
            }
            if (pItem->u.star.nBaseInnerDef != 0 && type == IN_DEFENCE)
            {
                nAttr += pItem->u.star.nBaseInnerDef * star_array[i].level;
            }
            if (pItem->u.star.nBaseDef != 0 && type == OUT_DEFENCE)
            {
                nAttr += pItem->u.star.nBaseDef * star_array[i].level;
            }
        }
    }
    return nAttr;
}

int RoleData::getExtraMaxHp()
{
    return getExtraAttr(HP);
}

int RoleData::getExtraOutAttack()
{
    return getExtraAttr(OUT_ATTACK);
}

int RoleData::getExtraOutDefence()
{
    return getExtraAttr(OUT_DEFENCE);
}

int RoleData::getExtraInAttack()
{
    return getExtraAttr(IN_ATTACK);
}

int RoleData::getExtraInDefence()
{
    return getExtraAttr(IN_DEFENCE);
}


//--------------------------------------------------------------------------------------------------------------------------
//Begin HerorData

uint32_t HeroData::getNerveLevel(void)
{
    return nNerveLevel;
};
void HeroData::setNerveLevel(uint32_t var)
{
    nNerveLevel = var;
};

void HeroData::addNerveLevel(uint32_t NerveLevel)
{
    nNerveLevel += NerveLevel;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(ATTRBITECHANGE_NOTE);
}

uint32_t HeroData::getInForceExp(void)
{
    return nInForceExp;
};

void HeroData::setInForceExp(uint32_t var)
{
    nInForceExp = var;
};

int HeroData::getSKillLv(int skillID)
{
    if(skillID%100>=8)
    {
        int index= skillID%100 - 8;
        return skillBuffLv[index];
    }
    else
    {
        int index= skillID%100 - 1;
        return GET_SKILL_LEVEL(skill[index]);
    }
}

void HeroData::skillLvChange(int buffID,int lv)
{
    
    if(buffID%100>=8)
    {
        int index= buffID%100 - 8;
        skillBuffLv[index] = lv;
    }
    else
    {
        int index= buffID%100 - 1;
        skill[index] = MERGE_SKILL_ID_LV(buffID,lv);
    }
    CCNotificationCenter::sharedNotificationCenter()->postNotification(ATTRBITECHANGE_NOTE);
}

void HeroData::addInForceExp(uint32_t InForceExp)
{
    nInForceExp += InForceExp;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(ATTRBITECHANGE_NOTE);
}

const RoleAttrConf_t* HeroData::getBaseAttr()
{
    return GameData::getRoleAttrConf(getLV(), nUnitID);
}

RoleAttrConf_t HeroData::getBuffAttr()
{
    RoleAttrConf_t Conf;
    memset(&Conf,0x0,sizeof(Conf));
    for (int i = 0; i<4; i++)
    {
        if (skillBuffLv[i] == 0)
        {
            continue;
        }
        const SkillLevel_t*skilllv = GameData::getSkillConf(1000*nUnitID +8 + i, skillBuffLv[i]);
        Conf.nCrit += skilllv->skillLvBase.nCrit;
        Conf.nLife += skilllv->skillLvBase.nHPValue;
        Conf.nToughness += skilllv->skillLvBase.nToughness;
        Conf.nInAttack += skilllv->skillLvBase.nInAttackValue;
        Conf.nOutAttack += skilllv->skillLvBase.nOutAttackValue;
        Conf.nInDef += skilllv->skillLvBase.nInDefValue;
        Conf.nOutDef += skilllv->skillLvBase.nOutDefValue;
        Conf.nAddHurt += skilllv->skillLvBase.nAddhurtValue;
        Conf.nReduceHurt += skilllv->skillLvBase.nReducehurtValue;
    }
    return Conf;
}

void HeroData::addExp(uint64_t addexp)
{
    int lv = getLV();
    setExp(addexp + nExp);
    int newlv = getLV();
    if (addexp != 0)
    {
        CCNotificationCenter::sharedNotificationCenter()->postNotification(EXPCHANGE_NOTE);
    }
    if (lv < newlv)
    {
        CCNotificationCenter::sharedNotificationCenter()->postNotification(LVCHANGE_NOTE);
        CCNotificationCenter::sharedNotificationCenter()->postNotification(ATTRBITECHANGE_NOTE);
        OnlineManager::sharedManager()->userGetTaskList();
    }
}

void HeroData::refreshBaseAttr()
{
    const RoleForceConf_t* pForceConf = GameData::getRoleForceSum(GameData::getRoleForceLevel(nInForceExp));
    const RoleAttrConf_t*  pBaseRole  = this->getBaseAttr();
    const RoleNerveConf_t* pNerveConf = GameData::getRoleNerve(nNerveLevel);
    RoleAttrConf_t pBuffConf = getBuffAttr();
    baseAttr.nLife      = pBaseRole->nLife + getExtraMaxHp() + trainAttr.nLife + pForceConf->nLife + pNerveConf->nAttrValue[HP] + pBuffConf.nLife;
    baseAttr.nLv        = getLV();
    baseAttr.fOutAttack = pBaseRole->nOutAttack + getExtraOutAttack() + trainAttr.nOutAttack + pForceConf->nOutAttack + pNerveConf->nAttrValue[OUT_ATTACK] + pBuffConf.nOutAttack;;
    baseAttr.fOutDef    = pBaseRole->nOutDef + getExtraOutDefence() + trainAttr.nOutDef + pForceConf->nOutDef + pNerveConf->nAttrValue[OUT_DEFENCE] + pBuffConf.nOutDef;
    baseAttr.fInAttack  = pBaseRole->nInAttack + getExtraInAttack() + trainAttr.nInAttack + pForceConf->nInAttack + pNerveConf->nAttrValue[IN_ATTACK] + pBuffConf.nInAttack;
    baseAttr.fInDef     = pBaseRole->nInDef + getExtraInDefence() + trainAttr.nInDef + pForceConf->nInDef + pNerveConf->nAttrValue[IN_DEFENCE] + pBuffConf.nInDef;
    baseAttr.fCrit      = pBaseRole->nCrit + getExtraAttr(CRIT) + pForceConf->nCrit + pNerveConf->nAttrValue[CRIT] + pBuffConf.nCrit;
    baseAttr.fToughness = pBaseRole->nToughness + getExtraAttr(TOUGHNESS) + trainAttr.nToughness + pForceConf->nToughness + pNerveConf->nAttrValue[TOUGHNESS] + pBuffConf.nToughness;
    baseAttr.fReducehurt= pBaseRole->nReduceHurt + getExtraAttr(REDUCE_HURT) + pForceConf->nReduceHurt + pNerveConf->nAttrValue[REDUCE_HURT] + pBuffConf.nReduceHurt;
    baseAttr.fAddhurt   = pBaseRole->nAddHurt + getExtraAttr(ADD_HURT) + pForceConf->nAddHurt + pNerveConf->nAttrValue[ADD_HURT] + pBuffConf.nAddHurt;
    baseAttr.fRecoverHp = pBaseRole->nRecoverHP + getExtraAttr(RECOVER_HP) + pForceConf->nRecoverHP + pNerveConf->nAttrValue[RECOVER_HP];
}
//--------------------------------------------------------------------------------------------------------------------------
//Begin PartnerData

void PartnerData::addExp(uint64_t addexp)
{
    setExp(addexp + nExp);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(ATTRBITECHANGE_NOTE);
}

void PartnerData::refreshBaseAttr()
{
    RoleAttrConf_t pConf;
    GameData::getPartnerAttrConf(getLV(),nUnitID, &pConf);
    baseAttr.nLv        = getLV();
    
    baseAttr.nLife      = (pConf.nLife + getExtraMaxHp() + trainAttr.nLife);
    
    baseAttr.fOutAttack = (pConf.nOutAttack + getExtraOutAttack() + trainAttr.nOutAttack);
    
    baseAttr.fOutDef    = (pConf.nOutDef + getExtraOutDefence() + trainAttr.nOutDef);
    
    baseAttr.fInAttack  = (pConf.nInAttack + getExtraInAttack() + trainAttr.nInAttack);
    
    baseAttr.fInDef     = (pConf.nInDef + getExtraInDefence() + trainAttr.nInDef);
    
    baseAttr.fCrit      = (pConf.nCrit + getExtraAttr(CRIT));
    
    baseAttr.fToughness = (pConf.nToughness + getExtraAttr(TOUGHNESS) + trainAttr.nToughness);
    
    baseAttr.fReducehurt= (pConf.nReduceHurt + getExtraAttr(REDUCE_HURT));
    baseAttr.fAddhurt   = (pConf.nAddHurt + getExtraAttr(ADD_HURT));
    baseAttr.fRecoverHp = (pConf.nRecoverHP + getExtraAttr(RECOVER_HP));
}
