 //
//  BattleMissionConf.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-16.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifdef WONPEE_SERVER
#include "cocos2d_diff.h"
#else
#include "GlobalDefine.h"
#include "cocos2d.h"
#endif
using namespace cocos2d;

#include "BattleMissionConf.h"

BattleMissionConf::~BattleMissionConf()
{
    this->unload();
}

bool BattleMissionConf::loadTraps(xmlNodePtr chl, MissionConf_t* pMission)
{
    xmlNodePtr cur = chl->xmlChildrenNode;
    while (cur)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar*)"TRAP")))
        {
            TrapUnit tu;
            DECODE_XML_PROP_INT_DEFAULT(tu.nResourceID, cur, "ID", 0);
            DECODE_XML_PROP_INT_DEFAULT(tu.nUnitType, cur, "trapType", 0);
            DECODE_XML_PROP_INT_DEFAULT(tu.nLife, cur, "hp",4);
            DECODE_XML_PROP_INT_DEFAULT(tu.nJudgeCollide, cur, "judgeCollide", 0);
            float pos[2] = {0};
            decode_xml_prop_arr_float_default(pos, 2, cur, (void*)"pos", 0);
            tu.pos = CCPoint(pos[0], pos[1]);
            float size[2] = {0};
            decode_xml_prop_arr_float_default(size, 2, cur, (void*)"size", 0);
            tu.size = CCSize(size[0], size[1]);
            DECODE_XML_PROP_FLOAT_DEFAULT(tu.fAttackInterval, cur, "attackInterval", 0);
            
            DECODE_XML_PROP_INT_DEFAULT(tu.fOutAttack, cur, "attackout",1);
            DECODE_XML_PROP_INT_DEFAULT(tu.fInAttack, cur, "attackin",1);
            DECODE_XML_PROP_INT_DEFAULT(tu.fOutDef, cur, "defout",1);
            DECODE_XML_PROP_INT_DEFAULT(tu.fInDef, cur, "defin",1);
            DECODE_XML_PROP_INT_DEFAULT(tu.nExp, cur, "exp",0);
            this->decodeDropItemArray(cur, (char*)"drop", &(tu.dropItemVec), true);
            
            pMission->trapUnitVec.push_back(tu);
        }
        cur = cur->next;
    }
    return true;
}

bool BattleMissionConf::loadObstacles(xmlNodePtr chl, MissionConf_t* pMission)
{
    xmlNodePtr cur = chl->xmlChildrenNode;
    while (cur)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar*)"MAP_OBSTACLE")))
        {
            Obstacle_t o;
            DECODE_XML_PROP_INT_DEFAULT(o.nResID, cur, "resID", 0);
            DECODE_XML_PROP_FLOAT_DEFAULT(o.fWidth, cur, "width", 0);
            DECODE_XML_PROP_FLOAT_DEFAULT(o.fHeight, cur, "height", 0);
            float pos[2] = {0};
            decode_xml_prop_arr_float_default(pos, 2, cur, (void*)"pos", 0);
            o.ptPosition = cocos2d::CCPoint(pos[0], pos[1]);
            
            pMission->obstacleVec.push_back(o);
        }
        cur = cur->next;
    }
    return true;    
}

bool BattleMissionConf::loadMissionBgScene(xmlNodePtr chl, float* pPosY, MissionBGVec* pFileVec)
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

