//
//  SceneConf.cpp
//  DragonBattle
//
//  Created by zeng xiaolong on 13-8-27.
//
//

#include "SceneConf.h"

#ifdef WONPEE_SERVER
#include "cocos2d_diff.h"
#else
#include "cocos2d.h"
#endif
using namespace cocos2d;

BattleSceneConf::BattleSceneConf()
{
}

BattleSceneConf::~BattleSceneConf()
{
}

bool BattleSceneConf::loadFromFile(const char* fileName, bool bEncrypt)
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
        ERROR_RETURN(("BattleSceneConf::loadFromFile xmlParseFile BattleSceneConf.xml failded"), false);
    }
    
    xmlNodePtr cur = xmlDocGetRootElement(doc);
    if (!cur)
    {
        ERROR_LOG("BattleSceneConf::loadFromFile xmlDocGetRootElement BattleSceneConf.xml failded");
        goto exit;
	}
    cur = cur->xmlChildrenNode;
    while (cur)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar*)"SCENE")))
        {
            char buff[128] = {0};
            SceneConf_t* pScene = new SceneConf_t();
            DECODE_XML_PROP_INT(pScene->nResourceID, cur, "ID");
            DECODE_XML_PROP_INT_DEFAULT(pScene->bMarquee, cur, "Marquee",0);
            DECODE_XML_PROP_STR_DEFAULT(buff, cur, "music","");
            pScene->strMusic = buff;
            xmlNodePtr chl = cur->xmlChildrenNode;
            while (chl)
            {
                if ((!xmlStrcmp(chl->name, (const xmlChar*)"BG_SCENE")))
                {
                    char strType[64] = {0};
                    DECODE_XML_PROP_STR(strType, chl, "SCENE_TYPE");
                    if (strcmp(strType, "FAR") == 0)
                    {
                        float posy = 0;
                        DECODE_XML_PROP_FLOAT_DEFAULT(pScene->fFarBgWidth, chl, "widh", 0);
                        this->loadMissionBgScene(chl, &posy, &(pScene->farFileVec));
                        pScene->fFarBgPosY = posy;
                    }
                    else if (strcmp(strType, "MID") == 0)
                    {
                        float posy = 0;
                        DECODE_XML_PROP_FLOAT_DEFAULT(pScene->fMidBgWidth, chl, "widh", 0);
                        this->loadMissionBgScene(chl, &posy, &(pScene->midFileVec));
                        pScene->fMidBgPosY = posy;
                    }
                    else if (strcmp(strType, "LAND") == 0)
                    {
                        float posy = 0;
                        DECODE_XML_PROP_FLOAT_DEFAULT(pScene->fLandBgWidth, chl, "widh", 0);
                        this->loadMissionBgScene(chl, &posy, &(pScene->landFileVec));
                        pScene->fLandBgPosY = posy;
                    }
                    else if (strcmp(strType, "NEAR") == 0)
                    {
                        float fDefaultPosY = 0;
                        DECODE_XML_PROP_FLOAT_DEFAULT(pScene->fNearBgWidth, chl, "widh", 0);
                        this->loadMissionBgScene(chl, &fDefaultPosY, &(pScene->nearFileVec));
                    }
                }
                else if ((!xmlStrcmp(chl->name, (const xmlChar*)"SOURCE_LIST")))
                {
                    loadMissionResource(chl,&pScene->fileVec);
                }
                chl = chl->next;
            }
            m_SceneConfMap.insert(std::pair<uint32_t, SceneConf_t*>(pScene->nResourceID, pScene));
        }
        cur = cur->next;
    }
    ret = true;
exit:
    xmlFreeDoc(doc);
    return ret;
}

void BattleSceneConf::unload()
{

}

bool BattleSceneConf::loadMissionResource(xmlNodePtr chl,std::vector<std::string>* pFileVec)
{
    xmlNodePtr cur = chl->xmlChildrenNode;
    char strFile[64] = {0};
    while (cur)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar*)"SOURCE")))
        {
            DECODE_XML_PROP_STR(strFile, cur, "file");
            pFileVec->push_back(strFile);
        }
        cur = cur->next;
    }
    return true;
}

bool BattleSceneConf::loadMissionBgScene(xmlNodePtr chl, float* pPosY, MissionBGVec* pFileVec)
{
    DECODE_XML_PROP_FLOAT_DEFAULT((*pPosY), chl, "posY", 0);
    xmlNodePtr cur = chl->xmlChildrenNode;
    char strFile[64] = {0};
    while (cur)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar*)"PICTURE")))
        {
            MissionBG_t* pMissionBg = new MissionBG_t();
            DECODE_XML_PROP_STR(strFile, cur, "pic");
            pMissionBg->strBgName = strFile;
            DECODE_XML_PROP_FLOAT_DEFAULT(pMissionBg->fBgPosX, cur, "fPosX", 0);
            DECODE_XML_PROP_FLOAT_DEFAULT(pMissionBg->fBgPosY, cur, "fPosY", 0);
            DECODE_XML_PROP_INT_DEFAULT(pMissionBg->isFlipX, cur, "flipX", 0);
            pFileVec->push_back(pMissionBg);
        }
        cur = cur->next;
    }
    return true;
}

SceneConf_t* BattleSceneConf::getSceneConf(uint32_t nID)
{
    SceneMap::iterator it = m_SceneConfMap.find(nID);
	if (it != m_SceneConfMap.end())
    {
		return it->second;
    }
    return NULL;
}

