//
//  MagicStoreConf.h
//  DragonBattle
//
//  Created by 何 永翔 on 13-9-10.
//
//

#ifndef __DragonBattle__MagicStoreConf__
#define __DragonBattle__MagicStoreConf__

#ifdef WONPEE_SERVER
#include "../../share/xmldecode.h"
#else
#include "xml_decode.h"
#endif

#define MAX_STORE_ITEM_COUNT		500

struct storeItemInfo_t
{
	uint32_t nItemID;
	uint32_t nItemCount;
	uint32_t nPrice;
};

class MagicStoreConf
{
public:
	MagicStoreConf();
	~MagicStoreConf();
    
    int getRandStoreArray(uint32_t curLevel, storeItemInfo_t* pItemArray, uint32_t* arrayCount);
    
    bool loadFromFile(char* filePath, bool bEncrypt);
    void unload();
    
private:
	storeItemInfo_t m_itemArray[MAX_STORE_ITEM_COUNT];
	uint32_t   m_nLevelStartIdx[30];
	uint32_t   m_nMaxItemCount;
};

#endif
