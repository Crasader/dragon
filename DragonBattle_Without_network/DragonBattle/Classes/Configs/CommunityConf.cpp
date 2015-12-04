//
//  CommunityConf.cpp
//  DragonBattle
//
//  Created by zeng xiaolong on 13-6-3.
//
//

#include "CommunityConf.h"
#ifdef WONPEE_SERVER
#include "cocos2d_diff.h"
#else
#include "cocos2d.h"
#endif
using namespace cocos2d;

CommunitymapConf::CommunitymapConf()
{
}

CommunitymapConf::~CommunitymapConf()
{
    this->unload();
}

bool CommunitymapConf::loadFromFile(const char* fileName, bool bEncrypt)
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
        ERROR_RETURN(("CommunitymapConf::loadFromFile xmlParseDoc community_config.xml failded"), false);
    }
    
    xmlNodePtr cur = xmlDocGetRootElement(doc);
    if (!cur)
    {
        ERROR_LOG("CommunitymapConf::loadFromFile xmlDocGetRootElement community_config.xml failded");
        ret = false;
        goto exit;
    }
    cur = cur->xmlChildrenNode;
    while (cur)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar*)"COMMUNITY")))
        {
            CommunitymapConf_t *pMap =  new CommunitymapConf_t;
            DECODE_XML_PROP_INT(pMap->nMapId, cur, "ID");
            DECODE_XML_PROP_INT(pMap->nNeedLevel, cur, "needLevel");
            char strName[64] = {0};
            DECODE_XML_PROP_STR(strName, cur, "name");
            pMap->strMapName = strName;
            DECODE_XML_PROP_STR_DEFAULT(strName, cur, "music","");
            pMap->strMusic = strName;
            DECODE_XML_PROP_STR_DEFAULT(strName, cur, "effect","");
            pMap->strEffect = strName;
            xmlNodePtr chl = cur->xmlChildrenNode;
            while (chl)
            {
                if((!xmlStrcmp(chl->name, (const xmlChar*)"BG_SCENE")))
                {
                    char strType[64] = {0};
                    DECODE_XML_PROP_STR(strType, chl, "SCENE_TYPE");
                    if (strcmp(strType, "LAND")==0)
                    {
                        this->loadCommunityBgScene(chl, &pMap->strMapPic);
                    }
                    else if(strcmp(strType, "NEAR")==0)
                    {
                        this->loadCommunityBgScene(chl, &pMap->strNearMapPic);
                    }
                }
                else if((!xmlStrcmp(chl->name, (const xmlChar*)"PLISTS")))
                {
                    this->loadPlist(chl, &pMap->strPlist);
                }
                else if((!xmlStrcmp(chl->name, (const xmlChar*)"NPCS")))
                {
                    this->loadNPC(chl, &pMap->npcVec);
                }
                else if((!xmlStrcmp(chl->name, (const xmlChar*)"MISSIONS")))
                {
                    this->loadMissions(chl, &pMap->missionVec);
                }
                else if((!xmlStrcmp(chl->name, (const xmlChar*)"BOSS_MISSIONS")))
                {
                    this->loadBossPropArr(chl, (char*)"ID", &pMap->bossMissionVec);
                }
                chl = chl->next;
            }
			CmtMapConfMap::value_type val(pMap->nMapId, pMap);
			if ((m_CmtMapConfMap.insert(val)).second == false) {
				ERROR_LOG("CommunityID already exists ID = %u", pMap->nMapId);
				return false;
			}
        }
        else if((!xmlStrcmp(cur->name, (const xmlChar*)"NPCRESOURCE")))
        {
            xmlNodePtr chl = cur->xmlChildrenNode;
            while (chl)
            {
                if((!xmlStrcmp(chl->name, (const xmlChar*)"NPC")))
                {
                    CmtNPCResource_t *pNpcResource = new CmtNPCResource_t;
                    char buff[32] = {0};
                    DECODE_XML_PROP_INT(pNpcResource->nResourceID, chl, "ResourceID");
                    DECODE_XML_PROP_STR(buff, chl, "roleFile");
                    pNpcResource->strRoleFile = buff;
                    DECODE_XML_PROP_STR(buff, chl, "headImage");
                    pNpcResource->headImage = buff;
                    DECODE_XML_PROP_STR(buff, chl, "bustImage");
                    pNpcResource->bustImage = buff;
                    DECODE_XML_PROP_STR(buff, chl, "resFile");
                    pNpcResource->strFile = buff;
                    DECODE_XML_PROP_STR(buff, chl, "plistFile");
                    pNpcResource->strplistFile = buff;
                    float contentSize[2] = {0};
                    float offPos[2] = {0};
                    decode_xml_prop_arr_float_default(contentSize, size_t(2), chl, (void*)"contentSize", float(0));
                    decode_xml_prop_arr_float_default(offPos, 2, chl, (void*)"offPos", float(0));
                    
                    pNpcResource->contentSize = CCSize(contentSize[0], contentSize[1]);
                    pNpcResource->offPos = CCPoint(offPos[0], offPos[1]);
                    
                    DECODE_XML_PROP_INT(pNpcResource->nStartFrame, chl, "frameStart");
                    DECODE_XML_PROP_INT(pNpcResource->nFrameCount, chl, "frameCount");
                    
                    cmtNPCResourceMap.insert(CmtNPCResourceMap::value_type(pNpcResource->nResourceID, pNpcResource));
                }
                chl = chl->next;
            }
        }
        cur = cur->next;
    }
