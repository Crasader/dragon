//
//  UserData.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-16.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//
#include "UserData.h"
#include "GameData.h"
#include "OnlineManager.h"
#include "lib_formula.h"
#include "ServerConfig.h"
#include "NotificeDefine.h"
#include "UserDataEx.h"
#include "GameManager.h"

using namespace LIB_FORMULA;
UserData* UserData::pSharedInstace = NULL;

UserData::UserData()
{
    m_findTarget = FindTarget::create();
    CC_SAFE_RETAIN(m_findTarget);
    
    hiddenOtherUser = false;
    
    server_id = 0;
    
    nTotalUserCount = 0;
    nLastCommunityID = 0;
    nWorldBossCommunityID = 0;
    
    nMedicine = 0;
    
    nBuyGold = 0;
    nCostGold = 0;
    
    m_heroData = HeroData::create();
    CC_SAFE_RETAIN(m_heroData);
    
    m_dataEx = UserDataEx::shareInstance();
    CC_SAFE_RETAIN(m_dataEx);

    m_partnerDic = CCDictionary::create();
    CC_SAFE_RETAIN(m_partnerDic);
    
    m_arrPartner = CCArray::create();
    CC_SAFE_RETAIN(m_arrPartner);
    
    OnceSetMap.clear();
    OnceSetMap.insert(std::pair<int, int>(NO_GOLD_SPEND_WARNING, 0));
    OnceSetMap.insert(std::pair<int, int>(NO_SELL_WARNING, 0));
    OnceSetMap.insert(std::pair<int, int>(NO_GOLD_BOOKED_WARNING, 0));
    OnceSetMap.insert(std::pair<int, int>(NO_GOLD_STRENGTHEN_CANCEL_COOLDOWN_WARNING, 0));
    OnceSetMap.insert(std::pair<int, int>(NO_GOLD_STRENGTHEN_WASH_WARNING, 0));
    OnceSetMap.insert(std::pair<int, int>(NO_GOLD_INFORCE_WARNING, 0));
    OnceSetMap.insert(std::pair<int, int>(NO_GOLD_NERVE_WARNING, 0));
    OnceSetMap.insert(std::pair<int, int>(NO_GOLD_NERVE_CANCEL_COOLDOWN_WARNING, 0));
    OnceSetMap.insert(std::pair<int, int>(NO_GOLD_SECRET_SHOP_WARNING, 0));
    OnceSetMap.insert(std::pair<int, int>(NO_GOLD_CLEAN_SPORTS_CD_WARNING, 0));
    OnceSetMap.insert(std::pair<int, int>(NO_GOLD_CLEAN_TAVERN_CD_WARNING, 0));
    OnceSetMap.insert(std::pair<int, int>(NO_GOLD_WANTED_ALL_STAR_WARNING, 0));
    OnceSetMap.insert(std::pair<int, int>(NO_GOLD_WANTED_QUICK_FINISH_WARNING, 0));
    OnceSetMap.insert(std::pair<int, int>(NO_YXB_WANTED_REFRESH_WARNING, 0));
    OnceSetMap.insert(std::pair<int, int>(NO_GOLD_WORSHIP_100_WARNING, 0));
    OnceSetMap.insert(std::pair<int, int>(NO_GOLD_WORSHIP_20_WARNING, 0));
    OnceSetMap.insert(std::pair<int, int>(NO_GOLD_LUCKY_BONUS_WARNING, 0));
}

UserData::~UserData()
{
    CC_SAFE_RELEASE_NULL(m_findTarget);
    CC_SAFE_RELEASE_NULL(m_partnerDic);
    CC_SAFE_RELEASE_NULL(m_heroData);
    CC_SAFE_RELEASE_NULL(m_dataEx);
    CC_SAFE_RELEASE_NULL(m_arrPartner);
    OnceSetMap.clear();
}

UserData* UserData::sharedInstance()
{
    if (!pSharedInstace)
    {
        pSharedInstace = new UserData();
    }
    return pSharedInstace;
}