bool BattleMissionConf::loadMissionStage(xmlNodePtr chl, MissionConf_t* pMission)
{
    MissionStage_t stage = {0};
    DECODE_XML_PROP_FLOAT(stage.fClipScreenPosX, chl, "clipScreenX");
    DECODE_XML_PROP_FLOAT(stage.fClipScreenLength, chl, "clipScreenLen");
    DECODE_XML_PROP_FLOAT_DEFAULT(stage.fRefreshTime, chl, "refreshTime", 1);
    xmlNodePtr cur = chl->xmlChildrenNode;
    while (cur)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar*)"BORN_MONSTER")))
        {
            xmlNodePtr bornChl = cur->xmlChildrenNode;
            int i = 0;
            MonsterBornInfoVec pVector;
            while (bornChl)
            {
                 
                if ((!xmlStrcmp(bornChl->name, (const xmlChar*)"MONSTER")))
                {
                    int count = 0;
                    DECODE_XML_PROP_INT_DEFAULT(count, bornChl, "count",1);
                    MonsterUnit info;
                    DECODE_XML_PROP_INT_DEFAULT(info.nMonsterID, bornChl, "ID", 1);
                    DECODE_XML_PROP_INT_DEFAULT(info.nBornType, bornChl, "bornType", 0);
                    float pos[2] = {0};
                    decode_xml_prop_arr_float_default(pos, 2, bornChl, (void*)"pos", 0);
                    info.pos = CCPoint(pos[0], pos[1]);
                    DECODE_XML_PROP_INT_DEFAULT(info.fBasePosY, bornChl, "basePosY",1);
                    DECODE_XML_PROP_INT_DEFAULT(info.isRight, bornChl, "isRight", 1);
                    DECODE_XML_PROP_INT_DEFAULT(info.fBasePosX, bornChl, "clipScreenX", 0);
                    for (int k = 0; k < count; k++)
                    {
                        pVector.push_back(info);
                    }
                   
                    i++;
                }
                bornChl = bornChl->next;
            }
            stage.monsterBorn.push_back(pVector);
        }
        cur = cur->next;
    }
    pMission->missionStageVec.push_back(stage);
    
    return true;
}

bool BattleMissionConf::decodeDropItemArray(xmlNodePtr chl, char* field, std::vector<itemDrop_t>* pVec, bool hasRate)
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

