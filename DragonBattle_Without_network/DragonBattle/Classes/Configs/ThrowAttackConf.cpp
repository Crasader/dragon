//
//  ThrowAttackConf.cpp
//  DragonBattle
//
//  Created by 永翔 何 on 13-5-17.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "ThrowAttackConf.h"

#ifdef WONPEE_SERVER
#include "cocos2d_diff.h"
#else
#include "cocos2d.h"
#endif
using namespace cocos2d;

ThrowAttackConfig::ThrowAttackConfig()
{
    m_ThrowAttackConfigMap.clear();
}

ThrowAttackConfig::~ThrowAttackConfig()
{
    
}

bool ThrowAttackConfig::loadFromFile(const char* fileName, bool bEncrypt)
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
        ERROR_RETURN(("ThrowAttackConfig::loadThrowAttackFromXML xmlParseDoc throw_attack_config.xml failded"), false);
    }
    
    xmlNodePtr cur = xmlDocGetRootElement(doc);
    if (!cur)
    {
        ERROR_LOG("ThrowAttackConfig::loadThrowAttackFromXML xmlDocGetRootElement throw_attack_config.xml failded");
        ret = false;
        goto exit;
    }

    {   
        xmlNodePtr chl = cur->xmlChildrenNode;
        while (chl)
        {
            if (!xmlStrcmp(chl->name, reinterpret_cast<const xmlChar*>("THROW_ATTACK")))
            {
                ThrowAttackConfig_t* pConfig = new ThrowAttackConfig_t;
                DECODE_XML_PROP_INT(pConfig->nID, chl, "ID");
                DECODE_XML_PROP_INT(pConfig->nResID, chl, "resID");
                int tmp;
                DECODE_XML_PROP_INT(tmp, chl, "attackType");
                pConfig->nAttackType = (kThrowAttackType)tmp;
                DECODE_XML_PROP_FLOAT(pConfig->fAttackValue, chl, "attackValue");
                DECODE_XML_PROP_INT(tmp, chl, "throwType");
                pConfig->nThrowType = (kThrowType)tmp;
                
                float pos[2] = {0};
                decode_xml_prop_arr_float_default(pos, 2, chl, (void*)"throwStartX", 0);
                pConfig->ptThrowStart = cocos2d::CCPoint(pos[0], pos[1]);
                
                DECODE_XML_PROP_INT_DEFAULT(pConfig->nFixDirection, chl, "fixDirection", 0);
                DECODE_XML_PROP_INT_DEFAULT(pConfig->nFixPoi, chl, "fixPoi", 0);
                DECODE_XML_PROP_INT_DEFAULT(pConfig->nJudgeCollide, chl, "judgeCollide", 0);
                DECODE_XML_PROP_FLOAT_DEFAULT(pConfig->fAttackInterval, chl, "attackInterval", 0.5);
                                
                xmlNodePtr nodeChl = chl->xmlChildrenNode;
                switch (pConfig->nThrowType) 
                {
                    case kSpeed:
                    {
                        while (nodeChl)
                        {
                            if (!xmlStrcmp(nodeChl->name, reinterpret_cast<const xmlChar*>("THROW_SPEED")))
                            {
                                DECODE_XML_PROP_FLOAT(pConfig->throwUnion.throwSpeed.fThrowSpeedX, nodeChl, "throwSpeedX");
                                DECODE_XML_PROP_FLOAT(pConfig->throwUnion.throwSpeed.fThrowSpeedY, nodeChl, "throwSpeedY");
                                DECODE_XML_PROP_FLOAT(pConfig->throwUnion.throwSpeed.fThrowSpeedAddX, nodeChl, "throwSpeedAddX");
                                DECODE_XML_PROP_FLOAT(pConfig->throwUnion.throwSpeed.fThrowSpeedAddY, nodeChl, "throwSpeedAddY");
                            } 
                            nodeChl = nodeChl->next;
                        }
                    }
                        break;
                    case kBezier:
                    {
                        while (nodeChl)
                        {
                            if (!xmlStrcmp(nodeChl->name, reinterpret_cast<const xmlChar*>("THROW_BEZIER")))
                            {
                                DECODE_XML_PROP_FLOAT(pConfig->throwUnion.throwBezier.fThrowSpeed, nodeChl, "throwSpeed");
                                DECODE_XML_PROP_FLOAT(pConfig->throwUnion.throwBezier.fThrowHeight, nodeChl, "throwHeight");
                            } 
                            nodeChl = nodeChl->next;
                        }
                    }
                        break;
                    case kFollow:
                    {
                        while (nodeChl)
                        {
                            if (!xmlStrcmp(nodeChl->name, reinterpret_cast<const xmlChar*>("THROW_FOLLOW")))
                            {
                                DECODE_XML_PROP_FLOAT(pConfig->throwUnion.throwFollow.fThrowFollowX, nodeChl, "throwFollowX");
                                DECODE_XML_PROP_FLOAT(pConfig->throwUnion.throwFollow.fThrowFollowY, nodeChl, "throwFollowY");
                            }   
                            nodeChl = nodeChl->next;
                        }                     
                    }
                        break;
                    case kTrace:
                    {
                        while (nodeChl)
                        {
                            if (!xmlStrcmp(nodeChl->name, reinterpret_cast<const xmlChar*>("THROW_TRACE")))
                            {
                                DECODE_XML_PROP_FLOAT(pConfig->throwUnion.throwTrace.fThrowTraceX, nodeChl, "throwTraceX");
                                DECODE_XML_PROP_FLOAT(pConfig->throwUnion.throwTrace.fThrowTraceY, nodeChl, "throwTraceY");
                            }   
                            nodeChl = nodeChl->next;
                        }                          
                    }
                        break;
                }
                m_ThrowAttackConfigMap.insert(std::pair<uint16_t, ThrowAttackConfig_t*>(pConfig->nID, pConfig));
            }
            chl = chl->next;
        }
    }
exit:
    xmlFreeDoc(doc);
    return ret;
}

void ThrowAttackConfig::unloadThrowAttackConfig()
{
    ThrowAttackConfigMap::iterator it = m_ThrowAttackConfigMap.begin();
    while (it != m_ThrowAttackConfigMap.end())
    {
        delete it->second;
        it->second = NULL;
        it++;
    }
    m_ThrowAttackConfigMap.clear();
}

ThrowAttackConfig_t* ThrowAttackConfig::getThrowAttackFromID(uint16_t throwAttackID) const
{
    ThrowAttackConfigMap::const_iterator it = m_ThrowAttackConfigMap.find(throwAttackID);
    if (it != m_ThrowAttackConfigMap.end())
    {
        return it->second;
    }
    return NULL;
}

bool ThrowAttackConfig::checkThrowAttackConfigRight()
{
    return true;
}
