//
//  TaskConf.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-9-23.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "TaskConf.h"


TaskConfig::TaskConfig()
{
	memset(m_nTaskLevelLimit, 0, sizeof(m_nTaskLevelLimit));
	m_taskMap.clear();
}

TaskConfig::~TaskConfig()
{
	this->unload();
}

bool TaskConfig::loadFromFile(const char* filePath, bool bEncrypt)
{
	bool ret = false;
    xmlDocPtr doc;
    xmlNodePtr cur, chl;
	int nCurLevel = 0;
	uint32_t nMaxBranchTaskID = 0;
    
#ifdef WONPEE_SERVER
	const char* xmlFile = filePath;
#else
	const char* xmlFile = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(filePath).c_str();
#endif
    
    doc = XmlEncrypt::getXmlDocPtr(xmlFile, bEncrypt);
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
        if ((!xmlStrcmp(cur->name, (const xmlChar*)"Task")))
        {
			TaskConfig_t* pTaskConf = new TaskConfig_t();
			DECODE_XML_PROP_INT(pTaskConf->nTaskID, cur, "ID");
            DECODE_XML_PROP_STR(pTaskConf->name, cur, "name");
            DECODE_XML_PROP_STR_DEFAULT(pTaskConf->tips, cur, "tips","");
            DECODE_XML_PROP_STR_DEFAULT(pTaskConf->desc, cur, "desc", "");
            DECODE_XML_PROP_INT_DEFAULT(pTaskConf->nTaskType, cur, "taskType", 1);
            DECODE_XML_PROP_INT_DEFAULT(pTaskConf->nPreTaskID, cur, "preTaskID", 0);
            DECODE_XML_PROP_INT_DEFAULT(pTaskConf->nAfterTaskID, cur, "afterTaskID", 0);
			DECODE_XML_PROP_INT_DEFAULT(pTaskConf->nAcceptNPC, cur, "accpeptNPC", 0);
            DECODE_XML_PROP_INT_DEFAULT(pTaskConf->nFinishNPC, cur, "finishNPC", 0);
            DECODE_XML_PROP_INT_DEFAULT(pTaskConf->nNeedLevel, cur, "needLevel", 0);
			DECODE_XML_PROP_INT_DEFAULT(pTaskConf->nMaxLevel, cur, "maxLevel", 0);
            DECODE_XML_PROP_INT_DEFAULT(pTaskConf->nFinishType, cur, "finishType", 0);
            DECODE_XML_PROP_INT_DEFAULT(pTaskConf->nSkillIndex, cur, "skillIndex", 0);
			DECODE_XML_PROP_INT_DEFAULT(pTaskConf->nNeedID, cur, "needID", 0);
			DECODE_XML_PROP_INT_DEFAULT(pTaskConf->nNeedCount, cur, "needCount", 0);
            DECODE_XML_PROP_INT_DEFAULT(pTaskConf->nBossID, cur, "bossID", 0);
			
			if (nCurLevel < (int)pTaskConf->nNeedLevel && IS_BRANCH_LINE_TASK(pTaskConf->nTaskID)) {
				for (int i = nCurLevel + 1; i <= (int)pTaskConf->nNeedLevel; i++) {
					m_nTaskLevelLimit[i] = pTaskConf->nTaskID;
				}
				nCurLevel = pTaskConf->nNeedLevel;
			}
            
			if (IS_BRANCH_LINE_TASK(pTaskConf->nTaskID)) {
				nMaxBranchTaskID = pTaskConf->nTaskID;
			}
			
			int nList[8];
			decode_xml_prop_arr_int_default(nList, 8, cur, (void*)"swapID", 0);
			memset(pTaskConf->nPrizeSwap, 0, sizeof(pTaskConf->nPrizeSwap));
			pTaskConf->nPrizeSwap[nList[0]] = nList[1];
			if (nList[0] != 0) {
				pTaskConf->nPrizeSwap[nList[2]] = nList[3];
				pTaskConf->nPrizeSwap[nList[4]] = nList[5];
			}
			//DEBUG_LOG("@@@@@@@ print task prize:[%d %d %d %d %d %d]", pTaskConf->nTaskID, pTaskConf->nPreTaskID, pTaskConf->nPrizeSwap[0], pTaskConf->nPrizeSwap[1], pTaskConf->nPrizeSwap[2], pTaskConf->nPrizeSwap[3]);
            //DECODE_XML_PROP_STR_DEFAULT(pTaskConf->name, cur, "name", "");
            
            chl = cur->xmlChildrenNode;
            while (chl)
            {
                if (!xmlStrcmp(chl->name, (const xmlChar*)("Stage")))
                {
					DECODE_XML_PROP_INT_DEFAULT(pTaskConf->stageTalk.nTalkTyep, chl, "stageType", 0);
					DECODE_XML_PROP_INT_DEFAULT(pTaskConf->stageTalk.nNpcID, chl, "npcID", 0);
					DECODE_XML_PROP_INT_DEFAULT(pTaskConf->stageTalk.nOverIndex, chl, "overIndex", 0);
					
    				xmlNodePtr stageChl = chl->xmlChildrenNode;
					while (stageChl)
					{
						if (!xmlStrcmp(stageChl->name, (const xmlChar*)("Talk")))
						{
							RoleTalk_t roleTalk;
							DECODE_XML_PROP_INT_DEFAULT(roleTalk.nRoleID, stageChl, "talkRoleID", 0);
							char msg[256] = {0};
            				DECODE_XML_PROP_STR_DEFAULT(msg, stageChl, "msg", "");
							roleTalk.roleTalk = msg;
							pTaskConf->stageTalk.stageTalkVec.push_back(roleTalk);
                			//DEBUG_LOG("STAGE TALK: %d %s", roleTalk.nRoleID, roleTalk.roleTalk.c_str());
						}
						stageChl = stageChl->next;
					}
                }
                if (!xmlStrcmp(chl->name, (const xmlChar*)("NpcTalk")))
                {
					int idx = 0;
					DECODE_XML_PROP_INT_DEFAULT(idx, chl, "idx", 0);
            		DECODE_XML_PROP_STR_DEFAULT(pTaskConf->npcTalk[idx - 1], chl, "msg", "");
                	//DEBUG_LOG("NPC TALK: %d %s", idx, pTaskConf->npcTalk[idx-1]);
				}
                chl = chl->next;
            }
            m_taskMap.insert(std::pair<uint32_t, TaskConfig_t*>(pTaskConf->nTaskID, pTaskConf));
        }
        cur = cur->next;
    }
    
	for (int i = nCurLevel + 1; i <= 200; i++) {
		m_nTaskLevelLimit[i] = nMaxBranchTaskID;
	}
    
    ret = true;