exit:
    xmlFreeDoc(doc);
    return ret;
}

bool CommunitymapConf::loadBossPropArr(xmlNodePtr chl, char* field, std::vector<CmtBossMission*>* vec)
{
    int tmp_data[100] = {0};
    decode_xml_prop_arr_int_default(tmp_data, 100, chl, (void*)field, -1);
    for (int i = 0; i < 100;)
    {
        if (tmp_data[i] == -1)
        {
            break;
        }
        CmtBossMission* misstion = new CmtBossMission;
        misstion->nBossMissionID = tmp_data[i ++];
        misstion->nMissionID = tmp_data[i ++];
        vec->push_back(misstion);
    }
    
    return true;
}

bool CommunitymapConf::loadCommunityBgScene(xmlNodePtr chl,std::vector<std::string> * pFileVec)
{
    xmlNodePtr cur = chl->xmlChildrenNode;
    char strFile[64] = {0};
    while (cur)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar*)"PICTURE")))
        {
            DECODE_XML_PROP_STR(strFile, cur, "pic");
            pFileVec->push_back(strFile);
        }
        cur = cur->next;
    }
    return true;
}

bool CommunitymapConf::loadMissions(xmlNodePtr chl,std::vector<int> *missionvec)
{
    xmlNodePtr cur = chl->xmlChildrenNode;
    while (cur)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar*)"MISSION")))
        {
            int nmissionID;
            DECODE_XML_PROP_INT(nmissionID, cur, "ID");
            missionvec->push_back(nmissionID);
        }
        cur = cur->next;
    }
    return true;
}
bool CommunitymapConf::loadPlist(xmlNodePtr chl, std::vector<std::string>* pStrPlist)
{
    xmlNodePtr cur = chl->xmlChildrenNode;
    while (cur)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar*)"PLIST")))
        {
            char strName[64] = {0};
            DECODE_XML_PROP_STR(strName, cur, "name");
            pStrPlist->push_back(strName);
        }
        cur = cur->next;
    }
    return true;
}

bool CommunitymapConf::loadNPC(xmlNodePtr chl,std::vector<int> *npcvec)
{
    xmlNodePtr cur = chl->xmlChildrenNode;
    while (cur)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar*)"NPC")))
        {
            CommunityNPC_t *pNPC = new CommunityNPC_t;
            DECODE_XML_PROP_INT(pNPC->nNPCId, cur, "npcID");
            DECODE_XML_PROP_INT_DEFAULT(pNPC->secretShop, cur, "secretShop",0);
            DECODE_XML_PROP_INT_DEFAULT(pNPC->shop, cur, "shop",0);
            DECODE_XML_PROP_INT(pNPC->nResourceID, cur, "ResourceID");
            char strSay[256] = {0};
            DECODE_XML_PROP_STR(strSay, cur, "say");
            pNPC->strNPCSay = strSay;
            
            char strName[128] = {0};
            DECODE_XML_PROP_STR(strName, cur, "name");
            pNPC->strNPCName = strName;
            char strMusic[128] = {0};
            DECODE_XML_PROP_STR_DEFAULT(strMusic, cur, "music","");
            pNPC->strMusic = strMusic;
            
            float rolePos[2] = {0};
            decode_xml_prop_arr_float_default(rolePos, 2, cur, (void*)"pos", 0);
            pNPC->fPosX = rolePos[0];
            pNPC->fPosY = rolePos[1];
            
            npcvec->push_back(pNPC->nNPCId);
            cmtNPCMap.insert(CmtNPCMap::value_type(pNPC->nNPCId,pNPC));
        }
        cur = cur->next;
    }
    return true;
}

void CommunitymapConf::unload()
{
	for (CmtMapConfMap::iterator it = m_CmtMapConfMap.begin();
			it != m_CmtMapConfMap.end(); ++it) {
        CommunitymapConf_t* pCmtmap = it->second;
        pCmtmap->strMapPic.clear();
        if (pCmtmap != NULL)
        {
            delete pCmtmap;
            pCmtmap = NULL;
        }
    }
    CmtNPCMap::iterator it;
    for (it = cmtNPCMap.begin(); it != cmtNPCMap.end(); )
    {
        CommunityNPC_t *pNPC = (CommunityNPC_t*)it->second;
        delete pNPC;
        pNPC = NULL;
        it++;
    }
    cmtNPCMap.clear();
}

void CommunitymapConf::getCmdMapConfByLevel(uint32_t nLevel, CmtMapConfVec& cmtVec)
{
    for ( CmtMapConfMap::iterator it = m_CmtMapConfMap.begin();
			it != m_CmtMapConfMap.end(); ++it) {
        if (it->second->nNeedLevel <= nLevel) {
            cmtVec.push_back(it->second);
        }
    }
}
