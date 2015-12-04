//
//  PartnerUpgradeConf.h
//  DragonBattle
//
//  Created by 何 永翔 on 13-12-3.
//
//

#ifndef __DragonBattle__PartnerUpgradeConf__
#define __DragonBattle__PartnerUpgradeConf__

#include "ItemConf.h"
#include <map>
#include <vector>
#include "XmlEncrypt.h"

struct PartnerUpgradeItem_t
{
	uint32_t nLv;
	uint32_t nRate;
    uint32_t nCostGold;
    uint32_t nStoneNum;
};

typedef std::map<uint32_t, PartnerUpgradeItem_t> PartnerUpgradeItemMap;

class PartnerUpgradeConf
{
public:
    PartnerUpgradeConf();
    ~PartnerUpgradeConf();
    
    const PartnerUpgradeItem_t* getItem(uint32_t nLv) const;
    
    bool loadFromFile(char* filePath, bool bEncrypt = false);
    void unload();
    
private:
    bool loadItems(xmlNodePtr chl, PartnerUpgradeItem_t* pItemVec);
    
private:
	PartnerUpgradeItemMap m_itemMap;
};

#endif /* defined(__DragonBattle__PartnerUpgradeConf__) */