exit:
    xmlFreeDoc(doc);
    return ret;
}

void TaskConfig::unload()
{
	TaskMap::iterator it = m_taskMap.begin();
    while (it != m_taskMap.end())
    {
		(it->second)->stageTalk.stageTalkVec.clear();
        delete it->second;
        it->second = NULL;
        ++it;
    }
    m_taskMap.clear();
}

const TaskConfig_t* TaskConfig::getTaskConfig(uint32_t nTaskID) const
{
	TaskMap::const_iterator it = m_taskMap.find(nTaskID);
    if (it != m_taskMap.end())
    {
        return it->second;
    }
    return NULL;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
//begin AwardTaskConfig
AwardTaskConfig::AwardTaskConfig()
{
	memset(m_awardTaskArray, 0, sizeof(m_awardTaskArray));
	memset(m_nTaskLevelLimit, 0, sizeof(m_nTaskLevelLimit));
}

AwardTaskConfig::~AwardTaskConfig()
{
	memset(m_awardTaskArray, 0, sizeof(m_awardTaskArray));
	memset(m_nTaskLevelLimit, 0, sizeof(m_nTaskLevelLimit));
}

bool AwardTaskConfig::loadFromFile(const char* filePath, bool bEncrypt)
{
	bool ret = false;
    xmlNodePtr cur;
	uint32_t nCurLevel = 0;
    
#ifdef WONPEE_SERVER
	const char* xmlFile = filePath;
#else
	const char* xmlFile = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(filePath).c_str();
#endif
    
    xmlDocPtr doc = XmlEncrypt::getXmlDocPtr(xmlFile, bEncrypt);
    if (!doc)
    {
        ERROR_RETURN(("TaskConfig::loadFromFile xmlParseDoc award_tasks.xml failded"), false);
    }
    
    cur = xmlDocGetRootElement(doc);
    if (!cur)
    {
        ERROR_LOG("xmlDocGetRootElement %s error!", filePath);
        goto exit;
    }
    
	m_nTaskLevelLimit[nCurLevel] = 0;
    cur = cur->xmlChildrenNode;
    while (cur)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar*)"Task")))
        {
			int nTaskID = 0;
			DECODE_XML_PROP_INT(nTaskID, cur, "ID");
			if (nTaskID > MAX_AWARD_TASK_ID || m_awardTaskArray[nTaskID].nTaskID != 0) {
				ERROR_LOG("Award taskId too large %u", nTaskID);
				return false;
			}

			m_awardTaskArray[nTaskID].nTaskID = nTaskID;
			DECODE_XML_PROP_STR_DEFAULT(m_awardTaskArray[nTaskID].name, cur, "name", "");
            DECODE_XML_PROP_INT_DEFAULT(m_awardTaskArray[nTaskID].nNeedLevel, cur, "needLevel", 0);
            DECODE_XML_PROP_INT_DEFAULT(m_awardTaskArray[nTaskID].nFinishType, cur, "finishType", 0);
			DECODE_XML_PROP_INT_DEFAULT(m_awardTaskArray[nTaskID].nNeedID, cur, "needID", 0);
			DECODE_XML_PROP_INT_DEFAULT(m_awardTaskArray[nTaskID].nNeedCount, cur, "needCount", 0);
			DECODE_XML_PROP_INT_DEFAULT(m_awardTaskArray[nTaskID].nPrizeSwap, cur, "swapID", 0);
            DECODE_XML_PROP_STR(m_awardTaskArray[nTaskID].desc, cur, "desc");

        	if (nCurLevel != m_awardTaskArray[nTaskID].nNeedLevel) {
				m_nTaskLevelLimit[nCurLevel] = nTaskID;
				nCurLevel = m_awardTaskArray[nTaskID].nNeedLevel;
			}
		}
        cur = cur->next;
    }
    
    ret = true;
exit:
    xmlFreeDoc(doc);
    return ret;
}

void AwardTaskConfig::unload()
{
	memset(m_awardTaskArray, 0, sizeof(m_awardTaskArray));
}

const AwardTaskConfig_t* AwardTaskConfig::getAwardTaskConfig(uint32_t nTaskID) const
{
	if (nTaskID > MAX_AWARD_TASK_ID || m_awardTaskArray[nTaskID].nTaskID == 0) {
		return NULL;
	}
	return &(m_awardTaskArray[nTaskID]);
}

