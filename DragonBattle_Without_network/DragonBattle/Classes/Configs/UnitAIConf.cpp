//
//  BattleUnitConf.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-19.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "UnitAIConf.h"

#ifdef WONPEE_SERVER
#include "cocos2d_diff.h"
#else
#include "cocos2d.h"
#endif
using namespace cocos2d;

AIConfig::AIConfig()
{
    m_AIJudgeTreeMap.clear();
}

AIConfig::~AIConfig()
{
    
}
    
bool AIConfig::loadFromFile(const char* fileName, bool bEncrypt)
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
        ERROR_RETURN(("AIConfig::loadAIFromXML xmlParseDoc AiConfig.xml failded"), false);
    }
    
    xmlNodePtr cur = xmlDocGetRootElement(doc);
    if (!cur)
    {
        ERROR_LOG("AIConfig::loadAIFromXML xmlDocGetRootElement AiConfig.xml failded");
        ret = false;
        goto exit;
    }

    cur = cur->xmlChildrenNode;
    while (cur)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar*)"AI_TREE")))
        {
            uint32_t treeID = 0;
            DECODE_XML_PROP_INT(treeID, cur, "ID");
            AIJudgeTree* pJudgeTree = new AIJudgeTree;
            
            xmlNodePtr chl = cur->xmlChildrenNode;
            while (chl)
            {
                if (!xmlStrcmp(chl->name, reinterpret_cast<const xmlChar*>("TREE_NODE")))
                {
                    AIJudgeTreeNode_t* pTreeNode = new AIJudgeTreeNode_t();
                    pTreeNode->childVec.clear();
                    pTreeNode->policyVec.clear();
                    pTreeNode->nJudgeType = 0;
                    char nodeType[20] = {0};
                    DECODE_XML_PROP_INT(pTreeNode->nNodeID, chl, "nodeID");
                    DECODE_XML_PROP_STR(nodeType, chl, "nodeType");
                    if (strcmp(nodeType, "JUDGE") == 0)
                    {
                        DECODE_XML_PROP_INT(pTreeNode->nJudgeType, chl, "judgeType");
                        DECODE_XML_PROP_FLOAT_DEFAULT(pTreeNode->fJudgeNorm, chl, "judgeNorm", 0);
                        xmlNodePtr nodeChl = chl->xmlChildrenNode;
                        while (nodeChl)
                        {
                            if (!xmlStrcmp(nodeChl->name, reinterpret_cast<const xmlChar*>("AI_JUDGE")))
                            {
                                AIJudgeCondition_t* pJudegeCondition = new AIJudgeCondition_t();
                                DECODE_XML_PROP_INT(pJudegeCondition->bTrue, nodeChl, "judgeValue");
                                DECODE_XML_PROP_INT_DEFAULT(pJudegeCondition->nChildNodeID, nodeChl, "childId", 0);
                                DECODE_XML_PROP_INT_DEFAULT(pJudegeCondition->nNextTreeID, nodeChl, "nextTreeID", 0);
                                
                                //DEBUG_LOG("%d %d %d", pJudegeCondition->bTrue, pJudegeCondition->nChildNodeID, pJudegeCondition->nNextTreeID);
                                pTreeNode->childVec.push_back(pJudegeCondition);
                            }
                            nodeChl = nodeChl->next;
                        }
                    }
                    else if (strcmp(nodeType, "POLICY") == 0)
                    {
                        xmlNodePtr nodeChl = chl->xmlChildrenNode;
                        while (nodeChl)
                        {
                            if (!xmlStrcmp(nodeChl->name, reinterpret_cast<const xmlChar*>("AI_POLICY")))
                            {
                                AIProbabilyState_t* pPolicy = new AIProbabilyState_t();
                                DECODE_XML_PROP_INT(pPolicy->nAiAction, nodeChl, "policyResult");
                                DECODE_XML_PROP_FLOAT_DEFAULT(pPolicy->rangeStart, nodeChl, (void*)"randStart", 0.0);
                                DECODE_XML_PROP_FLOAT_DEFAULT(pPolicy->rangeEnd, nodeChl, (void*)"randEnd", 0.0);
                                DECODE_XML_PROP_INT_DEFAULT(pPolicy->nRunFrames, nodeChl, (void*)"runFrames", 20);
                                pTreeNode->policyVec.push_back(pPolicy);
                                //DEBUG_LOG("%d %f %f", pPolicy->nAiAction, pPolicy->rangeStart, pPolicy->rangeEnd);
                            }
                            nodeChl = nodeChl->next;
                        }
                    }
                    else
                    {
                        ERROR_LOG("load AI config nodeType error!");
                        ret = false;
                        goto exit;
                    }
                    pJudgeTree->push_back(pTreeNode);
                }
                chl = chl->next;
            }
            m_AIJudgeTreeMap.insert(std::pair<uint32_t, AIJudgeTree*>(treeID, pJudgeTree));
        }
        cur = cur->next;
    }
    
exit:
    xmlFreeDoc(doc);
    return ret;
}

void AIConfig::unloadAIConfig()
{
    AIJudgeTreeMap::iterator it = m_AIJudgeTreeMap.begin();
    while (it != m_AIJudgeTreeMap.end())
    {
        AIJudgeTree* pTree = it->second;
        if (pTree)
        {
            for (int i = 0; i < (int)pTree->size(); ++i)
            {
                AIJudgeTreeNode_t* pNode = (*pTree)[i];
                if (pNode)
                {
                    for (int j = 0; j < (int)pNode->childVec.size(); j++)
                    {
                        delete (pNode->childVec)[j];
                        (pNode->childVec)[j] = NULL;
                    }
                    pNode->childVec.clear();
                    
                    for (int j = 0; j < (int)pNode->policyVec.size(); j++)
                    {
                        delete (pNode->policyVec)[j];
                        (pNode->policyVec)[j] = NULL;
                    }
                    pNode->policyVec.clear();
                }
                delete pNode;
                pNode = NULL;
            }
            pTree->clear();
        }
        delete it->second;
        it->second = NULL;
        it++;
    }
    m_AIJudgeTreeMap.clear();
}

const AIJudgeTree* AIConfig::getAIFromID(uint16_t treeID) const
{
    //return m_AIJudgeTreeMap[treeID];
    AIJudgeTreeMap::const_iterator it = m_AIJudgeTreeMap.find(treeID);
    if (it != m_AIJudgeTreeMap.end())
    {
        return it->second;
    }
    return NULL;
}

bool AIConfig::checkAIConfigRight()
{
    
    return true;
}