bool BattleMissionConf::loadFromFile(const char* fileName, bool bEncrypt)
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
        ERROR_RETURN(("BattleMissionConf::loadFromFile xmlParseDoc mission_config.xml failded"), false);
    }
    
    xmlNodePtr cur = xmlDocGetRootElement(doc);
    if (!cur)
    {
        ERROR_LOG("BattleMissionConf::loadFromFile xmlDocGetRootElement mission_config.xml failded");
        ret = false;
        goto exit;
    }
    
    cur = cur->xmlChildrenNode;
    while (cur)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar*)"THIEF_DROP")))
        {
            xmlNodePtr chl = cur->xmlChildrenNode;
            while (chl)
            {
                if ((!xmlStrcmp(chl->name, (const xmlChar*)"THIEF_LIMIT")))
                {
                    decode_xml_prop_arr_int_default(m_ThiefConf.occurRates, ThiefConf_t::RATE_LEN, chl, (void*)"itemList", 0);
                }
                else if ((!xmlStrcmp(chl->name, (const xmlChar*)"DROP_LIMIT")))
                {
                    this->decodeDropItemArray(chl, (char*)"itemList", &(m_ThiefConf.dropChangeVec), true);
                }
				chl = chl->next;
            }
			/*for (int i = 0; i < ThiefConf_t::RATE_LEN; ++i)
			{
				DEBUG_LOG("thief occurRates[%d]=%u", i, m_ThiefConf.occurRates[i]);
			}
			float sum_rate = 0;
			for (uint32_t i = 0; i < m_ThiefConf.dropChangeVec.size(); ++i)
			{
				itemDrop_t & itm = m_ThiefConf.dropChangeVec.at(i);
				sum_rate += itm.fRate;
				DEBUG_LOG("thief drop items[%d]: %u %u %f, sum_rate=%f", i, itm.nItemID, itm.nItemCnt, itm.fRate, sum_rate);
			}*/
        }
        else if ((!xmlStrcmp(cur->name, (const xmlChar*)"MISSION")))
        {
            MissionConf_t* pMission = new MissionConf_t;
            DECODE_XML_PROP_INT(pMission->nMissionId, cur, "ID");
            
            char buff[256] = {0};
            DECODE_XML_PROP_STR(buff, cur, "name");
            pMission->strMissionName = buff;
            
            DECODE_XML_PROP_STR_DEFAULT(buff, cur, "desc", "");
            pMission->strMissionDesc = buff;
            
            DECODE_XML_PROP_INT(pMission->nMissionType, cur, "missionType");
            DECODE_XML_PROP_INT_DEFAULT(pMission->bFancy, cur, "fancy", 0);
            DECODE_XML_PROP_INT_DEFAULT(pMission->nCommunityID, cur, "communityID", 1);
            DECODE_XML_PROP_INT_DEFAULT(pMission->nNeedLevel, cur, "enterLevel", 1);
			DECODE_XML_PROP_FLOAT(pMission->fPassTime, cur, "passTime");
            DECODE_XML_PROP_FLOAT_DEFAULT(pMission->fHorizonHeight, cur, "horizonHeight", 0);
            DECODE_XML_PROP_INT_DEFAULT(pMission->winCondition.nWinType, cur, "winType", 0);
            DECODE_XML_PROP_INT_DEFAULT(pMission->winCondition.nKillCnt, cur, "killCnt", 0);
            DECODE_XML_PROP_INT_DEFAULT(pMission->bossID, cur, "bossID", 0);
            DECODE_XML_PROP_INT_DEFAULT(pMission->nScenesResourceID,cur,"scenesResourceID",0);
            DECODE_XML_PROP_INT_DEFAULT(pMission->nLine,cur,"line",0);

            float rolePos[2] = {0};
            decode_xml_prop_arr_float_default(rolePos, 2, cur, (void*)"rolePos", 0);
            pMission->fRolePosX = rolePos[0];
            pMission->fRolePosY = rolePos[1];
            
            float missionPos[2] = {0};
            decode_xml_prop_arr_float_default(missionPos, 2, cur, (void*)"missionPos", 0);
            pMission->fMissionPosX = missionPos[0];
            pMission->fMissionPosY = missionPos[1];
            
            float linePos[2] = {0};
            decode_xml_prop_arr_float_default(linePos, 2, cur, (void*)"linePos", 0);
            pMission->fLinePosX = linePos[0];
            pMission->fLinePosY = linePos[1];
            
			int costItemArr[20] = {0};
			decode_xml_prop_arr_int_default(costItemArr, 20, cur, (void*)"needItem", 0);
			int itemIdx = 0;
			pMission->costItemVec.clear();
			while (costItemArr[2 * itemIdx] != 0 || costItemArr[2 * itemIdx + 1] != 0)
			{
				ItemUnit_t item = {0};
				item.nItemID = costItemArr[2 * itemIdx];
				item.nItemCount = costItemArr[2 * itemIdx + 1];
				pMission->costItemVec.push_back(item);
				DEBUG_LOG("mission Need Item: %d %d %d", pMission->nMissionId, item.nItemID, item.nItemCount);
				itemIdx ++;
			}

			int tmp_array[4] = {0};
			decode_xml_prop_arr_int_default(tmp_array, 4, cur, (void*)"missionSore", 0);
			pMission->scoreRule.nMaxCombo = tmp_array[0];
			pMission->scoreRule.nBeHitCount = tmp_array[1];
			pMission->scoreRule.nBattleTime = tmp_array[2];
            
			DECODE_XML_PROP_INT_DEFAULT(pMission->nFirstGiftID, cur,"passGiftID", 1);

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
                        this->loadMissionBgScene(chl, &posy, &(pMission->farFileVec));
                        pMission->fFarBgPosY = posy;
                    }
                    else if (strcmp(strType, "MID") == 0)
                    {
                        float posy = 0;
                        this->loadMissionBgScene(chl, &posy, &(pMission->midFileVec));
                        pMission->fMidBgPosY = posy;
                    }
                    else if (strcmp(strType, "LAND") == 0)
                    {
                        float posy = 0;
                        this->loadMissionBgScene(chl, &posy, &(pMission->landFileVec));
                        pMission->fLandBgPosY = posy;
                    }
                    else if (strcmp(strType, "NEAR") == 0)
                    {
                        float fDefaultPosY = 0;
                        this->loadMissionBgScene(chl, &fDefaultPosY, &(pMission->nearFileVec));
                    }
                }
                else if ((!xmlStrcmp(chl->name, (const xmlChar*)"OBSTACLES")))
                {
                    this->loadObstacles(chl, pMission);
                }
                else if ((!xmlStrcmp(chl->name, (const xmlChar*)"TRAPS")))
                {
                    this->loadTraps(chl, pMission);
                }
                else if ((!xmlStrcmp(chl->name, (const xmlChar*)"PASS_PRIZE")))
                {
                    this->decodeDropItemArray(chl, (char*)"prize", &(pMission->passPrizeVec), false);
                    this->decodeDropItemArray(chl, (char*)"changePrize", &(pMission->passChangePrizeVec), true);
                }
                else if ((!xmlStrcmp(chl->name, (const xmlChar*)"FIRST_PRIZE")))
                {
                    this->decodeDropItemArray(chl, (char*)"prize", &(pMission->firstPrizeVec), false);
                    this->decodeDropItemArray(chl, (char*)"changePrize", &(pMission->firstChangePrizeVec), true);
                }
                else if ((!xmlStrcmp(chl->name, (const xmlChar*)"DROP_LIMIT")))
                {
                    this->decodeDropItemArray(chl, (char*)"itemList", &(pMission->itemDropLimitVec), false);
                }
                else if ((!xmlStrcmp(chl->name, (const xmlChar*)"MISSION_STAGE")))
                {
                    this->loadMissionStage(chl, pMission);
                }
                chl = chl->next;
            }
			MissionConfMap::value_type val(pMission->nMissionId, pMission);
			if ((m_MissionConfMap.insert(val)).second == false) {
				ERROR_LOG("mission aleady exists ID = %u", pMission->nMissionId);
				return false;
			}

			uint32_t type_cmt_id = pMission->nMissionId / MISSION_CMT_BASE_NUM;
			m_missionIDvecMap[type_cmt_id].push_back(pMission->nMissionId);
        }
		cur = cur->next;
    }
    ret = true;
