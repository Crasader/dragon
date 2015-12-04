//
//  MissionAttribute.cpp
//  DragonBattle
//
//  Created by 何 永翔 on 13-6-25.
//
//

#include "MissionAttribute.h"

using namespace attrcal;

int MissionAttribute::getMissionExp(uint16_t nID)
{
    //每次过关奖励=(B2*5+5)*(60+B2)/10
    int missionExp = (nID * 5 + 5) * (60 + nID) / 10;
    //杀怪奖励=(B2*1+1)
    int monsterExp = nID * 1 + 1;
    //BOSS奖励=(B2*5+5)
    int bossExp = nID * 5 + 5;
    return missionExp + monsterExp + bossExp;
}

int MissionAttribute::getMissionFirstExp(uint16_t nID)
{
    //首次过关奖励=(B2*5+5)*(60+B2)/10
    return (nID * 5 + 5) * (60 + nID) / 10;
}
