//
//  ShopConf.cpp
//  DragonBattle
//
//  Created by 何 永翔 on 13-9-10.
//
//

#ifdef WONPEE_SERVER
#include "../../share/xmldecode.h"
#include "cocos2d_diff.h"
#else
#include "xml_decode.h"
#include "cocos2d.h"
#endif

#include "MagicStoreConf.h"
#include <algorithm>

MagicStoreConf::MagicStoreConf()
{
	memset(m_itemArray, 0, sizeof(m_itemArray));
	memset(m_nLevelStartIdx, 0, sizeof(m_nLevelStartIdx));
	m_nMaxItemCount = 0;
}

MagicStoreConf::~MagicStoreConf()
{
	this->unload();
}

/*
ItemUnit_t m_itemArray[MAX_STORE_ITEM_COUNT];
uint32_t   m_nLevelStartIdx[30];
uint32_t   m_nMaxItemCount;
*/
bool MagicStoreConf::loadFromFile(char* filePath, bool bEncrypt)
{
	bool ret = false;
    xmlDocPtr doc;
    xmlNodePtr cur;
	int curIdx = 0;
	int curLevel = 0;
  
#ifdef WONPEE_SERVER
	const char* xmlFile = filePath;
#else
    const char* xmlFile = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(filePath).c_str();
#endif

   	doc = xmlParseFile(xmlFile);
    if (!doc)
    {
        ERROR_RETURN(("MagicStoreConf::loadFromFile xmlParseDoc magic_store.xml failded"), false);
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
        if ((!xmlStrcmp(cur->name, (const xmlChar*)"ITEM")))
        {
			if (curIdx >= MAX_STORE_ITEM_COUNT) {
				break;
			}
            DECODE_XML_PROP_INT(m_itemArray[curIdx].nItemID, cur, "ID");
            int level = 0;
			DECODE_XML_PROP_INT(level, cur, "level");
			if (level != curLevel) {
				m_nLevelStartIdx[level] = curIdx;
				curLevel = level;
			}

			DECODE_XML_PROP_INT(m_itemArray[curIdx].nItemCount, cur, "count");
            DECODE_XML_PROP_INT(m_itemArray[curIdx].nPrice, cur, "price");
        	curIdx++;
		}
        cur = cur->next;
    }
	m_nLevelStartIdx[curLevel + 1] = curIdx;
	m_nMaxItemCount = curIdx;
    
    ret = true;
exit:
    xmlFreeDoc(doc);
    return ret;

}

void MagicStoreConf::unload()
{
	memset(m_itemArray, 0, sizeof(m_itemArray));
	memset(m_nLevelStartIdx, 0, sizeof(m_nLevelStartIdx));
	m_nMaxItemCount = 0;
}

/*
 * @brief 刷新神秘商店6个物品
 */
int MagicStoreConf::getRandStoreArray(uint32_t curLevel, storeItemInfo_t* pItemArray, uint32_t* arrayCount)
{
	int startIdx;
	int endIdx;
	if (curLevel <= 3) {
		startIdx = 1;
		endIdx = 6;
	} else if (curLevel >= 17) {
		startIdx = 15;
		endIdx = 20;
	} else {
		startIdx = curLevel - 2;
		endIdx = curLevel + 3;
	}
	startIdx = m_nLevelStartIdx[startIdx];
	endIdx = m_nLevelStartIdx[endIdx];
	int count = endIdx - startIdx;
	
	if (count <= 0) {
		return -1;
	}

	*arrayCount = 6;
	if (count < 6) {
		*arrayCount = count;
	}

	for (uint32_t i = 0; i < *arrayCount; i++) {
		int idx = rand() % count;
		//判断是否生成一样的了
		while (1) {
			uint32_t j;
			for (j = 0; j < i && pItemArray[j].nItemID != m_itemArray[idx + startIdx].nItemID; j++);
			if (j == i) {
				break;
			}
			//index = rand() % size;
			idx = ((idx + 1) == count) ? 0 : (idx + 1);
		}
		memcpy(pItemArray + i, &(m_itemArray[idx + startIdx]), sizeof(storeItemInfo_t));
	}
	
	return 0;
}


