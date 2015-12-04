//
//  RoleNerveConf.h
//  DragonBattle
//
//  Created by 小敦 王 on 13-7-9.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_RoleNerveConf_h
#define DragonBattle_RoleNerveConf_h

#include "XmlEncrypt.h"
#include <iostream>

#include "ItemConf.h"

#define MAX_NERVE_LV 640 //最大经脉等级

struct RoleNerveConf_t
{
    uint32_t nNerveID;
    char     nerveName[16];
    uint32_t nNeedAir;
    uint32_t nRate;
    uint32_t nEffectType;
    uint32_t nEffectValue;
    uint32_t nCoolDown;
    uint32_t nNeedLevel;
    uint32_t nAttrValue[EQUIP_ATTR_MAX + 1];
};

class RoleNerveConfig
{
public:
    RoleNerveConfig();
    ~RoleNerveConfig();
    
    bool loadFromFile(const char* filePath, bool bEncrypt = false);
	void unload();
    
	const RoleNerveConf_t* getRoleNerve(uint32_t nNerveID) const;
    
private:
	bool load_one_nerve_config(const xmlNodePtr& cur, uint32_t i);

    RoleNerveConf_t m_pRoleNerveConf[MAX_NERVE_LV + 1];
};

#endif
