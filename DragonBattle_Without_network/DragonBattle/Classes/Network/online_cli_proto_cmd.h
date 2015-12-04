#ifndef ONLINE_CLI_PROTO_CMD_HPP__
#define ONLINE_CLI_PROTO_CMD_HPP__

#ifdef WONPEE_SERVER
#include "../share/proto_base.h"
#else
#include "proto_base.h"
#include "error_code.h"
#include "share_enums.h"
#endif

enum enum_online_cli_cmd {
	PROTO_KICK_USER_OFFLINE_CMD		= 499,	//踢用户下线
	PROTO_USER_LOGIN_CMD			= 500,	//用户登录包，校验session
	PROTO_USER_GET_PACKAGE_CMD		= 501,	//用户拉取背包
	PROTO_USER_GET_ITEM_LIST_COUNT_CMD	= 502, 	//用户拉取指定itemid物品的数量
    PROTO_USER_USE_ITEM_CMD         = 503,  //使用道具
    PROTO_USER_ACTION_CMD			= 504,	//用户状态
	PROTO_USER_KEEP_ALIVE_CMD		= 505,	//前端心跳
	PROTO_USER_SWAP_CMD             = 506,  //兑换或随机获取物品
	PROTO_USER_BUY_ITEM_CMD			= 507,	//yxb购买物品
	PROTO_USER_EXPAND_PACKAGE_CMD	= 508,	//扩展背包
	PROTO_USER_ADJUST_PACKAGE_CMD	= 509,	//整理背包
	PROTO_USER_SELL_ITEM_CMD		= 510,	//用户卖出道具

	PROTO_USER_TRAIN_EQUIPMENT_CMD  = 513,	//装备洗练
	PROTO_USER_CHANGE_EQUIPMENT_CMD = 514,	//用户换装(穿上,替换,脱下)
	PROTO_USER_STRENGTHEN_EQUIPMENT_CMD = 515,	//装备强化
	PROTO_USER_UPGRADE_EQUIPMENT_CMD	= 516,	//装备进阶
	//PROTO_USER_INLAY_EQUIPMENT_CMD 	= 517,	//装备注灵

	PROTO_USER_TRAIN_BASE_ATTR_CMD 	= 518,  //培养
    PROTO_USER_CONFIRM_TRAIN_CMD 	= 519,  //确定或取消培养
	PROTO_USER_GET_SKILL_CMD		= 520,	//用户拉取所有技能
	PROTO_USER_UPGRADE_SKILL_CMD	= 521,	//技能升级
	PROTO_USER_CHANGE_SKILL_CMD		= 522,	//用户装载技能(装载,替换,卸下)

	PROTO_USER_WEAR_EQUIPMENT_CMD	= 523,	//用户穿装(指定装备id而不指定package_id)(与514重复,TODO:DEL)
	PROTO_USER_PUNCH_EQUIPMENT_CMD	= 524,	//装备打孔
	PROTO_USER_INLAY_GEM_CMD		= 525,	//装备镶嵌宝石
	PROTO_USER_COMPOSE_GEM_CMD		= 526,	//宝石合成

	PROTO_USER_UPGRADE_FORCE_CMD  	= 530,  //练气
	PROTO_USER_UPGRADE_NERVE_CMD    = 531,  //经脉注入真气
	PROTO_USER_GET_COOLDOWN_CMD 	= 532,  //得到CD时间
	PROTO_USER_CLEAR_COOLDOWN_CMD 	= 533, 	//清楚某类型的CD时间
	PROTO_USER_GET_DAILY_EVENT_CMD	= 534,	//得到t_daily_event表中某事件的完成次数
	//PROTO_USER_SET_DAILY_EVENT_CMD	= 535,
	PROTO_USER_GET_TYPE_EVENT_CMD	= 536,	//得到t_user_type_event表中某类型的值
	//PROTO_USER_SET_DAILY_EVENT_CMD	= 537,	//设置
	PROTO_USER_GET_HARD_MISSION_CMD	= 539,	//拉取困难关卡信息
	PROTO_USER_GET_MISSION_CMD		= 540,	//拉取主线关卡信息
	PROTO_USER_BUY_ENERY_CMD		= 541,	//黄金购买体力
	PROTO_USER_RECOVER_ENERY_CMD	= 542,	//恢复体力
	PROTO_USER_START_AUTO_BATTLE_CMD= 543,	//开始扫荡
	PROTO_USER_STOP_AUTO_BATTLE_CMD = 544,	//结束扫荡
	PROTO_USER_GET_ROUNDS_BATTLE_CMD= 545,	//得到当前扫荡的结果
	PROTO_USER_SPEED_AUTO_BATTLE_CMD= 546,	//加速扫荡
	PROTO_USER_BATTLE_START_CMD		= 547,	//进入关卡
	PROTO_USER_BATTLE_OVER_CMD		= 548,	//关卡战斗结束
	PROTO_USER_GET_BATTLE_PRIZE_CMD = 549,	//翻牌获得奖励
	PROTO_USER_BALLOT_CMD			= 550,	//藏经阁抽签
	PROTO_USER_GET_ALL_BATTLE_PRIZE_CMD	= 551,	//获得所有老虎机的奖励
	
	PROTO_USER_USE_PARTNER_CARD_CMD = 556,	//使用弟子卡牌
	PROTO_USER_INHERIT_PARTNER_CMD  = 558,	//弟子传承
	PROTO_USER_PARTNER_FRIENDVAL_TRAIN_CMD = 702, //弟子好友度培养 
	PROTO_USER_PARTNER_UNHAD_LIST_CMD = 703, //拉取没有获得的弟子列表及友好度
	PROTO_USER_PARTNER_UNHAD_ONE_CMD = 704, //拉取指定弟子的友好度

	PROTO_USER_GET_JAR_LIST_CMD		= 564,	//得到寻宝的罐子的列表
	PROTO_USER_UPGRADE_JAR_CMD		= 565,	//罐子升级
	PROTO_USER_BREAK_JAR_CMD		= 566,	//砸罐子
	PROTO_USER_BUY_HAMMER_CMD		= 567,	//购买用于砸罐子的锤子

	PROTO_USER_SWAP_PARTNER_SEAT_CMD= 569,	//上阵侠客交换位置
	PROTO_USER_ENLIST_PARTNER_CMD	= 570,	//招募侠客
	PROTO_USER_GET_PARTNER_LIST_CMD	= 571,	//侠客列表
	PROTO_USER_ONPLAY_PARTNER_CMD	= 572,	//侠客上阵
	PROTO_USER_OFFPLAY_PARTNER_CMD	= 573,	//侠客下阵
	PROTO_USER_UPGRADE_PARTNER_SKILL_CMD= 575,	//侠客技能升级
	PROTO_USER_TRAVEL_PARTNER_CMD	= 576,		//侠客游历,TODO:DEL
	PROTO_USER_PARTNER_EAT_EXP_CMD	= 577,		//吃经验丹
	PROTO_USER_RECALL_PARTNER_CMD	= 578,		//用户放逐和召回侠客
	PROTO_USER_GET_PRESTIGE_TITLE_CMD	= 579,	//得到声望挑战信息
	PROTO_USER_GET_PRESTIGE_PRIZE_CMD	= 580,	//领取
	PROTO_USER_PRESTIGE_LOTTERY_CMD	= 581,	//声望摇奖
	
	PROTO_USER_GET_TASK_LIST_CMD	= 585,	//得到任务列表
	PROTO_USER_GET_TASK_CMD			= 586,	//获得任务当前完成情况
	PROTO_USER_ACCEPT_TASK_CMD		= 587,	//接取任务
	PROTO_USER_SET_TASK_CMD			= 588,	//设置任务
	PROTO_USER_FINISH_TASK_CMD		= 589,	//完成任务
	PROTO_USER_DROP_TASK_CMD		= 590,	//放弃任务
	
	PROTO_USER_GET_AWARD_TASK_CMD		= 591,	//拉取悬赏任务
	PROTO_USER_ACCEPT_AWARD_TASK_CMD 	= 592,	//接取悬赏任务
	PROTO_USER_UPGRADE_AWARD_TASK_CMD 	= 593,	//一件满星
	PROTO_USER_SET_AWARD_TASK_FINISH_CMD = 594,	//一键完成
	PROTO_USER_FINISH_AWARD_TASK_CMD	= 595,	//完成后提交任务
	PROTO_USER_REFRESH_AWARD_TASK_CMD	= 596,	//刷新悬赏任务
	PROTO_USER_SET_GUIDE_STEP_CMD	= 597,	//设置引导步骤
	
	PROTO_USER_GET_ATTR_ITEM_CMD	= 600,	//得到itemid类型为0的属性值
	PROTO_USER_LOGIN_EX_CMD			= 601,	//用户登录后，紧跟着拉取任务、在线礼包等数据
	PROTO_USER_GET_MAGIC_STORE_CMD	= 610,	//获得神秘商店
	PROTO_USER_BUY_MAGIC_ITEM_CMD	= 611,	//购买神秘商店中的物品
	PROTO_USER_FLUSH_MAGIC_STORE_CMD= 612,	//刷新神秘商店
	PROTO_USER_GET_PLATFORM_BONUS_CMD = 620,//领取奖励平台的奖励
	PROTO_USER_LOGIN_SIGN_CMD		= 621,	//签到
	PROTO_USER_GET_TESTING_RANK_CMD	= 622,	//得到封测等级和战力排行
	PROTO_USER_CHAT_CMD          = 630,//客户端发送聊天消息到服务器
	PROTO_USER_GET_USER_ONLINE_STATUS_CMD	= 639,	//查询玩家在线状态
	PROTO_USER_GET_OTHER_USER_CMD	= 640,	//得到其他用户信息
	PROTO_USER_GET_ARENA_INFO_CMD	= 641,	//得到竞技场信息
	PROTO_USER_GET_ARENA_TOP_100_CMD= 642,	//得到竞技场top100
	PROTO_USER_GET_ARENA_FILE_CMD	= 643,	//得到竞技场战报
	PROTO_USER_WORSHIP_TOP_USER_CMD	= 644,	//膜拜	
	PROTO_USER_START_ARENA_BATTLE_CMD = 645,//开始竞技场战斗
	PROTO_USER_OVER_ARENA_BATTLE_CMD = 646,	//结束竞技场战斗
	PROTO_USER_GET_ARENA_RANK_PRIZE_CMD	= 647,//竞技场领取奖励
	PROTO_USER_GET_WORSHIP_INFO_CMD	= 648,	//得到膜拜信息
	PROTO_USER_SHOP_INFO_CMD		= 650,	//拉取商城数据,传递参数 为1代表是道具 为2代表是礼包
	PROTO_USER_SHOP_BUY_CMD			= 651,	//购买商城物品	
	PROTO_USER_RAND_INFO_CMD		= 653,	//排行榜数据 1等级 2战力 3周充值
	PROTO_USER_PRIVATE_GIFT_CMD		= 654,	//个人礼包
	PROTO_USER_PRIVATE_GIFT_GET_CMD	= 655,	//个人礼包领取
	PROTO_USER_GET_BOSS_MISSION_CMD	= 656,	//BOSS关卡信息
	PROTO_USER_RESET_BOSS_MISSION_CMD=657,	//重置BOSS关卡
	PROTO_USER_AUTO_BOSS_BATTLE_CMD = 658,	//BOSS扫荡
	PROTO_USER_GET_MAX_MISSION_CMD 	= 659,	//拉取开放的最大MISSION ID
	PROTO_USER_GET_VILLA_INFO_CMD	= 660,	//得到山庄种植信息
	PROTO_USER_PLANT_CMD			= 661,	//种植种子
	PROTO_USER_SPEED_PLANT_CMD		= 662,	//加速植物
	PROTO_USER_HAVEST_PLANT_CMD		= 663,	//收获果实
	PROTO_USER_EXPAND_LAND_CMD		= 664,	//扩展土地
	PROTO_USER_SHAKE_MONEY_CMD		= 665,	//摇钱树
	PROTO_USER_ACTIVITY_DICE_CMD	= 666,	//活动摇色子获得体力
	PROTO_USER_TOWER_CMD			= 667, 	//爬塔拉取信息
	PROTO_USER_AUTO_TOWERBATTLE_CMD = 668,	//爬塔自动挑战
	PROTO_USER_RESET_TOWER_CMD		= 669, //重置爬塔次数
	PROTO_USER_GET_FRIEND_LIST_CMD  = 670, //拉取好友信息
	PROTO_USER_ADD_FRIEND_CMD       = 671, //用户添加好友
	PROTO_USER_DEAL_FRIEND_REQUEST_CMD = 672,//用户处理好友请求
	PROTO_USER_DEL_FRIEND_CMD       = 673, //用户删除好友
	PROTO_USER_RECV_FRIEND_REQUEST_CMD = 674,//用户主动收到好友请求
	PROTO_USER_FIND_USER_FROM_NICK_CMD = 675,//通过nick找到用户基本信息
	PROTO_USER_GET_VILLA_STATE_CMD	= 676,	//得到山庄状态

	PROTO_USER_GET_STAR_LIST_CMD	= 677,	//得到占星列表
	PROTO_USER_ASTROLOGY_CMD		= 678,	//占星
	PROTO_USER_TRANSFORM_STAR_CMD	= 679,	//转化单个星石
	PROTO_USER_AUTO_TRANSFORM_STAR_CMD	= 680,	//一键转化星石
	PROTO_USER_EQUIP_STAR_CMD 		= 681,	//装备或卸下星石
	PROTO_USER_UPGRADE_STAR_CMD		= 682,	//升级星石
	PROTO_USER_CALL_ASTROLOGER_CMD	= 683,	//召唤占星师

