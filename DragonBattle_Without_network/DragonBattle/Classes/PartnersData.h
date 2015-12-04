//
//  PartnersData.h
//  DragonBattle
//
//  Created by zeng xiaolong on 13-8-17.
//
//

#ifndef __DragonBattle__PartnersData__
#define __DragonBattle__PartnersData__

#include "UserData.h"
#include "cocos2d.h"
USING_NS_CC;

class PartnersData:public CCObject
{
public:
    PartnersData();
    ~PartnersData();
    static PartnersData* sharedInstance();
public:
    CCArray* m_arrSkill;
private:
    static PartnersData* pSharedInstace;
};
#define gPartnersData (PartnersData::sharedInstance())

#endif /* defined(__DragonBattle__PartnersData__) */
