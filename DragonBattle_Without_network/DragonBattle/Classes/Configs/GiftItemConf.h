//
//  GiftItemConf.h
//  GiftItemConf
//
//  Created by wxd on 14-2-20.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_GiftItemConf_h
#define DragonBattle_GiftItemConf_h

#include "GiftItemConf.h"
#include "XmlEncrypt.h"

#include <map>
#include <vector>
#include <string>
#include "ItemConf.h"

#define MAX_MISSION_GIFT_CNT		20
#define MAX_MISSION_GIFT_QUALITY	4

struct SingleGift_t
{
	uint32_t nSelectCnt;
	std::vector<itemDrop_t> giftItemVec;
};

struct MissionGift_t
{
	uint32_t nID;
	uint32_t nCostGold;
	SingleGift_t  singleGift[MAX_MISSION_GIFT_QUALITY];
};

class GiftItemConf
{
public:
	GiftItemConf();
	~GiftItemConf();
 
	bool loadFromFile(const char* fileName, bool bEncrypt = false);
	void unload();
	
	const MissionGift_t* getMissionGift(uint32_t nID);
	void getBattleGift(uint32_t nID, itemDrop_t* pItemList, uint32_t* pItemCnt);

private:
	MissionGift_t m_missionGift[MAX_MISSION_GIFT_CNT];
	uint32_t m_nMaxGiftCnt;
};

#endif