	PROTO_USER_CHOOSE_HORSE_CMD = 684, //选马了
	PROTO_USER_REFRESH_HORSE_CMD = 685, //刷马
	PROTO_USER_GET_ESCORT_INFO_CMD = 686, //押镖运信息
	PROTO_USER_ESCORT_COMBAT_INFO_CMD = 687, //战报信息
	PROTO_USER_START_KEAP_HORSE_CMD = 688, //开始劫持白龙马
	PROTO_USER_OVER_KEAP_HORSE_CMD = 689, //结束劫持白龙马
	PROTO_USER_GET_ESCORT_USER_INFO_CMD = 690, //拉取押镖玩家信息
	PROTO_USER_START_ESCORT_CMD = 691, //开始押镖
	PROTO_USER_ESCORT_GIFT_CMD = 692, //领押镖奖励
	PROTO_USER_ESCORT_EXISTS_GIFT_CMD = 693, //是否存在押镖奖励

	PROTO_USER_INVITE_INFO_CMD = 694, //好友邀请信息

	PROTO_USER_GET_WORLD_BOSS_INFO_CMD = 695, //拉取世界boss信息
	PROTO_USER_WORLD_BOSS_BTL_START_CMD = 696, //开始单局世界boss
	PROTO_USER_WORLD_BOSS_BTL_OVER_CMD = 697, //单局世界boss结束
	PROTO_USER_WORLD_BOSS_INVIGORATE_CMD = 698, //世界boss鼓舞
	PROTO_USER_WORLD_BOSS_OVER_NOTI_CMD = 699, //世界boss结束通知
	PROTO_USER_CHECK_WORLD_BOSS_CMD = 700, //检查世界boss活动状态

	//活动
	PROTO_USER_GET_ACTIVITY_INFO_CMD = 701, //获取活动信息
	PROTO_USER_BY_INVITE_CODE_CMD = 705, //设置邀请我进来的好友码

	//系统消息
	PROTO_USER_SYS_MSG_NOTI_CMD = 710, //系统消息通知
	PROTO_USER_SET_READ_SYS_MSG_CMD = 711, //设置系统消息为已读

	PROTO_USER_GET_ORDER_BILL_NO_CMD	= 800,	//获取充值订单号
	PROTO_USER_NOTIFY_BUY_GOLD_CMD		= 801,	//充值结果通知

	PROTO_USER_CHEAT_ITEM_CMD		= 1000,	//用户改变数量
};

enum enum_user_event_type {
	STRENGTHEN_EQUIPMENT_CD 	= 1000,	//装备强化CD类型
	NERVE_UNGRADE_CD 			= 1001,	//经脉升级的CD类型
	USER_PRESTIGE_TITLE_EVENT	= 1010,	//用户江湖声望封号的奖励
	BONUS_ARENA_DAILY_TYPE		= 1011,	//竞技场挑战奖励

	BONUS_LOGIN_EX_BEG			= 1100,	//登陆时需拉取状态的起始id
	BONUS_DAILY_ONLINE_TYPE		= 1100,	//每日在线礼包类型
	BONUS_TURN_TABLE_TYPE		= 1101,	//每日大转盘
	BONUS_WEEK_LOGIN_TYPE		= 1102,	//周登录奖励
	BONUS_LEVEL_TYPE			= 1103,	//等级奖励
	BONUS_LOGIN_SIGN_TYPE		= 1104,	//签到
	BONUS_VIP_WELFARE_TYPE		= 1105,	//每日福利
	BONUS_OFFLINE				= 1106, //离线福利 领奖的时候需要传bonus_index 当日是1 多日是2,同其他领奖一致
	BONUS_SEVEN_DAY_PRIZE		= 1107,	//创建角色开始7日目标完成
	BONUS_FIRST_BUY_GOLD		= 1108,	//开服首冲返利	首冲返利
	BONUS_FIRST_BUY_PRIZE		= 1109,	//开服首冲奖励	首冲礼包
	BONUS_VIP_SIX_PRIZE			= 1110,	//开服谁与争锋	VIP6
	BONUS_QUALIFY_PRIZE			= 1111,	//开服一统江湖 	资质提升
	BONUS_COLLECT_PARTNER		= 1112,	//开服侠义无双
	BONUS_LOGIN_COUNT_PRIZE		= 1113,	//封测登录7日奖励
	BONUS_LEVEL_PRIZE			= 1114,	//封测等级提升奖励
	BONUS_BATTLE_VALUE_PRIZE	= 1115,	//封测战力提升奖励
	BONUS_TOP_LEVEL_PRIZE		= 1116,	//封测前10名等级排名奖励
	BONUS_TOP_BATTLE_PRIZE		= 1117,	//封测前10名战力排名奖励
	BONUS_DAILY_WORSHIP_FAIRY	= 1118, //参拜神仙姐姐(连续三天为一个周期)
	BONUS_DAILY_VIP_FAKE		= 1119, //假的vip充值(根据在线时长领黄金)//TODO:正式开服后需要去掉
	BONUS_INVITE_CODE			= 1120,	//好友邀请码达到等级对应的奖励信息（1-5对应五个奖位）
	BONUS_ACTIVE_DEGREE_PRIZE	= 1121,	//每日活跃度
	BONUS_TOTOL_VIP_TYPE		= 1122,	//vip累计充值奖励
	BONUS_LOGIN_EX_END,					//登陆时需拉取状态的结束id

	DAILY_AWARD_TASK_NUM		= 1200, //悬赏任务次数
	DAILY_MONEY_SHAKE_TYPE		= 1201,	//每日摇钱树

	TAKE_PARTNER_CD1			= 1150,	//十里挑一
	TAKE_PARTNER_CD2			= 1151,	//百里挑一
	TAKE_PARTNER_CD3			= 1152,	//万里挑一

	ACTIVITY_DICE_TYPE		= 1401,//活动摇色子
	BOSS_RESET_NUM			= 1402,//精英BOSS战重置次数
	TOWER_INFO_TYPE			= 1403,//爬塔类型
	INVITE_LOGIN_NUM		= 1404,//邀请过来的好友连续登陆情况 value是登陆日期,field_1>>16(0-3位分别代表是否已经达到连续登陆2天，连续登陆3天，连续登陆5天),field_1 && 0x0000FFFF(0xFFFF,连续登陆天数)
};

enum enum_daily_limit_type {
	DAILY_PARTNER_TRAVEL_TYPE	= 3001, //VIP每日侠客游历的次数
	DAILY_PRESTIGE_LOTTERY_TYPE = 3002,	//VIP每日声望摇奖的次数
	DAILY_AWARD_TASK_TYPE		= 3003,	//VIP每日悬赏任务的次数
	DAILY_BUY_ENERY_TYPE		= 3004,	//VIP每日购买体力次数
	DAILY_GET_VIP_GIFT			= 3005,	//VIP每日领取礼包次数
	DAILY_BUY_HAMMER			= 3006,	//每日购买锤子次数
	DAILY_GET_YXB				= 3007,	//VIP每日领取盘缠次数
	DAILY_GOLD_SWAP_YXB			= 3008,	//VIP每日黄金换银两次数
	DAILY_BUY_ARENA_COUNT		= 3009,	//VIP每日购买竞技次数
	DAILY_WORSHIP_COUNT			= 3010,	//VIP每日膜拜字段
};

