//
//  GiftItemConf.h
//  GiftItemConf
//
//  Created by wxd on 14-2-20.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "GiftItemConf.h"
#ifdef WONPEE_SERVER
#include "cocos2d_diff.h"
#else
#include "GlobalDefine.h"
#include "cocos2d.h"
#endif

#include <algorithm>

using namespace std;
using namespace cocos2d;

GiftItemConf::GiftItemConf()
{
	m_nMaxGiftCnt = 0;
	for (int i = 0; i < MAX_MISSION_GIFT_CNT; i++)
	{
		m_missionGift[i].nID = 0;
		m_missionGift[i].nCostGold = 0;
		for (int j = 0; j < MAX_MISSION_GIFT_QUALITY; j++)
		{
			m_missionGift[i].singleGift[j].nSelectCnt = 0;
			m_missionGift[i].singleGift[j].giftItemVec.clear();
		}
	}
}

GiftItemConf::~GiftItemConf()
{
    this->unload();
}

bool GiftItemConf::loadFromFile(const char* fileName, bool bEncrypt)
{
	bool ret = false;
	const char* xmlFile = fileName;
	xmlDocPtr doc = XmlEncrypt::getXmlDocPtr(xmlFile, bEncrypt);
	int i = 0;
    
    if (!doc)
    {
        ERROR_RETURN(("GiftItemConf::loadFromFile xmlParseDoc gift_items.xml failded"), false);
    }
    
    xmlNodePtr cur = xmlDocGetRootElement(doc);
    if (!cur)
    {
        ERROR_LOG("GiftItemConf::loadFromFile xmlDocGetRootElement gift_items.xml failded");
        ret = false;
        goto exit;
    }
	
    cur = cur->xmlChildrenNode;
	while (cur)
	{
		if ((!xmlStrcmp(cur->name, (const xmlChar*)"GIFT")))
		{
			DECODE_XML_PROP_INT(m_missionGift[i].nID, cur, "ID");
			DECODE_XML_PROP_INT(m_missionGift[i].nCostGold, cur, "cost_gold");
			xmlNodePtr chl = cur->xmlChildrenNode;
			int idx = 0;
			DEBUG_LOG("read Gift Item: %d", m_missionGift[i].nID);
			while (chl)
			{
				if ((!xmlStrcmp(chl->name, (const xmlChar*)"ITEMS")))
				{
					int giftInfo[100] = {0};
            		int count = decode_xml_prop_arr_int_default(giftInfo, 100, chl, (void*)"items", -1);
            		for (int j = 0; j < count / 3; j++)
            		{
						itemDrop_t unit = {0};
						unit.nItemID = giftInfo[j * 3];
						unit.nItemCnt = giftInfo[j * 3 + 1];
						unit.fRate = giftInfo[j * 3 + 2];
						m_missionGift[i].singleGift[idx].giftItemVec.push_back(unit);
					}

					DECODE_XML_PROP_INT(m_missionGift[i].singleGift[idx].nSelectCnt, chl, "select_num");
					idx ++;
				}
				chl = chl->next;
			}
            DEBUG_LOG("gift item info:[%d %d]", m_missionGift[i].nID, m_missionGift[i].nCostGold);
			i++;
		}
		cur = cur->next;
    }
    ret = true;
exit:
    xmlFreeDoc(doc);
    return ret;
}

void GiftItemConf::unload()
{
	for (int i = 0; i < MAX_MISSION_GIFT_CNT; i++)
	{
		m_missionGift[i].nID = 0;
		for (int j = 0; j < MAX_MISSION_GIFT_QUALITY; j++)
		{
			m_missionGift[i].singleGift[j].nSelectCnt = 0;
			m_missionGift[i].singleGift[j].giftItemVec.clear();
		}
	}
}

const MissionGift_t* GiftItemConf::getMissionGift(uint32_t nID)
{
	for (int i = 0; i < MAX_MISSION_GIFT_CNT; i++)
	{
		if (m_missionGift[i].nID == nID)
		{
			return &(m_missionGift[i]);
		}
	}
	return NULL;
}

void GiftItemConf::getBattleGift(uint32_t nID, itemDrop_t* pItemList, uint32_t* pItemCnt)
{
	int index = -1;
	for (int i = 0; i < MAX_MISSION_GIFT_CNT; i++)
	{
		if (m_missionGift[i].nID == nID)
		{
			index = i;
			break;
		}
	}
	if (index == -1)
	{
		return ;
	}
	
	int itemCnt = 0;
	for (int i = 0; i < MAX_MISSION_GIFT_QUALITY; i++)
	{
		std::vector<itemDrop_t>* pVec = &(m_missionGift[index].singleGift[i].giftItemVec);
        random_shuffle(pVec->begin(), pVec->end());
		for (int j = 0; j < (int)m_missionGift[index].singleGift[i].nSelectCnt && j < (int)pVec->size(); j++)
		{
			pItemList[itemCnt].nItemID = (*pVec)[j].nItemID;
			pItemList[itemCnt].nItemCnt = (*pVec)[j].nItemCnt;
			pItemList[itemCnt].fRate = (*pVec)[j].fRate;
			itemCnt ++;
		}
	}
	*pItemCnt = itemCnt;
} 

