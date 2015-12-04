//
//  LVConf.cpp
//  DragonBattle
//
//  Created by long on 13-5-27.
//
//

#ifdef WONPEE_SERVER
#include "cocos2d_diff.h"
#else
#include "GlobalDefine.h"
#include "cocos2d.h"
#endif
using namespace cocos2d;

#include "LVConf.h"


LVConf::LVConf()
{
    m_nCurMaxLevel = 0;
    memset(m_exp, 0, sizeof(m_exp));
}

LVConf::~LVConf()
{

}

bool LVConf::loadFromFile(const char* fileName, bool bEncrypt)
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
        ERROR_RETURN(("BattleMissionConf::loadFromFile xmlParseDoc level_config.xml failded"), false);
    }
    
    xmlNodePtr cur = xmlDocGetRootElement(doc);
    if (!cur)
    {
        ERROR_LOG("BattleMissionConf::loadFromFile xmlDocGetRootElement level_config.xml failded");
        ret = false;
        goto exit;
    }
    
    cur = cur->xmlChildrenNode;
    while (cur)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar*)"LEVEL")))
        {
            int nlv = 0;
            uint64_t nexp = 0;
            DECODE_XML_PROP_INT(nlv, cur, "LV");
            DECODE_XML_PROP_INT64(nexp, cur, "EXP");
            
            if (nlv != (int)m_nCurMaxLevel + 1)//等级必须连续配置
            {
                ret = false;
                goto exit;
            }
            
            m_exp[nlv] = nexp + m_exp[nlv - 1];
            m_nCurMaxLevel = nlv;
        }
        cur = cur->next;
    }
    ret = true;
    
exit:
    xmlFreeDoc(doc);
    return ret;
}

// 通过总经验得到等级
int LVConf::getLevelFromExp(uint64_t exp)
{
    for (int i = 1; i <= (int)m_nCurMaxLevel; i++)
    {
        if (exp < m_exp[i])
        {
            return i;
        }
    }
    //DEBUG_LOG("input error exp [%llu]", exp);
    return m_nCurMaxLevel;
}

int LVConf::getLVLimit()
{
    return m_nCurMaxLevel;
}

uint64_t LVConf::getExpLimit()
{
    return m_exp[m_nCurMaxLevel];
}
uint64_t LVConf::getLVExp(int lv)
{
    return m_exp[lv-1];
}
uint64_t LVConf::getNextLvExp(uint64_t nExp)
{
    uint32_t nLevel = this->getLevelFromExp(nExp);
    if (nLevel == m_nCurMaxLevel)
    {
    return m_exp[m_nCurMaxLevel];
    }
    return m_exp[nLevel+1];
}

float LVConf::getExpPercent(uint64_t nExp)
{
    uint32_t nLevel = this->getLevelFromExp(nExp);
    if (nLevel >= m_nCurMaxLevel)
    {
        return 0;
    }
    
    return 1.0 * (nExp - m_exp[nLevel-1]) / (m_exp[nLevel] - m_exp[nLevel-1]);
}
