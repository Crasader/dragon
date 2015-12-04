//
//  EquipAttribute.cpp
//  DragonBattle
//
//  Created by 何 永翔 on 13-6-25.
//
//

#include "LibAttrCal.h"

using namespace attrcal;

std::string EquipAttribute::getQualityName(uint32_t nLevel)
{
    int nQuality = (nLevel % 10 == 0) ? (nLevel / 10 - 1) : (nLevel / 10);
    int nGrade = (nLevel % 10 == 0) ? 10 : (nLevel % 10);
    std::string str;
    switch (nQuality) {
        case 0:
            str = "劣品";
            break;
        case 1:
            str = "下品";
            break;
        case 2:
            str = "中品";
            break;
        case 3:
            str = "上品";
            break;
        case 4:
            str = "绝品";
            break;
        case 5:
            str = "极品";
            break;
        case 6:
            str = "巨品";
            break;
        case 7:
            str = "青铜";
            break;
        case 8:
            str = "黑铁";
            break;
        case 9:
            str = "白银";
            break;
        case 10:
            str = "黄金";
            break;
        case 11:
            str = "暗金";
            break;
        case 12:
            str = "红玉";
            break;
        case 13:
            str = "灵器";
            break;
        case 14:
            str = "宝器";
            break;
        case 15:
            str = "侠器";
            break;
        case 16:
            str = "玄武";
            break;
        case 17:
            str = "朱雀";
            break;
        case 18:
            str = "白虎";
            break;
        case 19:
            str = "青龙";
            break;
        case 20:
            str = "玛瑙";
            break;
        case 21:
            str = "翡翠";
            break;
        case 22:
            str = "钻石";
            break;
        case 23:
            str = "天地";
            break;
        case 24:
            str = "玄黄";
            break;
        default:
            str = "";
    }
    char str1[128] = {0};
    sprintf(str1, "%d级", nGrade);
    str.append(str1);
    return str;
}

std::string EquipAttribute::getTypeName(EquipAttributeType attrType)
{
    switch (attrType)
    {
        case EQUIP_ATTR_NONE:
            return "";
            break;
        case OUT_ATTACK:
            return "外攻";
            break;
        case IN_ATTACK:
            return "内攻";
            break;
        case OUT_DEFENCE:
            return "外防";
            break;
        case IN_DEFENCE:
            return "内防";
            break;
        case HP:
            return "血量";
            break;
        case CRIT:
            return "暴击";
            break;
        case TOUGHNESS:
            return "韧性";
            break;
        case REDUCE_HURT:
            return "减伤";
            break;
        case ADD_HURT:
            return "加伤";
            break;
        case RECOVER_HP:
            return "回血";
            break;
        default:
            return "";
    }
}