bool UserData::resetData()
{
    memset(nick, 0, sizeof(nick));
    yxb = 0;
    nGold = 0;             //黄金
    nVipLevel = 1;          //vip等级
    m_findTarget->nCommunityID = -1;
    m_findTarget->nNPCID = -1;
    m_findTarget->nMissionID = -1;
    GlobalMessages.clear();
    FriendMessages.clear();
    BagEquips.clear();
    BagItems.clear();
    BagCards.clear();
    Bagstuff.clear();

    return true;
}

uint32_t UserData::getCurCommunityID()
{
    int community = 0;
    community = nWorldBossCommunityID == 0 ? nLastCommunityID : nWorldBossCommunityID;
    return community;
}

void UserData::setCurCommunityID(uint32_t curID, bool in_boss_community)
{
    CCAssert(curID < WORLD_BOSS_MAP_ID, "Usage: curID must be community ID.");
    
    if (in_boss_community)
    {
        nWorldBossCommunityID = WORLD_BOSS_MAP_ID;
        nLastCommunityID = curID;
    }
    else
    {
        nWorldBossCommunityID = 0;
        nLastCommunityID = curID;
    }
}

uint32_t UserData::getEnery()
{
    return nEnery;
}

void UserData::setEnery(uint32_t enery)
{
    nEnery = enery;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(SETUSERATTR_NOTE);
}

void UserData::addEnery(uint32_t addenery)
{
    //nEnery += addenery;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(SETUSERATTR_NOTE);
}

uint32_t UserData::getMaxEnery()
{
    return nMaxEnery;
}

void UserData::setMaxEnery(uint32_t maxEnery)
{
    nMaxEnery = maxEnery;
}

void UserData::addExp(uint64_t addExp)
{
    if (addExp == 0) return;
    CCArray *arr = getBattleRoleArray();
    CCObject *obj = NULL;
    CCARRAY_FOREACH(arr, obj)
    {
       RoleData *role = (RoleData*)obj;
       role->addExp(addExp);
    }
}

uint32_t UserData::getBuyEneryCount()
{
    return nBuyEneryCount;
}

void UserData::setBuyEneryCount(uint32_t enery)
{
    nBuyEneryCount = enery;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(SETUSERATTR_NOTE);
}

void UserData::addBuyEneryCount(int addCount)
{
    nBuyEneryCount += addCount;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(SETUSERATTR_NOTE);
}

int UserData::getEneryTime()
{
    return nEneryTime;
}

void UserData::setEneryTime(int enerytime)
{
    nEneryTime = enerytime;
    if(nEneryTime>0)
    {
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(UserData::refreshEnery), this, 1, false);
    }
}

void UserData::refreshEnery(float ftime)
{
    if (nEneryTime<=0 && getEnery()<getMaxEnery())
    {
        if(OnlineManager::sharedManager()->userRecoverEneryToOnline()>0)
        {
            CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(UserData::refreshEnery), this);
        }
    }
    else
    {
        nEneryTime --;
    }
}

char* UserData::getShortNum(uint32_t nNum, char* str)
{
    if (nNum >= 100000)
    {
        sprintf(str, "%d万", nNum / 10000);
    }
    else
    {
        sprintf(str, "%d", nNum);
    }
    return str;
}

void UserData::addPower(int addPower)
{
    nPower += addPower;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(SETUSERATTR_NOTE);
}

void UserData::addYXB(int addYXB)
{
    yxb += addYXB;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(SETUSERATTR_NOTE);
}

void UserData::setCostGold(uint32_t gold)
{
    nCostGold = gold;
}

uint32_t UserData::getCostGold()
{
    return nCostGold;
}

void UserData::resetBuyGold(uint32_t nGold)
{
    nBuyGold = nGold;
    
    nVipLevel = GameData::getVipLvByCostGold(nBuyGold);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(SETUSERATTR_NOTE);
}

void UserData::setBuyGold(uint32_t var)
{
    if (var >= nBuyGold)
    {
        nBuyGold = var;
    }
    nVipLevel = GameData::getVipLvByCostGold(nBuyGold);
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(SETUSERATTR_NOTE);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(HAS_DAILY_NOTE);
}

uint32_t UserData::getBuyGold()
{
    return nBuyGold;
}

