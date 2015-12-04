//
//  PartnerUpgradeConf.cpp
//  DragonBattle
//
//  Created by 何 永翔 on 13-12-3.
//
//

#ifdef WONPEE_SERVER
#include "cocos2d_diff.h"
#else
#include "cocos2d.h"
#endif

#include "PartnerUpgradeConf.h"
#include <map>

using namespace cocos2d;

PartnerUpgradeConf::PartnerUpgradeConf()
{
    
}

PartnerUpgradeConf::~PartnerUpgradeConf()
{
	this->unload();
}

bool PartnerUpgradeConf::loadFromFile(char* filePath, bool bEncrypt)
{
    bool ret = false;
    xmlNodePtr cur;
#ifdef WONPEE_SERVER
	const char* xmlFile = filePath;
#else
    const char* xmlFile = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(filePath).c_str();
#endif
   	xmlDocPtr doc = XmlEncrypt::getXmlDocPtr(xmlFile, bEncrypt);
    if (!doc)
    {
        ERROR_RETURN(("PartnerUpgradeConf::loadFromFile xmlParseDoc shop.xml failded"), false);
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
        if ((!xmlStrcmp(cur->name, (const xmlChar*)"UPGRADE_QUARTY")))
        {
            PartnerUpgradeItem_t t;
            DECODE_XML_PROP_INT_DEFAULT(t.nLv, cur, "QuartyLevel", 0);
            DECODE_XML_PROP_INT_DEFAULT(t.nRate, cur, "UpgradeRate", 0);
            DECODE_XML_PROP_INT_DEFAULT(t.nCostGold, cur, "CostGold", 0);
            DECODE_XML_PROP_INT_DEFAULT(t.nStoneNum, cur, "StoneNum", 0);
            m_itemMap.insert(std::pair<uint32_t, PartnerUpgradeItem_t>(t.nLv, t));
        }
        cur = cur->next;
    }
    
    ret = true;
exit:
    xmlFreeDoc(doc);
    return ret;
}

void PartnerUpgradeConf::unload()
{
    m_itemMap.clear();
}

const PartnerUpgradeItem_t* PartnerUpgradeConf::getItem(uint32_t nLv) const
{
	PartnerUpgradeItemMap::const_iterator it = m_itemMap.find(nLv);
	if (it != m_itemMap.end())
    {
		return &(it->second);
    }
    
    return NULL;
}

