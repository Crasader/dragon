//
//  SkillConf.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-19.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "SkillConf.h"

#ifdef WONPEE_SERVER
#include "cocos2d_diff.h"
#else
#include "cocos2d.h"
using namespace cocos2d;
#endif


SkillConf::SkillConf()
{
    m_heroSkillVec[0].clear();
    m_heroSkillVec[1].clear();
    m_heroSkillVec[2].clear();
    m_skillMap.clear();
}

SkillConf::~SkillConf()
{
    this->unload();
}

bool parseSkill(SkillBaseConf_t *pSkill,xmlNodePtr cur)
{
    char buff[256] = {0};
    DECODE_XML_PROP_INT(pSkill->nSkillID, cur, "ID");
    DECODE_XML_PROP_STR(buff, cur, "name");
    strncpy(pSkill->skillName, buff, sizeof(pSkill->skillName));
    DECODE_XML_PROP_STR_DEFAULT(buff, cur, "desc", "");
    strncpy(pSkill->skillDesc, buff, sizeof(pSkill->skillDesc));
    DECODE_XML_PROP_INT(pSkill->nHeroLevel, cur, "LEVEL");
    DECODE_XML_PROP_INT_DEFAULT(pSkill->nTaskID, cur, "taskID", 0);
    return true;
}

bool parseSkilllv(SkillLevelBase_t *pLevel,xmlNodePtr chl)
{
    DECODE_XML_PROP_INT(pLevel->nSkillLevel, chl, "skillLevel");
    DECODE_XML_PROP_INT_DEFAULT(pLevel->nCostEnergy, chl, "costEnergy",0);
    DECODE_XML_PROP_FLOAT_DEFAULT(pLevel->fCDTime, chl, "cdTime",1.0f);
    char buff[256] = {0};
    DECODE_XML_PROP_STR(buff, chl, "desc");
    strncpy(pLevel->skillDesc, buff, sizeof(pLevel->skillDesc));
    DECODE_XML_PROP_INT_DEFAULT(pLevel->nInAttackValue,chl, "InAttackValue",0);
    DECODE_XML_PROP_FLOAT_DEFAULT(pLevel->fInAttackPrecent,chl, "InAttackPrecent",0);
    DECODE_XML_PROP_INT_DEFAULT(pLevel->nOutAttackValue,chl, "OutAttackValue",0);
    DECODE_XML_PROP_FLOAT_DEFAULT(pLevel->fOutAttackPrecent,chl, "OutAttackPrecent",0);
    DECODE_XML_PROP_INT_DEFAULT(pLevel->nInDefValue,chl, "InDefValue",0);
    DECODE_XML_PROP_FLOAT_DEFAULT(pLevel->fInDefPrecent,chl, "InDefPrecent",0);
    DECODE_XML_PROP_INT_DEFAULT(pLevel->nOutDefValue,chl, "OutDefValue",0);
    DECODE_XML_PROP_FLOAT_DEFAULT(pLevel->fOutDefPrecent,chl, "OutDefPrecent",0);
    DECODE_XML_PROP_INT_DEFAULT(pLevel->nAddhurtValue,chl, "AddhurtValue",0);
    DECODE_XML_PROP_FLOAT_DEFAULT(pLevel->fAddhurtPrecent,chl, "AddhurtPrecent",0);
    DECODE_XML_PROP_INT_DEFAULT(pLevel->nReducehurtValue,chl, "ReducehurtValue",0);
    DECODE_XML_PROP_FLOAT_DEFAULT(pLevel->fReducehurtPrecent,chl, "ReducehurtPrecent",0);
    DECODE_XML_PROP_INT_DEFAULT(pLevel->nHPValue,chl, "HPValue",0);
    DECODE_XML_PROP_INT_DEFAULT(pLevel->nToughness,chl, "defCritValue",0);
    DECODE_XML_PROP_INT_DEFAULT(pLevel->nCrit,chl, "CritPrecent",0);
    return true;
}