void UserData::addGold(int addgold)
{
    if (addgold == 0) return;
    int curgold = addgold + (int)nGold;
    if( curgold < 0)
    {
        CCAssert(curgold<0, "不可能");
        nGold = 0;
    }
    else
    {
        nGold += addgold;
    }
    CCNotificationCenter::sharedNotificationCenter()->postNotification(SETUSERATTR_NOTE);
}

void UserData::addPrestige(int nAddPrestige)
{
    nPrestige += nAddPrestige;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(SETUSERATTR_NOTE);
}

void UserData::addNerve(int addNerve)
{
    nNerveValue += addNerve;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(SETUSERATTR_NOTE);
}

void UserData::addStarStoneExp(int starStoneexp)
{
    if (starStoneexp == 0) return;
    nStarStoneExp += starStoneexp;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(SETUSERATTR_NOTE);
}

CCArray *UserData::getBattleRoleArray()
{
    CCArray *arr = CCArray::create();
    arr->addObject(m_heroData);
    for (int i = 1; i<=m_partnerDic->count(); i++)
    {
        if (m_partnerDic->objectForKey(i))
        {
            arr->addObject(m_partnerDic->objectForKey(i));
        }
    }
    return arr;
}

CCArray *UserData::getAllRoleArray()
{
    CCArray *arr = CCArray::create();
    arr->addObject(m_heroData);
    arr->addObjectsFromArray(getAllPartner());
    return arr;
}


RoleData* UserData::getBattleRoleByID(uint32_t nRoleID)
{
    if (m_heroData->nUnitID == nRoleID)
    {
        return m_heroData;
    }
    CCDictElement* pElement = NULL;
    CCDICT_FOREACH(m_partnerDic, pElement)
    {
        RoleData *pRoleData = (RoleData*)pElement->getObject();
        if(pRoleData->nUnitID == nRoleID)
        {
            return pRoleData;
        }
    }
    return NULL;
}

RoleData* UserData::getRoleByID(uint32_t nRoleID)
{
    if (m_heroData->nUnitID == nRoleID)
    {
        return m_heroData;
    }
    CCDictElement* pElement = NULL;
    CCDICT_FOREACH(m_partnerDic, pElement)
    {
        RoleData *pRoleData = (RoleData*)pElement->getObject();
        if(pRoleData->nUnitID == nRoleID)
        {
            return pRoleData;
        }
    }
    CCObject *obj = NULL;
    CCARRAY_FOREACH(m_arrPartner, obj)
    {
        RoleData *pRoleData = dynamic_cast<RoleData *>(obj);
        if (pRoleData->nUnitID == nRoleID)
        {
            return pRoleData;
        }
    }
    return NULL;
}

void UserData::removePartnerByID(int unitID)
{
    CCObject *obj = NULL;
    CCARRAY_FOREACH(m_arrPartner, obj)
    {
        PartnerData *pData = dynamic_cast<PartnerData *>(obj);
        if (pData->nUnitID == unitID)
        {
            m_arrPartner->removeObject(pData);
            break;
        }
    }
}

CCArray* UserData::getAllPartner(int unlessID)
{
    CCArray *arr = CCArray::create();
    for (int i = 1; i<=3; i++)
    {
        if (gUserData->m_partnerDic->objectForKey(i))
        {
            arr->addObject(gUserData->m_partnerDic->objectForKey(i));
        }
    }
    arr->addObjectsFromArray(m_arrPartner);
    for (int i = 0; i<arr->count(); i++)
    {
        for (int k = 0; k<arr->count()-1-i;k++)
        {
            PartnerData *info1,*info2;
            info1 = (PartnerData *)arr->objectAtIndex(k);
            info2 = (PartnerData *)arr->objectAtIndex(k+1);
            const PartnerConf_t* conf1 = GameData::getPartnerConf(info1->nUnitID);
            const PartnerConf_t* conf2 = GameData::getPartnerConf(info2->nUnitID);
            int quarty1 = conf1->nQuarty*10 + (0==info1->curseat?0:7-info1->curseat);
            int quarty2 = conf2->nQuarty*10 + (0==info2->curseat?0:7-info2->curseat);
            if (quarty1 < quarty2)
            {
                arr->exchangeObjectAtIndex(k, k+1);
            }
        }
    }
    if (unlessID != -1)
    {
        for (int i = 0; i<arr->count(); i++)
        {
            PartnerData *info = (PartnerData *)arr->objectAtIndex(i);
            if (info->nUnitID == unlessID)
            {
                arr->removeObject(info);
                break;
            }
        }
    }
    return arr;
}

