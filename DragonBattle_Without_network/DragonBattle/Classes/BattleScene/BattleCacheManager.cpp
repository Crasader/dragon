//
//  BattleCacheManager.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-24.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "BattleCacheManager.h"
#include "GameData.h"
#include "UserData.h"
USING_NS_CC;

BattleCacheManager::BattleCacheManager()
{
    m_LoadedFileNames.clear();
    m_MonsterFileNames.clear();
}

BattleCacheManager::~BattleCacheManager()
{

}
BattleCacheManager* BattleCacheManager::pSharedInstance = NULL;
BattleCacheManager* BattleCacheManager::sharedInstance()
{
    if (!pSharedInstance)
    {
        pSharedInstance = new BattleCacheManager();
    }
    return pSharedInstance;
}

void BattleCacheManager::loadResourcesBeforBattle(int missionID)
{
    m_ParnterFileNames.clear();
    m_LoadedFileNames.clear();
    m_MonsterFileNames.clear();
    //沙盒读取文件
    m_LoadedFileNames.insert("action/explode.plist");
    m_LoadedFileNames.insert("action/AttackHit.plist");
    m_LoadedFileNames.insert("action/AttackFlash.plist");
    m_LoadedFileNames.insert("action/born.plist");
    MissionConf_t* conf = GameData::getBattleMission(missionID);
    //场景资源加载
    SceneConf_t *sceneConf = GameData::getSceneConf(conf->nScenesResourceID);
    for (int i = 0; i<sceneConf->fileVec.size(); i++)
    {
        string str = "BattleScene/" + sceneConf->fileVec.at(i);
        m_LoadedFileNames.insert(str);
    }
    //小偷资源
    UnitResource_t *source = UnitResource::sharedInstance()->getUnitResConf(1006);
    for (int z=0; z<source->fileResourceVec.size(); z++)
    {
        std::string strResource = source->fileResourceVec.at(z);
        if (m_LoadedFileNames.find(strResource) == m_LoadedFileNames.end())
        {
            m_LoadedFileNames.insert(strResource);
        }
        
        if (m_MonsterFileNames.find(strResource) == m_MonsterFileNames.end())
        {
            m_MonsterFileNames.insert(strResource);
        }
    }
    //怪物、陷阱资源加载
    for (int i = 0;i<conf->missionStageVec.size() ; i++)
    {
        for (TrapUnitVec::iterator it = conf->trapUnitVec.begin(); it != conf->trapUnitVec.end(); it++)//陷阱
        {
            UnitResource_t *source = UnitResource::sharedInstance()->getUnitResConf(it->nResourceID);
            for (int z=0; z<source->fileResourceVec.size(); z++)
            {
                std::string strResource = source->fileResourceVec.at(z);
                if (m_LoadedFileNames.find(strResource) == m_LoadedFileNames.end())
                {
                    m_LoadedFileNames.insert(strResource);
                }
                
                if (m_MonsterFileNames.find(strResource) == m_MonsterFileNames.end())
                {
                    m_MonsterFileNames.insert(strResource);
                }
            }
        }
        //怪物
        MissionStage_t stageConf = conf->missionStageVec.at(i);
        for (int j = 0; j<stageConf.monsterBorn.size(); j++)
        {
            MonsterBornInfoVec bornVec = stageConf.monsterBorn.at(j);
            for (int k = 0; k<bornVec.size(); k++)
            {
                MonsterUnit unit = bornVec.at(k);
                MonsterUnitConf* unitConf = GameData::getMonsterConf(unit.nMonsterID);
                UnitResource_t *source = UnitResource::sharedInstance()->getUnitResConf(unitConf->nResourceID);
                for (int z=0; z<source->fileResourceVec.size(); z++)
                {
                    std::string strResource = source->fileResourceVec.at(z);
                    if (m_LoadedFileNames.find(strResource) == m_LoadedFileNames.end())
                    {
                        m_LoadedFileNames.insert(strResource);
                    }
                    
                    if (m_MonsterFileNames.find(strResource) == m_MonsterFileNames.end())
                    {
                        m_MonsterFileNames.insert(strResource);
                    }
                }
            }
        }
    }
    
    gUserData->m_heroData->nUnitID = 1;
    
    if (true /*ISTUTORIAMISSION(missionID)*/)//新手任务
    {
        gUserData->m_heroData->skill[0] = (gUserData->m_heroData->nUnitID*1000+1)*1000 + 1;
        gUserData->m_heroData->skill[1] = (gUserData->m_heroData->nUnitID*1000+2)*1000 + 1;
        gUserData->m_heroData->skill[2] = (gUserData->m_heroData->nUnitID*1000+3)*1000 + 1;
        gUserData->m_heroData->skill[3] = (gUserData->m_heroData->nUnitID*1000+4)*1000 + 1;
    }
    
    
    
    loadUnitResource(gUserData->m_heroData->nUnitID,gUserData->m_heroData->skill);
}

void BattleCacheManager::loadUnitResource(int nReourceID,uint32_t *skill)
{
    UnitResource_t *source = UnitResource::sharedInstance()->getUnitResConf(nReourceID);
    for (int z=0; z<source->fileResourceVec.size(); z++)
    {
        std::string strResource = source->fileResourceVec.at(z);
        if (m_LoadedFileNames.find(strResource) == m_LoadedFileNames.end())
        {
            m_LoadedFileNames.insert(strResource);
        }
    }
    
    for (int i = 0; i<4; i++)
    {
        loadHeroSkillResource(source,skill[i]);
    }
}

void BattleCacheManager::loadHeroSkillResource(UnitResource_t *source,int skillInfo)
{
    SkillInfo_t* skill = source->skillMap[GET_SKILL_ID(skillInfo)];
    if (skill)
    {
        for (int z=0; z<skill->fileResourceVec.size(); z++)
        {
            std::string strResource = skill->fileResourceVec.at(z);
            if (m_LoadedFileNames.find(strResource) == m_LoadedFileNames.end())
            {
                m_LoadedFileNames.insert(strResource);
            }
        }
    }
}

void BattleCacheManager::loadPartnerResource(int nReourceID)
{
    if (m_ParnterFileNames.size()>0)
    {
        for (std::set<std::string>::iterator it = m_ParnterFileNames.begin(); it != m_ParnterFileNames.end(); it++)
        {
            string strResource = it->data();
            printf("%s\n",strResource.data());
            CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(strResource.c_str());
        }
    }
    m_ParnterFileNames.clear();
    
    UnitResource_t *source = UnitResource::sharedInstance()->getUnitResConf(nReourceID);
    for (int z=0; z<source->fileResourceVec.size(); z++)
    {
        std::string strResource = source->fileResourceVec.at(z);
        if (m_LoadedFileNames.find(strResource) == m_LoadedFileNames.end())
        {
            m_ParnterFileNames.insert(strResource);
            printf("%s\n",strResource.data());
            CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(strResource.c_str());
        }
    }
}

void BattleCacheManager::getResourcesVec(std::vector<std::string>* pResourceVec)
{
    for (std::set<std::string>::iterator it = m_LoadedFileNames.begin(); it != m_LoadedFileNames.end(); it++)
    {
        string str = it->data();
        printf("%s\n",str.data());
        pResourceVec->push_back(str);
    }
}