bool SkillConf::loadFromFile(const char* fileName, bool bEncrypt)
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
        ERROR_RETURN(("SkillConf::loadFromFile xmlParseFile skill_config.xml failded"), false);
    }
    
    xmlNodePtr cur = xmlDocGetRootElement(doc);
    if (!cur)
    {
        ERROR_LOG("SkillConf::loadFromFile xmlDocGetRootElement skill_config.xml failded");
        ret = false;
    }
    
    cur = cur->xmlChildrenNode;
    while (cur)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar*)"SKILL")))
        {
            SkillConf_t* pSkill = new SkillConf_t();
			pSkill->nMaxLevel = 0;
            parseSkill(&pSkill->skillBase,cur);
            pSkill->nRoleType = pSkill->skillBase.nSkillID / 1000;
            decode_xml_prop_arr_int_default(pSkill->nPreSkill, MAX_PRESKILL_NUM, cur, (void*)"preSkill", 0);
            xmlNodePtr chl = cur->xmlChildrenNode;
            int i =0;
            while (chl)
            {
                if ((!xmlStrcmp(chl->name, (const xmlChar*)"LEVEL")))
                {
                    int level = 0;
                    DECODE_XML_PROP_INT(level, chl, "skillLevel");
                    if (i + 1 != level || level > MAX_SKILL_LEVEL)
                    {
						DEBUG_LOG("SKILL level ERR: ID=%d i=%u level=%u max_level=%u", pSkill->skillBase.nSkillID, i, level, MAX_SKILL_LEVEL);
						ERROR_LOG("SKILL level ERR: ID=%d i=%u level=%u max_level=%u", pSkill->skillBase.nSkillID, i, level, MAX_SKILL_LEVEL);
                        goto exit;
                    }
					if (pSkill->nMaxLevel < level) {
						pSkill->nMaxLevel = level;
					}
                    SkillLevel_t *pLevel = &pSkill->skillLevel[i];
                    DECODE_XML_PROP_INT(pLevel->nCostPower, chl, "costPower");
                    DECODE_XML_PROP_INT_DEFAULT(pLevel->nYxb, chl, "costYXB",0);
                    parseSkilllv(&pLevel->skillLvBase,chl);
                    i++;
                }
                chl = chl->next;
            }
            m_heroSkillVec[pSkill->nRoleType - 1].push_back(pSkill->skillBase.nSkillID);
			//DEBUG_LOG("add SKILL: %d %d, %p", pSkill->skillBase.nSkillID, i, pSkill);
            m_skillMap.insert(std::pair<uint32_t, SkillConf_t*>(pSkill->skillBase.nSkillID, pSkill));
        }
        cur = cur->next;
    }
    ret = true;
exit:
    xmlFreeDoc(doc);
    return ret;
}

void SkillConf::unload()
{
    SkillConfMap::iterator it = m_skillMap.begin();
    while (it != m_skillMap.end())
    {
        if (it->second)
        {
            delete it->second;
            it->second = NULL;
        }
        ++it;
    }
    m_skillMap.clear();
}

const SkillLevel_t* SkillConf::getSkillLevel(uint32_t nSkillID, uint32_t nSkillLevel)
{
    const SkillConf_t *pSkill = getSkill(nSkillID);
	if (!pSkill)
	{
		return NULL;
	}
	if (nSkillLevel > MAX_SKILL_LEVEL)
   	{
		return NULL;
	}
    return &(pSkill->skillLevel[nSkillLevel - 1]);
}

const SkillConf_t* SkillConf::getSkill(uint32_t nSkill)
{
    SkillConfMap::const_iterator it = m_skillMap.find(nSkill);
    if (it != m_skillMap.end())
    {
        return it->second;
    }
    return NULL;
}

bool SkillConf::isEnableUnlockSkill(int selectSkill,std::map<int,user_skill_t> *skill_map)
{
    bool enable = true;
    const SkillConf_t *pskillconf = getSkill(selectSkill);
    for (int i = 0; i<2; i++)
    {
        if (pskillconf->nPreSkill[i] == 0)
            continue;
        if ( skill_map->end()!= skill_map->find(pskillconf->nPreSkill[i]))
        {
            user_skill_t skillInfo = skill_map->at(pskillconf->nPreSkill[i]);
            if (skillInfo.skill_lvl<10)
            {
                enable = false;
                break;
            }
        }
        else
        {
            enable = false;
            break;
        }
    }
    return enable;
}

PartnerSkillConf::PartnerSkillConf()
{
    m_skillMap.clear();
}

PartnerSkillConf::~PartnerSkillConf()
{
    this->unload();
}

