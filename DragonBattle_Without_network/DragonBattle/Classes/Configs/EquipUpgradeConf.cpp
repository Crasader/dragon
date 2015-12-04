#include "EquipUpgradeConf.h"

EquipUpgradeConfig::EquipUpgradeConfig()
{
	m_equipUpgradeMap.clear();
}

EquipUpgradeConfig::~EquipUpgradeConfig()
{
	this->unload();
}

bool EquipUpgradeConfig::loadFromFile(const char* filePath, bool bEncrypt)
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
		ERROR_RETURN(("EquipUpgradeConfig::loadFromFile xmlParseDoc EquipmentUpgrade.xml failded"), false);
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
			EquipUpgradeConf_t* pConf = new EquipUpgradeConf_t();
			DECODE_XML_PROP_INT(pConf->nEquipmentID, cur, "equip_id");
			DECODE_XML_PROP_INT(pConf->nNextEquipmentID, cur, "next_equip");
			DECODE_XML_PROP_INT(pConf->nNeedBookID, cur, "book_id");
			
			int stuff_list[10] = {0};
			decode_xml_prop_arr_int_default(stuff_list, 10, cur, (void*)"need", 0);
			for (int i = 0; i < 5; i++)
			{
				if (stuff_list[2 * i] != 0 && stuff_list[2 * i+1] != 0)
				{
					pConf->itemArray[i].nItemID = stuff_list[2 * i];
					pConf->itemArray[i].nItemCount = stuff_list[2 * i + 1];
				}
				else
				{
					break;
				}
			}
            
			DECODE_XML_PROP_INT_DEFAULT(pConf->nNeedYxb, cur, "money", 0);
			m_equipUpgradeMap.insert(std::pair<uint32_t, EquipUpgradeConf_t*>(pConf->nEquipmentID, pConf));
		}
		cur = cur->next;
	}
	ret = true;
    
exit:
	xmlFreeDoc(doc);
	return ret;
}

void EquipUpgradeConfig::unload()
{
	EquipUpgradeMap::iterator it = m_equipUpgradeMap.begin();
	while (it != m_equipUpgradeMap.end())
	{
		delete it->second;
		it->second = NULL;
		++it;
	}
	m_equipUpgradeMap.clear();
}

const EquipUpgradeConf_t* EquipUpgradeConfig::getEquipUpgrade(uint32_t nEquipID) const
{
	EquipUpgradeMap::const_iterator it = m_equipUpgradeMap.find(nEquipID);
	if (it != m_equipUpgradeMap.end())
	{
		return it->second;
	}
	return NULL;
}

