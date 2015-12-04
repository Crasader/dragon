//
//  EquipAttribute.h
//  DragonBattle
//
//  Created by zeng xiaolong on 13-6-14.
//
//

#ifndef DragonBattle_EquipAttribute_h
#define DragonBattle_EquipAttribute_h
#include "GameData.h"
#include "ItemConf.h"

namespace attrcal
{
    class EquipAttribute
    {
    public:
        //得到品级的中文名
        static std::string getQualityName(uint32_t nLevel);
        //得到类型中文名
        static std::string getTypeName(EquipAttributeType attrType);
    };
}

#endif
