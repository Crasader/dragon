//
//  ShopConf.cpp
//  DragonBattle
//
//  Created by 何 永翔 on 13-9-10.
//
//

#ifdef WONPEE_SERVER
#include "cocos2d_diff.h"
#else
#include "cocos2d.h"
#endif

#include "ShopConf.h"
#include <map>

using namespace cocos2d;

ShopConf::ShopConf()
{
    
}

ShopConf::~ShopConf()
{
	this->unload();
}

bool ShopConf::loadFromFile(char* filePath, bool bEncrypt)
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
        ERROR_RETURN(("ShopConf::loadFromFile xmlParseDoc shop.xml failded"), false);
    }
    
    cur = xmlDocGetRootElement(doc);
    if (!cur)
    {
        ERROR_LOG("xmlDocGetRootElement %s error!", filePath);
        goto exit;
    }
    
    m_itemMap.clear();
    
    cur = cur->xmlChildrenNode;
    while (cur)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar*)"SHOP")))
        {
            uint32_t kindID = 0;
            DECODE_XML_PROP_INT_DEFAULT(kindID, cur, "ID", 0);
            
            std::vector<ItemUnit_t>* pItemVec = new std::vector<ItemUnit_t>();
            m_itemMap.insert(std::pair<uint32_t, std::vector<ItemUnit_t>* >(kindID, pItemVec));
            
            chl = cur->xmlChildrenNode;
            if (!loadItems(chl, pItemVec))
            {
                goto exit;
            }
        }
        cur = cur->next;
    }
    
    ret = true;
exit:
    xmlFreeDoc(doc);
    return ret;
}

bool ShopConf::loadItems(xmlNodePtr chl, std::vector<ItemUnit_t>* pItemVec)
{
    while (chl)
    {
        if ((!xmlStrcmp(chl->name, (const xmlChar*)"ITEM")))
        {
            ItemUnit_t itm;
            DECODE_XML_PROP_INT_DEFAULT(itm.nItemID, chl, "ID", 0);
            DECODE_XML_PROP_INT_DEFAULT(itm.nItemCount, chl, "count", 1);

			pItemVec->push_back(itm);
        }
        chl = chl->next;
    }
    return true;
}

void ShopConf::unload()
{
    ShopItemMap::iterator it = m_itemMap.begin();
    while (it != m_itemMap.end())
    {
        it->second->clear();
		delete it->second;
		it->second = NULL;
    	++it;
	}
    m_itemMap.clear();
}

const std::vector<ItemUnit_t>* ShopConf::getItemVec(uint32_t itemID) const
{
	ShopItemMap::const_iterator it = m_itemMap.find(itemID);
	if (it != m_itemMap.end())
    {
		return it->second;
    }
    
    return NULL;
}