#define CREATE_CMESSAGE_WITH_ONE_MEMBER(_class_, _type_, _member_, _buf_type_) \
class _class_ : public Cmessage {\
public:\
	_type_ _member_;\
	_class_() { this->init(); }\
	void init() { this->_member_ = 0; }\
	int get_msg_len() { return sizeof(_member_); }\
	bool read_from_buf(byte_array_t & ba) {\
		if (!ba.read_##_buf_type_(_member_)) { return false; } return true; }\
	bool write_to_buf(byte_array_t & ba) {\
		if (!ba.write_##_buf_type_(_member_)) { return false; } return true; }\
};

struct item_unit_t
{
	uint32_t item_id;
	uint32_t item_cnt;
};

struct package_item_t
{
	uint32_t package_id;
	uint32_t item_id;
	uint32_t item_cnt;
};

struct equipment_attach_attr_t
{
	uint32_t attr_type;
	uint32_t attr_value;
};

struct package_equipment_t
{
	uint32_t package_id;
	uint32_t equipment_id;
	uint32_t equipment_level;
	uint32_t add_attr_1;
	uint32_t add_attr_2;
	equipment_attach_attr_t attach_attr[4];
	uint32_t gems[3];//三个孔位的状态：0 锁定，1 开启但未镶嵌，大于1 代表镶嵌的宝石的id 

	//显示定义构造函数，会导到不能以C风格的{0}来初始化结构体，同时也会导致offsetof宏warning
	/*package_equipment_t():package_id(0), equipment_id(0), equipment_level(0),
				add_attr_1(0), add_attr_2(0) {
		memset(attach_attr, 0, sizeof(attach_attr));
		memset(gems, 0, sizeof(gems));
	}*/
	void init();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

struct item_used_t
{
	uint32_t item_id;
	uint32_t last_cnt;
};

struct user_skill_t
{
	uint32_t skill_id;
	uint32_t skill_lvl;
	uint32_t wear_seat;
};

struct user_mission_t
{
	user_mission_t(uint32_t id = 0, uint32_t s = 0, uint32_t v = 0)
		: mission_id(id), score(s), value(v) {}
	uint32_t mission_id;
	uint32_t score;
	uint32_t value;
};

struct user_boss_mission_t
{
	uint32_t mission_id;
	uint32_t score;
	uint32_t value;
};

struct star_ston_info_t
{
	uint32_t star_id;
	uint32_t unique_id;		//每个石头有个唯一ID
	uint32_t seat_id;		//如果在背包中，则为0 带在身上为unit_id*1000 + 位置
	uint32_t level;
};

struct partner_info_t
{
	uint32_t partner_id;//伙伴ID
	uint64_t exp;       //经验
	uint32_t level;     //等级
	uint32_t inherit_state;
	uint32_t qualify_value;	//资质
	uint32_t cur_seat;  //当前位置
	uint32_t skill_level;//技能对应的等级
	package_equipment_t equipment[8]; //穿戴的装备
	int      train_hp;                //培养增加的生命值
	int      train_out_attack;        //培养增加的攻击力
	int      train_out_def;           //培养增加的防御力
	int      train_in_attack;         //培养增加的内攻
	int      train_in_def;            //培养增加的内防
	int      train_toughness;         //培养增加的内力
	star_ston_info_t star_array[4];
};

struct one_round_battle_prize_t
{
	uint32_t cur_round;	//当前第几轮
	std::vector<item_unit_t> item_vec;//战斗奖励
	std::vector<item_unit_t> drop_vec;//掉落奖励
};

struct auto_battle_t
{
	uint32_t map_id;
	std::vector<item_unit_t> item_vec;//战斗奖励
	std::vector<item_unit_t> drop_vec;//掉落奖励
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

struct sell_item_t
{
	uint32_t item_id;
	uint32_t item_cnt;
	uint32_t price;
	uint32_t is_sell;		//1表示已经被卖了
};

struct task_info_t
{
	uint32_t task_id;
	uint8_t  task_type;	//1主线 2支线
	uint8_t	 task_state;//1已接 2可接
	uint8_t  cur_step;	//当前过程
	uint8_t  max_step;	//要完成所需过程
};

struct award_task_info_t
{
	uint16_t task_id;
	uint16_t cur_step;
	uint16_t max_step;
	uint8_t	 task_star;
	uint8_t	 task_state;
};

struct active_degree_info_t
{
	uint32_t prize_state;		//活跃度奖励领取状态
	uint8_t active_state[60];	//各项任务完成状态
};

struct user_pos_info_t
{
	uint32_t userid;
	uint32_t unit_id;
	uint16_t equip_series;
	uint32_t level;
	uint32_t vip_level;
	char	 nick[32];
	float	 pos_x;
	float	 pos_y;
};

struct user_arena_rank_info_t {
	uint32_t rank_id;
	uint32_t userid;
	uint32_t unit_id;
	char	 nick[32];
	uint32_t level;
	uint32_t battle_value;	//战力
};

struct rank_info_t
{
	uint32_t rank_id;
	uint32_t userid;
	uint32_t role_type;
	char nick[32];
	uint32_t faction;	//帮派
	uint32_t value;
};

struct simpl_rank_info_t
{
	char nick[32];
	uint32_t value;	//数量

	void init();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

/* @brief 世界boss结算前3名奖励
 */
struct world_boss_top_info_t
{
	char nick[32];
	uint32_t reward_yxb;	//银两奖励
	uint32_t reward_nerve;	//真气奖励

	void init();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

/* @brief 活动开启时间信息
 */
struct activity_time_t {
	uint32_t activity_id;
	uint32_t begin_time;
	uint32_t end_time;

	activity_time_t() {
		init();
	}
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

struct usr_online_status_t {
	uint32_t userid;
	uint8_t flag;//是否在线
	usr_online_status_t(): userid(0), flag(0) {}
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

struct villa_land_info_t
{
	uint32_t land_id;		//土地
	uint32_t plant_id;
	uint32_t left_time;		//剩余时间
};

//PROTO_USER_LOGIN_CMD			= 500,	//用户登录包，校验session
class cli_user_login_in : public Cmessage
{
public:
	uint32_t time;
	char login_sess[20];

	cli_user_login_in();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

class cli_user_login_out : public Cmessage
{
public:
	uint32_t userid;
	char	 nick[32];
	uint32_t unit_id;
	uint32_t gold;
	uint32_t buy_gold;//充值的黄金数
	uint64_t exp;
	uint32_t level;
	uint32_t yxb;
	uint32_t last_community_id;
	uint32_t spec_community;	//特殊社区标记，1需进入世界boss房间
	uint32_t enery;
	uint32_t enery_max;
	uint32_t enery_counter;	//下次体力回复所需的时间
	uint32_t force_exp;		//练气经验
	uint32_t nerve_level;	//经脉等级
	uint32_t nerve_value;	//真气值
	uint32_t power_value;	//内功值（战斗获得）
	uint32_t prestige_value;//声望
	uint32_t star_exp;		//星石经验
	uint32_t package_cnt;	//背包中物品数量
	uint32_t max_package_size;	//背包大小

    int      train_hp;                //培养增加的生命值
    int      train_out_attack;        //培养增加的攻击力
    int      train_out_def;           //培养增加的防御力
    int      train_in_attack;         //培养增加的内攻
    int      train_in_def;            //培养增加的内防
	int      train_toughness;         //培养增加的内力

	uint32_t skills[4];			//招式(技能ID*1000+等级)
	uint16_t passive_skills[4];	//被动技能level
	
	star_ston_info_t star_array[4];
	
	uint32_t auto_battling;
	uint32_t is_exists_invite;//是否填过了邀请码
	std::vector<package_equipment_t> equipment_vec;//已穿戴的装备
	std::vector<item_used_t> 		 item_used_vec;
	std::vector<partner_info_t> 	 partner_vec;

	cli_user_login_out();
	~cli_user_login_out();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_KEEP_ALIVE_CMD		= 505,	//前端心跳
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_keep_alive_out, uint32_t, time, uint32);//要删除的好友ID

//PROTO_USER_LOGIN_EX_CMD			= 601,	//用户登录后，紧跟着拉去任务、在线礼包等数据
class cli_user_login_ex_out : public Cmessage
{
public:
	std::vector<task_info_t>	task_vec;	//主支线任务(已接 可接)
	uint32_t max_main_task;					//已完成的最大主线任务id

	uint32_t last_login_tm;			//上次登陆时间,从未登陆过时是0

	//悬赏任务
	uint32_t award_counter;			//倒计时
	award_task_info_t award_task[4];//悬赏任务

	//在线礼包
	uint32_t online_prize_second;//在线礼包倒计时
	uint32_t online_prize_id;	//奖励ID
	
	//幸运大转盘时间，swapID
	uint32_t turntable_id;			//第几个
	uint32_t left_second;			//大转盘的倒计时
	
	//奖励平台 周登录礼包与等级礼包物品配表需要预先确定，后期如不做版本变更，无法修改
	//周登录礼包
	uint32_t week_login_prize;		//登录次数 | 是否领取(用位表示)
	//等级奖励
	uint32_t level_prize;			//是否领取(用位表示)
	
	//活动奖励 需要配置
	std::vector<uint32_t> action_prize_vec;//活动ID
	
	//签到
	uint32_t cur_date;		//当前日期
	uint32_t sign_value;	//位表示
	uint32_t sign_prize;	//次数
	
	//福利：VIP每日领取礼包  离线经验  离线补偿
	uint32_t is_vip_gift;	//是否领取
	uint32_t vip_level_gift;//VIP成长奖励
	uint32_t totol_vip_gift;//vip充值礼包 totol_vip_gift>>16首冲礼包 totol_vip_gift&0x0000FFFF累计充值
	
	uint32_t offline_exp_hour;//离线经验小时数
	uint32_t offline_exp;
	uint32_t offline_cost_cold;//三倍离线经验领取消耗黄金

	uint32_t offline_days;//离线天数,//TODO:DEL
	uint32_t offline_days_exp;//离线期内显示的经验//TODO:DEL
	uint32_t offline_days_yxb;//离线期内显示的银两//TODO:DEL
	//离线补偿
	uint32_t offline_prize_id;	//离线补偿//TODO:DEL

	//剩余次数
	std::vector<uint32_t>	num_vec;	//剩余次数
	
	uint32_t shake_money;	//摇钱树 今日次数

	uint32_t login_day;     //创建账号开始的第几天登录
	uint32_t seven_day_prize;//头7日登录目标
							//(seven_day_prize>>16)表示当天第1-3个任务的完成状态
							//seven_day_prize&0xFF表示第1-7天目标的领取状态
	uint32_t prize_state_1; //(prize_state_1>>24)&0xFF谁与争锋领奖 
							//(prize_state_1>>8)&0xFFFF一统江湖奖励, 其中(prize_state_1>>16)&0xFF最大弟子等级
							//     (prize_state_1>>12)&0x0F最大弟子技能等级 (prize_state_1>>8)&0x0F领取奖励状态
							//(prize_state&0xFF)侠义无双奖励 prize_state&(0x80)表示是否领取了奖励 其他表示抽取到的位数
	uint32_t login_count;   //登录天数 login_count>>16表示登录天数 login_count&0xFFFF表示领取奖励状态
	uint32_t prize_state_2;	//prize_state_2>>24 封测等级礼包领取状态
							//(prize_state_2>>16)&0xFF 封测战力礼包领取状态
							//(prize_state_2>>8)&0xFF 封测等级排名礼包领取状态 0不在10名内 1可领 2已领
							//(prize_state_2&0xFF)封测战力排名礼包领取状态 0不在10名内 1可领 2已领
	uint32_t worship_fairy;	//参拜神仙姐姐 worship_fairy>>16 表示当前是连续第几天, worship_fairy&0x01表示今天是否参拜过
	uint32_t gift_info;//奖励及领奖信息,gift_info>16 低0-4位对应5个奖品条件是否满足,gift_info<16 低0-4位对应5个奖品是否已经被领取
	uint32_t chat_info;//0 位代表是否有离线消息,1位代表是否有好友请求
	uint32_t self_gift;//个人礼包 0位为1代表有个人礼包
	active_degree_info_t active_degree;//活跃度
	uint32_t fake_vip_state;	//封测期间领取每日总在线时长奖励状态
	uint32_t online_time;		//当天的总在线时长(秒数)
	uint32_t reserved_state1;	//保留字段1
	uint32_t reserved_state2;	//保留字段2
	uint32_t reserved_state3;	//保留字段3

	cli_user_login_ex_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_GET_PACKAGE_CMD		= 501,	//用户拉取背包
class cli_user_get_package_out : public Cmessage
{
public:
	uint32_t item_sum;	//当前背包中物品数量
	uint32_t package_size;//背包上限
	std::vector<package_item_t> item_vec;	//道具
	std::vector<package_item_t> stuff_vec;	//材料
	std::vector<package_item_t> card_vec;	//弟子卡牌
	std::vector<package_equipment_t> equipment_vec;//装备

	cli_user_get_package_out();
    void init();
    int get_msg_len();
    bool read_from_buf(byte_array_t & ba);
    bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_GET_ITEM_LIST_COUNT_CMD	= 502, 	//用户拉取指定itemid物品的数量
class cli_user_get_item_list_count_in : public Cmessage
{
public:
	std::vector<uint32_t> item_vec;
    
    cli_user_get_item_list_count_in();
    void init();
    int get_msg_len();
    bool read_from_buf(byte_array_t & ba);
    bool write_to_buf(byte_array_t & ba);
};

class cli_user_get_item_list_count_out : public Cmessage
{
public:
    std::vector<item_unit_t> item_vec;
    
    cli_user_get_item_list_count_out();
    ~cli_user_get_item_list_count_out();
    void init();
    int get_msg_len();
    bool read_from_buf(byte_array_t & ba);
    bool write_to_buf(byte_array_t & ba);
};

//  PROTO_USER_USE_ITEM_CMD         = 503,  //使用道具
class cli_user_use_item_in : public Cmessage
{
public:
    uint32_t package_id;
    uint32_t item_id;
    
    cli_user_use_item_in();
    void init();
    int get_msg_len();
    bool read_from_buf(byte_array_t & ba);
    bool write_to_buf(byte_array_t & ba);
};

class cli_user_use_item_out : public Cmessage
{
public:
    uint32_t state;
    std::vector<package_item_t> item_vec;
    
	cli_user_use_item_out();
    void init();
    int get_msg_len();
    bool read_from_buf(byte_array_t & ba);
    bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_ACTION_CMD			= 504,	//用户状态
class cli_user_action_in : public Cmessage
{
public:
	uint32_t action_type;	//3:切换地图(进入地图) 5:在场景中移动
	uint32_t community_id;	//社区ID
	float pos_x;
	float pos_y;

	cli_user_action_in();
    void init();
    int get_msg_len();
    bool read_from_buf(byte_array_t & ba);
    bool write_to_buf(byte_array_t & ba);
};

class cli_user_action_out : public Cmessage
{
public:
	uint32_t community_id;	//社区ID
	uint32_t action_type;	//1:上线(登录进入游戏) 2:下线(退出游戏) 3:切换地图(进入地图) 4:切换地图(离开地图) 5:在场景中移动
							//(1-5类型为后台主动通知包) 10:地图中所有用户的信息
	uint32_t total_user_cnt;//社区中的总人数
	std::vector<user_pos_info_t> user_pos_vec;
 
    cli_user_action_out();
    void init();
    int get_msg_len();
    bool read_from_buf(byte_array_t & ba);
    bool write_to_buf(byte_array_t & ba);
};

//  PROTO_USER_SWAP_CMD             = 506,  //兑换或随机获取物品
class cli_user_swap_in : public Cmessage
{
public:
    uint32_t swap_id;
    
    cli_user_swap_in();
    void init();
    int get_msg_len();
    bool read_from_buf(byte_array_t & ba);
    bool write_to_buf(byte_array_t & ba);
};

class cli_user_swap_out : public Cmessage
{
public:
    uint32_t state;
    std::vector<package_item_t> item_vec;
    
    cli_user_swap_out();
    ~cli_user_swap_out();
    void init();
    int get_msg_len();
    bool read_from_buf(byte_array_t & ba);
    bool write_to_buf(byte_array_t & ba);
};

// PROTO_USER_BUY_ITEM_CMD			= 507,	//yxb购买物品
class cli_user_buy_item_in : public Cmessage
{
public:
	uint32_t item_id;
	uint32_t item_cnt;

	cli_user_buy_item_in();
    void init();
    int get_msg_len();
    bool read_from_buf(byte_array_t & ba);
    bool write_to_buf(byte_array_t & ba);
};

CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_buy_item_out, uint32_t, cost_yxb, uint32);

// PROTO_USER_EXPAND_PACKAGE_CMD	= 508,	//扩展背包
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_expand_package_in, uint32_t, package_index, uint32);
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_expand_package_out, uint32_t, cost_gold, uint32);

// PROTO_USER_ADJUST_PACKAGE_CMD    = 509,  //整理背包
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_adjust_package_in, uint32_t, package_type, uint32);

class cli_user_adjust_package_out : public Cmessage
{
public:
	uint32_t package_type;	// 1道具，4装备，3卷轴，2材料
	std::vector<package_item_t> item_vec;
	std::vector<package_equipment_t> equipment_vec;

	cli_user_adjust_package_out();
	~cli_user_adjust_package_out();
	void init();
    int get_msg_len();
    bool read_from_buf(byte_array_t & ba);
    bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_SELL_ITEM_CMD		= 510,	//用户卖出道具
class cli_user_sell_item_in : public Cmessage
{
public:
	uint32_t package_id;
	uint32_t item_id;
	uint32_t item_cnt;

	cli_user_sell_item_in();
    void init();
    int get_msg_len();
    bool read_from_buf(byte_array_t & ba);
    bool write_to_buf(byte_array_t & ba);
};

class cli_user_sell_item_out : public Cmessage
{
public:
	uint32_t package_id;
	uint32_t item_id;
	uint32_t item_cnt;
	uint32_t add_yxb;
	
	cli_user_sell_item_out();
    void init();
    int get_msg_len();
    bool read_from_buf(byte_array_t & ba);
    bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_TRAIN_EQUIPMENT_CMD  = 513,	//装备洗练
class cli_user_train_equipment_in : public Cmessage
{
public:
	uint32_t train_type;	//0 确认培养 1普通培养 2指定培养
	uint32_t package_id;
	uint32_t equipment_id;
	
	cli_user_train_equipment_in();
	void init();
    int get_msg_len();
    bool read_from_buf(byte_array_t & ba);
    bool write_to_buf(byte_array_t & ba);
};

class cli_user_train_equipment_out : public Cmessage
{
public:
	package_equipment_t equipment;
	cli_user_train_equipment_out();

	void init();
    int get_msg_len();
    bool read_from_buf(byte_array_t & ba);
    bool write_to_buf(byte_array_t & ba);
};

//	PROTO_USER_CHANGE_EQUIPMENT_CMD = 514,	//用户换装(1穿上,0脱下)
class cli_user_change_equipment_in : public Cmessage
{
public:    
    uint32_t is_wear;   //1穿上,0脱下
	uint32_t unit_id;	//换装的单位类型，自己还是伙伴
	uint32_t package_id;	 //所在背包中的ID
    uint32_t equipment_id;   //要处理的装备的ID
    
    cli_user_change_equipment_in();
    void init();
    int get_msg_len();
    bool read_from_buf(byte_array_t & ba);
    bool write_to_buf(byte_array_t & ba);
};

class cli_user_change_equipment_out : public Cmessage
{
public:
    uint32_t is_wear;
	uint32_t unit_id;	//换装的单位类型，自己还是伙伴
   	uint32_t on_package_id;		//操作之后的package_id
   	uint32_t on_old_package_id;	//操作之前的package_id
   	uint32_t off_package_id;	//is_wear=1时，脱下的
   	uint32_t off_old_package_id;//脱下之后的
    
    cli_user_change_equipment_out();
    void init();
    int get_msg_len();
    bool read_from_buf(byte_array_t & ba);
    bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_WEAR_EQUIPMENT_CMD	= 523,	//用户穿装(指定装备id而不指定package_id)	
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_wear_equipment_in, uint32_t, equipment_id, uint32);
typedef cli_user_train_equipment_out cli_user_wear_equipment_out;

//	PROTO_USER_STRENGTHEN_EQUIPMENT_CMD = 515,	//装备强化
class cli_user_strengthen_equipment_in : public Cmessage
{
public:
    uint32_t is_teen;//是否十次
    uint32_t package_id;
    uint32_t item_id;

    cli_user_strengthen_equipment_in();
    void init();
    int get_msg_len();
    bool read_from_buf(byte_array_t & ba);
    bool write_to_buf(byte_array_t & ba);
};

class cli_user_strengthen_equipment_out : public Cmessage
{
public:
    uint32_t cost_yxb;  //花费的游戏币
	uint32_t cd_time;	//强化后的CD时间
	uint32_t cd_state;	//强化后的CD类型

    cli_user_strengthen_equipment_out();
    void init();
    int get_msg_len();
    bool read_from_buf(byte_array_t & ba);
    bool write_to_buf(byte_array_t & ba);
};

//	PROTO_USER_UPGRADE_EQUIPMENT_CMD	= 516,	//装备进阶
class cli_user_upgrade_equipment_in : public Cmessage
{
public:
	uint32_t cost_gold;
    uint32_t package_id;
    uint32_t item_id;
 
    cli_user_upgrade_equipment_in();
    void init();
    int get_msg_len();
    bool read_from_buf(byte_array_t & ba);
    bool write_to_buf(byte_array_t & ba);   
};

class cli_user_upgrade_equipment_out : public Cmessage
{
public:
	uint32_t cost_gold;
    uint32_t package_id;
    uint32_t new_item_id;       //进阶后的装备ID
    uint32_t new_item_level;    //等级

    cli_user_upgrade_equipment_out();
    void init();
    int get_msg_len();
    bool read_from_buf(byte_array_t & ba);
    bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_PUNCH_EQUIPMENT_CMD	= 524,	//装备打孔
class cli_user_punch_equipment_in : public Cmessage
{
public:
    uint32_t package_id;
    uint32_t item_id;
    uint32_t position;//0，1，2分别代表第1-3个孔位
    
    cli_user_punch_equipment_in();
    void init();
    int get_msg_len();
    bool read_from_buf(byte_array_t & ba);
    bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_INLAY_GEM_CMD		= 525,	//装备镶嵌宝石
class cli_user_inlay_gem_in : public Cmessage
{
public:
	uint32_t op_type;//操作类型：0 镶嵌，1 卸下, 2 一键卸下(不需传position及gem_id)
    uint32_t package_id;
    uint32_t item_id;
    uint32_t position;//0，1，2分别代表第1-3个孔位
	uint32_t gem_id;//宝石的id

    cli_user_inlay_gem_in();
    void init();
    int get_msg_len();
    bool read_from_buf(byte_array_t & ba);
    bool write_to_buf(byte_array_t & ba);
};
typedef cli_user_inlay_gem_in cli_user_inlay_gem_out;

//PROTO_USER_COMPOSE_GEM_CMD		= 526,	//宝石合成
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_compose_gem_in, uint32_t, gem_id, uint32);//合成的目标宝石id

class cli_user_compose_gem_out : public Cmessage
{
public:
    uint32_t del_gem_id;//消耗的宝石的id
    uint32_t del_gem_cnt;//消耗的宝石的数量
    uint32_t del_rune_id;//消耗的合成符的id
	uint32_t del_rune_cnt;//消耗的合成符的数量
 
    cli_user_compose_gem_out();
    void init();
    int get_msg_len();
    bool read_from_buf(byte_array_t & ba);
    bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_TRAIN_BASE_ATTR_CMD = 518,    //培养
class cli_user_train_base_attr_in : public Cmessage
{
public:
	uint32_t unit_id;		//人物ID
    uint32_t train_type;
    
    cli_user_train_base_attr_in();
    void init();
    int get_msg_len();
    bool read_from_buf(byte_array_t & ba);
    bool write_to_buf(byte_array_t & ba);
};

class cli_user_train_base_attr_out : public Cmessage
{
public:
	uint32_t unit_id;
    int      train_hp;                //培养增加的生命值
    int      train_out_attack;        //培养增加的攻击力
    int      train_out_def;           //培养增加的防御力
    int      train_in_attack;         //培养增加的内攻
    int      train_in_def;            //培养增加的内防
	int      train_toughness;             //培养增加的韧性
    
    cli_user_train_base_attr_out();
    void init();
    int get_msg_len();
    bool read_from_buf(byte_array_t & ba);
    bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_CONFIRM_TRAIN_CMD = 519,     //确定培养
typedef cli_user_train_base_attr_out cli_user_confirm_train_out;

//	PROTO_USER_GET_SKILL_CMD		= 520,	//用户拉取所有技能
class cli_user_get_skill_in : public Cmessage
{
public:
    uint32_t userid;
    
    cli_user_get_skill_in();
    void init();
    int get_msg_len();
    bool read_from_buf(byte_array_t & ba);
    bool write_to_buf(byte_array_t & ba);
};

class cli_user_get_skill_out : public Cmessage
{
public:
    std::vector<user_skill_t> skill_vec;
    
    cli_user_get_skill_out();
    ~cli_user_get_skill_out();
    void init();
    int get_msg_len();
    bool read_from_buf(byte_array_t & ba);
    bool write_to_buf(byte_array_t & ba);
};

//	PROTO_USER_UPGRADE_SKILL_CMD	= 521,	//技能升级
class cli_user_upgrade_skill_in : public Cmessage
{
public:
    uint32_t skill_id;		//技能ID
   	uint32_t skill_level;

    cli_user_upgrade_skill_in();
    void init();
    int get_msg_len();
    bool read_from_buf(byte_array_t & ba);
    bool write_to_buf(byte_array_t & ba);
};

class cli_user_upgrade_skill_out : public Cmessage
{
public:
    uint32_t skill_id;
    uint32_t skill_level;
	uint32_t cost_yxb;	//消耗的游戏币
	uint32_t cost_power;//消耗的内力值
    
    cli_user_upgrade_skill_out();
    void init();
    int get_msg_len();
    bool read_from_buf(byte_array_t & ba);
    bool write_to_buf(byte_array_t & ba);
};

//	PROTO_USER_CHANGE_SKILL_CMD		= 522,	//用户装载技能(装载,卸下)
class cli_user_change_skill_in : public Cmessage
{
public:
    uint32_t skill_id;		//技能ID
    uint32_t wear_seat;		//0卸下 1装载位置一 2装载位置二
    
    cli_user_change_skill_in();
    void init();
    int get_msg_len();
    bool read_from_buf(byte_array_t & ba);
    bool write_to_buf(byte_array_t & ba);
};

class cli_user_change_skill_out : public Cmessage
{
public:
	uint32_t skills[4];			//招式(技能ID*1000+等级)
    
    cli_user_change_skill_out();
    void init();
    int get_msg_len();
    bool read_from_buf(byte_array_t & ba);
    bool write_to_buf(byte_array_t & ba);
};


//PROTO_USER_UPGRADE_FORCE_CMD  	= 530,  //练气
class cli_user_upgrade_force_in : public Cmessage
{
public:
	uint32_t type;		//练气的类型 1丹药 2元宝
	uint32_t cost_value;//练气的次数

	cli_user_upgrade_force_in();
	void init();
	int get_msg_len();
    bool read_from_buf(byte_array_t & ba);
    bool write_to_buf(byte_array_t & ba);
};

class cli_user_upgrade_force_out : public Cmessage
{
public:
	uint32_t is_crit;//是否暴击
	uint32_t force_exp;//增加的经验
	uint32_t type;		//练气的类型 1丹药 2元宝
	uint32_t cost_value;//消耗丹药或元宝的数量
	uint32_t is_finished;//批量是否全部完成

	cli_user_upgrade_force_out();
	void init();
	int get_msg_len();
    bool read_from_buf(byte_array_t & ba);
    bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_UPGRADE_NERVE_CMD    = 531,  //经脉升级
class cli_user_upgrade_nerve_in : public Cmessage
{
public:
	uint32_t is_batch;
	uint32_t nerve_id;
	uint32_t nerve_type;//1普通 2元宝

	cli_user_upgrade_nerve_in();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

class cli_user_upgrade_nerve_out : public Cmessage
{
public:
	uint32_t upgrade_succ;
	uint32_t cost_air;
	uint32_t cost_gold;
	uint32_t cd_type;
	uint32_t cd_time;
	uint32_t cd_state;
	uint32_t add_level;
	uint32_t inject_times;//注入次数

	cli_user_upgrade_nerve_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_GET_COOLDOWN_CMD = 532, //得到某类型的CD时间
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_get_cooldown_in, uint32_t, cd_type, uint32);

class cli_user_get_cooldown_out : public Cmessage
{
public:
	uint32_t cd_type;
	uint32_t cd_time;
	uint32_t cd_state;
	
	cli_user_get_cooldown_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_CLEAR_COOLDOWN_CMD = 533, //清楚某类型的CD时间
class cli_user_clear_cooldown_in: public Cmessage
{
public:
	uint32_t cd_type;
	uint32_t cost_gold;
	
	cli_user_clear_cooldown_in();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

typedef cli_user_clear_cooldown_in cli_user_clear_cooldown_out;
//CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_clear_cooldown_out, uint32_t, cost_gold, uint32);

//PROTO_USER_GET_DAILY_EVENT_CMD	= 534,	//得到某事件的完成次数
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_get_daily_event_in, uint32_t, event_type, uint32);
class cli_user_get_daily_event_out : public Cmessage
{
public:
	uint32_t event_type; 
	uint32_t event_count;
	
	cli_user_get_daily_event_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};


//PROTO_USER_GET_TYPE_EVENT_CMD	= 536,	//得到t_user_type_event表中某类型的值
typedef cli_user_get_daily_event_in cli_user_get_type_event_in;
typedef cli_user_get_daily_event_out cli_user_get_type_event_out;

//PROTO_USER_GET_MISSION_CMD      = 540,  //拉去关卡信息 map_id为社区id
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_get_mission_in, uint32_t, map_id, uint32);

class cli_user_get_mission_out: public Cmessage
{
public:
	uint32_t is_reseted;
	std::vector<user_mission_t> mission_vec;

	cli_user_get_mission_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_GET_HARD_MISSION_CMD	= 539,	//拉取困难关卡信息
typedef cli_user_get_mission_in cli_user_get_hard_mission_in;
typedef cli_user_get_mission_out cli_user_get_hard_mission_out;

//PROTO_USER_GET_BOSS_MISSION_CMD      = 656,  //拉取BOSS关卡信息 map_id为社区id
typedef cli_user_get_mission_in cli_user_get_boss_mission_in;
typedef cli_user_get_mission_out cli_user_get_boss_mission_out;

//PROTO_USER_BUY_ENERY_CMD        = 541,  //黄金购买体力
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_buy_enery_in, uint32_t, cost_gold, uint32);

class cli_user_buy_enery_out : public Cmessage
{
public:
	uint32_t cost_gold;//消耗的黄金
	uint32_t cur_enery;//当前的总体力
	uint32_t add_enery;//购买增加的体力
	uint32_t enery_counter;//下次体力回复的倒计时

	cli_user_buy_enery_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_RECOVER_ENERY_CMD    = 542,  //恢复体力
class cli_user_recover_enery_out : public Cmessage
{
public:
	uint32_t cur_enery;//当前的总体力
	uint32_t enery_max;
	uint32_t enery_counter;
	
	cli_user_recover_enery_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_START_AUTO_BATTLE_CMD= 543,  //开始扫荡
class cli_user_start_auto_battle_in : public Cmessage
{
public:
	uint32_t map_id;//关卡ID
	uint32_t battle_times;//扫荡次数
	
	cli_user_start_auto_battle_in();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

class cli_user_start_auto_battle_out : public Cmessage
{
public:
	uint32_t wait_flag;//是否需要等待
	std::vector<auto_battle_t> prize_vec;

	cli_user_start_auto_battle_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_STOP_AUTO_BATTLE_CMD = 544,  //结束扫荡

//PROTO_USER_GET_ROUNDS_BATTLE_CMD= 545,	//得到当前扫荡的结果
class cli_user_get_rounds_battle_out : public Cmessage
{
public:
	uint32_t is_elite;		//是否为精英副本
	uint32_t map_id;		//精英副本时为社区id, 普通扫荡时为关卡id
	uint32_t battle_counter;//战斗倒计时
	uint32_t battle_times;	//剩余战斗次数
	uint32_t cur_round;		//当前战斗是第几轮
	std::vector<auto_battle_t> prize_vec;//战斗奖励

	cli_user_get_rounds_battle_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_SPEED_AUTO_BATTLE_CMD= 546,  //加速扫荡(立即完成)
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_speed_auto_battle_in, uint32_t, cost_gold, uint32);//消耗的黄金

class cli_user_speed_auto_battle_out : public Cmessage
{
public:
	uint32_t is_package_full;//格子已满,TODO:DEL
	uint32_t cost_gold;		//加速花费的黄金
	uint32_t map_id;
	uint32_t battle_counter;//倒计时,TODO:DEL
	uint32_t battle_times;	//剩余战斗次数,TODO:DEL
	uint32_t cur_round;		//当前战斗时第几轮,TODO:DEL
	std::vector<auto_battle_t> prize_vec;//战斗奖励
	
	cli_user_speed_auto_battle_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_BATTLE_START_CMD		= 547,	//进入关卡
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_battle_start_in, uint32_t, map_id, uint32);

class cli_user_battle_start_out : public Cmessage
{
public:
	uint32_t start_time;
	uint32_t battle_key;
	uint32_t hp_item_cnt;	//生命药剂的数量
	std::vector<item_unit_t> item_vec;//掉落物品上限
    
    uint32_t thief_start_time;	//小偷开始出现时间
    uint32_t thief_interval_time;//小偷出现的时间间隔
    std::vector<item_unit_t> thief_item_vec;//小偷的掉落物品

	cli_user_battle_start_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

struct born_monster_info_t
{
	uint16_t born_second;	//第几秒刷出
	uint16_t monster_id;	//刷出怪的ID
};

struct kill_monster_info_t
{
	uint16_t monster_id;
	uint16_t kill_cnt;
};

//PROTO_USER_BATTLE_OVER_CMD		= 548,	//关卡战斗结束
class cli_user_battle_over_in : public Cmessage
{
public:
	uint32_t map_id;
	uint32_t over_reason;		//0战斗失败 1战斗胜利 2战斗中退出 3超时结束
	uint32_t kill_monster[4];	//杀死怪物数量 0随从 1普通 2随从 3boss
	uint32_t add_yxb;
	uint32_t add_exp;
	uint32_t add_gold;
	uint32_t used_item[3];		//使用的物品ID(ID)
	uint32_t hp_item_cnt;		//使用的血瓶数量
	uint32_t max_combo;
	uint32_t be_hit_count;
	uint32_t hp_remain_rate;//战斗后剩余的血量值 整数% 0-100
	
	std::vector<kill_monster_info_t> kill_vec;	//杀怪的信息(杀死多少只怪)
	std::vector<born_monster_info_t> born_info;	//刷怪的信息，用户校验
	std::vector<item_unit_t> item_vec;			//掉落的物品信息
    std::vector<item_unit_t> thief_item_vec;    //小偷的物品信息

	cli_user_battle_over_in();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

class cli_user_battle_over_out : public Cmessage
{
public:
	uint32_t map_type;
	uint32_t first_pass;	//首次通关 0没有结束 1是 2不是
	uint32_t score;			//评分
	uint32_t first_gold;	//首次通关获取老虎机所有物品所需的黄金数
	std::vector<item_unit_t> first_prize;	//首次通关奖励
	std::vector<item_unit_t> card_vec;		//首次过关，翻牌
	std::vector<item_unit_t> pass_prize;	//通关奖励
	
	cli_user_battle_over_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_GET_BATTLE_PRIZE_CMD   = 549,  //翻牌获得奖励
class cli_user_get_battle_prize_in : public Cmessage
{
public:
	uint32_t map_id;
	uint32_t cost_gold;
	uint32_t card_state; //翻第几次牌(1-3)

	cli_user_get_battle_prize_in();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

class cli_user_get_battle_prize_out : public Cmessage
{
public:
	item_unit_t card;//翻牌得到的奖励
	uint32_t gift_state;	//0放到背包中了 1放到个人礼包中了

	cli_user_get_battle_prize_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_BALLOT_CMD			= 550,	//藏经阁抽签(确定或抽签)
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_ballot_in, uint32_t, ballot_type, uint32);//抽签类型ballot_type=0确定 1普通 2高级 3钻石

class cli_user_ballot_out : public Cmessage
{
public:
	float mul;	//倍数
	uint32_t cost_yxb;
	uint32_t cost_gold;
	std::vector<item_unit_t> item_vec;

	cli_user_ballot_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_GET_ALL_BATTLE_PRIZE_CMD	= 551,	//获得所有老虎机的奖励
class cli_user_get_all_battle_prize_out : public Cmessage
{
public:
	uint32_t cost_gold;	//花费的黄金
	uint32_t gift_state;	//0放到背包中了 1放到个人礼包中了
	//std::vector<item_unit_t> prize_vec;
	
	cli_user_get_all_battle_prize_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_USE_PARTNER_CARD_CMD= 556,	//使用弟子卡牌
class cli_user_use_partner_card_in : public Cmessage
{
public:
	uint32_t card_id;
	uint32_t package_id;

	cli_user_use_partner_card_in();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_INHERIT_PARTNER_CMD = 558,	//弟子传承
class cli_user_inherit_partner_in : public Cmessage
{
public:
	uint32_t partner_id;		//传承者
	uint32_t target_id;			//继承者
	uint32_t use_gold;			//是否使用黄金

	cli_user_inherit_partner_in();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

class cli_user_inherit_partner_out : public Cmessage
{
public:
	uint32_t partner_id;
	uint32_t target_id;
	uint32_t cost_gold;
	uint32_t exp;
    int      train_hp;                //培养增加的生命值
    int      train_out_attack;        //培养增加的攻击力
    int      train_out_def;           //培养增加的防御力
    int      train_in_attack;         //培养增加的内攻
    int      train_in_def;            //培养增加的内防
	int      train_toughness;         //培养增加的内力
	uint32_t quart;
	
	cli_user_inherit_partner_out();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_UPGRADE_PARTNER_SKILL_CMD
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_upgrade_partner_skill_in, uint32_t, partner_id, uint32);//需要升级的弟子ID

class cli_user_upgrade_partner_skill_out : public Cmessage
{
public:
	uint32_t is_succ;		//是否成功
	std::vector<item_unit_t> cost_vec;		//消耗的物品

	cli_user_upgrade_partner_skill_out();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_GET_JAR_LIST_CMD     = 564,  //得到寻宝的罐子的列表
class cli_user_get_jar_list_out : public Cmessage
{
public:
	uint8_t jar_level[16];

	cli_user_get_jar_list_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_UPGRADE_JAR_CMD      = 565,  //罐子升级
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_upgrade_jar_in, uint32_t, type, uint32);//type=0普通 1高级 2钻石
typedef cli_user_get_jar_list_out cli_user_upgrade_jar_out;

//PROTO_USER_BREAK_JAR_CMD        = 566,  //砸罐子
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_break_jar_in, uint32_t, jar_info, uint32); 	//jar_info = jar_id << 8 + jar_level;

class cli_user_break_jar_out : public Cmessage
{
public:
	item_unit_t prize;

	cli_user_break_jar_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_BUY_HAMMER_CMD       = 567,  //购买用于砸罐子的锤子
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_buy_hammer_in, uint32_t, cost_gold, uint32);

class cli_user_buy_hammer_out : public Cmessage
{
public:
	uint32_t cost_gold;

	cli_user_buy_hammer_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_SWAP_PARTNER_SEAT_CMD= 569,	//上阵侠客交换位置
class cli_user_swap_partner_seat_in : public Cmessage
{
public:
	uint32_t seat_idx_1;
	uint32_t seat_idx_2;//seat_idx_1与seat_index_2位置的侠客互换
	
	cli_user_swap_partner_seat_in();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

typedef cli_user_swap_partner_seat_in cli_user_swap_partner_seat_out;

//PROTO_USER_ENLIST_PARTNER_CMD   = 570,  //招募侠客
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_enlist_partner_in, uint32_t, partner_id, uint32);

class cli_user_enlist_partner_out : public Cmessage
{
public:
	uint32_t partner_id;
    uint32_t cost_money;
    uint32_t cost_gold;

	cli_user_enlist_partner_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_GET_PARTNER_LIST_CMD = 571,  //侠客列表
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_get_partner_list_in, uint32_t, type, uint32);//type=0所有侠客(已上阵 未上阵 放逐的) type=1为已上阵侠客 2未上阵侠客 3放逐的侠客 4已带着的侠客(以上阵 未上阵)

class cli_user_get_partner_list_out : public Cmessage
{
public:
	std::vector<partner_info_t> 	 partner_vec;
	
	cli_user_get_partner_list_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_ONPLAY_PARTNER_CMD   = 572,  //侠客上阵
class cli_user_onplay_partner_in : public Cmessage
{
public:
	uint32_t partner_id;
	uint32_t seat_index;	//从1开始
	
	cli_user_onplay_partner_in();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

class cli_user_onplay_partner_out : public Cmessage
{
public:
	uint32_t on_partner_id;	//换上阵的侠客ID
	uint32_t on_seat_index;	//换上阵的侠客位置
	uint32_t off_partner_id;//换下来的侠客ID

	cli_user_onplay_partner_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_OFFPLAY_PARTNER_CMD   = 573,  //侠客下阵
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_offplay_partner_in, uint32_t, seat_id, uint32);//位置从1开始

//PROTO_USER_TRAVEL_PARTNER_CMD   = 576,      //侠客游历
class cli_user_travel_partner_in : public Cmessage
{
public:
	uint32_t partner_id;
	uint32_t travel_type;	//0、游戏币 1、10黄金 2、30黄金 3、60黄金
	uint32_t is_ten_travel;

	cli_user_travel_partner_in();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

class cli_user_travel_partner_out : public Cmessage
{
public:
	uint32_t partner_id;
	uint32_t qualify_value;	//增加之后的资质
	uint32_t cost_yxb;
	uint32_t cost_gold;
	
	cli_user_travel_partner_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_PARTNER_EAT_EXP_CMD	= 577,		//吃经验丹
class cli_user_partner_eat_exp_in : public Cmessage
{
public:
	uint32_t unit_id;
	item_unit_t item;
	
	cli_user_partner_eat_exp_in();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

class cli_user_partner_eat_exp_out : public Cmessage
{
public:
	uint32_t unit_id;
	uint32_t add_exp;

	cli_user_partner_eat_exp_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_RECALL_PARTNER_CMD	= 578,		//用户放逐和召回侠客
class cli_user_recall_partner_in : public Cmessage
{
public:
	uint32_t partner_id;
	uint32_t recall;		//0为放逐 1为召回 

	cli_user_recall_partner_in();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

class cli_user_recall_partner_out : public Cmessage
{
public:
	uint32_t is_recall;		//0为放逐 1为召回
	partner_info_t partner;
	
	cli_user_recall_partner_out();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_GET_PRESTIGE_TITLE_CMD	= 579,	//得到声望挑战信息
class cli_user_get_prestige_title_out : public Cmessage
{
public:
	uint32_t prestige_title;	//当前声望封号
	uint32_t is_get_prize;		//是否领取了奖励
	
	cli_user_get_prestige_title_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_GET_PRESTIGE_PRIZE_CMD	= 580,	//领取
class cli_user_get_prestige_prize_out : public Cmessage
{
public:
	std::vector<item_unit_t> prize_vec;

	cli_user_get_prestige_prize_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_PRESTIGE_LOTTERY_CMD	= 581,	//声望摇奖
class cli_user_prestige_lottery_in : public Cmessage
{
public:
	uint32_t lottery_type;	//0 1 2
	uint32_t is_gold;		//是否是用黄金

	cli_user_prestige_lottery_in();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

class cli_user_prestige_lottery_out : public Cmessage
{
public:
	uint32_t select_right;
	uint32_t cost_yxb;
	uint32_t cost_gold;
	uint32_t add_prestige;

	cli_user_prestige_lottery_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_GET_TASK_LIST_CMD	= 585,	//得到任务列表
class cli_user_get_task_list_out : public Cmessage
{
public:
	std::vector<task_info_t> task_vec;//主线 支线任务(在任务面板中显示的任务)
	
	cli_user_get_task_list_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_GET_TASK_CMD			= 586,	//获得任务当前完成情况

//PROTO_USER_ACCEPT_TASK_CMD		= 587,	//接取任务
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_accept_task_in, uint32_t, task_id, uint32);
//若接取时支线任务时完成条件已经达成，则原样返回task_id, 其他情况均返回0
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_accept_task_out, uint32_t, task_id, uint32);

//PROTO_USER_SET_TASK_CMD			= 588,	//设置任务
class cli_user_set_task_in : public Cmessage
{
public:
	uint32_t task_id;
	uint32_t add_step;	//增加进度
	
	cli_user_set_task_in();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//设置任务的返回可以作为后端主动发送的任务任务状态改变的协议
typedef cli_user_get_task_list_out cli_user_set_task_out;
/*
class cli_user_set_task_out : public Cmessage
{
public:
	std::vector<task_info_t> task_vec;//主线 支线任务(在任务面板中显示的任务)
	
	cli_user_set_task_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};
*/

//PROTO_USER_FINISH_TASK_CMD		= 589,	//完成任务
class cli_user_finish_task_in : public Cmessage
{
public:
	uint32_t task_id;
	uint32_t use_gold;		//是否使用黄金
	
	cli_user_finish_task_in();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

class cli_user_finish_task_out : public Cmessage
{
public:
	uint32_t task_id;
	uint32_t cost_gold;
	std::vector<item_unit_t> prize_vec;
	
	cli_user_finish_task_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_DROP_TASK_CMD		= 590,	//放弃任务
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_drop_task_in, uint32_t, task_id, uint32);

//PROTO_USER_GET_AWARD_TASK_CMD		= 591,	//点开悬赏面板时
class cli_user_get_award_task_out : public Cmessage
{
public:
	uint32_t daily_count;		//今日剩余次数
	uint32_t scroll_count[4];		//英雄榜数量
	//悬赏任务
	uint32_t award_counter;			//倒计时
	award_task_info_t award_task[4];//悬赏任务
	
	cli_user_get_award_task_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_ACCEPT_AWARD_TASK_CMD 	= 592,	//接取悬赏任务
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_accept_award_task_in, uint32_t, task_id, uint32);

//PROTO_USER_UPGRADE_AWARD_TASK_CMD 	= 593,	//一件满星
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_upgrade_award_task_in, uint32_t, task_id, uint32);

class cli_user_upgrade_award_task_out : public Cmessage
{
public:
	uint32_t task_id;
	uint32_t cost_gold;
	
	cli_user_upgrade_award_task_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_SET_AWARD_TASK_FINISH_CMD = 594,	//一键完成
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_set_award_task_finish_in, uint32_t, task_id, uint32);
typedef cli_user_upgrade_award_task_out cli_user_set_award_task_finish_out;

//PROTO_USER_FINISH_AWARD_TASK_CMD	= 595,	//完成后提交任务
class cli_user_finish_award_task_in : public Cmessage
{
public:
	uint32_t task_id;
	uint32_t item_id;		//英雄帖类型

	cli_user_finish_award_task_in();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

class cli_user_finish_award_task_out: public Cmessage
{
public:
	uint32_t task_id;
	std::vector<item_unit_t> prize_vec;

	cli_user_finish_award_task_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_REFRESH_AWARD_TASK_CMD	= 596,	//刷新悬赏任务
class cli_user_refresh_award_task_out : public Cmessage
{
public:
	uint32_t cost_yxb;
	uint32_t award_counter;			//倒计时
	award_task_info_t award_task[4];//悬赏任务

	cli_user_refresh_award_task_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_SET_GUIDE_STEP_CMD	= 597,	//设置引导步骤
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_set_guide_step_in, uint32_t, guide_step, uint32);

//PROTO_USER_GET_ATTR_ITEM_CMD	= 600,	//得到itemid类型为0的属性值
class cli_user_get_attr_item_out : public Cmessage
{
public:
	uint32_t yxb;
	uint32_t exp;
	uint32_t gold;
	uint32_t buy_gold;
	uint32_t power_value;
	uint32_t nerve_value;
	uint32_t enery;
	uint32_t enery_counter;//下次恢复精力的倒计时
	uint32_t star_exp;
	uint32_t package_cnt;	//当前物品数量
	uint32_t max_package_size;//背包容量
	
	cli_user_get_attr_item_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};


//PROTO_USER_GET_MAGIC_STORE_CMD	= 610,	//获得神秘商店
class cli_user_get_magic_store_out : public Cmessage
{
public:
	uint32_t left_second;
	std::vector<sell_item_t> sell_vec;
	
	cli_user_get_magic_store_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_BUY_MAGIC_ITEM_CMD	= 611,	//购买神秘商店中的物品
class cli_user_buy_magic_item_in : public Cmessage
{
public:
	uint32_t item_index;
	uint32_t cost_yxb;
	
	cli_user_buy_magic_item_in();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

class cli_user_buy_magic_item_out : public Cmessage
{
public:
	item_unit_t item;
	uint32_t cost_yxb;
	
	cli_user_buy_magic_item_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_FLUSH_MAGIC_STORE_CMD= 612,	//刷新神秘商店
class cli_user_flush_magic_store_out : public Cmessage
{
public:
	uint32_t cost_gold;
	uint32_t left_second;
	std::vector<sell_item_t> sell_vec;

	cli_user_flush_magic_store_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_GET_PLATFORM_BONUS_CMD = 620,	//领取奖励平台的奖励
class cli_user_get_platform_bonus_in : public Cmessage
{
public:
	uint32_t bonus_type;	//1100 -- 1110
	uint32_t bonus_index;	//各类型的第几个奖励
	
	cli_user_get_platform_bonus_in();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

class cli_user_get_platform_bonus_out : public Cmessage
{
public:
	uint32_t bonus_type;
	uint32_t swap_id;
	uint32_t rand_idx;	//如果swap_id是rand类型的，则要返回rand出来的奖励index
	//std::vector<item_unit_t> del_bonus_vec;//扣除的物品，如扣除相应的黄金等
	std::vector<item_unit_t> add_bonus_vec;//获得的奖励

	cli_user_get_platform_bonus_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_LOGIN_SIGN_CMD		= 621,		//签到,sign_type = 0是签当天的 1是补签
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_login_sign_in, uint32_t, sign_type, uint32);
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_login_sign_out, uint32_t, cost_gold, uint32);

//PROTO_USER_GET_TESTING_RANK_CMD	= 622,	//得到封测等级和战力排行
class cli_user_get_testing_rank_out : public Cmessage
{
public:
	std::vector<rank_info_t> rank_level;
	std::vector<rank_info_t> rank_fight;

	cli_user_get_testing_rank_out();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_CHEAT_ITEM_CMD		= 1000,	//用户添加物品
class cli_user_cheat_item_in: public Cmessage
{
public:
	uint32_t item_id;
	int32_t  item_cnt;

	cli_user_cheat_item_in();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};


//PROTO_USER_CHAT_CMD          = 630,//客户端发送聊天消息到服务器
class cli_user_chat_in: public Cmessage
{
public:
	uint32_t channel_id;//频道ID 1世界 2好友 3帮派 4群组 5地图 
	uint32_t target_id;//接收对象根据频道进行区分 1是全部玩家 2是好友USERID  3帮派ID 4群组ID 5是地图ID<具体根据需要进行修改>
	uint32_t mlen;//消息内容长度,可定会小于255了
	char msg[255];
	
	cli_user_chat_in();
	void init();
	int get_msg_len();//计算消息包体的大小
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//考虑到简洁只好用union了
union notice_message_t {
	//channel_id:600  公告或是char字符一类的
	char buf[255];

	//channel_id:601	获取道具
	struct {
		uint8_t type;//1 掉落，2是抽取
		uint8_t cnt;//掉落数量
		item_unit_t items[0];//掉落ITEM
	} items;

	//channel_id:602	次数变化
	struct times {
		uint16_t type;//记录类型 1竞技场连胜次数
		uint32_t cnt;//变化次数
	} times;

	//channel_id:603	等级变化
	struct vip_change {
		uint8_t	type;//1是VIP等级
		uint8_t old_level;//变化前VIP
		uint8_t new_level;//变化后VIP
	} vip_change;

	//channel_id:604	活动通知
	struct activity {
		uint8_t type;//1是活动开启
		uint32_t activity_id;//活动ID
	} activity;

	//channel_id:605	弟子通知
	struct partner {
		uint8_t type;//1是招募
		uint32_t partner_id;//弟子ID
	} partner;

	//channel_id:606   开宝箱通知
	struct openbox {
		uint32_t box_item_id;//宝箱ID
		uint32_t item_id;//得到的ID
	} openbox;

	//channel_id:607   玩家算卦获得橙色星石
	struct fortunes {
		uint32_t item_id;
	} fortunes;

	//channel_id:608   欢迎广播
	struct welcome {
		uint32_t frequency;//频率
		char buf[200];
	} welcome;

	//channel_id:609   击杀BOSS
	struct kill_world_boss {
		uint32_t boss_id;//boss_id
	} kill_world_boss;

	//channel_id:610  橙色马押镖
	struct horse_escort {
		uint32_t horse_quality; 
	} horse_escort;

	//channel_id:611  爬塔
	struct {
		uint32_t tower_floor;	//多少层ID
	} tower_info;

	//channel_id:612  神仙姐姐
	struct {
		uint32_t item_id;
		uint32_t item_cnt;
	} worship_fairy;

	//channel_id:613  竞技场
	struct {
		uint32_t is_rank_one;	//打赢第一名了
		char	 nick[32];
		uint32_t combo_win;		//连胜多少场
	} arena_battle;
}__attribute__((packed));

class cli_user_chat_out: public Cmessage
{
public:
	uint32_t userid;//发送者ID
	uint8_t nick[32];//昵称
	uint32_t level;//等级
	uint32_t unit_id;//3种职业，只能是1 2 3
	uint32_t chattime;//聊天的时间戳
	uint32_t vip_level;//VIP等级
	uint32_t channel_id;//频道ID 1世界 2好友<20-30段好友操作  2好友聊天 21好友申请请求 22好友申请通过 23删除好友> 3帮派 4群组 5地图 600-699是公告相关 
	uint32_t target_id;//接收对象根据频道进行区分 1是全部玩家 2是好友USERID  3帮派ID 4群组ID 5是地图ID<具体根据需要进行修改>
	uint32_t mlen;//消息内容长度,可定会小于255了

	char msg[255];
	cli_user_chat_out();
	void init();
	int get_msg_len();//计算消息包体的大小
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_GET_USER_ONLINE_STATUS_CMD	= 639,	//查询玩家在线状态
class cli_user_get_user_online_status_in : public Cmessage
{
public:
	std::vector<uint32_t> uid_vec;
	
	cli_user_get_user_online_status_in();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

class cli_user_get_user_online_status_out : public Cmessage
{
public:
	std::vector<usr_online_status_t> info_list;
	
	cli_user_get_user_online_status_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_GET_OTHER_USER_CMD	= 640,	//得到其他用户信息
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_get_other_user_in, uint32_t, userid, uint32);
/*
class cli_user_get_other_user_in : public Cmessage
{
public:
	uint32_t userid;

	cli_user_get_other_user_in();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};
*/
class cli_user_get_other_user_out : public Cmessage
{
public:
	uint32_t userid;
	char	 nick[32];
	uint32_t unit_id;
	uint32_t vip_level;
	uint32_t level;
	uint32_t force_exp;		//练气经验
	uint32_t nerve_level;	//经脉等级
	uint32_t nerve_value;	//真气值
	uint32_t power_value;	//内功值（战斗获得）
	uint32_t prestige_value;//声望

    int      train_hp;                //培养增加的生命值
    int      train_out_attack;        //培养增加的攻击力
    int      train_out_def;           //培养增加的防御力
    int      train_in_attack;         //培养增加的内攻
    int      train_in_def;            //培养增加的内防
	int      train_toughness;         //培养增加的内力

	uint32_t skills[4];			//招式(技能ID*1000+等级)
	uint16_t passive_skills[4];	//心法的等级
	star_ston_info_t star_array[4];
	
	std::vector<package_equipment_t> equipment_vec;//已穿戴的装备
	std::vector<partner_info_t> 	 partner_vec;

	cli_user_get_other_user_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_GET_ARENA_INFO_CMD	= 641,	//得到竞技场信息
class cli_user_get_arena_info_out : public Cmessage
{
public:
	uint32_t daily_cnt;		//今日挑战剩余次数
	uint32_t arena_cd;		//竞技场挑战CD
	uint32_t can_get_prize;	//是否可以领取奖励,0不能领取，其它为能领取奖励的排名
	user_arena_rank_info_t top_rank[6];

	cli_user_get_arena_info_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_GET_ARENA_TOP_100_CMD= 642,	//得到竞技场top100
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_get_arena_top_100_in, uint32_t, cur_page, uint32);//rand_id 1 2 3名次

class cli_user_get_arena_top_100_out : public Cmessage
{
public:
	uint32_t cur_page;
	user_arena_rank_info_t top_rank[20];

	cli_user_get_arena_top_100_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

struct user_arena_file_info_t
{
	uint32_t type;			//挑战 1挑战 2被挑战
	uint32_t battle_id;		//挑战对象
	char	 battle_nick[32];	//挑战对象nick
	uint32_t battle_time;	//挑战时间
	uint32_t battle_result;	//战斗结果
	uint32_t new_rank;		//新的排名等级
};

//PROTO_USER_GET_ARENA_FILE_CMD	= 643,	//得到竞技场战报
class cli_user_get_arena_file_out : public Cmessage
{
public:
	std::vector<user_arena_file_info_t> arena_file_vec;

	cli_user_get_arena_file_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_WORSHIP_TOP_USER_CMD	= 644,	//1膜拜 2敬仰 3佩服	
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_worship_top_user_in, uint32_t, rank_id, uint32);//rand_id 1 2 3名次
	
class cli_user_worship_top_user_out : public Cmessage
{
public:
	uint32_t cost_yxb;
	uint32_t cost_gold;
	uint32_t add_prestige;	//增加声望

	cli_user_worship_top_user_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_START_ARENA_BATTLE_CMD = 645,//开始竞技场战斗
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_start_arena_battle_in, uint32_t, target_userid, uint32);//rand_id 1 2 3名次

class cli_user_start_arena_battle_out : public Cmessage
{
public:
	uint32_t userid;
	uint32_t unit_id;
	uint32_t cost_gold;
	char	 nick[32];
	uint32_t level;             //等级
	uint32_t life;             	//生命值
	float    out_attack;        //攻击力
	float    out_def;           //防御力
	float    in_attack;         //内攻
	float    in_def;            //内防
	float    crit;             //暴击
	float    toughness;        //韧性
	float    recover_hp;        //回血
	float    reduce_hurt;       //减伤
	float    add_hurt;          //伤害加深
	float    suck_hp;           //吸血 TODO:已废弃
	uint32_t skills[4];			//招式(技能ID*1000+等级)
	uint8_t	 battle_session[32];	//战斗session

	cli_user_start_arena_battle_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_OVER_ARENA_BATTLE_CMD = 646,//结束竞技场战斗
class cli_user_over_arena_battle_in : public Cmessage
{
public:
	uint32_t battle_result;		//0失败 1胜利
	uint32_t target_id;
	uint8_t battle_session[32];

	cli_user_over_arena_battle_in();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

class cli_user_over_arena_battle_out : public Cmessage
{
public:
	uint32_t battle_result;
	std::vector<item_unit_t> prize_vec;

	cli_user_over_arena_battle_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_GET_ARENA_RANK_PRIZE_CMD	= 647,//竞技场领取奖励
class cli_user_get_arena_rank_prize_out : public Cmessage
{
public:
	uint32_t prize_yxb;
	uint32_t prize_prestige;

	cli_user_get_arena_rank_prize_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_GET_WORSHIP_INFO_CMD	= 648,	//得到膜拜信息
class cli_user_get_worship_info_out : public Cmessage
{
public:
	uint32_t worship_field;		//worship_field & 0x01 (第一名已完成)
	user_arena_rank_info_t top_rank[3];

	cli_user_get_worship_info_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_SHOP_INFO			= 650,//拉取商城数据,传递参数 为1代表是道具 为2代表是礼包,后端返给给前端每页按12条返回
class cli_user_shop_info_in : public Cmessage
{
public:
	uint32_t shop_type;
	uint32_t page_no;
	cli_user_shop_info_in();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

struct shop_item_info_t
{
	uint32_t product_id;
	char title[24];
	char des[40];
	uint32_t flag1;//1 new 2 hot

	uint32_t show_price;
	uint32_t now_price;
	uint32_t vip_price;
	uint32_t vip_level;

	int32_t times;//限购买次数 为0就是不限制,非零就代表限制次数
	uint32_t second;//限购倒计时 为0就是不时间限制,非零就是代表倒计时

	std::vector<item_unit_t> item_vec;
};

class cli_user_shop_info_out : public Cmessage
{
public:
	uint32_t shop_type;
	uint32_t page_no;
	uint32_t record_num;
	std::vector<shop_item_info_t> shop_vec;

	cli_user_shop_info_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_SHOP_BUY				= 651,//购买商城物品	
class cli_user_shop_buy_in : public Cmessage
{
public:
	uint32_t product_id;
	uint32_t product_cnt;

	cli_user_shop_buy_in();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

class cli_user_shop_buy_out : public Cmessage
{
public:
	uint32_t product_id;
	uint32_t product_cnt;
	uint32_t gold_cnt;
	uint32_t move_to_gift;
	std::vector<item_unit_t> item_vec;

	cli_user_shop_buy_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_RAND_INFO_CMD		= 653,//排行榜数据
class cli_user_rank_info_in : public Cmessage
{
public:
	uint32_t rank_type;

	cli_user_rank_info_in();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

class cli_user_rank_info_out : public Cmessage
{
public:
	uint32_t rank_type;
	std::vector<rank_info_t> rank_vec;

	cli_user_rank_info_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_PRIVATE_GIFT_CMD = 654 个人礼包  type 1代表商城背包不足时过来的物品
class cli_user_gift_box_info_in : public Cmessage
{
public:
	uint32_t type;

	cli_user_gift_box_info_in();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

struct gift_box_t
{
	uint32_t id;
	uint32_t gift_id;
	uint32_t insert_time;
	char title[24];
	char des[40];
	std::vector<item_unit_t> items_vec;
};

class cli_user_gift_box_info_out : public Cmessage
{
public:
	uint32_t type;
	std::vector<gift_box_t> box_vec;

	cli_user_gift_box_info_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_PRIVATE_GIFT_GET_CMD = 655 个人礼包领取  type 1代表商城背包不足时过来的物品 id为后端传说给前端的ID
class cli_user_gift_box_get_in : public Cmessage
{
public:
	uint32_t type;
	uint32_t id;
	cli_user_gift_box_get_in();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

class cli_user_gift_box_get_out : public Cmessage
{
public:
	uint32_t type;
	uint32_t id;
	uint32_t result;//标识是否成功领取到背包1 是成功
	std::vector<item_unit_t> items_vec;

	cli_user_gift_box_get_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_GET_BOSS_MISSION_CMD      = 656,  //拉去BOSS关卡信息 map_id为社区id
typedef cli_user_get_mission_in cli_user_get_boss_mission_in;

//PROTO_USER_RESET_BOSS_MISSION_CMD =657,  //重置BOSS关卡 map_id为社区id
typedef cli_user_get_mission_in cli_user_reset_boss_mission_in;
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_reset_boss_mission_out, uint32_t, cost_gold, uint32);

//PROTO_USER_AUTO_BOSS_BATTLE_CMD = 658, //BOSS扫荡
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_auto_boss_battle_in, uint32_t, map_id, uint32);

class cli_user_auto_boss_battle_out : public Cmessage
{
public:
	uint32_t wait_flag;//是否需要等待
	std::vector<auto_battle_t> boss_vec;//击败的BOSS

	cli_user_auto_boss_battle_out();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_GET_MAX_MISSION_CMD = 659，//拉取开放的最大MISSION ID
class cli_user_get_max_mission_out : public Cmessage
{
public:
	std::vector<uint32_t> maxid_vec;//0 普通 1 精英 2 困难

	cli_user_get_max_mission_out();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_GET_VILLA_INFO_CMD	= 660,	//得到山庄信息
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_get_villa_info_in, uint32_t, userid, uint32);	//要拉取的用户ID,暂时只对自己有效

class cli_user_get_villa_info_out : public Cmessage
{
public:
	uint32_t land_sum;	//总共几块土地
	std::vector<item_unit_t> plant_vec;
	std::vector<villa_land_info_t> land_vec;	//只返回有种植的土地

	cli_user_get_villa_info_out();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_PLANT_CMD			= 661,	//种植种子
class cli_user_plant_in : public Cmessage
{
public:
	uint32_t plant_id;		//种子ID
	uint32_t land_id;		//土地ID

	cli_user_plant_in();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

class cli_user_plant_out : public Cmessage
{
public:
	villa_land_info_t land_info;

	cli_user_plant_out();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_SPEED_PLANT_CMD		= 662,	//加速植物
class cli_user_speed_plant_in : public Cmessage
{
public:
	uint32_t land_id;
	uint32_t plant_id;
	uint32_t cost_gold;

	cli_user_speed_plant_in();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

class cli_user_speed_plant_out : public Cmessage
{
public:
	uint32_t land_id;
	uint32_t cost_gold;

	cli_user_speed_plant_out();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_HAVEST_PLANT_CMD		= 663,	//收获果实
class cli_user_havest_plant_in : public Cmessage
{
public:
	uint32_t land_id;
	uint32_t plant_id;
	
	cli_user_havest_plant_in();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

class cli_user_havest_plant_out : public Cmessage
{
public:
	uint32_t land_id;
	item_unit_t	prize;		//种植获得的奖励

	cli_user_havest_plant_out();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_EXPAND_LAND_CMD		= 664,	//扩展土地
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_expand_land_in, uint32_t, land_id, uint32);

class cli_user_expand_land_out : public Cmessage
{
public:
	uint32_t land_id;		//土地ID
	uint32_t cost_yxb;		//花费的银两

	cli_user_expand_land_out();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_SHAKE_MONEY_CMD		= 665,	//摇钱树
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_shake_money_in, uint32_t, cost_gold, uint32);

class cli_user_shake_money_out : public Cmessage
{
public:
	uint32_t cost_gold;		//消耗黄金
	uint32_t add_yxb;		//获得银两
	uint32_t is_mul;		//本次金子是否点亮

	cli_user_shake_money_out();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_ACTIVITY_DICE_CMD					= 666, //活动摇色子
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_activity_dice_in, uint32_t, is_cost_gold, uint32);//0 游戏币, 1 黄金

class cli_user_activity_dice_out: public Cmessage
{
public:
	uint32_t enery;//增加的体力值
	uint32_t cost_gold;
	uint32_t cost_yxb;
	//下期开启时间
	cli_user_activity_dice_out();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_TOWER_CMD					= 667, //爬塔拉取信息
class cli_user_tower_out : public Cmessage
{
public:
	uint32_t cur_pos;
	uint32_t max_pos;
	uint32_t reseted_cnt;

	cli_user_tower_out();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_AUTO_TOWERBATTLE_CMD = 668,//爬塔自动挑战
typedef cli_user_auto_boss_battle_out cli_user_auto_towerbattle_out;

//PROTO_USER_RESET_TOWER_CMD		= 669, //重置爬塔次数
typedef cli_user_reset_boss_mission_out cli_user_reset_tower_out;

//PROTO_USER_GET_FRIEND_LIST_CMD  = 670, //拉取好友信息
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_get_friend_list_in, uint32_t, type, uint32);//0好友和好友请求  1好友 2好友请求 3黑名单 4在线玩家

struct offline_msg_t{
	uint32_t time;
	char msg[255];
};

struct friend_info_t {
	uint32_t type;//类型 1好友 2好友请求
	uint32_t friend_id;
	uint16_t svr_id;
	uint32_t unit_id;
	char	 friend_nick[32];
	uint32_t level;
	uint32_t time;
	std::vector<offline_msg_t> offline_vec;
	//uint32_t offline_cnt;//离线消息数
};

class cli_user_get_friend_list_out : public Cmessage
{
public:
	std::vector<friend_info_t> friend_vec;

	cli_user_get_friend_list_out();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_ADD_FRIEND_CMD       = 671, //用户添加好友或黑名单
class cli_user_add_friend_in : public Cmessage
{
public:
	char friend_nick[32];
	uint32_t friend_id;
	uint32_t svr_id;//好友的服id
	uint32_t is_black;//0好友 1 黑名单

	cli_user_add_friend_in();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_DEAL_FRIEND_REQUEST_CMD = 672,//用户处理好友请求
class cli_user_deal_friend_reqeust_in : public Cmessage
{
public:
	std::vector<uint32_t> friend_vec;
	uint32_t opt;		//0取消 1确认添加
	
	cli_user_deal_friend_reqeust_in();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_DEL_FRIEND_CMD       = 673, //用户删除好友或黑名单
typedef cli_user_add_friend_in cli_user_del_friend_in;

//PROTO_USER_RECV_FRIEND_REQUEST_CMD = 674,//用户主动收到好友请求
typedef cli_user_get_friend_list_out cli_user_recv_friend_request_out;

//PROTO_USER_FIND_USER_FROM_NICK_CMD = 675,//通过nick找到用户基本信息
class cli_user_find_user_from_nick_in : public Cmessage
{
public:
	char friend_nick[32];

	cli_user_find_user_from_nick_in();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

class cli_user_find_user_from_nick_out : public Cmessage
{
public:
	uint32_t userid;
	uint16_t svr_id;
	uint32_t unit_id;
	uint32_t level;
	char 	 nick[32];
	
	cli_user_find_user_from_nick_out();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_GET_VILLA_STATE_CMD	= 676,	//得到山庄状态
class cli_user_get_villa_state_out : public Cmessage
{
public:
	uint32_t plant_state;	//种植状态 0都没有 plant_state&0x01有可收获的植物 plant_state&0x02有可种植的地 

	cli_user_get_villa_state_out();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_GET_STAR_LIST_CMD    = 677,  //得到占星列表
class cli_user_get_star_list_out : public Cmessage
{
public:
	uint32_t astrologer_state;		//可占星的占星师状态，用位表示
	uint32_t star_chip_cnt;			//星石碎片数量
	std::vector<star_ston_info_t> star_vec;

	cli_user_get_star_list_out();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_ASTROLOGY_CMD        = 678,  //占星
class cli_user_astrology_in : public Cmessage
{
public:
	uint32_t astrology_type;	//1占星一次 2一键占星
	uint32_t astrologer_id;		//astrology_type=1，则需要填写

	cli_user_astrology_in();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

class cli_user_astrology_out : public Cmessage
{
public:
	uint32_t astrologer_state;	//占星师状态
	uint32_t cost_yxb;
	std::vector<star_ston_info_t> star_vec;
	std::vector<uint32_t> astrology_vec;	//随机的召唤师序列

	cli_user_astrology_out();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_TRANSFORM_STAR_CMD   = 679,  //转化单个星石
class cli_user_transform_star_in : public Cmessage
{
public:
	uint32_t unique_id;

	cli_user_transform_star_in();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_transform_star_out, uint32_t, transform_value, uint32);

//PROTO_USER_AUTO_TRANSFORM_STAR_CMD  = 680,  //一键转化星石
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_auto_transform_star_in, uint32_t, star_state, uint32);
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_auto_transform_star_out, uint32_t, transform_value, uint32);

//PROTO_USER_EQUIP_STAR_CMD       = 681,  //装备或卸下星石
class cli_user_equip_star_in : public Cmessage
{
public:
	uint32_t equip_type;	//1穿上 2卸下
	uint32_t unique_id;
	uint32_t seat_id;

	cli_user_equip_star_in();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

class cli_user_equip_star_out : public Cmessage
{
public:
	uint32_t equip_type;	//1穿上 2卸下
	uint32_t unique_id;		//star_id
	uint32_t seat_id;		//穿上的位置
	
	cli_user_equip_star_out();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_UPGRADE_STAR_CMD     = 682,  //升级星石
class cli_user_upgrade_star_in : public Cmessage
{
public:
	uint32_t unique_id;
	
	cli_user_upgrade_star_in();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

class cli_user_upgrade_star_out : public Cmessage
{
public:
	uint32_t cost_star_exp;	//消耗星石经验
	uint32_t unique_id;

	cli_user_upgrade_star_out();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_CALL_ASTROLOGER_CMD	= 683,	//召唤占星师
class cli_user_call_astrologer_out : public Cmessage
{
public:
	uint32_t astrologer_state;
	uint32_t cost_gold;

	cli_user_call_astrologer_out();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_CHOOSE_HORSE_CMD = 684, //选马了
class cli_user_choose_horse_out : public Cmessage
{
public:
	uint32_t horse_quality;//马的品质
	uint32_t refreshed_cnt;//已经刷新的次数
	uint32_t escort_time;//押运的次数
	uint32_t keap_time;//劫持的次数
	cli_user_choose_horse_out();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_REFRESH_HORSE_CMD = 685, //刷马
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_refresh_horse_in, uint32_t, is_gold, uint32);//0 是不花钱 1是花钱招橙色

class cli_user_refresh_horse_out : public Cmessage
{
public:
	uint32_t horse_quality;//马的品质
	uint32_t refreshed_cnt;//已经刷新的次数
	uint32_t cost_gold;//扣除的费用
	cli_user_refresh_horse_out();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_GET_ESCORT_INFO_CMD = 686, //押镖运信息
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_get_escort_info_in, uint32_t, is_new, uint32);//0全屏数据,1拉取最新的一小撮马

struct escort_info
{
	uint32_t userid;
	char	 escort_nick[32];
	uint32_t horse_quality;
	uint32_t is_foe;//是否是仇人
	uint32_t escort_left_sec;//押运结束的秒数
	uint32_t keaped_time;//被劫持的次数
};

class cli_user_get_escort_info_out : public Cmessage
{
public:
	uint32_t finish_sec;//距离押运结束的秒数
	uint32_t escort_time;//押运的次数
	uint32_t keap_time;//劫持的次数
	uint32_t horse_quality;//马品质
	uint32_t keaped_time;//被劫持的次数
	item_unit_t gift[2];//奖励信息
	uint32_t is_new;//是否是拉取的最新60秒数据
	std::vector<escort_info> escort_vec;//押运的信息
	cli_user_get_escort_info_out();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_ESCORT_COMBAT_INFO_CMD = 687, //战报信息
//CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_escort_combat_info_in, uint32_t, page_no, uint32);

struct user_escort_file_info_t
{
	uint32_t type;			//类型 1押镖 2劫镖 3被劫镖
	uint32_t horse_quality;//押|劫|被劫 马的品质
	char	 keaped_nick[32];	//被劫镖|劫镖对象nick
	uint32_t keaped_result; //type =1 的时候是被劫持的次数(0-2),type = 2 劫镖是否成功(0 失败,1成功),type =3 被劫镖是否失败(0 失败,1成功)
	uint32_t finish_time;	//结束时间 为押镖结束时间 或是 劫镖结束时间(实为产生该记录的时间)
	uint32_t level;
};

class cli_user_escort_combat_info_out : public Cmessage
{
public:
	uint32_t page_no;//战报的页数,1-...
	std::vector<user_escort_file_info_t> escort_combat_vec;

	cli_user_escort_combat_info_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_START_KEAP_HORSE_CMD = 688, //开始劫持白龙马
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_start_keap_horse_in, uint32_t, keaped_userid, uint32);

typedef cli_user_start_arena_battle_out cli_user_start_keap_horse_out;

//PROTO_USER_OVER_KEAP_HORSE_CMD = 689, //结束劫持白龙马
typedef cli_user_over_arena_battle_in cli_user_over_keap_horse_in;

class cli_user_over_keap_horse_out : public Cmessage
{
public:
	uint32_t battle_result;
	uint32_t silver_cnt;//battle_result才有效
	cli_user_over_keap_horse_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_GET_ESCORT_USER_INFO_CMD = 690, //拉取押镖玩家信息
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_get_escort_user_info_in, uint32_t, userid, uint32);

struct escort_user_info
{
	uint32_t userid;
	uint32_t level;//等级
	char nick[32];
	uint32_t fight_value;//战力
	uint32_t horse_quality;//马品质
	uint32_t keaped_cnt;//被拦截的次数
};

class cli_user_get_escort_user_info_out : public Cmessage
{
public:
	escort_user_info escort_user;

	cli_user_get_escort_user_info_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_START_ESCORT_CMD = 691, //开始押镖

//PROTO_USER_ESCORT_GIFT_CMD = 692, //领押镖奖励
class cli_user_escort_gift_out : public Cmessage
{
public:
	uint32_t is_moved;//是否移到个人礼包,当1是代表背包满了移到个人礼包中去了,这是显示的奖励信息只供显示
	item_unit_t gift[2];//奖励信息
	cli_user_escort_gift_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_ESCORT_EXISTS_GIFT_CMD = 693, //是否存在押镖奖励
class cli_user_escort_exists_gift_out : public Cmessage
{
public:
	uint32_t is_exist;//1存在奖励 0没有奖励
	item_unit_t gift[2];//奖励信息
	uint32_t escort_time;//押运的次数
	uint32_t keap_time;//劫持的次数

	cli_user_escort_exists_gift_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_INVITE_INFO_CMD = 694, //好友邀请信息
class cli_user_invite_info_out : public Cmessage
{
public:
	uint32_t invite_code;//我的邀请码
	uint32_t friend_num;
	cli_user_invite_info_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_GET_WORLD_BOSS_INFO_CMD = 695, //拉取世界boss信息
class cli_user_get_world_boss_info_out: public Cmessage
{
public:
	uint32_t boss_hp_max;	//boss最大血量
	uint32_t boss_hp;		//boss当前血量
	uint32_t boss_out_atk;	//boss外攻值
	uint32_t boss_in_atk;	//boss内攻值
	uint32_t boss_out_def;	//boss外防值
	uint32_t boss_in_def;	//boss内防值
	uint32_t boss_add_hurt;	//boss加伤值

	uint32_t my_hurt_hp;	//自己对boss造成的伤害
	uint32_t reward_yxb;	//已经获得的银两
	uint32_t reward_nerve;	//已经获得的经脉真气
	uint32_t invigorate;	//鼓舞的增加的战力百分比
	int32_t time_counter;	//倒计时秒数负值为世界boss开始前的倒计时，正值为结束倒计时
	std::vector<simpl_rank_info_t> users;//伤害排行列表

	cli_user_get_world_boss_info_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_WORLD_BOSS_BTL_START_CMD = 696, //开始单局世界boss
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_world_boss_btl_start_in, uint32_t, cost_gold, uint32);//立即复活消耗的黄金

class cli_user_world_boss_btl_start_out: public Cmessage
{
public:
	uint32_t need_btl;		//是否需要进入战斗(vip4以上不需战斗，直接造成伤害)
	uint32_t boss_hp;		//boss当前血量
	uint32_t hurt_hp;		//本次对boss造成的伤害(vip4以上直接造成的伤害)
	uint32_t reward_yxb;	//本次获得的银两奖励
	uint32_t reward_nerve;	//本次获得的真气奖励
	std::vector<simpl_rank_info_t> users;//伤害排行列表

	cli_user_world_boss_btl_start_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_WORLD_BOSS_BTL_OVER_CMD = 697, //单局世界boss结束
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_world_boss_btl_over_in, uint32_t, hurt_hp, uint32);//本局打掉的boss血量

class cli_user_world_boss_btl_over_out: public Cmessage
{
public:
	uint32_t boss_hp;		//boss当前血量
	uint32_t hurt_hp;		//本次对boss造成的伤害
	uint32_t reward_yxb;	//本次获得的银两奖励
	uint32_t reward_nerve;	//本次获得的真气奖励
	std::vector<simpl_rank_info_t> users;//伤害排行列表

	cli_user_world_boss_btl_over_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_WORLD_BOSS_INVIGORATE_CMD = 698, //世界boss鼓舞
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_world_boss_invigorate_in, uint32_t, cost_gold, uint32);//鼓舞消耗的黄金

class cli_user_world_boss_invigorate_out: public Cmessage
{
public:
	uint32_t cost_gold;		//鼓舞消耗的黄金，0表示非黄金鼓舞
	uint32_t invigorate;	//鼓舞的结果

	cli_user_world_boss_invigorate_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_WORLD_BOSS_OVER_NOTI_CMD = 699, //世界boss结束通知
class cli_user_world_boss_over_noti_out: public Cmessage
{
public:
	uint32_t boss_hp_max;	//boss总血量
	uint32_t boss_hp;		//boss当前血量(若为0则代表被击杀，否则时间到)
	uint32_t my_rank;		//排名
	uint32_t my_hurt_hp;	//累计对boss造成的伤害
	uint32_t reward_yxb;	//累计获得的银两奖励
	uint32_t reward_nerve;	//累计获得的真气奖励

	world_boss_top_info_t fatal_killer;//击杀者
	world_boss_top_info_t top_killers[3];//前三名

	cli_user_world_boss_over_noti_out();
	void init();
	int get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_CHECK_WORLD_BOSS_CMD = 700, //检查世界boss活动状态
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_check_world_boss_out, uint32_t, status, uint32);
//status 活动状态，0 正常可进入，1 boss已经死或不在活动时间内

//PROTO_USER_GET_ACTIVITY_INFO_CMD = 701, //获取活动信息
class cli_user_get_activity_info_in : public Cmessage
{
public:
	std::vector<uint32_t> id_vec;
	
	cli_user_get_activity_info_in();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

class cli_user_get_activity_info_out : public Cmessage
{
public:
	std::vector<activity_time_t> activity_vec;//活动的开启时间信息
	
	cli_user_get_activity_info_out();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_PARTNER_FRIENDVAL_TRAIN_CMD = 702, //弟子好友度培养 type=0为一般培养,type=1为一键培养
class cli_user_partner_friendval_train_in : public Cmessage
{
public:
	uint32_t partner_id;//弟子ID
	uint32_t type;//培养类型

	cli_user_partner_friendval_train_in();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

class cli_user_partner_friendval_train_out : public Cmessage
{
public:
	uint32_t partner_id;//
	uint32_t cost_yxb;//消耗的银两
	uint32_t add_friend_val;//增加的友好度

	cli_user_partner_friendval_train_out();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_PARTNER_UNHAD_LIST_CMD = 703, //拉取没有获得的弟子列表及友好度
struct partner_unhad_info_t
{
	uint32_t partner_id;
	uint32_t friend_val;
	uint32_t cur_state;

	void init();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

class cli_user_partner_unhad_list_out : public Cmessage
{
public:
	std::vector<partner_unhad_info_t> unhad_vec;
	cli_user_partner_unhad_list_out();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_PARTNER_UNHAD_ONE_CMD = 704, //拉取指定弟子的友好度
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_partner_unhad_one_in, uint32_t, partner_id, uint32);//弟子ID
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_partner_unhad_one_out, uint32_t, friend_val, uint32);//友好度

//PROTO_USER_BY_INVITE_CODE_CMD = 705, //设置邀请我进来的好友码
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_by_invite_code_in, uint32_t, invite_code, uint32);//邀请码
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_by_invite_code_out, uint32_t, gold, uint32);//奖励数量

//PROTO_USER_SYS_MSG_NOTI_CMD = 710, //系统消息通知
enum sys_noti_type_e {//系统通知类型
	SYS_NOTI_ESCORT_REPORT		= 1, //运镖战报
	SYS_NOTI_ESCORT_REWARD		= 2, //运镖完成奖励
	SYS_NOTI_ARENA_REPORT		= 3, //竞技场战报
	SYS_NOTI_WORLD_BOSS_REWARD	= 4, //世界boss奖励
	SYS_NOTI_MAX_TYPE, 				 //系统通知最大类型
};

struct sys_noti_info_t {
	uint32_t msg_type;//消息类型
	uint32_t num;	  //消息数量

	sys_noti_info_t(uint32_t t = 0, uint32_t n = 0): msg_type(t), num(n) {}

	void init(); 
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

class cli_user_sys_msg_noti_out : public Cmessage
{
public:
	std::vector<sys_noti_info_t> msg_list;

	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_SET_READ_SYS_MSG_CMD = 711, //设置系统消息为已读
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_set_read_sys_msg_in, uint32_t, msg_type, uint32);//消息类型

//PROTO_USER_GET_ORDER_BILL_NO_CMD	= 800,	//获取充值订单号
CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_get_order_bill_no_in, uint32_t, product_type, uint32);//充值类型，取范围为1-6

class cli_user_get_order_bill_no_out : public Cmessage
{
public:
	char	 bill_no[32];
	
	cli_user_get_order_bill_no_out();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

//PROTO_USER_NOTIFY_BUY_GOLD_CMD		= 801,	//充值结果通知
class cli_user_notify_buy_gold_out : public Cmessage
{
public:
	uint32_t buy_gold_total;	//充值黄金总数
	uint32_t buy_gold;			//当次购买得到的黄金数
	uint32_t is_first_buy;		//是否是首次充值

	cli_user_notify_buy_gold_out();
	void init();
	int  get_msg_len();
	bool read_from_buf(byte_array_t & ba);
	bool write_to_buf(byte_array_t & ba);
};

#endif
