//
//  MissionAttribute.h
//  DragonBattle
//
//  Created by 何 永翔 on 13-6-25.
//
//

#ifndef DragonBattle_MissionAttribute_h
#define DragonBattle_MissionAttribute_h

#include <stdint.h>

namespace attrcal
{
    class MissionAttribute
    {
    public:
        //得到关卡经验
        static int getMissionExp(uint16_t nID);
        //得到关卡第一次通过经验
        static int getMissionFirstExp(uint16_t nID);
    };
}

#endif
