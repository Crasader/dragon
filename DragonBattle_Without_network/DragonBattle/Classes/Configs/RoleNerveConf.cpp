//
//  RoleNerveConf.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-7-9.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "RoleNerveConf.h"


RoleNerveConfig::RoleNerveConfig()
{
    
}

RoleNerveConfig::~RoleNerveConfig()
{
    
}

bool RoleNerveConfig::loadFromFile(const char* filePath, bool bEncrypt)
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
		ERROR_RETURN(("RoleNerveConfig::loadFromFile xmlParseDoc nerve_config.xml failded"), false);
        
    }
    
	cur = xmlDocGetRootElement(doc);
	if (!cur)
	{
		ERROR_LOG("xmlDocGetRootElement %s error!", filePath);
        xmlFreeDoc(doc);
        return false;
	}
    
    uint32_t i = 1;
    memset(m_pRoleNerveConf[0].nAttrValue, 0, sizeof(m_pRoleNerveConf[0].nAttrValue));
	cur = cur->xmlChildrenNode;
	while (cur)
	{
		if ((!xmlStrcmp(cur->name, (const xmlChar*)"NERVE")))
		{
			ret = load_one_nerve_config(cur, i);
			if (!ret) {
				ERROR_LOG("load nerve config err i=%u", i);
				goto out;
			}
			++ i;
        }
		cur = cur->next;
	}
	ret = true;
    
out:
    xmlFreeDoc(doc);
	return ret;
}

bool RoleNerveConfig::load_one_nerve_config(const xmlNodePtr& cur, uint32_t i)
{
	if (i > MAX_NERVE_LV)
	{
		ERROR_LOG("too many rows %u", i);
		return false; 
	}
	DECODE_XML_PROP_INT(m_pRoleNerveConf[i].nNerveID, cur, "id");
	if (i != m_pRoleNerveConf[i].nNerveID)
	{
		ERROR_LOG("rows %u not equal nerve_id %u", i, m_pRoleNerveConf[i].nNerveID);
		return false; 
	}

	DECODE_XML_PROP_STR(m_pRoleNerveConf[i].nerveName, cur, "name");
	DECODE_XML_PROP_INT(m_pRoleNerveConf[i].nNeedAir, cur, "need_air");
	DECODE_XML_PROP_INT(m_pRoleNerveConf[i].nRate, cur, "rate");
	DECODE_XML_PROP_INT(m_pRoleNerveConf[i].nEffectType, cur, "effect_type");
	DECODE_XML_PROP_INT(m_pRoleNerveConf[i].nEffectValue, cur, "effect_value");
	DECODE_XML_PROP_INT(m_pRoleNerveConf[i].nCoolDown, cur, "cooldown");
	DECODE_XML_PROP_INT(m_pRoleNerveConf[i].nNeedLevel, cur, "need_level");
	if (m_pRoleNerveConf[i].nEffectType > EQUIP_ATTR_MAX) {
		ERROR_LOG("nerve_config err nerve_id %u invalid effect_type %u", 
				m_pRoleNerveConf[i].nNerveID, m_pRoleNerveConf[i].nEffectType);
		return false;
	}

	//DEBUG_LOG("nerve_id %u", m_pRoleNerveConf[i].nNerveID);
	memcpy(m_pRoleNerveConf[i].nAttrValue, m_pRoleNerveConf[i - 1].nAttrValue, sizeof(m_pRoleNerveConf[i].nAttrValue));
	m_pRoleNerveConf[i].nAttrValue[m_pRoleNerveConf[i].nEffectType] += m_pRoleNerveConf[i].nEffectValue;
	return true;
}

void RoleNerveConfig::unload()
{
    
}

const RoleNerveConf_t* RoleNerveConfig::getRoleNerve(uint32_t nNerveID) const
{
    if (nNerveID > 640)
    {
        return NULL;
    }
    
    return &(m_pRoleNerveConf[nNerveID]);
}


