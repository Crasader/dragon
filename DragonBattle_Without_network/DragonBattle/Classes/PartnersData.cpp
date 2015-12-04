//
//  PartnersData.cpp
//  DragonBattle
//
//  Created by zeng xiaolong on 13-8-17.
//
//

#include "PartnersData.h"
PartnersData* PartnersData::pSharedInstace = NULL;
PartnersData::PartnersData()
{
    m_arrSkill = CCArray::create();
    CC_SAFE_RETAIN(m_arrSkill);
}

PartnersData::~PartnersData()
{
    CC_SAFE_RELEASE_NULL(m_arrSkill);
}

PartnersData* PartnersData::sharedInstance()
{
    if (!pSharedInstace)
    {
        pSharedInstace = new PartnersData();
    }
    return pSharedInstace;
}