bool PartnerSkillConf::loadFromFile(const char* fileName, bool bEncrypt)
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
        ERROR_RETURN(("PartnerSkillConf::loadFromFile xmlParseFile Partnerskill.xml failded"), false);
    }
    
    xmlNodePtr cur = xmlDocGetRootElement(doc);
    if (!cur)
    {
        ERROR_LOG("PartnerSkillConf::loadFromFile xmlDocGetRootElement Partnerskill.xml failded");
        ret = false;
    	goto exit;
	}
    
    cur = cur->xmlChildrenNode;
    while (cur)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar*)"SKILL")))
        {
            PartnerSkillConf_t* pSkill = new PartnerSkillConf_t();
            parseSkill(&pSkill->skillBase,cur);
            DECODE_XML_PROP_INT_DEFAULT(pSkill->relateHurt, cur, "relateHurt",10);
            DECODE_XML_PROP_INT_DEFAULT(pSkill->addHurt, cur, "addHurt",10);
            DECODE_XML_PROP_INT_DEFAULT(pSkill->quarty, cur, "quarty",2);
            DECODE_XML_PROP_FLOAT_DEFAULT(pSkill->bescper, cur, "bescper",0.1);
            DECODE_XML_PROP_INT_DEFAULT(pSkill->nThrowSequence, cur, "throwMode",0);
            
			DEBUG_LOG("add SKILL: %d", pSkill->skillBase.nSkillID);
            
            int throwId[50] = {0};
            int throwCnt = decode_xml_prop_arr_int_default(throwId, 50, cur, (void*)"throwIdVec", 0);
            for (int i = 0; i < throwCnt; i++)
            {
                pSkill->throwVec.push_back(throwId[i]);
            }
            loadUnitSkillResource(cur,&(pSkill->fileResourceVec));
			loadUnitSkillLevel(cur, &(pSkill->upgrdateConfMap));
            m_skillMap.insert(PartnerSkillConfMap::value_type(pSkill->skillBase.nSkillID, pSkill));
        }

        cur = cur->next;
    }
    ret = true;

exit:
    xmlFreeDoc(doc);
    return ret;
}

bool PartnerSkillConf::loadUnitSkillResource(xmlNodePtr chl,std::vector<std::string>* pFileVec)
{
    xmlNodePtr cur = chl->xmlChildrenNode;
    while (cur)
    {
        if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar*>("SOURCE_LIST")))
        {
            xmlNodePtr curl = cur->xmlChildrenNode;
            char strFile[64] = {0};
            while (curl)
            {
                if ((!xmlStrcmp(curl->name, (const xmlChar*)"SOURCE")))
                {
                    DECODE_XML_PROP_STR(strFile, curl, "file");
                    pFileVec->push_back(strFile);
                }
                curl = curl->next;
            }
        }
        cur = cur->next;
    }
    return true;
}

bool PartnerSkillConf::loadUnitSkillLevel(xmlNodePtr chl,std::map<uint32_t, PartnerSkillUpgradeConf_t*>* pupgrdateConfMap)
{
	xmlNodePtr cur = chl->xmlChildrenNode;
	while (cur)
	{
		if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar*>("UPGRADE_LIST")))
		{
			xmlNodePtr curl = cur->xmlChildrenNode;
			while (curl)
			{
				if ((!xmlStrcmp(curl->name, reinterpret_cast<const xmlChar*>("LEVEL"))))
				{
					PartnerSkillUpgradeConf_t* upgrade_lv = new PartnerSkillUpgradeConf_t();
					DECODE_XML_PROP_INT(upgrade_lv->lv, curl, "LEVEL");
					DECODE_XML_PROP_INT(upgrade_lv->rate, curl, "UpgradeRate");
					DECODE_XML_PROP_INT_DEFAULT(upgrade_lv->cost_yxb, curl, "COSTYXB",0);
					DECODE_XML_PROP_INT(upgrade_lv->item_id, curl, "SPLINTERID");
					DECODE_XML_PROP_INT(upgrade_lv->item_cnt, curl, "SPLINTERNUM");
					pupgrdateConfMap->insert(PartnerSkillUpgradeMap::value_type(upgrade_lv->lv, upgrade_lv));
				}
				curl = curl->next;
			}
		}
		cur = cur->next;
	}
	return true;
}

void PartnerSkillConf::unload()
{
    PartnerSkillConfMap::iterator it = m_skillMap.begin();
    while (it != m_skillMap.end())
    {
        if (it->second)
        {
			for (std::map<uint32_t, PartnerSkillUpgradeConf_t*>::iterator _it = (it->second)->upgrdateConfMap.begin(); _it != (it->second)->upgrdateConfMap.end(); ++_it) {
				delete _it->second;
			}
			(it->second)->upgrdateConfMap.clear();
            delete it->second;
            it->second = NULL;
        }
        ++it;
    }
    m_skillMap.clear();
}

const PartnerSkillConf_t* PartnerSkillConf::getPartnerSkill(uint32_t nSkill)
{
    PartnerSkillConfMap::const_iterator it = m_skillMap.find(nSkill);
    if (it != m_skillMap.end())
    {
        return it->second;
    }
    return NULL;
}