CCArray* UserData::getUnbattlePartner()
{
    CCArray *arr = CCArray::create();
    arr->addObjectsFromArray(m_arrPartner);
    for (int i = 0; i<arr->count(); i++)
    {
        for (int k = 0; k<arr->count()-1-i;k++)
        {
            PartnerData *info1,*info2;
            info1 = (PartnerData *)arr->objectAtIndex(k);
            info2 = (PartnerData *)arr->objectAtIndex(k+1);
            const PartnerConf_t* conf1 = GameData::getPartnerConf(info1->nUnitID);
            const PartnerConf_t* conf2 = GameData::getPartnerConf(info2->nUnitID);
            int quarty1 = conf1->nQuarty*10 + (0==info1->curseat?0:7-info1->curseat);
            int quarty2 = conf2->nQuarty*10 + (0==info2->curseat?0:7-info2->curseat);
            if (quarty1 < quarty2)
            {
                arr->exchangeObjectAtIndex(k, k+1);
            }
        }
    }
    return arr;
}

void UserData::getParnterTalent(float *talent)
{
    CCDictElement* pElement = NULL;
    CCDICT_FOREACH(m_partnerDic, pElement)
    {
        RoleData *pRoleData = (RoleData*)pElement->getObject();
        const PartnerConf_t* conf= GameData::getPartnerConf(pRoleData->nUnitID);
        if (conf->nTalent <=5)
        {
            talent[conf->nTalent] += conf->fTalentValue;
        }
    }
}
float UserData::getParnterTalentByID(int talentID)
{
    float talent = 0;
    CCDictElement* pElement = NULL;
    CCDICT_FOREACH(m_partnerDic, pElement)
    {
        RoleData *pRoleData = (RoleData*)pElement->getObject();
        const PartnerConf_t* conf= GameData::getPartnerConf(pRoleData->nUnitID);
        if (conf->nTalent ==talentID)
        {
            talent += conf->fTalentValue;
        }
    }
    return talent;
}

void UserData::addBagEquipItem(package_equipment_t bagItem)
{
    BagEquips.erase(bagItem.package_id);
    BagEquips[bagItem.package_id] = bagItem;
    gUserData->package_item_sum += 1;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(BAGSIZECHANGE_NOTE);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(BAGEQUIPCHANGE_NOTE);
}

void UserData::removeBagEquipItem(int nServerID)
{
    if(BagEquips.end()!=BagEquips.find(nServerID))
    {
    BagEquips.erase(nServerID);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(BAGEQUIPCHANGE_NOTE);
    }
}

void UserData::setEquips(std::vector<package_equipment_t> equipment_vec)
{
    BagEquips.clear();
    for (int i = 0; i<equipment_vec.size(); i++)
    {
        package_equipment_t unit = equipment_vec.at(i);
        BagEquips[unit.package_id] = unit;
    }
    CCNotificationCenter::sharedNotificationCenter()->postNotification(BAGEQUIPCHANGE_NOTE);
}

void UserData::setBagItems(std::vector<package_item_t> item_vec)
{
    BagItems.clear();
    for (int i = 0; i<item_vec.size(); i++)
    {
        package_item_t unit = item_vec.at(i);
        BagItems[unit.package_id] = unit;
    }
    CCNotificationCenter::sharedNotificationCenter()->postNotification(BAGITEMCHANGE_NOTE);
}

void UserData::setBagCards(std::vector<package_item_t> book_vec)
{
    BagCards.clear();
    for (int i = 0; i<book_vec.size(); i++)
    {
        package_item_t unit = book_vec.at(i);
        BagCards[unit.package_id] = unit;
    }
    CCNotificationCenter::sharedNotificationCenter()->postNotification(BAGCARDCHANGE_NOTE);
}

