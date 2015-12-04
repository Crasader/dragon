//
//  BattleCacheManager.h
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-24.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_BattleCacheManager_h
#define DragonBattle_BattleCacheManager_h

#include "cocos2d.h"
#include "UnitResource.h"

class BattleCacheManager
{
public:
    BattleCacheManager();
    ~BattleCacheManager();
    
    static BattleCacheManager* sharedInstance();
    void loadResourcesBeforBattle(int missionID);
    void loadUnitResource(int nReourceID,uint32_t *skill);
    void loadPartnerResource(int nReourceID);
    void getResourcesVec(std::vector<std::string>* pResourceVec);
private:
    void loadHeroSkillResource(UnitResource_t *source,int skillInfo);
    static BattleCacheManager* pSharedInstance;
    std::set<std::string> m_LoadedFileNames;
    std::set<std::string> m_MonsterFileNames;
    std::set<std::string> m_ParnterFileNames;
    uint32_t m_nCurMissionID;   //当前所在的战斗场景ID
    uint32_t m_nCurCommunityID; //当前所在的主城ID
};

#endif
