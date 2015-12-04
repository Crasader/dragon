#ifndef PROJECT_ERROR_CODE_H__
#define PROJECT_ERROR_CODE_H__

enum cli_err_code {
	cli_critical_err		= 10001,
	cli_net_unconnect_err 	= 10002,	//svr连接出错
	cli_user_hadnot_login_to_online_err = 10003,//用户还没有登陆online
	cli_user_had_login_to_online_err = 10004,	//用户已经等陆online
	cli_login_session_error	= 10005,	//登陆session出错
	cli_system_error		= 10006,	//系统错误
	cli_message_in_error	= 10007,	//客户端发送的数据不合法
	cli_not_enough_money_error = 10008,	//游戏币不够
	cli_db_data_error		= 10009,	//数据出错
	cli_game_config_error	= 10010,	//游戏配置错误
	cli_not_enough_level_error = 10011,	//等级不够
	cli_max_level_error		= 10012,	//等级已经到顶了
	cli_not_enough_gold_error = 10013,	//金币不足
	cli_not_enough_power_error = 10014, 	//内功不够
	cli_not_enough_nerve_error = 10015,		//真气值不够
	cli_not_enough_package_size = 10016,	//用户的背包满了
	cli_not_enough_vip_level_error = 10017,	//VIP等级不够
	cli_not_enough_enery_error	= 10018,	//体力不够
	cli_enery_is_full_error     = 10019,	//购买体力时，体力值已满
	cli_package_size_limit_error = 10020,	//购买时，用户的背包上线已到
	cli_enter_mission_error		= 10021,	//进入战斗关卡出错，没有
	cli_auto_battle_error		= 10022,	//扫图出错
	cli_check_battle_over_error	= 10023,
	cli_system_busy_error		= 10024,	//系统繁忙
	cli_not_enough_item_error	= 10025,	//物品数量不足
	cli_enter_mission_without_item_err = 10026,//进入关卡所需材料不足
	cli_not_enough_prestige_error = 10027,	//声望不够
	cli_user_has_got_prize_error = 10028,	//已经领取过奖励了
	cli_user_times_limit_error	= 10029,	//次数上限已到
	cli_multiple_login_err		= 10030,	//用户重复登录
	cli_date_limit_error		= 10031,	//日期错误
	cli_daily_limit_error		= 10032,	//兑换限制错误
	cli_has_accept_task_error   = 10033,	//任务已接
	cli_has_not_accept_task_error = 10034,	//任务没有接取
	cli_cooldown_time_error		= 10035,	//冷却时间没到
	cli_equipment_profess_error = 10036,	//装备职业不对
	cli_max_package_size_error = 10037,		//背包容量已达上限
	cli_chatchannel_notfound_error = 10038, //聊天的频道ID不合法
	cli_offline_getgift_error   = 10039,    //领取离线礼包错误
	cli_had_partner_error		= 10040,	//已经拥有了这个弟子
	cli_partner_count_limit_error = 10041,	//携带弟子数到达上限
	cli_shop_count_limit_error = 10042,  //商城购买超过限制数量
	cli_shop_product_items_error = 10043,//商品配置的道具内容有误
	cli_shop_out_time_error = 10044,//超出购买时间
	cli_boss_battle_noneed_reset = 10045,//不需要重置
	cli_boss_battle_had_reseted = 10046,//战已经重置过了
	cli_boss_battle_everyday_onetime = 10047,//BOSS每天战斗一次
	cli_boss_battle_pre_not_pass = 10048,//上一个BOSS未通关
	cli_activity_not_time = 10049,//非活动时间
	cli_no_find_nick_error	= 10050,	//用户名不存在
	cli_user_partner_card_used_error = 10051, //侠客卡对应的侠客已经存在
	cli_user_fight_not_enough_error = 10052, //角色当前战力值不足
	cli_user_horse_is_best_error = 10053, //拥有的马已经是顶配了
	cli_user_star_package_full_error = 10054,	//占星背包已满
	cli_user_equip_star_error = 10055,			//装备星石出错
	cli_user_escort_overflow_limit_error = 10056, //押镖超每日上限
	cli_user_escort_keaped_overflow_limit_error = 10057, //被拦截次数超过上限
	cli_user_escort_keap_overflow_limit_error = 10058, //拦截次数超过上限
	cli_user_escort_keapeding_error = 10059, //正在被拦截中
	cli_user_escort_left_gift_error = 10060, //有奖品没有领,不能开展新的押镖
	cli_user_transform_star_error = 10061,	//没有可转换的星石
	cli_user_invite_not_exist_error = 10062, //好友邀请码不存在
	cli_world_boss_over_err = 10063, 		//世界boss已经被击杀
	cli_user_partner_not_enough_friendval_err = 10065, //友好度错误
	cli_user_partner_enough_friendval_err = 10066, //友好度已足够
	cli_user_invite_exists_err = 10067, //好友邀请码已经设置过了
	cli_function_not_open_err = 10068, //该功能未开启
	cli_pre_mission_not_pass_err = 10069, //前置关卡还未通关
	NO_FIND_ONLINE_SVR_ERR	= 20000,		//switch返回login错误码
};

