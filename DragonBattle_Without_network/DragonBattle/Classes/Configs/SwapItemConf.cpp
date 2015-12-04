//
//  SwapItemConf.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-5-28.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "SwapItemConf.h"

SwapItemConfig::SwapItemConfig()
{
    m_swapItemMap.clear();
}

SwapItemConfig::~SwapItemConfig()
{
    this->unload();
}

static bool loadSwapItems(ItemUnitVec &itemInOut, xmlNodePtr cur)
{
    itemInOut.clear();
    xmlNodePtr chl = cur->xmlChildrenNode;
    while (chl)
    {
        if (!xmlStrcmp(chl->name, (const xmlChar*)("ITEM")))
        {
            ItemUnit_t itemInfo;
            DECODE_XML_PROP_INT(itemInfo.nItemID, chl, "ID");
            
            //if (!GameData::getItem(itemInfo.itemID))
            {
                //return false;
            }
            
            DECODE_XML_PROP_INT_DEFAULT(itemInfo.nItemCount, chl, "count", 0);
            itemInOut.push_back(itemInfo);
        }
		chl = chl->next;
    }
    return true;
}

static bool loadRandItems(RandItemOutVec &itemOut, xmlNodePtr cur)
{
    itemOut.clear();
    xmlNodePtr chl = cur->xmlChildrenNode;
    while (chl)
    {
        if (!xmlStrcmp(chl->name, (const xmlChar*)("ITEM")))
        {
            RandItemUint_t itemInfo;
            DECODE_XML_PROP_INT(itemInfo.nItemID, chl, "ID");
            
            //if (!GameData::getItem(itemInfo.itemID))
            {
                //return false;
            }
            
            DECODE_XML_PROP_INT_DEFAULT(itemInfo.nItemCount, chl, "count", 0);
            DECODE_XML_PROP_FLOAT(itemInfo.fRandStart, chl, "randStart");
            DECODE_XML_PROP_FLOAT(itemInfo.fRandEnd, chl, "randEnd");
            itemOut.push_back(itemInfo);
        }
        chl = chl->next;
    }
    return true;
}

bool SwapItemConfig::loadFromFile(const char* filePath, bool bEncrypt)
{
    bool ret = false;
    xmlNodePtr cur, chl;
#ifdef WONPEE_SERVER
	const char* xmlFile = filePath;
#else
	const char* xmlFile = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(filePath).c_str();
#endif	
    xmlDocPtr doc = XmlEncrypt::getXmlDocPtr(xmlFile, bEncrypt);
    
    if (!doc)
    {
        ERROR_RETURN(("SwapItemConfig::loadFromFile xmlParseDoc item.xml failded"), false);
    }
    
    cur = xmlDocGetRootElement(doc);
    if (!cur)
    {
        ERROR_LOG("xmlDocGetRootElement %s error!", filePath);
        goto exit;
    }
    
    cur = cur->xmlChildrenNode;
    while (cur)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar*)"SWAP_ENTRY")))
        {
            SwapItem_t* pSwapItem = new SwapItem_t();
			DECODE_XML_PROP_INT(pSwapItem->nEntryID, cur, "ID");
            DECODE_XML_PROP_INT_DEFAULT(pSwapItem->nSwapType, cur, "swapType", 1);
            DECODE_XML_PROP_INT_DEFAULT(pSwapItem->nShowItemType, cur, "showItemType", 1);
            DECODE_XML_PROP_INT_DEFAULT(pSwapItem->nCliReq, cur, "cliReq", 0);
            DECODE_XML_PROP_INT_DEFAULT(pSwapItem->nLimitType, cur, "limitType", 0);
            DECODE_XML_PROP_INT_DEFAULT(pSwapItem->nMulSet, cur, "mulSet", 0);
            DECODE_XML_PROP_INT_DEFAULT(pSwapItem->nDateStart, cur, "dateStart", 0);
            DECODE_XML_PROP_INT_DEFAULT(pSwapItem->nDateEnd, cur, "dateEnd", 0);
            DECODE_XML_PROP_INT_DEFAULT(pSwapItem->nLimitCount, cur, "limitCount", 0);
            DECODE_XML_PROP_INT_DEFAULT(pSwapItem->nLevel, cur, "level", 0);
            char desc[256] = {0};
            DECODE_XML_PROP_STR_DEFAULT(desc, cur, "des", "");
            pSwapItem->desc = desc;

            chl = cur->xmlChildrenNode;
            while (chl)
            {
                if (!xmlStrcmp(chl->name, (const xmlChar*)("ITEM_IN")))
                {
                    if (!loadSwapItems(pSwapItem->itemIn, chl))
                    {
                        return false;
                    }
                }
                if (!xmlStrcmp(chl->name, (const xmlChar*)("ITEM_OUT")))
                {
                    if (pSwapItem->nSwapType == 1 && !loadSwapItems(pSwapItem->swapItemOut, chl))
                    {
                        return false;
                    }
                    if (pSwapItem->nSwapType == 2 && !loadRandItems(pSwapItem->randItemOut, chl))
                    {
                        return false;
                    }
                }
                chl = chl->next;
            }
            m_swapItemMap.insert(std::pair<uint32_t, SwapItem_t*>(pSwapItem->nEntryID, pSwapItem));
        }
        cur = cur->next;
    }
    
    ret = true;
exit:
    xmlFreeDoc(doc);
    return ret;
}


void SwapItemConfig::unload()
{
    SwapItemMap::iterator it = m_swapItemMap.begin();
    while (it != m_swapItemMap.end())
    {
        delete it->second;
        it->second = NULL;
        ++it;
    }
    m_swapItemMap.clear();
}

const SwapItem_t* SwapItemConfig::getSwapItem(uint32_t swapID) const
{
    SwapItemMap::const_iterator it = m_swapItemMap.find(swapID);
    if (it != m_swapItemMap.end())
    {
        return it->second;
    }
    return NULL;
}

