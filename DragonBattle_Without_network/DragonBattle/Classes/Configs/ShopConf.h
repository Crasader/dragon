//
//  ShopConf.h
//  DragonBattle
//
//  Created by 何 永翔 on 13-9-10.
//
//

#ifndef __DragonBattle__ShopConf__
#define __DragonBattle__ShopConf__

#include "ItemConf.h"
#include <map>
#include <vector>
#include "XmlEncrypt.h"

typedef std::map<uint32_t, std::vector<ItemUnit_t>* > ShopItemMap;

class ShopConf
{
public:
    ShopConf();
    ~ShopConf();
    
    const std::vector<ItemUnit_t>* getItemVec(uint32_t shopID) const;
    
    bool loadFromFile(char* filePath, bool bEncrypt = false);
    void unload();
    
private:
    bool loadItems(xmlNodePtr chl, std::vector<ItemUnit_t>* pItemVec);
    
private:
	ShopItemMap		m_itemMap;
};

#endif
