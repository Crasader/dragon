
//
//  GlobalDefine.h
//  DragonBattle
//R
//  Created by 小敦 王 on 13-4-18.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_GlobalDefine_h
#define DragonBattle_GlobalDefine_h

#include "cocos2d.h"
#include "LocalLanguage.h"
USING_NS_CC;

#define APP_VERSION "0.2.3"

#ifndef _INT8_T
#define _INT8_T
typedef signed char           int8_t;
#endif /*_INT8_T */

#ifndef _INT16_T
#define _INT16_T
typedef short                int16_t;
#endif /* _INT16_T */

#ifndef _INT32_T
#define _INT32_T
typedef int                  int32_t;
#endif /* _INT32_T */

#ifndef _INT64_T
#define _INT64_T
typedef long long            int64_t;
#endif /* _INT64_T */

#ifndef _UINT8_T
#define _UINT8_T
typedef unsigned char         uint8_t;
#endif /*_UINT8_T */

#ifndef _UINT16_T
#define _UINT16_T
typedef unsigned short       uint16_t;
#endif /* _UINT16_T */

#ifndef _UINT32_T
#define _UINT32_T
typedef unsigned int         uint32_t;
#endif /* _UINT32_T */

#ifndef _UINT64_T
#define _UINT64_T
typedef unsigned long long   uint64_t;
#endif /* _UINT64_T */

#define IS_IPHONE_5_INCH                (SCREEN_WIDTH == 1136 && SCREEN_HEIGTH == 640)
#define JUMP_ACCELERATED_SPEED_Y        (Global::_jump_accelerated_speed_y)
#define ROLE_RUNNING_LEFT_CLIP_POS_X    (Global::_role_running_left_clip_pos_x)
#define ROLE_RUNNING_RIGHT_CLIP_POS_X   (Global::_role_running_right_clip_pos_x)
#define ROLE_LEFT_CLIP_POS_X            (Global::_role_left_clip_pos_x)
#define ROLE_RIGHT_CLIP_POS_X           (Global::_role_right_clip_pos_x)
#define SCREEN_WIDTH                (Global::_screen_width)
#define SCREEN_HEIGTH               (Global::_screen_heigth)
#define SCREEN_MID_POS              (Global::_screen_middle)
#define STR_CHINESE_NUMERAL         (Global::_str_chinese_numerral)
#define STR_LOADING_WORDS           (Global::_str_loading_words)
#define RAND_STR_LOADING_WORDS      (STR_LOADING_WORDS[rand() % 21])

#define ANIMAT_DELAY_PER_UNIT     (1.0 / 35)
#define TUTORIAL_MISSION (90001)
#define ISTUTORIAMISSION(missionID) (missionID == TUTORIAL_MISSION)//新手任务
#define FONTNAME "Arial-BoldMT"
// font colors
#define FONT_COLOR_BLUE         ccc3(0,255,255)
#define FONT_COLOR_GREEN        ccc3(150,255,0)
#define FONT_COLOR_RED          ccc3(255,0,0)
#define FONT_COLOR_GRAY         ccc3(43,16,5)
#define FONT_COLOR_BROWN        ccc3(242,190,106)
#define FONT_COLOR_BROWN_NEW    ccc3(74.0f ,50.0f ,19.0f)
#define FONT_COLOR_BROWN_DARK   ccc3(38.0f, 21.0f, 38.0f)
#define FONT_COLOR_GOLD         ccc3(255,250,0)
#define FONT_COLOR_LIGHT_GREEN  ccc3(144, 238, 144)
#define FONT_COLOR_SILVER       ccc3(230, 232, 250)
#define FONT_COLOR_DARD_PURPLE  ccc3(135, 31, 120)
#define FONT_COLOR_ORANGE  ccc3(250, 164, 42)
#define GLOBAL_MESSAGE_LIMIT (50)
#define FRIEND_MESSAGE_LIMIT (50)

#define MONEY_ICON_RES ("item_0.png")
#define EXP_ICON_RES ("item_1.png")
#define GOLD_ICON_RES ("item_2.png")
#define POWER_ICON_RES ("item_3.png")
#define NERVE_ICON_RES ("item_4.png")
#define ENERGY_ICON_RES ("item_5.png")
#define PRESTIGE_ICON_RES ("item_6.png")

static ccColor3B ITEM_QUARTY_COLOR[6] = {ccc3(0,0,0), ccWHITE, ccc3(62,181,62),ccc3(42,125,238),ccc3(141,42,223),ccc3(250,126,10)};
//static int ITEM_QUARTY_EFFECT[6] = {0, 37, 38, 39, 40, 41};

// 全局属性定义
class Global
{
public:
    static void initGlobalParams(float width,float heigh);
    
public:
    static float _jump_accelerated_speed_y;
    static float _role_running_left_clip_pos_x;
    static float _role_running_right_clip_pos_x;
    static float _role_left_clip_pos_x;
    static float _role_right_clip_pos_x;
    
    static float _screen_width;
    static float _screen_heigth;
    static CCPoint _screen_middle;
    
    static const char* _str_chinese_numerral[13];
    static const char* _str_loading_words[21];
};

