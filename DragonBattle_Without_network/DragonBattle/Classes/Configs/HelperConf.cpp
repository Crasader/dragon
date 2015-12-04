//
//  HelperConf.cpp
//  DragonBattle
//
//  Created by zeng xiaolong on 13-12-21.
//
//

#include "HelperConf.h"
#ifdef WONPEE_SERVER
#include "cocos2d_diff.h"
#else
#include "cocos2d.h"
using namespace cocos2d;
#endif
HelpConf::HelpConf()
{
    
}

HelpConf::~HelpConf()
{
    this->unload();
}

bool HelpConf::loadFromFile(const char* fileName, bool bEncrypt)
{
    bool ret = false;
    
#ifdef WONPEE_SERVER
	const char* xmlFile = fileName;
#else
    const char* xmlFile = CCFileUtils::sharedFileUtils()->fullPathForFilename(fileName).c_str();
#endif
	xmlDocPtr doc = XmlEncrypt::getXmlDocPtr(xmlFile, bEncrypt);
    
    if (!doc)
    {
        ERROR_RETURN(("HelpConf::loadFromFile xmlParseFile helper_config.xml failded"), false);
    }
    
    xmlNodePtr cur = xmlDocGetRootElement(doc);
    if (!cur)
    {
        ERROR_LOG("HelpConf::loadFromFile xmlDocGetRootElement helper_config.xml failded");
		goto exit;
    }
    
    cur = cur->xmlChildrenNode;
    while (cur)
    {
		if ((!xmlStrcmp(cur->name, (const xmlChar*)"ACTIVE_DEGREE"))) {
			xmlNodePtr chl = cur->xmlChildrenNode;
			while (chl)
			{
				if ((!xmlStrcmp(chl->name, (const xmlChar*)"DEGREE_ITEM")))
				{
					DegreeItem_t* pItem = new DegreeItem_t();
					memset(pItem->name, 0, sizeof(pItem->name));
					DECODE_XML_PROP_STR(pItem->name, chl, "name");
					DECODE_XML_PROP_INT_DEFAULT(pItem->type, chl, "type", 0);
					DECODE_XML_PROP_INT(pItem->nTaskID, chl, "id");
					DECODE_XML_PROP_INT(pItem->points, chl, "points");
					DECODE_XML_PROP_INT(pItem->maxPoints, chl, "max_points");
					m_activeVec.push_back(pItem);
					m_activeMap.insert(ActiveDegreeConfMap::value_type(pItem->nTaskID, pItem));
				}
				chl = chl->next;
			}
		} else if ((!xmlStrcmp(cur->name, (const xmlChar*)"HELPER"))) {
            HelpConf_t* pHelper = new HelpConf_t();
            DECODE_XML_PROP_INT(pHelper->nID, cur, "ID");
            char buff[64] = {0};
            DECODE_XML_PROP_STR(buff, cur, "name");
            strncpy(pHelper->name, buff, sizeof(pHelper->name));
            DECODE_XML_PROP_STR(buff, cur, "desc");
            strncpy(pHelper->desc, buff, sizeof(pHelper->desc));
            xmlNodePtr chl = cur->xmlChildrenNode;
            while (chl)
            {
                if ((!xmlStrcmp(chl->name, (const xmlChar*)"HELP")))
                {
                    HelpItem_t helpItem;
                    DECODE_XML_PROP_INT_DEFAULT(helpItem.type, chl, "type",0);
                    DECODE_XML_PROP_INT(helpItem.nstar, chl, "star");
                    DECODE_XML_PROP_INT(helpItem.nResourceID, chl, "id");
                    char itembuff[256] = {0};
                    DECODE_XML_PROP_STR(itembuff, chl, "name");
                    strncpy(helpItem.name, itembuff, sizeof(helpItem.name));
                    DECODE_XML_PROP_STR(itembuff, chl, "desc");
                    strncpy(helpItem.desc, itembuff, sizeof(helpItem.desc));
                    pHelper->HelpItemVec.push_back(helpItem);
                }
                chl = chl->next;
            }
            m_helpMap.insert(std::pair<uint32_t, HelpConf_t*>(pHelper->nID,pHelper));
        }
        cur = cur->next;
    }
    ret = true;
exit:
    xmlFreeDoc(doc);
    return ret;
}

void HelpConf::unload()
{
	for (ActiveDegreeConfVec::iterator it = m_activeVec.begin(); it != m_activeVec.end(); ++it)
   	{
		delete *it;
	}
	m_activeVec.clear();
	m_activeMap.clear();

    HelpConfMap::iterator it = m_helpMap.begin();
    while (it != m_helpMap.end())
    {
        if (it->second)
        {
            delete it->second;
            it->second = NULL;
        }
        ++it;
    }
    m_helpMap.clear();
}
