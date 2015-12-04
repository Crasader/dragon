//
//  PrestigeChallengeConf.h
//  DragonBattle
//
//  Created by 小敦 王 on 13-8-28.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_PrestigeChallengeConf__h
#define DragonBattle_PrestigeChallengeConf__h

#include "XmlEncrypt.h"
#include <iostream>

#define MAX_CHALLENGE_CONF_CNT 50

struct PrestigeChallengeConf_t
{
	uint32_t nTitleID;
    char     titleName[16];
    uint32_t nNeedPrestige;
    uint32_t nNeedLevel;
    uint32_t nNeedYxb;
    uint32_t nGiftID;
    uint32_t nMissionID;
};

class PrestigeChallengeConfig
{
public:
    PrestigeChallengeConfig();
    ~PrestigeChallengeConfig();
    
    bool loadFromFile(char* filePath, bool bEncrypt = false);
    void unload();
    
    const PrestigeChallengeConf_t* getPrestigeChallenge(uint32_t nTitleID);
    int getPrestigeTitleCount() { return m_nMaxPrestigeTitle; }

    const PrestigeChallengeConf_t* getPrestigeChallengeByMissionID(uint32_t nMissionID);
private:
    uint32_t                m_nMaxPrestigeTitle;
    PrestigeChallengeConf_t m_challengeConf[MAX_CHALLENGE_CONF_CNT + 1];
};

#endif
