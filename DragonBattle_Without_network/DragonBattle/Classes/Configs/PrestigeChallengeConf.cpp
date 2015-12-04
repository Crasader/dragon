//
//  PrestigeChallengeConf.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-8-28.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "PrestigeChallengeConf.h"

PrestigeChallengeConfig::PrestigeChallengeConfig()
{
    m_nMaxPrestigeTitle = 0;
    memset(m_challengeConf, 0, sizeof(m_challengeConf));
}

PrestigeChallengeConfig::~PrestigeChallengeConfig()
{
    
}

bool PrestigeChallengeConfig::loadFromFile(char* filePath, bool bEncrypt)
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
		ERROR_RETURN(("RoleForceConf::loadFromFile xmlParseDoc %s failded", filePath), false);
    }
    
	cur = xmlDocGetRootElement(doc);
	if (!cur)
	{
		ERROR_LOG("xmlDocGetRootElement %s error!", filePath);
        xmlFreeDoc(doc);
        return false;
	}
    
	cur = cur->xmlChildrenNode;
    uint32_t i = 1;
	while (cur)
	{
		if ((!xmlStrcmp(cur->name, (const xmlChar*)"CHALLENGE_CONF")))
		{
            if (i > MAX_CHALLENGE_CONF_CNT)
            {
                ret = true;
                goto out;
            }
            
			DECODE_XML_PROP_INT(m_challengeConf[i].nTitleID, cur, "id");
            if (m_challengeConf[i].nTitleID != i)
            {
                goto out;
            }
            
			DECODE_XML_PROP_STR(m_challengeConf[i].titleName, cur, "titleName");
			DECODE_XML_PROP_INT(m_challengeConf[i].nNeedPrestige, cur, "needPrestige");
			DECODE_XML_PROP_INT(m_challengeConf[i].nNeedLevel, cur, "needLevel");
            DECODE_XML_PROP_INT(m_challengeConf[i].nNeedYxb, cur, "needYxb");            
			DECODE_XML_PROP_INT(m_challengeConf[i].nGiftID, cur, "giftID");
			DECODE_XML_PROP_INT(m_challengeConf[i].nMissionID, cur, "missionID");
           
            i++;
        }
		cur = cur->next;
	}
    m_nMaxPrestigeTitle = i - 1;
	ret = true;
    
out:
    xmlFreeDoc(doc);
	return ret;
}

void PrestigeChallengeConfig::unload()
{
    
}

const PrestigeChallengeConf_t* PrestigeChallengeConfig::getPrestigeChallenge(uint32_t nTitleID)
{
    if (nTitleID == 0 || nTitleID > m_nMaxPrestigeTitle)
    {
        return NULL;
    }
    return &(m_challengeConf[nTitleID]);
}

const PrestigeChallengeConf_t* PrestigeChallengeConfig::getPrestigeChallengeByMissionID(uint32_t nMissionID)
{
	for (int i = 0; i < (int)m_nMaxPrestigeTitle; i++)
	{
		if (nMissionID == m_challengeConf[i].nMissionID)
		{
    		return &(m_challengeConf[i]);
		}
	}
	return NULL;
}

