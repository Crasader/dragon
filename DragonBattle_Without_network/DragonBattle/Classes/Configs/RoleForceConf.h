//
//  RoleForceConf.h
//  DragonBattle
//
//  Created by 小敦 王 on 13-7-10.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_RoleForceConf_h
#define DragonBattle_RoleForceConf_h
#include "XmlEncrypt.h"
#include <iostream>

struct RoleForceConf_t
{
    uint32_t nForceID;
    uint32_t nNeedExp;
    uint32_t nLife;
    uint32_t nOutAttack;
    uint32_t nOutDef;
    uint32_t nInAttack;
    uint32_t nInDef;
    uint32_t nCrit;
    uint32_t nToughness;
    uint32_t nReduceHurt;
    uint32_t nAddHurt;
    uint32_t nRecoverHP;
};

class RoleForceConfig
{
public:
    RoleForceConfig();
    ~RoleForceConfig();
    
    bool loadFromFile(const char* filePath, bool bEncrypt = false);
	void unload();
    
    // 根据炼气经验得到等级
    int getRoleForceLevel(uint32_t nForceExp);
    // 当前等级增加的所有属性
    const RoleForceConf_t* getRoleForceSum(uint32_t nForceLevel);
    // 当前等级增加的属性
    RoleForceConf_t* getRoleForceConf(uint32_t nForceLevel, RoleForceConf_t* pConf);
        
private:
    uint32_t m_nMaxLevel;
    RoleForceConf_t m_roleForceConf[101];
};

#endif