void UserData::setBagStuff(std::vector<package_item_t> stuff_vec)
{
   Bagstuff.clear();
    for (int i = 0; i<stuff_vec.size(); i++)
    {
        package_item_t unit = stuff_vec.at(i);
        Bagstuff[unit.package_id] = unit;
    }
    CCNotificationCenter::sharedNotificationCenter()->postNotification(BAGSTUFFCHANGE_NOTE);
}

void UserData::reduce(std::map<int,package_item_t> &map,uint32_t package_id,uint32_t item_id,uint32_t item_cnt)
{
    if (map.end()!=map.find(package_id))
    {
        package_item_t *item = &map[package_id];
        item->item_cnt -= item_cnt;
        if (item->item_cnt == 0)
        {
            map.erase(item->package_id);
        }
    }
}

void UserData::reduceBag(uint32_t package_id,uint32_t item_id,uint32_t item_cnt)
{
    switch (getKindOfItem(item_id))
    {
        case kBattleItemKind:
        {
            reduce(BagItems, package_id, item_id, item_cnt);
            CCNotificationCenter::sharedNotificationCenter()->postNotification(BAGITEMCHANGE_NOTE);
        }
        break;
        case kComposeBookKind:
        case kGoodsKind:
            reduce(Bagstuff, package_id, item_id, item_cnt);
            CCNotificationCenter::sharedNotificationCenter()->postNotification(BAGSTUFFCHANGE_NOTE);
            break;
        case kPartnerCardKind:
            reduce(BagCards, package_id, item_id, item_cnt);
            CCNotificationCenter::sharedNotificationCenter()->postNotification(BAGCARDCHANGE_NOTE);
            break;
        case kEquipmentKind:
            removeBagEquipItem(package_id);
                break;
            default:
            break;
    }
}

void UserData::increase(std::map<int,package_item_t> &map, package_item_t itm) {
    if (map.end() != map.find(itm.package_id))
    {
        package_item_t *item = &map[itm.package_id];
        item->item_cnt += itm.item_cnt;
        
    } else {
        map[itm.package_id] = itm;
    }
}

void UserData::increaseBag(package_item_t itm)
{
    switch (getKindOfItem(itm.item_id))
    {
        case kBattleItemKind:
        {
            increase(BagItems, itm);
            CCNotificationCenter::sharedNotificationCenter()->postNotification(BAGITEMCHANGE_NOTE);
        }
            break;
        case kGoodsKind:
            increase(Bagstuff, itm);
            CCNotificationCenter::sharedNotificationCenter()->postNotification(BAGSTUFFCHANGE_NOTE);
            break;
        case kPartnerCardKind:
            increase(BagCards, itm);
            CCNotificationCenter::sharedNotificationCenter()->postNotification(BAGCARDCHANGE_NOTE);
            break;
        default:
            break;
    }

}

void UserData::removeBagItemByID(int nItemID)
{
    switch (getKindOfItem(nItemID))
    {
        case kBattleItemKind:
        {
            for (std::map<int,package_item_t>::iterator it = BagItems.begin(); it != BagItems.end(); it ++)
            {
                if (it->second.item_id == nItemID) {
                    gUserData->reduceBag(it->second.package_id, it->second.item_id, 1);
                    break;
                }
            }
            break;
        }
           
        case kGoodsKind:
        {
            for (std::map<int,package_item_t>::iterator it = Bagstuff.begin(); it != Bagstuff.end(); it ++)
            {
                if (it->second.item_id == nItemID) {
                    gUserData->reduceBag(it->second.package_id, it->second.item_id, 1);
                    break;
                }
            }
            break;
        }
        case kPartnerCardKind:
        {
            for (std::map<int,package_item_t>::iterator it = BagCards.begin(); it != BagCards.end(); it ++)
            {
                if (it->second.item_id == nItemID) {
                    gUserData->reduceBag(it->second.package_id, it->second.item_id, 1);
                    break;
                }
            }
            break;
        }
        case 4:
            break;
        default:
            break;
    }
}

bool UserData::nerveUpgrade(int nAir, int nGold, bool bIsSuccess, int nAddLv)
{
    if (nAir > this->nNerveValue || nGold > this->nGold)
    {
        return false;
    }
    else
    {
        this->nNerveValue -= nAir;
        this->nGold -= nGold;
        CCNotificationCenter::sharedNotificationCenter()->postNotification(SETUSERATTR_NOTE);
        if (bIsSuccess)
        {
            m_heroData->addNerveLevel(nAddLv);
        }
        return true;
    }
}