enum svr_err_code {
	SVR_NO_DEFINE_ERR           = 1000,         //没有定义
	SVR_SYS_ERR                 = 1001,         //系统出错 一般是内存出错
	SVR_DB_ERR                  = 1002,         //数据库出错
	SVR_NET_ERR                 = 1003,         //NET出错
	SVR_CMDID_NODEFINE_ERR      = 1004,         //命令ID没有定义
	SVR_PROTO_LEN_ERR           = 1005,         //协议长度不符合
	SVR_VALUE_OUT_OF_RANGE_ERR  = 1006,         //数值越界

	SVR_FLAY_ALREADY_SET_ERR    = 1007,     //要设置的flag和原有一致 
	SVR_VALUE_ISNOT_FULL_ERR    = 1008,     //数据不完整
	SVR_ENUM_OUT_OF_RANGE_ERR   = 1009,     //枚举越界

	SVR_PROTO_RETURN_LEN_ERR    = 1010,//返回报文长度有问题
	SVR_DB_DATA_ERR             = 1011,//数据库中数据有误
	SVR_NET_SEND_ERR            = 1012,
	SVR_NET_RECV_ERR            = 1013,
	SVR_KEY_EXISTED_ERR         = 1014,//在insert，KEY已存在
	SVR_KEY_NOFIND_ERR          = 1015,//在select update, delect 时，KEY不存在
	SVR_DATA_INVALID_ERR        = 1016,//接受到的数据不合法

	SVR_VALUE_NOENOUGH_E        = 1020,
	SVR_VALUE_MAX_E             = 1021,

	SVR_REGISTER_SAME_NAME_ERR  = 1100,     //用户名已注册
	SVR_LOGIN_NO_FIND_USER_ERR  = 1101,     //用户ID没有注册
	SVR_CHECK_PASSWD_ERR        = 1103,//登入时，检查用户名和密码出错
	SVR_USER_ID_EXISTED_ERR     = 1104,//在insert，ID已存在
	SVR_USER_ID_NOFIND_ERR      = 1105,//在select update, delete 时，ID不存在
	SVR_USER_EMAIL_NO_EXISTED_ERR = 1106, //用户的EMAIL 为空
	SVR_USER_NOT_ENABLED_ERR    = 1107, //用户已经冻结了
	SVR_USER_IS_ENABLED_ERR     = 1108, //用户已经解冻了
	SVR_LIST_ID_EXISTED_ERR     = 1109,
	SVR_LIST_ID_MAX_ERR         = 1110,
	SVR_ORDER_BILL_ALREADY_DEAL_ERR	= 1111,	//充值订单已经处理过了

	USER_PACKAGE_FULL_ERR	= 1200,		//用户的背包满了
	USER_PACKAGE_SIZE_FULL_ERR = 1201, 	//用户的背包上线已到
	USER_NOT_ENOUGH_GOLD_ERR= 1202,	//黄金不够
	USER_NOT_ENOUGH_YXB_ERR = 1203,	//游戏币不够
	USER_NOT_ENOUGH_POWER_ERR	= 1204,	//内力值不够
	USER_ENERY_IS_FULL_ERR	= 1205,	//购买体力错误
	USER_NOT_PASS_MAP_ERR	= 1206,	//没有通关
	USER_NOT_ENOUGH_ITEM_ERR= 1207,	//物品不足
	USER_COOLDOWN_TIME_ERR	= 1208,	//cd时间没有走完
	USER_PARTNER_COUNT_LIMIT_ERR= 1209,	//侠客已达到上限
	USER_JAR_UPGRADE_ERR	= 1210,		//没有罐子时，点罐子升级错误
	USER_NOT_ENOUGH_NERVE_ERR	= 1211,	//真气不足
	USER_PARTNER_TYPE_ERR	= 1212,		//拉取伙伴时的类型错误
	USER_FINISH_TASK_ERR	= 1213,		//完成任务出错
	USER_PLANT_EXIST_ERR	= 1214,		//种植出错
	USER_PLANT_NOT_FIND_ERR	= 1215,		//种植出错
	USER_PLANT_TIME_ERR		= 1216,		//种植时间没到
	USER_NONEED_RESET_BOSS = 1218,//不需要重置BOSS副本
	USER_HAD_RESET_BOSS = 1219,//已经重置过了不能再重置
	BOSS_EVERYDAY_ONETIME = 1220,//一个BOSS每天打一次
	BOSS_PRE_NOT_PASS = 1221,//BOSS关卡上一个BOSS没有通过
	USER_ACTIVITY_NOT_TIME = 1222,//非活动时间
	USER_FRIEND_COUNT_LIMIT_ERR	= 1223,	//好友人数上限
	USER_ADD_FRIEND_EXIST_ERR = 1224,	//已经添加好友了
	SHOP_PRODUCT_COUNT_LIMIT_ERR = 1225, //商城商品购买有数量限制
	USER_PARTNER_CARD_USED_ERR = 1226, //侠客卡已经被使用了
	USER_ESCORT_OVERFLOW_LIMIT_ERR = 1227, //押镖次数超上限
	USER_ESCORT_GOING_ERR = 1228, //押镖进行中
	USER_ESCORT_LEFT_GIFT_ERR = 1229, //还有奖品没有领
	USER_TRANSFORM_STAR_ERR = 1230,	//转换星石出错
	USER_PARTNER_FRIEND_VAL_ENOUGH_ERR = 1231, //弟子培养度足够
	USER_INVITE_EXISTS_ERR = 1232, //好友邀请码已经设置过了
};

struct error_code_map
{
	int cli_error;
	int svr_error;
};

#endif