// 玩家角色类型
enum kRoleType
{
    kRoleSword = 1, //弈剑
    kRoleSpear,     //岳枪
    kRoleFlower,    //花魁
};
// 关卡类型
enum kBattleType
{
    kBattle = 1,//普通关卡
    kArena,     //竞技场
    kEliteBoss, //精英boss
    kTowerBoss, //爬塔
    kBook,       //藏经阁
    kKeap,      //拦截
    kWorldBoss, //世界BOSS
};

enum kRoleDirectionType
{
    kNone = 1,
    kLeft,
    kRight,
};

enum kObstacleDirection 
{
    kObstacleNone =     0x00000000,
    kObstacleUp =       0x00000001,
    kObstacleDown =     0x00000010,
    kObstacleLeft =     0x00000100,
    kObstacleRight =    0x00001000,
    kObstacleUpLeft =   0x00000101,
    kObstacleDownLeft = 0x00000110,
    kObstacleUpRight =  0x00001001,
    kObstacleDownRight =0x00001010,
};

enum kBattleState
{
    kBattleing = 1,
    kBattleWin,
    kBattleLose,
};

enum kRoleActionType
{
    kActionNull,
    kStandby = 1,       //待机
    kRunning = 2,       //移动
    kJumpUp = 3,        //起跳
    kJumpInAir = 4,     //空中
    kJumpDown = 5,      //落地
    kSkillOverDown = 6, //技能空中下落
    kAirAttackBegin = 7,//空中攻击起招
    kAirAttackValid = 8,//空中攻击有效
    kAirAttackOver = 9, //空中攻击收招
    kAttackBegin = 10,   //普通攻击起招
    kAttackValid = 11,   //普通攻击有效
    kAttackOver = 12,    //普通攻击收招
    kSkillBegin = 13,    //技能攻击起招
    kSkillValid = 14,    //技能攻击有效
    kSkillOver = 15,     //技能攻击收招
    kBeHit = 16,         //普通受击
    kBeHitFly = 17,      //击飞
    kBeHitDown = 18,     //倒地
    kGetUp = 19,         //倒地起身
    kWin = 20,           //胜利动作
    kLose = 21,          //失败动作
    kRoll = 22           //翻滚动作
};

#define IS_ATTACK_ACTION(action) ((action) == kAttackBegin || (action) == kAttackValid || (action) == kAttackOver)
#define IS_AIRATTACK_ACTION(action) ((action) == kAirAttackBegin || (action) == kAirAttackValid || (action) == kAirAttackOver)
#define IS_SKILL_ACTION(action) ((action) == kSkillBegin || (action) == kSkillValid || (action) == kSkillOver || (action) == kSkillOverDown)
#define IS_JUMPING_ACTION(action) ((action) == kJumpUp || (action) == kJumpInAir || (action) == kJumpDown)

enum kMonsterType
{
    kHero = 0,                  //英雄
    kNeutralMonster = 1,        //中立怪，杂碎，不用杀死即可过关
    kNormalMonster = 2,         //普通怪物，
    kFollowMonster = 3,         //随从怪物
    kBoss = 4,                  //boss
    kBox = 5,
    kFire = 6,
    kPartnerType = 7,
    kEnemyHero = 8,
    kThief = 9,                 //小偷
    kBombBox = 10,
    kTree = 11                  //摇钱树
};



// 相对英雄来言
enum kUnitFightType
{
    kFightTypeNeutral,   //中立
    kFightTypeFriend,    //友方
    kFightTypeEnemy,     //敌方
};


// 删除std::vector中的指针对象
#define SAFE_DEL_VECTOR(vecType, pVec)\
    if (pVec)\
    {\
        if (!(pVec)->empty())\
        {\
        vecType::iterator it;\
            for (it = (pVec)->begin(); it != (pVec)->end();)\
            {\
                if (*it)\
                {\
                    delete *it;\
                    *it = NULL;\
                    (pVec)->erase(it);\
                }\
                else\
                {\
                    ++it;\
                }\
            }\
        }\
        (pVec)->clear();\
    }

// 单例类设置
#define C_SHARE_INSTANCE(className) \
private:\
    static className* p##className;\
public:\
    static className* share##className(){\
        if(p##className==0){\
            p##className=new className();\
            p##className->init();\
        }\
        return p##className;\
    }\
public:\
    static void release##className(){\
        if(p##className){delete p##className;p##className=0;}\
    }

#define MAX_USERID  10000000
#define MIN_USERID  100000
#define isNpcID(userid)         ((userid) >= 1 && (userid) <= 1000)
#define isInvalidID(userid)     ((userid) <= 0 || (userid) > MAX_USERID || (!isNpcID(userid) && (userid) < MIN_USERID))
#define isValidID(userid)       ((userid) >= MIN_USERID && (userid) <= MAX_USERID)

#define EPSILON (0.000001)
#define IS_FLOAT_ZERO(v) (fabs(v) < EPSILON) 

#define GET_KEY_WITH_USER(userid, key)  (CCString::createWithFormat("%d_%d_%s", gUserData->server_id, (userid), (key))->getCString())


#define CCLOCALSTRING(key) (LocalLanguage::getLocalCCString(key))
#define XYLOCALSTRING(key) (LocalLanguage::getLocalString(key))

//#define Skip_Tutorial           // 关闭引导
#define NOT_SKIP_FUNC_OPEN (1)  // 0：全部打开功能 1：按引导开启功能


#endif