bool UserData::strengthenEquipment(int nIndex, uint32_t nCost, bool bIsTeen)
{
    package_equipment_t* pUnit = &m_heroData->userEquip[nIndex];
    if (nCost > this->yxb)
    {
        return false;
    }
    else
    {
        this->yxb -= nCost;
        pUnit->equipment_level += bIsTeen ? 10 : 1;

        CCNotificationCenter::sharedNotificationCenter()->postNotification(SETUSERATTR_NOTE);
        CCNotificationCenter::sharedNotificationCenter()->postNotification(ATTRBITECHANGE_NOTE);
        CCNotificationCenter::sharedNotificationCenter()->postNotification(EQUIPMENT_CHANGE_NOTE);
        return true;
    }
}

bool UserData::upgradeEquipment(int nIndex, uint32_t nPackageID, uint32_t nNewItemID, uint32_t nNewItemLevel)
{
    package_equipment_t* pUnit = &m_heroData->userEquip[nIndex];
    uint32_t nCost = GameData::getEquipmentUpgradeConf(pUnit->equipment_id)->nNeedYxb;
    if (nCost > this->yxb)
    {
        return false;
    }
    else
    {
        this->yxb -= nCost;
        
        pUnit->package_id = nPackageID;
        pUnit->equipment_id = nNewItemID;
        pUnit->equipment_level = nNewItemLevel;
        
        OnlineManager::sharedManager()->userGetPackageToOnline();
        CCNotificationCenter::sharedNotificationCenter()->postNotification(SETUSERATTR_NOTE);
        CCNotificationCenter::sharedNotificationCenter()->postNotification(ATTRBITECHANGE_NOTE);
        CCNotificationCenter::sharedNotificationCenter()->postNotification(EQUIPMENT_CHANGE_NOTE);
        return true;
    }
}

int UserData::getItemCount(uint32_t nItemID)
{
    int count = 0;
    switch (getKindOfItem(nItemID))
    {
        case kBattleItemKind://Item
        {
            for (std::map<int,package_item_t>::iterator it = BagItems.begin(); it != BagItems.end(); it++)
            {
                package_item_t* pItem = &it->second;
                if (pItem->item_id == nItemID)
                {
                    count += pItem->item_cnt;
                }
            }
        }
            break;
        case kGoodsKind://Stuff
        case kComposeBookKind://卷轴
        {
            for (std::map<int,package_item_t>::iterator it = Bagstuff.begin(); it != Bagstuff.end(); it++)
            {
                package_item_t* pItem = &it->second;
                if (pItem->item_id == nItemID)
                {
                    count += pItem->item_cnt;
                }
            }

        }
            break;
        case kPartnerCardKind://card
        {
            for (std::map<int,package_item_t>::iterator it = BagCards.begin(); it != BagCards.end(); it++)
            {
                package_item_t* pItem = &it->second;
                if (pItem->item_id == nItemID)
                {
                    count += pItem->item_cnt;
                }
            }
        }
            break;
        case kEquipmentKind://Equips
        {
            for (std::map<int,package_equipment_t>::iterator it = BagEquips.begin(); it != BagEquips.end(); it++)
            {
                package_equipment_t* pItem = &it->second;
                if (pItem->equipment_id == nItemID)
                {
                    count += 1;
                }
            }
        }
            break;
        case 10://临时物品
        {
            
        }
        default:
            break;
    }
    return count;
}

bool UserData::existMedicine(int medID)
{
    return false;
}

void UserData::resetFindTarget()
{
    CCNotificationCenter::sharedNotificationCenter()->postNotification(USER_FINDROAD_NOTE,CCBool::create(false));
    
    gUserDataEx->setIsWantedTask(false);
    gUserDataEx->setWantedTask(-1);
    
    m_findTarget->nCommunityID = -1;
    m_findTarget->nNPCID = -1;
    m_findTarget->nMissionID = -1;
}

bool UserData::hasTarget()
{
    return (m_findTarget->nCommunityID == -1?false:true);
}
