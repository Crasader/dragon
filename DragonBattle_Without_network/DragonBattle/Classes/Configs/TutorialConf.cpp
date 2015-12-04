//
//  TutorialConf.cpp
//  DragonBattle
//
//  Created by 白明江 on 9/23/13.
//
//

#include "TutorialConf.h"


TutorialConfig::TutorialConfig()
{
}

TutorialConfig::~TutorialConfig()
{
    unload();
}

bool TutorialConfig::loadFromFile(const char* filePath, bool bEncrypt)
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
        ERROR_RETURN(("TaskConfig::loadFromFile xmlParseDoc tasks.xml failded"), false);
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
        if ((!xmlStrcmp(cur->name, (const xmlChar*)"tutorial")))
        {
            TutorialConfig_t* tutorial = new TutorialConfig_t;
            
            DECODE_XML_PROP_INT_DEFAULT(tutorial->nStep, cur, "step", 0);
            DECODE_XML_PROP_INT_DEFAULT(tutorial->nType, cur, "type", 0);
            DECODE_XML_PROP_INT_DEFAULT(tutorial->nNeedLevel, cur, "need_level", 0);
            DECODE_XML_PROP_INT_DEFAULT(tutorial->nTaskID, cur, "task_id", 0);
            DECODE_XML_PROP_INT_DEFAULT(tutorial->nUIType, cur, "ui_id", 0);
            DECODE_XML_PROP_INT_DEFAULT(tutorial->nFuncID, cur, "func_id", 0);

            DECODE_XML_PROP_INT_DEFAULT(tutorial->nDependStep, cur, "depend", 0);
            
#ifndef WONPEE_SERVER
            int tmp_data[100] = {0};
            decode_xml_prop_arr_int_default(tmp_data, 100, cur, (char*)"need_item_id", -1);
            for (int i = 0; i < 100; i ++)
            {
                if (tmp_data[i] == -1) break;
                tutorial->needItemVec.push_back(tmp_data[i]);
            }
           
            DECODE_XML_PROP_INT_DEFAULT(tutorial->nAction, cur, "action", 0);
            DECODE_XML_PROP_INT_DEFAULT(tutorial->nNextStep, cur, "next_step", 0);
            DECODE_XML_PROP_STR_DEFAULT(tutorial->play_describe, cur, "play_describe", "");
            
            char tmp [1024] = {0};
            DECODE_XML_PROP_STR_DEFAULT(tmp, cur, "assistant_say", "");
            tutorial->assistant_say = string(tmp);
            
            xmlNodePtr chl = cur->xmlChildrenNode;
            while (chl)
            {
                if ((!xmlStrcmp(chl->name, (const xmlChar*)"operate")))
                {
                    OperateConfig_t operate;
                    DECODE_XML_PROP_INT_DEFAULT(operate.nSubStep, chl, "sub_step", 0);
                    DECODE_XML_PROP_INT_DEFAULT(operate.nCount, chl, "count", 0);
                    DECODE_XML_PROP_INT_DEFAULT(operate.nForce, chl, "force", 0);
                    DECODE_XML_PROP_INT_DEFAULT(operate.nFuncID, chl, "func_id", 0);
                    DECODE_XML_PROP_STR_DEFAULT(operate.desc, chl, "describe", "");
                    
                    tutorial->subOperations.push_back(operate);
                }
                
                chl = chl->next;
            }
#endif
            m_tutorialMap.insert(std::pair<uint32_t, TutorialConfig_t*>(tutorial->nStep, tutorial));
        }

        cur = cur->next;
    }
    ret = true;

exit:
    xmlFreeDoc(doc);
    return ret;
}

void TutorialConfig::unload()
{
    TutorialMap::iterator it = m_tutorialMap.begin();
    
    while (it != m_tutorialMap.end()) {
        delete it->second;
        it->second = NULL;
        ++ it;
    }
    
    m_tutorialMap.clear();
    
}

#ifndef WONPEE_SERVER
const TutorialConfig_t* TutorialConfig::getTutorialConfWithTask(uint32_t nTaskID) const
{
    TutorialMap::const_iterator it = m_tutorialMap.begin();
    
    while (it != m_tutorialMap.end())
    {
        if (it->second->nTaskID == nTaskID)
        {
            return it->second;
        }
        
        ++ it;
    }
    
    return NULL;
}


const TutorialConfig_t* TutorialConfig::getTutorialConfWithLevel(uint32_t nLevelID) const
{
    TutorialMap::const_iterator it = m_tutorialMap.begin();
    
    while (it != m_tutorialMap.end())
    {
        if (it->second->nNeedLevel == nLevelID)
        {
            return it->second;
        }
        
        ++ it;
    }
    
    return NULL;
}

const TutorialConfig_t* TutorialConfig::getTutorialConfByStep(uint32_t step) const
{
    TutorialMap::const_iterator it = m_tutorialMap.find(step);
    if (it != m_tutorialMap.end())
    {
        return it->second;
    }
    
    return NULL;
}

const TutorialConfig_t* TutorialConfig::getTutorialConfByFuncID(uint32_t func) const
{
    for (TutorialMap::const_iterator it = m_tutorialMap.begin(); it != m_tutorialMap.end(); it ++)
    {
        TutorialConfig_t* conf = it->second;
        
        if (conf->nFuncID == func && conf->nTaskID != 0 && conf->nDependStep == 0)
        {
            return conf;
        }
        
        if (conf->nFuncID == func && conf->nNeedLevel != 0 && conf->nDependStep == 0)
        {
            return conf;
        }
    }
    
    return NULL;
}
#endif

Limit_Type TutorialConfig::checkFuncOpened(uint32_t func_id, uint32_t task_id, uint32_t level)
{
    Limit_Type iRet = eLimit_Type_None;
    
    for (TutorialMap::iterator it = m_tutorialMap.begin(); it != m_tutorialMap.end(); it ++)
    {
        TutorialConfig_t* conf = it->second;
        
        if (conf->nFuncID == func_id && conf->nTaskID > task_id && conf->nDependStep == 0)
        {
            iRet = eLimit_Type_Task;
            break;
        }
        
        if (conf->nFuncID == func_id && conf->nNeedLevel > level && conf->nDependStep == 0)
        {
            iRet = eLimit_Type_Level;
            break;
        }
    }
    
    return iRet;
}
