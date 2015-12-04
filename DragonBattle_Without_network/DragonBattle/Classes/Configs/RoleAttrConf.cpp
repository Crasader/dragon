//
//  RoleAttrConf.cpp
//  DragonBattle
//
//  Created by 何 永翔 on 13-7-3.
//
//

#include "RoleAttrConf.h"

#ifdef WONPEE_SERVER
#include "cocos2d_diff.h"
#else
#include "cocos2d.h"
#endif

using namespace cocos2d;

RoleAttrConf::RoleAttrConf()
{
    m_roleAttrMap.clear();
}
RoleAttrConf::~RoleAttrConf()
{
    this->unload();
}

bool RoleAttrConf::loadFromFile(const char* fileName, bool bEncrypt)
{
    bool ret = true;
    
#ifdef WONPEE_SERVER
	const char* xmlFile = fileName;
#else
    const char* xmlFile = CCFileUtils::sharedFileUtils()->fullPathForFilename(fileName).c_str();
#endif
	xmlDocPtr doc = XmlEncrypt::getXmlDocPtr(xmlFile, bEncrypt);
    
    if (!doc)
    {
        ERROR_RETURN(("RoleAttrConf::loadFromFile xmlParseFile role_attr_config.xml failded"), false);
    }
    
    xmlNodePtr cur = xmlDocGetRootElement(doc);
    if (!cur)
    {
        ERROR_LOG("RoleAttrConf::loadFromFile xmlDocGetRootElement role_attr_config.xml failded");
        ret = false;
        goto exit;
    }
    
    cur = cur->xmlChildrenNode;
    while (cur)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar*)"ROLE_TYPE")))
        {
            int nTypeID = 0;
            DECODE_XML_PROP_INT(nTypeID, cur, "ID");
        	
			float fFacList[6] = {0};

			decode_xml_prop_arr_float_default(fFacList, 6, cur, (void*)"roleFactor", 0);
			DEBUG_LOG("Role Attr Factor[%f %f %f %f %f %f]", fFacList[0], fFacList[1], fFacList[2], fFacList[3], fFacList[4], fFacList[5]);

            xmlNodePtr chl = cur->xmlChildrenNode;
            while (chl)
            {
                if ((!xmlStrcmp(chl->name, (const xmlChar*)"LEVEL")))
                {
                    int nLV = 0;
                    DECODE_XML_PROP_INT(nLV, chl, "LV");
                    
                    RoleAttrConf_t* pUnit = new RoleAttrConf_t();
                    DECODE_XML_PROP_INT_DEFAULT(pUnit->nLife, chl, "life", 0);
                    DECODE_XML_PROP_INT_DEFAULT(pUnit->nOutAttack, chl, "outAttack", 0);
                    DECODE_XML_PROP_INT_DEFAULT(pUnit->nOutDef, chl, "outDef", 0);
                    DECODE_XML_PROP_INT_DEFAULT(pUnit->nInAttack, chl, "inAttack", 0);
                    DECODE_XML_PROP_INT_DEFAULT(pUnit->nInDef, chl, "inDef", 0);
                    DECODE_XML_PROP_INT_DEFAULT(pUnit->nCrit, chl, "crit", 0);
                    DECODE_XML_PROP_INT_DEFAULT(pUnit->nToughness, chl, "toughness", 0);
                    DECODE_XML_PROP_INT_DEFAULT(pUnit->nReduceHurt, chl, "reduceHurt", 0);
                    DECODE_XML_PROP_INT_DEFAULT(pUnit->nAddHurt, chl, "addhurt", 0);
                    DECODE_XML_PROP_INT_DEFAULT(pUnit->nRecoverHP, chl, "recoverHP", 0);
                    pUnit->roleTrainAttr.fMaxLife = (pUnit->nLife * fFacList[0]);
					pUnit->roleTrainAttr.fMaxOutAttack = (pUnit->nOutAttack * fFacList[1]);
					pUnit->roleTrainAttr.fMaxInAttack = (pUnit->nInAttack  * fFacList[2]);
					pUnit->roleTrainAttr.fMaxOutDef = (pUnit->nOutDef  * fFacList[3]);
					pUnit->roleTrainAttr.fMaxInDef = (pUnit->nInDef  * fFacList[4]);
					pUnit->roleTrainAttr.fMaxToughness = (pUnit->nToughness  * fFacList[5]);
					m_roleAttrMap.insert(std::pair<int, RoleAttrConf_t*>(getMapKey(nLV, nTypeID), pUnit));
                }
                chl = chl->next;
            }
        }
        cur = cur->next;
    }
    ret = true;

exit:
    xmlFreeDoc(doc);
    return ret;
}

int RoleAttrConf::getMapKey(int nLevel, int nRoleType)
{
    return nRoleType * 10000 + nLevel;
}

void RoleAttrConf::unload()
{
    RoleAttrConfMap::iterator it = m_roleAttrMap.begin();
    while (it != m_roleAttrMap.end())
    {
        if (it->second)
        {
            delete it->second;
            it->second = NULL;
        }
        ++it;
    }
    m_roleAttrMap.clear();
}

const RoleAttrConf_t* RoleAttrConf::getRoleAttrConf(int nLevel, int nRoleType)
{
    RoleAttrConfMap::const_iterator it = m_roleAttrMap.find(getMapKey(nLevel, nRoleType));
    if (it != m_roleAttrMap.end())
    {
        return it->second;
    }
    else
    {
        return NULL;
    }
}