exit:
    xmlFreeDoc(doc);
    return ret;
}

void BattleMissionConf::unload()
{
	MissionConfMap::iterator it = m_MissionConfMap.begin();
	while (it != m_MissionConfMap.end()) {
		delete it->second;
		++ it;
	}
}

void BattleMissionConf::getMonsterCountInfo(uint32_t nMissionID, MonsterCountInfo_t* pMonsterArray, uint32_t* pArrayCount)
{
	const MissionConf_t* pConf = getBattleMission(nMissionID);
    if (pConf)
    {
        for (int i = 0; i < (int)pConf->missionStageVec.size(); i++)
        {
            for (int j = 0; j < (int)(pConf->missionStageVec[i].monsterBorn.size()); j++)
            {
                for (int k = 0; k < (int)(pConf->missionStageVec[i].monsterBorn[j].size()); k++)
                {
                    bool isFind = false;
                    for (int n = 0; n < (int)*pArrayCount; n++)
                    {
						//DEBUG_LOG("BattleMissionConf::getMonsterCountInfo:[%d %d]", pMonsterArray[n].nMonsterID, pConf->missionStageVec[i].monsterBorn[j][k].nMonsterID);
                        if (pMonsterArray[n].nMonsterID == pConf->missionStageVec[i].monsterBorn[j][k].nMonsterID)
                        {
                            isFind = true;
                            pMonsterArray[n].nMonsterCount += 1;
                            break;
                        }
                    }
                    if (!isFind)
                    {
                        pMonsterArray[*pArrayCount].nMonsterID = pConf->missionStageVec[i].monsterBorn[j][k].nMonsterID;
                        pMonsterArray[*pArrayCount].nMonsterCount = 1;
                        *pArrayCount += 1;
                    }
                }
            }
        }
    }
}

