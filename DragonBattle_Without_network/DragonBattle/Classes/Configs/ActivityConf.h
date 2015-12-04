//
//  ActivityConf.h
//  DragonBattle
//
//  Created by 白明江 on 11/18/13.
//
//

#ifndef __DragonBattle__ActivityConf__
#define __DragonBattle__ActivityConf__

#include <iostream>
#include <map>
#include <vector>

#include "XmlEncrypt.h"

struct StartEndDate_t
{
    uint32_t nStartDate;
    uint32_t nEndDate;
};

struct StartEndTime_t
{
    uint32_t nStartTime;
    uint32_t nEndTime;
};

struct ActivityConf_t
{
    uint32_t activity_id;
    uint32_t need_lv;
    uint32_t need_vip;
    
#ifndef WONPEE_SERVER
    char name[32];
    char desc[512];
    std::vector<uint32_t> prizes;
#endif
    
    std::vector<StartEndDate_t> date_limit;
    std::vector<StartEndTime_t> time_limit_vec;
};

typedef std::map<uint32_t, ActivityConf_t*> ActivityMap;

class ActivityConf
{
public:
    ActivityConf();
    ~ActivityConf();
    
    bool loadFromFile(char* filePath, bool bEncrypt = false);
    void unload();
    
    void decodeLimitDate(xmlNodePtr chl, char* field, std::vector<StartEndDate_t>* limit_date);
    void decodeLimitTime(xmlNodePtr cur, char* field, std::vector<StartEndTime_t>* limitTime);
    void decodePrizes(xmlNodePtr cur, char* field, std::vector<uint32_t>* prizes);
    
    const ActivityConf_t* getActivityByID(int activity_id) const;

    ActivityMap m_ActivityMap;
};

#endif /* defined(__DragonBattle__ActivityConf__) */
