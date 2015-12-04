//
//  RoleForceConf.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-7-10.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "RoleForceConf.h"

RoleForceConfig::RoleForceConfig()
{
    memset(m_roleForceConf, 0, sizeof(m_roleForceConf));
}

RoleForceConfig::~RoleForceConfig()
{
    
}

bool RoleForceConfig::loadFromFile(const char* filePath, bool bEncrypt)
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
		ERROR_RETURN(("RoleForceConf::loadFromFile xmlParseDoc role_force_config.xml failded"), false);
        
    }
    
	cur = xmlDocGetRootElement(doc);
	if (!cur)
	{
		ERROR_LOG("xmlDocGetRootElement %s error!", filePath);
        xmlFreeDoc(doc);
        return false;
	}
    
    m_nMaxLevel = 1;
	cur = cur->xmlChildrenNode;
    int i = 1;
	while (cur)
	{
		if ((!xmlStrcmp(cur->name, (const xmlChar*)"FORCE_CONF")))
		{
            if (m_nMaxLevel >= 100)
            {
                ret = true;
                goto out;
            }
            
			DECODE_XML_PROP_INT(m_roleForceConf[i].nForceID, cur, "id");
            if (m_nMaxLevel != m_roleForceConf[i].nForceID)
            {
                goto out;
            }
            
			DECODE_XML_PROP_INT_DEFAULT(m_roleForceConf[i].nNeedExp, cur, "forceExp", 0);
			DECODE_XML_PROP_INT_DEFAULT(m_roleForceConf[i].nLife, cur, "life", 0);
			DECODE_XML_PROP_INT_DEFAULT(m_roleForceConf[i].nOutAttack, cur, "outAttack", 0);
            DECODE_XML_PROP_INT_DEFAULT(m_roleForceConf[i].nOutDef, cur, "outDef", 0);            
			DECODE_XML_PROP_INT_DEFAULT(m_roleForceConf[i].nInAttack, cur, "inAttack", 0);
			DECODE_XML_PROP_INT_DEFAULT(m_roleForceConf[i].nInDef, cur, "inDef", 0);
            DECODE_XML_PROP_INT_DEFAULT(m_roleForceConf[i].nCrit, cur, "crit", 0);
            DECODE_XML_PROP_INT_DEFAULT(m_roleForceConf[i].nToughness, cur, "toughness", 0);
            DECODE_XML_PROP_INT_DEFAULT(m_roleForceConf[i].nReduceHurt, cur, "reduceHurt", 0);
            DECODE_XML_PROP_INT_DEFAULT(m_roleForceConf[i].nAddHurt, cur, "addHurt", 0);
            DECODE_XML_PROP_INT_DEFAULT(m_roleForceConf[i].nRecoverHP, cur, "recoverHP", 0);
            
            m_roleForceConf[i].nNeedExp += m_roleForceConf[i - 1].nNeedExp;
            m_roleForceConf[i].nLife += m_roleForceConf[i - 1].nLife;
            m_roleForceConf[i].nOutAttack += m_roleForceConf[i - 1].nOutAttack;
            m_roleForceConf[i].nOutDef += m_roleForceConf[i - 1].nOutDef;
            m_roleForceConf[i].nInAttack += m_roleForceConf[i - 1].nInAttack;
            m_roleForceConf[i].nInDef += m_roleForceConf[i - 1].nInDef;
            m_roleForceConf[i].nCrit += m_roleForceConf[i - 1].nCrit;
            m_roleForceConf[i].nToughness += m_roleForceConf[i - 1].nToughness;
            m_roleForceConf[i].nReduceHurt += m_roleForceConf[i - 1].nReduceHurt;
            m_roleForceConf[i].nAddHurt += m_roleForceConf[i - 1].nAddHurt;
            m_roleForceConf[i].nRecoverHP += m_roleForceConf[i - 1].nRecoverHP;
            i++;
            m_nMaxLevel++;
        }
		cur = cur->next;
	}
	ret = true;
    
out:
    xmlFreeDoc(doc);
	return ret;
}

void RoleForceConfig::unload()
{
    
}

// 根据炼气经验得到等级
int RoleForceConfig::getRoleForceLevel(uint32_t nForceExp)
{
    for (uint32_t i = 1; i <= m_nMaxLevel; i++)
    {
        if (nForceExp < m_roleForceConf[i].nNeedExp)
        {
            return i - 1;
        }
    }
    return m_nMaxLevel;
}

// 当前等级增加的所有属性
const RoleForceConf_t* RoleForceConfig::getRoleForceSum(uint32_t nForceLevel)
{
    if (nForceLevel > m_nMaxLevel)
    {
        return NULL;
    }
    return &(m_roleForceConf[nForceLevel]);
}

// 当前等级增加的属性
RoleForceConf_t* RoleForceConfig::getRoleForceConf(uint32_t nForceLevel, RoleForceConf_t* pConf)
{
    if (nForceLevel == 0 || nForceLevel > m_nMaxLevel)
    {
        return NULL;
    }
    
    pConf->nForceID = m_roleForceConf[nForceLevel].nForceID;
    pConf->nNeedExp = m_roleForceConf[nForceLevel].nNeedExp - m_roleForceConf[nForceLevel - 1].nNeedExp;
    pConf->nLife = m_roleForceConf[nForceLevel].nLife - m_roleForceConf[nForceLevel - 1].nLife;
    pConf->nOutAttack = m_roleForceConf[nForceLevel].nOutAttack - m_roleForceConf[nForceLevel - 1].nOutAttack;
    pConf->nOutDef = m_roleForceConf[nForceLevel].nOutDef - m_roleForceConf[nForceLevel - 1].nOutDef;
    pConf->nInAttack = m_roleForceConf[nForceLevel].nInAttack - m_roleForceConf[nForceLevel - 1].nInAttack;
    pConf->nInDef = m_roleForceConf[nForceLevel].nInDef - m_roleForceConf[nForceLevel - 1].nInDef;
    pConf->nCrit = m_roleForceConf[nForceLevel].nCrit - m_roleForceConf[nForceLevel - 1].nCrit;
    pConf->nToughness = m_roleForceConf[nForceLevel].nToughness - m_roleForceConf[nForceLevel - 1].nToughness;
    pConf->nReduceHurt = m_roleForceConf[nForceLevel].nReduceHurt - m_roleForceConf[nForceLevel - 1].nReduceHurt;
    pConf->nAddHurt = m_roleForceConf[nForceLevel].nAddHurt - m_roleForceConf[nForceLevel - 1].nAddHurt;
    pConf->nRecoverHP = m_roleForceConf[nForceLevel].nRecoverHP - m_roleForceConf[nForceLevel - 1].nRecoverHP;

    return pConf;
}


