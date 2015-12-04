//
//  DealErrorCode.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-7-11.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "DealErrorCode.h"
#include "error_code.h"
#include "XYTopLayer.h"

void  DealErrorCode::dealErrorFunc(int errorCode)
{
#define SHOW_ERROR_TIP_BEGIN()    switch (errorCode) {
#define SHOW_ERROR_TIP(nError, msg)  case (nError):XYTopLayer::getInstance()->ShowTip(msg);break;
#define SHOW_ERROR_TIP_END()      }
#define SHOW_DEFAULT_ERR_TIP()    default:{char buf[64] = {0};sprintf(buf, "系统出错!");\
                XYTopLayer::getInstance()->ShowTip(buf);break;}

    SHOW_ERROR_TIP_BEGIN()
        SHOW_ERROR_TIP(cli_system_error, "系统错误");
        SHOW_ERROR_TIP(cli_message_in_error, "客户端发送的数据不合法");
        SHOW_ERROR_TIP(cli_db_data_error, "数据出错!");
        SHOW_ERROR_TIP(cli_game_config_error, "游戏配置错误!");
        SHOW_ERROR_TIP(cli_not_enough_level_error, "等级不足!");
        SHOW_ERROR_TIP(cli_max_level_error, "等级已经到顶了!");
        SHOW_ERROR_TIP(cli_not_enough_money_error, "银两不够!");
        SHOW_ERROR_TIP(cli_not_enough_gold_error, "黄金不足!");
        SHOW_ERROR_TIP(cli_not_enough_power_error, "内功值不够!");
        SHOW_ERROR_TIP(cli_not_enough_nerve_error, "真气值不够!");
        SHOW_ERROR_TIP(cli_not_enough_package_size, "您的背包满了!");
        SHOW_ERROR_TIP(cli_not_enough_vip_level_error, "VIP等级不足!");
        SHOW_ERROR_TIP(cli_not_enough_enery_error, "体力值不够!");
        SHOW_ERROR_TIP(cli_enery_is_full_error, "体力值已满!");
        SHOW_ERROR_TIP(cli_package_size_limit_error, "背包已达上限!");
        SHOW_ERROR_TIP(cli_enter_mission_error, "进入战斗错误!");
        SHOW_ERROR_TIP(cli_auto_battle_error,"扫图出错!");
        SHOW_ERROR_TIP(cli_check_battle_over_error, "战斗错误!");
        SHOW_ERROR_TIP(cli_system_busy_error, "系统繁忙!");
        SHOW_ERROR_TIP(cli_not_enough_item_error, "物品数量不足!");
        SHOW_ERROR_TIP(cli_enter_mission_without_item_err, "进入关卡所需条件不足!");
        SHOW_ERROR_TIP(cli_not_enough_prestige_error, "您的声望不够!");
        SHOW_ERROR_TIP(cli_user_has_got_prize_error, "已经领取过奖励了!");
        SHOW_ERROR_TIP(cli_user_times_limit_error, "次数上限已到!");
        SHOW_ERROR_TIP(cli_multiple_login_err, "用户重复登录!");
        SHOW_ERROR_TIP(cli_date_limit_error, "日期错误!");
        SHOW_ERROR_TIP(cli_daily_limit_error, "兑换限制错误!");
        SHOW_ERROR_TIP(cli_has_accept_task_error, "任务已接!");
        SHOW_ERROR_TIP(cli_has_not_accept_task_error, "任务没有接取!");
        SHOW_ERROR_TIP(cli_cooldown_time_error, "冷却时间没到!");
        SHOW_ERROR_TIP(cli_equipment_profess_error, "装备职业不符!");
        SHOW_ERROR_TIP(cli_max_package_size_error, "背包容量已达上限！");
        SHOW_ERROR_TIP(cli_shop_count_limit_error, "超过购买限制数量");
        SHOW_ERROR_TIP(cli_activity_not_time, "不在活动时间!");
        SHOW_ERROR_TIP(cli_had_partner_error, "已经拥有了这个弟子，不能再次携带");
        SHOW_ERROR_TIP(cli_partner_count_limit_error, "携带弟子数到达上限");
        SHOW_ERROR_TIP(cli_user_partner_card_used_error, "弟子已经被携带！");
        SHOW_ERROR_TIP(cli_boss_battle_had_reseted, "重置次数已用完！");
        SHOW_ERROR_TIP(cli_user_fight_not_enough_error, "战力值不足!");
        SHOW_ERROR_TIP(cli_user_transform_star_error, "没有您选择可转化的星石!");
        SHOW_ERROR_TIP(cli_user_escort_keapeding_error, "正在被拦截中!");
        SHOW_ERROR_TIP(cli_user_invite_exists_err, "好友邀请码已经设置过了!");
        SHOW_ERROR_TIP(cli_user_invite_not_exist_error, "好友邀请码不存在!");
        SHOW_ERROR_TIP(cli_function_not_open_err, "功能未开启!");
        SHOW_ERROR_TIP(cli_pre_mission_not_pass_err, "前置关卡没有完成!");

        SHOW_ERROR_TIP(SVR_NET_ERR, "你的网络连接已断开，请重新连接!");
        SHOW_ERROR_TIP(SVR_REGISTER_SAME_NAME_ERR, "此用户名已注册!");
        SHOW_ERROR_TIP(SVR_LOGIN_NO_FIND_USER_ERR, "未找到该用户名!");
        SHOW_ERROR_TIP(SVR_CHECK_PASSWD_ERR, "用户名或密码出错!");
        SHOW_ERROR_TIP(SVR_USER_NOT_ENABLED_ERR, "用户已被冻结!");
        SHOW_ERROR_TIP(USER_ADD_FRIEND_EXIST_ERR, "已添加过该好友!");
        SHOW_ERROR_TIP(USER_INVITE_EXISTS_ERR, "好友邀请码已经设置过了!");
        
        SHOW_DEFAULT_ERR_TIP();
    SHOW_ERROR_TIP_END()
}
