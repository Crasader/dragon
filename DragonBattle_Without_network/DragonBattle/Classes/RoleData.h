//
//  RoleData.h
//  DragonBattle
//
//  Created by zeng xiaolong on 13-8-8.
//
//

#ifndef __DragonBattle__RoleData__
#define __DragonBattle__RoleData__

#include "cocos2d.h"
#include "GameData.h"
#include <vector>
#include "NotificeDefine.h"

USING_NS_CC;

class trainAttribute
{
public:
    trainAttribute()
    {
        nLife = 0;
        nOutAttack = 0;
        nOutDef = 0;
        nInAttack = 0;
        nInDef = 0;
    }
    ~trainAttribute() {}
    
public:
    int    nLife;
    int    nOutAttack;        //攻击力
    int    nOutDef;           //防御力
    int    nInAttack;         //内攻
    int    nInDef;            //内防
    int    nToughness;        //韧性
};

class packageUnit_t
{
public:
    uint32_t packageID;
    uint32_t itemID;
    uint32_t itemCount;
    uint32_t level;
};

class RoleData : public CCObject
{
public:
    static RoleData *create()
    { 
        RoleData *pRet = new RoleData();
        if (pRet)
        { 
            pRet->autorelease();
            return pRet;
        } 
        else 
        { 
            delete pRet; 
            pRet = NULL; 
            return NULL; 
        } 
    }
    
    RoleData()
    {
        nExp = 0;
        memset(&trainAttr, 0, sizeof(trainAttr));
        memset(star_array,  0, sizeof(star_array));
    }
    ~RoleData()
    {
        
    }
    
public:
    CC_PROPERTY(uint64_t,nExp,Exp);
    virtual void addExp(uint64_t addexp);
    
    uint32_t        nUnitID;        //人物ID
    trainAttribute  trainAttr;      //伙伴的培养属性
    UnitBaseConf    baseAttr;       //总属性, 初始化或属性有改变是重新计算
    std::map<int, package_equipment_t> userEquip;//1武器2帽子3衣服4鞋子5护腕6戒指7项链8玉玺
    star_ston_info_t star_array[4];
    
public:
    virtual int getExtraAttr(EquipAttributeType_t type);
    virtual int getExtraMaxHp();
    virtual int getExtraOutAttack();
    virtual int getExtraOutDefence();
    virtual int getExtraInAttack();
    virtual int getExtraInDefence();
    
    virtual int getLV()         { return GameData::getLV(nExp); }
    virtual int getMaxHp()      { return baseAttr.nLife; }
    virtual int getOutAttack()  { return baseAttr.fOutAttack; }
    virtual int getOutDefence() { return baseAttr.fOutDef; }
    virtual int getInAttack()   { return baseAttr.fInAttack; }
    virtual int getInDefence()  { return baseAttr.fInDef; }
    virtual int getCrit()       { return baseAttr.fCrit; }
    virtual int getToughness()  { return baseAttr.fToughness; }
    virtual int getReduceHurt() { return baseAttr.fReducehurt; }
    virtual int getAddhurt()    { return baseAttr.fAddhurt; }
    virtual int getRecoverHP()  { return baseAttr.fReducehurt; }
    virtual int getFightPoint()
    {
        refreshBaseAttr();
        int attack = baseAttr.getFightPoint();
        return attack;
    }
    void addEquipItem(package_equipment_t bagItem);
    void removeEquipItem(int Equiptype);
    
