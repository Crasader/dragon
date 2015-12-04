
//
//  GlobalDefine.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-18.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "GlobalDefine.h"
#include "IOSUtility.h"

#include "cocos2d.h"
USING_NS_CC;

float Global::_jump_accelerated_speed_y;
float Global::_role_running_left_clip_pos_x;
float Global::_role_running_right_clip_pos_x;
float Global::_role_left_clip_pos_x;
float Global::_role_right_clip_pos_x;

float Global::_screen_width;
float Global::_screen_heigth;
CCPoint Global::_screen_middle;

const char* Global::_str_chinese_numerral[13];
const char* Global::_str_loading_words[21];

void Global::initGlobalParams(float width,float heigh)
{
    _jump_accelerated_speed_y = -0.7;
    _role_running_left_clip_pos_x = 160;        
    _role_running_right_clip_pos_x = width/2;
    _role_left_clip_pos_x = 80;
    _role_right_clip_pos_x = width - 80;
    
    _screen_width = width;
    _screen_heigth = heigh;
    _screen_middle = ccp(_screen_width/2,_screen_heigth/2);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
    
    _str_chinese_numerral[0] = (char*)"0";
    _str_chinese_numerral[1] = (char*)"一";
    _str_chinese_numerral[2] = (char*)"二";
    _str_chinese_numerral[3] = (char*)"三";
    _str_chinese_numerral[4] = (char*)"四";
    _str_chinese_numerral[5] = (char*)"五";
    _str_chinese_numerral[6] = (char*)"六";
    _str_chinese_numerral[7] = (char*)"七";
    _str_chinese_numerral[8] = (char*)"八";
    _str_chinese_numerral[9] = (char*)"九";
    _str_chinese_numerral[10] = (char*)"十";
    _str_chinese_numerral[11] = (char*)"十一";
    _str_chinese_numerral[12] = (char*)"十二";
    
    _str_loading_words[0] = "打孔石可以开启锁定的宝石镶嵌孔";
    _str_loading_words[1] = "合成符可以合成更高级的宝石";
    _str_loading_words[2] = "装备洗练可以获得额外的附加属性";
    _str_loading_words[3] = "收集副本掉落的材料可以进阶更高级的装备";
    _str_loading_words[4] = "强化装备等级可以增强战斗力";
    _str_loading_words[5] = "悬赏任务可以获得大量经验";
    _str_loading_words[6] = "参拜神仙姐姐有机会获得冰火套装";
    _str_loading_words[7] = "农田可以产出大量银两，内功，真气";
    _str_loading_words[8] = "通天塔有机会获得镶嵌所需的宝石";
    _str_loading_words[9] = "摇钱树每天可以获得大量银两";
    _str_loading_words[10] = "击杀世界boss可以获得大量银两和声望";
    _str_loading_words[11] = "占星获得的星石可以提升个人属性";
    _str_loading_words[12] = "主角和侠客的装备及属性可在人物面板中查看";
    _str_loading_words[13] = "点击任务图标可以查看当前所有任务";
    _str_loading_words[14] = "成功通关副本后就能使用扫荡功能了";
    _str_loading_words[15] = "在线奖励非常丰富，要保持在线哟";
    _str_loading_words[16] = "提升等级增加战斗力并升级技能";
    _str_loading_words[17] = "打通经脉可以提升自身属性";
    _str_loading_words[18] = "提高声望可以招募强力弟子哦";
    _str_loading_words[19] = "精英副本有概率掉落装备合成卷轴";
    _str_loading_words[20] = "竞技场排名越前，每天所能领取的奖励越多哦";
}