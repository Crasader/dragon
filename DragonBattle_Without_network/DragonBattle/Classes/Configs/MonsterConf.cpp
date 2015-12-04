//
//  MonsterConf.cpp
//  DragonBattle
//
//  Created by zeng xiaolong on 13-8-2.
//
//

#ifdef WONPEE_SERVER
#include "cocos2d_diff.h"
#else
#include "cocos2d.h"
#endif

#include <map>
#include "MonsterConf.h"
using namespace cocos2d;
MonsterConf::MonsterConf()
{
    m_MonsterMap.clear();
}
MonsterConf::~MonsterConf()
{
    this->unload();	
}

bool MonsterConf::loadFromFile(char* filePath, bool bEncrypt)
{
    bool ret = true;
    
#ifdef WONPEE_SERVER
	const char* xmlFile = filePath;
#else
    const char* xmlFile = CCFileUtils::sharedFileUtils()->fullPathForFilename(filePath).c_str();
#endif
	xmlDocPtr doc = XmlEncrypt::getXmlDocPtr(xmlFile, bEncrypt);
    
    if (!doc)
    {
        ERROR_RETURN(("MonsterConf::loadFromFile xmlParseDoc Monster_config.xml failded"), false);
    }
    
    xmlNodePtr cur = xmlDocGetRootElement(doc);
    if (!cur)
    {
        ERROR_LOG("MonsterConf::loadFromFile xmlDocGetRootElement Monster_config.xml failded");
        ret = false;
        goto exit;
    }
    
    cur = cur->xmlChildrenNode;
    while (cur)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar*)"MONSTER")))
        {
            MonsterUnitConf *info = new MonsterUnitConf();
            DECODE_XML_PROP_INT(info->nUnitID, cur, "ID");
            DECODE_XML_PROP_INT(info->nResourceID, cur, "ResourceID");
            DECODE_XML_PROP_INT_DEFAULT(info->nAIID, cur, "aiID",1);
            DECODE_XML_PROP_STR_DEFAULT(info->unitName, cur, "name", "");
            DECODE_XML_PROP_INT_DEFAULT(info->nMission, cur, "missionID",1);
            DECODE_XML_PROP_INT(info->nUnitType, cur, "monsterType");
            DECODE_XML_PROP_INT_DEFAULT(info->nLife, cur, "hp", 400);
            DECODE_XML_PROP_INT_DEFAULT(info->fOutAttack, cur, "attackout",1);
            DECODE_XML_PROP_INT_DEFAULT(info->fInAttack, cur, "attackin",1);
            DECODE_XML_PROP_INT_DEFAULT(info->fOutDef, cur, "defout",1);
            DECODE_XML_PROP_INT_DEFAULT(info->fInDef, cur, "defin",1);
            DECODE_XML_PROP_INT_DEFAULT(info->fCrit, cur, "crit",0);
            DECODE_XML_PROP_INT_DEFAULT(info->fToughness, cur, "defcrit",0);
            DECODE_XML_PROP_INT_DEFAULT(info->fRecoverHp, cur, "recoverHP",0);
            DECODE_XML_PROP_INT_DEFAULT(info->fReducehurt, cur, "reduceHurt",0);
            DECODE_XML_PROP_INT_DEFAULT(info->fAddhurt, cur, "addHurt",0);
            DECODE_XML_PROP_INT_DEFAULT(info->nMaxDropGold, cur, "maxDropGold",1);
            DECODE_XML_PROP_INT_DEFAULT(info->nMinDropGold, cur, "minDropGold",1);
            DECODE_XML_PROP_INT_DEFAULT(info->fDropGoldRatio, cur, "dropGoldRatio",1);
            DECODE_XML_PROP_INT_DEFAULT(info->nLv, cur, "level",1);
            DECODE_XML_PROP_INT_DEFAULT(info->nExp, cur, "exp",0);
            this->decodeDropItemArray(cur, (char*)"drop", &(info->dropItemVec), true);
            m_MonsterMap.insert(std::pair<uint32_t, MonsterUnitConf*>(info->nUnitID, info));
        }
        cur = cur->next;
    }
    ret = true;
    
exit:
    xmlFreeDoc(doc);
    return ret;
}

bool MonsterConf::decodeDropItemArray(xmlNodePtr chl, char* field, std::vector<itemDrop_t>* pVec, bool hasRate)
{
    int tmp_data[100] = {0};
    decode_xml_prop_arr_int_default(tmp_data, 100, chl, (void*)field, -1);
    for (int i = 0; i < 100; )
    {
        if (tmp_data[i] == -1)
        {
            break;
        }
        itemDrop_t item;
        item.nItemID = tmp_data[i++];
        item.nItemCnt = tmp_data[i++];
        if (hasRate)
        {
            item.fRate = tmp_data[i++];
        }
        else
        {
            item.fRate = 100;
        }
        
        pVec->push_back(item);
    }
    return true;
}

MonsterUnitConf* MonsterConf::getMonster(uint32_t monsterID)
{
    MonsterMap::const_iterator it = m_MonsterMap.find(monsterID);
    if (it != m_MonsterMap.end())
    {
        return it->second;
    }
    return NULL;
}

void MonsterConf::unload()
{
    
}