    virtual void refreshBaseAttr()
    {
        RoleAttrConf_t pConf;
        GameData::getPartnerAttrConf(getLV(),nUnitID, &pConf);
        baseAttr.nLife      = pConf.nLife + getExtraMaxHp() + trainAttr.nLife;
        baseAttr.nLv        = getLV();
        baseAttr.fOutAttack = pConf.nOutAttack + getExtraOutAttack() + trainAttr.nOutAttack;
        baseAttr.fOutDef    = pConf.nOutDef + getExtraOutDefence() + trainAttr.nOutDef;
        baseAttr.fInAttack  = pConf.nInAttack + getExtraInAttack() + trainAttr.nInAttack;
        baseAttr.fInDef     = pConf.nInDef + getExtraInDefence() + trainAttr.nInDef;
        baseAttr.fCrit      = pConf.nCrit + getExtraAttr(CRIT);
        baseAttr.fToughness = pConf.nToughness + getExtraAttr(TOUGHNESS) + trainAttr.nToughness;
        baseAttr.fReducehurt= pConf.nReduceHurt + getExtraAttr(REDUCE_HURT);
        baseAttr.fAddhurt   = pConf.nAddHurt + getExtraAttr(ADD_HURT);
        baseAttr.fRecoverHp = pConf.nRecoverHP + getExtraAttr(RECOVER_HP);
    }
};

class PartnerData : public RoleData
{
public:
    PartnerData()
    {
        baseAttr.nUnitType = kPartnerType;
    }
    virtual void addExp(uint64_t addexp);
    static PartnerData *create(int unitID)
    {
        PartnerData *pRet = new PartnerData();
        if (pRet)
        {
            const PartnerConf_t* pPartnerConf = GameData::getPartnerConf(unitID);
            pRet->baseAttr.nUnitID = unitID;
            pRet->baseAttr.nUnitType = kPartnerType;
            pRet->baseAttr.nResourceID = pPartnerConf->nResourceID;
            pRet->nUnitID = unitID;
            
            memset(pRet->baseAttr.unitName, 0x0, 64);
            strcpy(pRet->baseAttr.unitName, pPartnerConf->strName);
            pRet->autorelease();
            return pRet;
        }
        else
        {
            delete pRet;
            pRet = NULL;
            return NULL;
        }
    }
    
    virtual int getFightPoint()
    {
        refreshBaseAttr();
        const PartnerConf_t *partnerConf = GameData::getPartnerConf(nUnitID);
        int attack = baseAttr.getFightPoint() + 10*GameData::getPartnerSkillHurt(partnerConf->nSkillID,skillLv);
        return attack;
    }

public:
    uint32_t        skillLv;	//技能等级
	uint32_t        curseat;  //当前位置
    uint32_t        inheritstate;//0x1已传承 0x2已继承 0x3已传承并已继承
    
public:
    virtual void refreshBaseAttr();
    virtual int getLV() { return GameData::getLV(nExp); }
};

class HeroData : public RoleData
{
public:
    HeroData() 
    {
        memset(&skillBuffLv, 0x0, sizeof(skillBuffLv));
        memset(&skill, 0x0, sizeof(skill));
        
    };
    static HeroData *create()
    {
        HeroData *pRet = new HeroData();
        if (pRet)
        {
            pRet->baseAttr.nUnitType = kHero;
            pRet->autorelease();
            return pRet;
        }
        else
        {
            delete pRet;
            pRet = NULL;
            return NULL;
        }
    }
    
    virtual int getFightPoint()
    {
        refreshBaseAttr();
        int skillPoint = 0;
        for (int i = 0; i<4; i++)
        {
            skillPoint += GET_SKILL_LEVEL(skill[i]) * 15;
        }
        int attack = baseAttr.getFightPoint() + skillPoint;
        return attack;
    }
    
    bool hasNewSkill;
    uint32_t skill[4];
    uint16_t skillBuffLv[4];
    void skillLvChange(int buffID,int lv);
    int getSKillLv(int skillID);
    
    CC_PROPERTY(uint32_t, nNerveLevel, NerveLevel); //经脉等级
    void addNerveLevel(uint32_t NerveLevel);
    CC_PROPERTY(uint32_t,nInForceExp,InForceExp);       //炼气经验值
    void addInForceExp(uint32_t InForceExp);

    RoleAttrConf_t getBuffAttr();//心法属性
    virtual void addExp(uint64_t addexp);
    const RoleAttrConf_t* getBaseAttr();
    virtual void refreshBaseAttr();
    virtual int getLV() { return GameData::getLV(nExp); }

};
#endif /* defined(__DragonBattle__RoleData__) */
