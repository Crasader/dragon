//
//  PartnerConf.cpp
//  DragonBattle
//
//
//

#ifdef WONPEE_SERVER
#include "cocos2d_diff.h"
#else
#include "cocos2d.h"
#endif

#include "PartnerConf.h"

using namespace cocos2d;
PartnerConfig::PartnerConfig()
{
    m_PartnerMap.clear();
}

PartnerConfig::~PartnerConfig()
{
    this->unload();	
}

bool PartnerConfig::loadFromFile(char* filePath, bool bEncrypt)
{
    bool ret = true;

#ifdef WONPEE_SERVER
	const char* xmlFile = filePath;
#else
    const char* xmlFile = CCFileUtils::sharedFileUtils()->fullPathForFilename(filePath).c_str();
#endif
	//xmlDocPtr doc = xmlParseFile(xmlFile);
    xmlDocPtr doc = XmlEncrypt::getXmlDocPtr(xmlFile, bEncrypt);

    if (!doc)
    {
        ERROR_RETURN(("PartnerConf::loadFromFile xmlParseDoc partner.xml failded"), false);
    }

    xmlNodePtr cur = xmlDocGetRootElement(doc);
    if (!cur)
    {
        ERROR_LOG("PartnerConf::loadFromFile xmlDocGetRootElement partner.xml failded");
        ret = false;
        xmlFreeDoc(doc);
        return ret;
    }

    cur = cur->xmlChildrenNode;
    while (cur)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar*)"PARTNER")))
        {
            //<PARTNER ID="1000" name="钟灵" profession="1" partner_fac="0.8" quarty="1" aptitude_fac="0.8" desc="">
            //<RELATION relate_partner="1002;1003" skill="0" effect_type="1" effect_value="2" desc=""/>
            PartnerConf_t *pConf = new PartnerConf_t;
            DECODE_XML_PROP_INT(pConf->nPartnerID, cur, "ID");
            DECODE_XML_PROP_INT(pConf->nResourceID, cur, "resourceID");
            DECODE_XML_PROP_INT(pConf->nItemID, cur, "item_id");
            DECODE_XML_PROP_STR(pConf->strName, cur, "name");
			DECODE_XML_PROP_INT_DEFAULT(pConf->nFriendVal, cur, "friend_val", 0);
            DECODE_XML_PROP_INT_DEFAULT(pConf->nNeedYxb, cur, "need_yxb", 0);
            DECODE_XML_PROP_INT_DEFAULT(pConf->nNeedGold, cur, "need_gold", 0);
            DECODE_XML_PROP_INT_DEFAULT(pConf->nNeedPrestige, cur, "need_prestige", 0);
            DECODE_XML_PROP_INT(pConf->nProfession, cur, "profession");
            DECODE_XML_PROP_INT(pConf->nQuarty, cur, "quarty");
            DECODE_XML_PROP_INT(pConf->nTalent, cur, "talent");
            DECODE_XML_PROP_FLOAT(pConf->fTalentValue, cur, "talent_value");
            DECODE_XML_PROP_FLOAT(pConf->fPartnerFac, cur, "partner_fac");
            DECODE_XML_PROP_FLOAT(pConf->fAptitudeFac, cur, "aptitude_fac");
            DECODE_XML_PROP_INT(pConf->nHp, cur, "HP");
            DECODE_XML_PROP_INT(pConf->nInAtt, cur, "inAtt");
            DECODE_XML_PROP_INT(pConf->nOutAtt, cur, "outAtt");
            DECODE_XML_PROP_INT(pConf->nInDef, cur, "inDef");
            DECODE_XML_PROP_INT(pConf->nOutDef, cur, "outDef");
            DECODE_XML_PROP_INT_DEFAULT(pConf->nSkillID, cur, "skillID", 50001);
            xmlNodePtr chl = cur->xmlChildrenNode;
            while (chl)
            {
                if ((!xmlStrcmp(chl->name, (const xmlChar*)"RELATION")))
                {
                    PartnerRelate_t* pRelate = new PartnerRelate_t;
                    decode_xml_prop_arr_int_default(pRelate->nRelatePartner, 4, chl, (void*)"relate_partner", 0);
                    DECODE_XML_PROP_INT_DEFAULT(pRelate->nPartnerSkill, chl, "skill", 0);
                    DECODE_XML_PROP_INT(pRelate->nEffectType, chl, "effect_type");
                    DECODE_XML_PROP_FLOAT(pRelate->fEffectValue, chl, "effect_value");
                    DECODE_XML_PROP_STR_DEFAULT(pRelate->desc, chl, "desc", "");
                    pConf->partnerRelateVec.push_back(pRelate);
                }
                chl = chl->next;
            }
            m_PartnerMap.insert(std::pair<uint32_t, PartnerConf_t*>(pConf->nPartnerID, pConf));
        }
        cur = cur->next;
    }
    ret = true;

    xmlFreeDoc(doc);
    return ret;
}

PartnerConf_t* PartnerConfig::getPartnerConf(uint32_t nPartnerID)
{
    PartnerMap::const_iterator it = m_PartnerMap.find(nPartnerID);
    if (it != m_PartnerMap.end())
    {
        return it->second;
    }
    return NULL;
}

PartnerMap* PartnerConfig::getPartnerMap()
{
    return &m_PartnerMap;
}

void PartnerConfig::unload()
{
    PartnerMap::iterator it = m_PartnerMap.begin();
    while (it != m_PartnerMap.end())
    {
        PartnerConf_t *pConf = it->second;
        for (int i = 0; i < (int)pConf->partnerRelateVec.size(); i++)
        {
            delete pConf->partnerRelateVec[i];
            pConf->partnerRelateVec[i] = NULL;
        }
        pConf->partnerRelateVec.clear();
        
        delete it->second;
        it->second = NULL;
        ++it;
    }
    m_PartnerMap.clear();
}

