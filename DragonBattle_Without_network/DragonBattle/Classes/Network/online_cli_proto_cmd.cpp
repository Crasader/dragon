#include "online_cli_proto_cmd.h"

void package_equipment_t::init()
{
	package_id = 0;
	equipment_id = 0;
	equipment_level = 0;
	add_attr_1 = 0;
	add_attr_2 = 0;
	memset(attach_attr, 0, sizeof(attach_attr));
	memset(gems, 0, sizeof(gems));
}

bool package_equipment_t::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(package_id)) return false;
	if (!ba.read_uint32(equipment_id)) return false;
	if (!ba.read_uint32(equipment_level)) return false;
	if (!ba.read_uint32(add_attr_1)) return false;
	if (!ba.read_uint32(add_attr_2)) return false;
	if (!ba.read_buf((char*)attach_attr, sizeof(attach_attr))) return false;
	if (!ba.read_buf((char*)gems, sizeof(gems))) return false;
	return true;
}

bool package_equipment_t::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(package_id)) return false;
	if (!ba.write_uint32(equipment_id)) return false;
	if (!ba.write_uint32(equipment_level)) return false;
	if (!ba.write_uint32(add_attr_1)) return false;
	if (!ba.write_uint32(add_attr_2)) return false;
	if (!ba.write_buf((const char*)attach_attr, sizeof(attach_attr))) return false;
	if (!ba.write_buf((const char*)gems, sizeof(gems))) return false;
	return true;
}

void simpl_rank_info_t::init()
{
	memset(nick, 0, sizeof(nick));
	value = 0;
}

bool simpl_rank_info_t::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_buf(nick, sizeof(nick))) return false;
	if (!ba.read_uint32(value)) return false;
	return true;
}

bool simpl_rank_info_t::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_buf(nick, sizeof(nick))) return false;
	if (!ba.write_uint32(value)) return false;
	return true;
}

void world_boss_top_info_t::init()
{
	memset(nick, 0, sizeof(nick));
	reward_yxb = 0;
	reward_nerve = 0;
}

bool world_boss_top_info_t::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_buf(nick, sizeof(nick))) return false;
	if (!ba.read_uint32(reward_yxb)) return false;
	if (!ba.read_uint32(reward_nerve)) return false;
	return true;
}

bool world_boss_top_info_t::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_buf(nick, sizeof(nick))) return false;
	if (!ba.write_uint32(reward_yxb)) return false;
	if (!ba.write_uint32(reward_nerve)) return false;
	return true;
}

void activity_time_t::init()
{
	activity_id = 0;
	begin_time = 0;
	end_time = 0;
}

int activity_time_t::get_msg_len()
{
	return sizeof(activity_id) + sizeof(begin_time) + sizeof(end_time);
}

bool activity_time_t::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(activity_id)) return false;
	if (!ba.read_uint32(begin_time)) return false;
	if (!ba.read_uint32(end_time)) return false;
	return true;
}

bool activity_time_t::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(activity_id)) return false;
	if (!ba.write_uint32(begin_time)) return false;
	if (!ba.write_uint32(end_time)) return false;
	return true;
}

void usr_online_status_t::init()
{
	userid = 0;
	flag = 0;
}

int usr_online_status_t::get_msg_len()
{
	return sizeof(userid) + sizeof(flag);
}

bool usr_online_status_t::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(userid)) return false;
	if (!ba.read_uint8(flag)) return false;
	return true;
}

bool usr_online_status_t::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(userid)) return false;
	if (!ba.write_uint8(flag)) return false;
	return true;
}

int auto_battle_t::get_msg_len()
{
	int len = sizeof(map_id) + sizeof(uint32_t) + sizeof(item_unit_t) * item_vec.size();
	len += sizeof(uint32_t) + sizeof(item_unit_t) * drop_vec.size();
	return len;
}

bool auto_battle_t::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(map_id)) return false;
	int item_size = 0;
	if (!ba.read_int32(item_size)) return false;
	item_vec.clear();
	for (int j = 0; j < item_size; j++) {
		item_unit_t item;
		if (!ba.read_uint32(item.item_id)) return false;
		if (!ba.read_uint32(item.item_cnt)) return false;
		item_vec.push_back(item);
	}
	if (!ba.read_int32(item_size)) return false;
	drop_vec.clear();
	for (int j = 0; j < item_size; j++) {
		item_unit_t item;
		if (!ba.read_uint32(item.item_id)) return false;
		if (!ba.read_uint32(item.item_cnt)) return false;
		drop_vec.push_back(item);
	}
	return true;
}

bool auto_battle_t::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(map_id)) return false;
	int item_size = item_vec.size();
	if (!ba.write_int32(item_size)) return false;
	for (int j = 0; j < item_size; j++) {
		if (!ba.write_uint32(item_vec[j].item_id)) return false;
		if (!ba.write_uint32(item_vec[j].item_cnt)) return false;
	}
	item_size = drop_vec.size();
	if (!ba.write_int32(item_size)) return false;
	for (int j = 0; j < item_size; j++) {
		if (!ba.write_uint32(drop_vec[j].item_id)) return false;
		if (!ba.write_uint32(drop_vec[j].item_cnt)) return false;
	}
	return true;
}

//-------------------------------------------------------
//协议号对应的请求包和返回包结构
cli_user_login_in::cli_user_login_in()
{
	this->init();
}

void cli_user_login_in::init()
{
	this->time = 0;
	memset(this->login_sess, 0, sizeof(this->login_sess));
}

int  cli_user_login_in::get_msg_len()
{
	return sizeof(this->time) + sizeof(this->login_sess);
}

bool cli_user_login_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(this->time)) return false;
	if (!ba.read_buf(this->login_sess, sizeof(this->login_sess))) return false;

	return true;
}

bool cli_user_login_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(this->time)) return false;
	if (!ba.write_buf(this->login_sess, sizeof(this->login_sess))) return false;

	return true;
}

cli_user_login_out::cli_user_login_out()
{
	this->init();
}

cli_user_login_out::~cli_user_login_out()
{
	this->equipment_vec.clear();
	this->item_used_vec.clear();
}

void cli_user_login_out::init()
{
	this->userid = 0;
	memset(this->nick, 0, sizeof(this->nick));
	this->unit_id = 0;
	this->gold = 0;
	this->buy_gold = 0;
	this->exp = 0;
	this->level = 0;
	this->yxb = 0;
	this->last_community_id = 0;
	this->spec_community = 0;
	this->enery = 0;
	this->enery_max = 0;
	this->enery_counter = 0;
    this->force_exp = 0;
	this->nerve_level = 0;
	this->nerve_value = 0;
	this->power_value = 0;
	this->prestige_value = 0;
	this->star_exp = 0;
	this->package_cnt = 0;
	this->max_package_size = 0;
	this->train_hp = 0;                //培养增加的生命值
    this->train_out_attack = 0;        //培养增加的攻击力
    this->train_out_def = 0;           //培养增加的防御力
    this->train_in_attack = 0;         //培养增加的内攻
    this->train_in_def = 0;            //培养增加的内防
	this->train_toughness = 0;         //培养增加的内力
	memset(this->skills, 0, sizeof(skills));
	memset(this->passive_skills, 0, sizeof(passive_skills));
	memset(this->star_array, 0, sizeof(star_array));
	this->auto_battling = 0;
	this->is_exists_invite = 0;
	this->equipment_vec.clear();
	this->item_used_vec.clear();
	this->partner_vec.clear();
}

int  cli_user_login_out::get_msg_len()
{
	return sizeof(this->userid) + sizeof(this->nick) + sizeof(unit_id) + sizeof(gold) + sizeof(buy_gold)
		+ sizeof(exp) + sizeof(level) + sizeof(yxb) + sizeof(last_community_id) + sizeof(spec_community) 
		+ sizeof(this->enery) + sizeof(enery_max) + sizeof(this->enery_counter) 
		+ sizeof(force_exp) + sizeof(nerve_level) + sizeof(nerve_value) + sizeof(power_value) + sizeof(prestige_value)
		+ sizeof(star_exp) + sizeof(package_cnt) + sizeof(max_package_size)
        + sizeof(int) * 6
		+ sizeof(skills) + sizeof(passive_skills) + sizeof(star_array) + sizeof(auto_battling) + sizeof(is_exists_invite)
		+ sizeof(uint32_t) + this->equipment_vec.size() * sizeof(package_equipment_t)
		+ sizeof(uint32_t) + this->item_used_vec.size() * sizeof(item_used_t)
		+ sizeof(uint32_t) + this->partner_vec.size() * sizeof(partner_info_t);
}

bool cli_user_login_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(this->userid)) return false;
	if (!ba.read_buf(this->nick, sizeof(this->nick))) return false;
	if (!ba.read_uint32(this->unit_id)) return false;
	if (!ba.read_uint32(this->gold)) return false;
	if (!ba.read_uint32(this->buy_gold)) return false;
	if (!ba.read_uint64(this->exp)) return false;
	if (!ba.read_uint32(this->level)) return false;
	if (!ba.read_uint32(this->yxb)) return false;
	if (!ba.read_uint32(this->last_community_id)) return false;
	if (!ba.read_uint32(this->spec_community)) return false;
	if (!ba.read_uint32(this->enery)) return false;
	if (!ba.read_uint32(this->enery_max)) return false;
	if (!ba.read_uint32(this->enery_counter)) return false;
	if (!ba.read_uint32(this->force_exp)) return false;
	if (!ba.read_uint32(this->nerve_level)) return false;
	if (!ba.read_uint32(this->nerve_value)) return false;
	if (!ba.read_uint32(this->power_value)) return false;
	if (!ba.read_uint32(this->prestige_value)) return false;
	if (!ba.read_uint32(this->star_exp)) return false;
	if (!ba.read_uint32(this->package_cnt)) return false;
	if (!ba.read_uint32(this->max_package_size)) return false;

    if (!ba.read_int32(this->train_hp)) return false;
    if (!ba.read_int32(this->train_out_attack)) return false;
    if (!ba.read_int32(this->train_out_def)) return false;
    if (!ba.read_int32(this->train_in_attack)) return false;
    if (!ba.read_int32(this->train_in_def)) return false;
    if (!ba.read_int32(this->train_toughness)) return false;
	
    if (!ba.read_buf((char*)this->skills, sizeof(this->skills))) return false;
	if (!ba.read_buf((char*)this->passive_skills, sizeof(this->passive_skills))) return false;
	if (!ba.read_buf((char*)this->star_array, sizeof(this->star_array))) return false;
    if (!ba.read_uint32(this->auto_battling)) return false;
    if (!ba.read_uint32(this->is_exists_invite)) return false;

	int vec_size = 0;
	if (!ba.read_int32(vec_size)) return false;
	this->equipment_vec.clear();
	for (int i = 0; i < vec_size; ++i) {
		package_equipment_t equipment;
		if (!equipment.read_from_buf(ba)) return false;
		this->equipment_vec.push_back(equipment);
	}

	vec_size = 0;
	if (!ba.read_int32(vec_size)) return false;
	this->item_used_vec.clear();
	for (int i = 0; i < vec_size; ++i) {
		item_used_t item;
		if (!ba.read_uint32(item.item_id)) return false;
		if (!ba.read_uint32(item.last_cnt)) return false;
		this->item_used_vec.push_back(item);	
	}
	
	vec_size = 0;
	if (!ba.read_int32(vec_size)) return false;
	this->partner_vec.clear();
	for (int i = 0; i < vec_size; ++i) {
		partner_info_t partner;
		if (!ba.read_uint32(partner.partner_id)) return false;
		if (!ba.read_uint64(partner.exp)) return false;
		if (!ba.read_uint32(partner.level)) return false;
		if (!ba.read_uint32(partner.inherit_state)) return false;
		if (!ba.read_uint32(partner.qualify_value)) return false;
		if (!ba.read_uint32(partner.cur_seat)) return false;
		if (!ba.read_uint32(partner.skill_level)) return false;
		//TODO:调用read_from_buf
		if (!ba.read_buf((char*)&(partner.equipment), sizeof(partner.equipment))) return false;     //使用的技能
		if (!ba.read_int32(partner.train_hp)) return false;
		if (!ba.read_int32(partner.train_out_attack)) return false;
		if (!ba.read_int32(partner.train_out_def)) return false;
		if (!ba.read_int32(partner.train_in_attack)) return false;
		if (!ba.read_int32(partner.train_in_def)) return false;
		if (!ba.read_int32(partner.train_toughness)) return false;
		if (!ba.read_buf((char*)partner.star_array, sizeof(partner.star_array))) return false;
		this->partner_vec.push_back(partner);
	}

	return true;
}

bool cli_user_login_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(this->userid)) return false;
	if (!ba.write_buf(this->nick, sizeof(this->nick))) return false;
	if (!ba.write_uint32(this->unit_id)) return false;
	if (!ba.write_uint32(this->gold)) return false;
	if (!ba.write_uint32(this->buy_gold)) return false;
	if (!ba.write_uint64(this->exp)) return false;
	if (!ba.write_uint32(this->level)) return false;
	if (!ba.write_uint32(this->yxb)) return false;
	if (!ba.write_uint32(this->last_community_id)) return false;
	if (!ba.write_uint32(this->spec_community)) return false;
	if (!ba.write_uint32(this->enery)) return false;
	if (!ba.write_uint32(this->enery_max)) return false;
	if (!ba.write_uint32(this->enery_counter)) return false;
	if (!ba.write_uint32(this->force_exp)) return false;
	if (!ba.write_uint32(this->nerve_level)) return false;
	if (!ba.write_uint32(this->nerve_value)) return false;
	if (!ba.write_uint32(this->power_value)) return false;
	if (!ba.write_uint32(this->prestige_value)) return false;
	if (!ba.write_uint32(this->star_exp)) return false;
	if (!ba.write_uint32(this->package_cnt)) return false;
	if (!ba.write_uint32(this->max_package_size)) return false;

	if (!ba.write_uint32(this->train_hp)) return false;
    if (!ba.write_uint32(this->train_out_attack)) return false;
    if (!ba.write_uint32(this->train_out_def)) return false;
    if (!ba.write_uint32(this->train_in_attack)) return false;
    if (!ba.write_uint32(this->train_in_def)) return false;
    if (!ba.write_uint32(this->train_toughness)) return false;
	
    if (!ba.write_buf((char*)this->skills, sizeof(this->skills))) return false;
	if (!ba.write_buf((char*)this->passive_skills, sizeof(this->passive_skills))) return false;
	if (!ba.write_buf((char*)this->star_array, sizeof(this->star_array))) return false;
    if (!ba.write_uint32(this->auto_battling)) return false;
    if (!ba.write_uint32(this->is_exists_invite)) return false;

	int	vec_size = this->equipment_vec.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; ++i) {
		if (!this->equipment_vec[i].write_to_buf(ba)) return false;
	}

	vec_size = this->item_used_vec.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; ++i) {
		if (!ba.write_uint32(this->item_used_vec[i].item_id)) return false;
		if (!ba.write_uint32(this->item_used_vec[i].last_cnt)) return false;
	}

	
	vec_size = this->partner_vec.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; ++i) {
		if (!ba.write_uint32(partner_vec[i].partner_id)) return false;
		if (!ba.write_uint64(partner_vec[i].exp)) return false;
		if (!ba.write_uint32(partner_vec[i].level)) return false;
		if (!ba.write_uint32(partner_vec[i].inherit_state)) return false;
		if (!ba.write_uint32(partner_vec[i].qualify_value)) return false;
		if (!ba.write_uint32(partner_vec[i].cur_seat)) return false;
		if (!ba.write_uint32(partner_vec[i].skill_level)) return false;
		//TODO:调用write_to_buf
		if (!ba.write_buf((char*)&(partner_vec[i].equipment), sizeof(partner_vec[i].equipment))) return false;     //使用的技能
		if (!ba.write_int32(partner_vec[i].train_hp)) return false;
		if (!ba.write_int32(partner_vec[i].train_out_attack)) return false;
		if (!ba.write_int32(partner_vec[i].train_out_def)) return false;
		if (!ba.write_int32(partner_vec[i].train_in_attack)) return false;
		if (!ba.write_int32(partner_vec[i].train_in_def)) return false;
		if (!ba.write_int32(partner_vec[i].train_toughness)) return false;
		if (!ba.write_buf((char*)(partner_vec[i].star_array), sizeof(partner_vec[i].star_array))) return false;
	}
	return true;
}


cli_user_login_ex_out::cli_user_login_ex_out()
{
	this->init();
}

void cli_user_login_ex_out::init()
{
	max_main_task = 0;
	task_vec.clear();	//主支线任务(已接 可接)
	last_login_tm = 0;			//上次登陆时间
	award_counter = 0;			//倒计时
	memset(award_task, 0, sizeof(award_task));//悬赏任务

	online_prize_second = 0;//在线礼包倒计时
	online_prize_id = 0;	//奖励ID
	turntable_id = 0;			//幸运大转盘第几个
	left_second = 0;			//大转盘的倒计时
	week_login_prize = 0;		//周登录礼包登录次数 | 是否领取(用位表示)
	level_prize = 0;			//等级奖励是否领取(用位表示)
	action_prize_vec.clear();//活动奖励 需要配置
	cur_date = 0;		//签到 当前日期
	sign_value = 0;		//签到 位表示
	sign_prize = 0;		//签到 位表示是否领取
	is_vip_gift = 0;	//VIP每日领取礼包 是否领取
	vip_level_gift = 0;//VIP等级奖励是否领取 用位表示
	totol_vip_gift = 0;//累计充值礼包
	offline_exp_hour = 0;//
	offline_exp = 0;
	offline_cost_cold = 0;

	offline_days = 0;//离线天数
	offline_days_exp = 0;//离线期内显示的经验
	offline_days_yxb = 0;

	offline_prize_id = 0;	//离线补偿
	shake_money = 0;

	login_day = 0;
	seven_day_prize = 0;
	prize_state_1 = 0;
	login_count = 0;
	prize_state_2 = 0;
	worship_fairy = 0;
	gift_info = 0;

	chat_info = 0;
	self_gift = 0;
	memset(&active_degree, 0, sizeof(active_degree));
	fake_vip_state = 0;
	online_time = 0;
	reserved_state1 = 0;
	reserved_state2 = 0;
	reserved_state3 = 0;

	num_vec.clear();	//次数
}

int cli_user_login_ex_out::get_msg_len()
{
	int len = 0;
	len += sizeof(int32_t) + sizeof(task_info_t) * task_vec.size() + sizeof(max_main_task);
	len += sizeof(last_login_tm) + sizeof(award_counter) + 4 * (sizeof(award_task[0].task_id) + sizeof(award_task[0].cur_step) + sizeof(award_task[0].max_step) + sizeof(award_task[0].task_star) + sizeof(award_task[0].task_state));
	len += sizeof(online_prize_second) + sizeof(online_prize_id) + sizeof(turntable_id) + sizeof(left_second);
	len += sizeof(week_login_prize) + sizeof(level_prize);
	len += sizeof(int32_t) + action_prize_vec.size() * sizeof(uint32_t);
	len += sizeof(cur_date) + sizeof(sign_value) + sizeof(sign_prize);
	len	+= sizeof(is_vip_gift) + sizeof(vip_level_gift) + sizeof(totol_vip_gift);
	len	+= sizeof(offline_exp_hour) + sizeof(offline_exp) + sizeof(offline_cost_cold);
	len += sizeof(offline_days) + sizeof(offline_days_exp) + sizeof(offline_days_yxb) + sizeof(shake_money);
	len += sizeof(login_day) + sizeof(seven_day_prize) + sizeof(prize_state_1) + sizeof(login_count) + sizeof(prize_state_2);
	len += sizeof(worship_fairy);
	len += sizeof(gift_info) + sizeof(chat_info) + sizeof(self_gift);
	len += sizeof(active_degree.prize_state) + sizeof(active_degree.active_state);
	len += sizeof(fake_vip_state) + sizeof(online_time);
   	len += sizeof(reserved_state1) + sizeof(reserved_state2) + sizeof(reserved_state3);
	len += sizeof(uint32_t) + sizeof(uint32_t) * num_vec.size();
	return len;
}

bool cli_user_login_ex_out::read_from_buf(byte_array_t & ba)
{
	int vec_size = 0;
	if (!ba.read_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		task_info_t task;
		if (!ba.read_uint32(task.task_id)) return false;
		if (!ba.read_uint8(task.task_type)) return false;
		if (!ba.read_uint8(task.task_state)) return false;
		if (!ba.read_uint8(task.cur_step)) return false;
		if (!ba.read_uint8(task.max_step)) return false;
		task_vec.push_back(task);
	}
	if (!ba.read_uint32(max_main_task)) return false;

	if (!ba.read_uint32(last_login_tm)) return false;
	if (!ba.read_uint32(award_counter)) return false;
	for (int i = 0; i < 4; i++) {
		if (!ba.read_uint16(award_task[i].task_id)) return false;
		if (!ba.read_uint16(award_task[i].cur_step)) return false;
		if (!ba.read_uint16(award_task[i].max_step)) return false;
		if (!ba.read_uint8(award_task[i].task_star)) return false;
		if (!ba.read_uint8(award_task[i].task_state)) return false;
	}

	if (!ba.read_uint32(online_prize_second)) return false;
	if (!ba.read_uint32(online_prize_id)) return false;
	if (!ba.read_uint32(turntable_id)) return false;
	if (!ba.read_uint32(left_second)) return false;
	if (!ba.read_uint32(week_login_prize)) return false;
	if (!ba.read_uint32(level_prize)) return false;
	
	if (!ba.read_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		uint32_t prize_id = 0;
		if (!ba.read_uint32(prize_id)) return false;
		action_prize_vec.push_back(prize_id);
	}
	
	if (!ba.read_uint32(cur_date)) return false;
	if (!ba.read_uint32(sign_value)) return false;
	if (!ba.read_uint32(sign_prize)) return false;
	if (!ba.read_uint32(is_vip_gift)) return false;
	if (!ba.read_uint32(vip_level_gift)) return false;
	if (!ba.read_uint32(totol_vip_gift)) return false;
	//当日离线福利
	if (!ba.read_uint32(offline_exp_hour)) return false;
	if (!ba.read_uint32(offline_exp)) return false;
	if (!ba.read_uint32(offline_cost_cold)) return false;
	//多日离线福利
	if (!ba.read_uint32(offline_days)) return false;
	if (!ba.read_uint32(offline_days_exp)) return false;
	if (!ba.read_uint32(offline_days_yxb)) return false;

	if (!ba.read_uint32(offline_prize_id)) return false;
	if (!ba.read_uint32(shake_money)) return false;

	if (!ba.read_uint32(login_day)) return false;
	if (!ba.read_uint32(seven_day_prize)) return false;
	if (!ba.read_uint32(prize_state_1)) return false;
	if (!ba.read_uint32(login_count)) return false;
	if (!ba.read_uint32(prize_state_2)) return false;
	if (!ba.read_uint32(worship_fairy)) return false;
	if (!ba.read_uint32(gift_info)) return false;
	if (!ba.read_uint32(chat_info)) return false;
	if (!ba.read_uint32(self_gift)) return false;

	if (!ba.read_uint32(active_degree.prize_state)) return false;
	if (!ba.read_buf((char*)(active_degree.active_state), sizeof(active_degree.active_state))) return false;
	if (!ba.read_uint32(fake_vip_state)) return false;
	if (!ba.read_uint32(online_time)) return false;
	if (!ba.read_uint32(reserved_state1)) return false;
	if (!ba.read_uint32(reserved_state2)) return false;
	if (!ba.read_uint32(reserved_state3)) return false;

	if (!ba.read_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		uint32_t num = 0;
		if (!ba.read_uint32(num)) return false;
		num_vec.push_back(num);
	}
	return true;
}

bool cli_user_login_ex_out::write_to_buf(byte_array_t & ba)
{
	int vec_size = task_vec.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		if (!ba.write_uint32(task_vec[i].task_id)) return false;
		if (!ba.write_uint8(task_vec[i].task_type)) return false;
		if (!ba.write_uint8(task_vec[i].task_state)) return false;
		if (!ba.write_uint8(task_vec[i].cur_step)) return false;
		if (!ba.write_uint8(task_vec[i].max_step)) return false;
	}
	if (!ba.write_uint32(max_main_task)) return false;

	if (!ba.write_uint32(last_login_tm)) return false;
	if (!ba.write_uint32(award_counter)) return false;
	for (int i = 0; i < 4; i++) {
		if (!ba.write_uint16(award_task[i].task_id)) return false;
		if (!ba.write_uint16(award_task[i].cur_step)) return false;
		if (!ba.write_uint16(award_task[i].max_step)) return false;
		if (!ba.write_uint8(award_task[i].task_star)) return false;
		if (!ba.write_uint8(award_task[i].task_state)) return false;
	}

	if (!ba.write_uint32(online_prize_second)) return false;
	if (!ba.write_uint32(online_prize_id)) return false;
	if (!ba.write_uint32(turntable_id)) return false;
	if (!ba.write_uint32(left_second)) return false;
	if (!ba.write_uint32(week_login_prize)) return false;
	if (!ba.write_uint32(level_prize)) return false;
	
	vec_size = action_prize_vec.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		if (!ba.read_uint32(action_prize_vec[i])) return false;
	}
	
	if (!ba.write_uint32(cur_date)) return false;
	if (!ba.write_uint32(sign_value)) return false;
	if (!ba.write_uint32(sign_prize)) return false;
	if (!ba.write_uint32(is_vip_gift)) return false;	
	if (!ba.write_uint32(vip_level_gift)) return false;
	if (!ba.write_uint32(totol_vip_gift)) return false;
	//当日离线福利
	if (!ba.write_uint32(offline_exp_hour)) return false;	
	if (!ba.write_uint32(offline_exp)) return false;
	if (!ba.write_uint32(offline_cost_cold)) return false;
	//多日离线福利
	if (!ba.write_uint32(offline_days)) return false;
	if (!ba.write_uint32(offline_days_exp)) return false;
	if (!ba.write_uint32(offline_days_yxb)) return false;

	if (!ba.write_uint32(offline_prize_id)) return false;
	if (!ba.write_uint32(shake_money)) return false;

	if (!ba.write_uint32(login_day)) return false;
	if (!ba.write_uint32(seven_day_prize)) return false;
	if (!ba.write_uint32(prize_state_1)) return false;
	if (!ba.write_uint32(login_count)) return false;
	if (!ba.write_uint32(prize_state_2)) return false;
	if (!ba.write_uint32(worship_fairy)) return false;
	if (!ba.write_uint32(gift_info)) return false;
	if (!ba.write_uint32(chat_info)) return false;
	if (!ba.write_uint32(self_gift)) return false;

	if (!ba.write_uint32(active_degree.prize_state)) return false;
	if (!ba.write_buf((char*)(active_degree.active_state), sizeof(active_degree.active_state))) return false;
	
	if (!ba.write_uint32(fake_vip_state)) return false;
	if (!ba.write_uint32(online_time)) return false;
	if (!ba.write_uint32(reserved_state1)) return false;
	if (!ba.write_uint32(reserved_state2)) return false;
	if (!ba.write_uint32(reserved_state3)) return false;
	
	vec_size = num_vec.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		if (!ba.write_uint32(num_vec[i])) return false;
	}
	return true;
}

//PROTO_USER_GET_PACKAGE_CMD		= 501,	//用户拉取背包
cli_user_get_package_out::cli_user_get_package_out()
{
	this->init();
}

void cli_user_get_package_out::init()
{
	item_sum = 0;
	package_size = 0;
	item_vec.clear();	//道具
	card_vec.clear();	//弟子卡牌
	stuff_vec.clear();	//材料
	equipment_vec.clear();//装备
}

int cli_user_get_package_out::get_msg_len()
{
	return sizeof(item_sum) + sizeof(package_size)
			+ sizeof(uint32_t) + item_vec.size() * sizeof(package_item_t)
			+ sizeof(uint32_t) + card_vec.size() * sizeof(package_item_t)
			+ sizeof(uint32_t) + stuff_vec.size() * sizeof(package_item_t)
			+ sizeof(uint32_t) + equipment_vec.size() * sizeof(package_equipment_t);
}
    
bool cli_user_get_package_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(item_sum)) return false;
	if (!ba.read_uint32(package_size)) return false;

	int vec_size = 0;
	if (!ba.read_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		package_item_t item;
		if (!ba.read_uint32(item.package_id)) return false;
		if (!ba.read_uint32(item.item_id)) return false;
		if (!ba.read_uint32(item.item_cnt)) return false;
		item_vec.push_back(item);
	}
	
	if (!ba.read_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		package_item_t item;
		if (!ba.read_uint32(item.package_id)) return false;
		if (!ba.read_uint32(item.item_id)) return false;
		if (!ba.read_uint32(item.item_cnt)) return false;
		card_vec.push_back(item);
	}
	
	if (!ba.read_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		package_item_t item;
		if (!ba.read_uint32(item.package_id)) return false;
		if (!ba.read_uint32(item.item_id)) return false;
		if (!ba.read_uint32(item.item_cnt)) return false;
		stuff_vec.push_back(item);
	}

	if (!ba.read_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		package_equipment_t equipment;
		if (!equipment.read_from_buf(ba)) return false;
		equipment_vec.push_back(equipment);
	}
	return true;
}

bool cli_user_get_package_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(item_sum)) return false;
	if (!ba.write_uint32(package_size)) return false;

	int vec_size = item_vec.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		if (!ba.write_uint32(item_vec[i].package_id)) return false;
		if (!ba.write_uint32(item_vec[i].item_id)) return false;
		if (!ba.write_uint32(item_vec[i].item_cnt)) return false;
	}
	
	vec_size = card_vec.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		if (!ba.write_uint32(card_vec[i].package_id)) return false;
		if (!ba.write_uint32(card_vec[i].item_id)) return false;
		if (!ba.write_uint32(card_vec[i].item_cnt)) return false;
	}
	
	vec_size = stuff_vec.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		if (!ba.write_uint32(stuff_vec[i].package_id)) return false;
		if (!ba.write_uint32(stuff_vec[i].item_id)) return false;
		if (!ba.write_uint32(stuff_vec[i].item_cnt)) return false;
	}

	vec_size = equipment_vec.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		if (!equipment_vec[i].write_to_buf(ba)) return false;
	}
	return true;
}

//PROTO_USER_GET_ITEM_LIST_COUNT_CMD	= 502, 	//用户拉取指定itemid物品的数量
cli_user_get_item_list_count_in::cli_user_get_item_list_count_in()
{
	this->init();
}

void cli_user_get_item_list_count_in::init()
{
	this->item_vec.clear();
}

int cli_user_get_item_list_count_in::get_msg_len()
{
	return sizeof(uint32_t) + item_vec.size() * sizeof(uint32_t);
}

bool cli_user_get_item_list_count_in::read_from_buf(byte_array_t & ba)
{
	int vec_size = 0;
	if (!ba.read_int32(vec_size)) return false;
	
	for (int i = 0; i < vec_size; i++) {
		uint32_t item_id;
		if (!ba.read_uint32(item_id)) return false;
		item_vec.push_back(item_id);
	}
	return true;
}

bool cli_user_get_item_list_count_in::write_to_buf(byte_array_t & ba)
{
	int vec_size = item_vec.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		if (!ba.write_uint32(item_vec[i])) return false;
	}
	return true;
}

cli_user_get_item_list_count_out::cli_user_get_item_list_count_out()
{
	this->init();
}
    
cli_user_get_item_list_count_out::~cli_user_get_item_list_count_out()
{
	this->init();
}

void cli_user_get_item_list_count_out::init()
{
	item_vec.clear();
}

int cli_user_get_item_list_count_out::get_msg_len()
{
	return sizeof(uint32_t) + item_vec.size() * sizeof(item_unit_t);
}

bool cli_user_get_item_list_count_out::read_from_buf(byte_array_t & ba)
{
    int vec_size = 0;
	if (!ba.read_int32(vec_size)) return false;
	this->item_vec.clear();
	for (int i = 0; i < vec_size; ++i) {
		item_unit_t item;
		if (!ba.read_uint32(item.item_id)) return false;
		if (!ba.read_uint32(item.item_cnt)) return false;
		this->item_vec.push_back(item);	
	}
    return true;
}

bool cli_user_get_item_list_count_out::write_to_buf(byte_array_t & ba)
{
    int vec_size = this->item_vec.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; ++i) {
		if (!ba.write_uint32(this->item_vec[i].item_id)) return false;
		if (!ba.write_uint32(this->item_vec[i].item_cnt)) return false;
	}
    return true;
}

//  PROTO_USER_USE_ITEM_CMD         = 503,  //使用道具
cli_user_use_item_in::cli_user_use_item_in()
{
    this->init();
}

void cli_user_use_item_in::init()
{
    this->package_id = 0;
    this->item_id = 0;
}

int cli_user_use_item_in::get_msg_len()
{
    return sizeof(this->package_id) + sizeof(this->item_id);
}

bool cli_user_use_item_in::read_from_buf(byte_array_t & ba)
{
    if (!ba.read_uint32(this->package_id)) return false;
    if (!ba.read_uint32(this->item_id)) return false;
    return true;
}

bool cli_user_use_item_in::write_to_buf(byte_array_t & ba)
{
    if (!ba.write_uint32(this->package_id)) return false;
    if (!ba.write_uint32(this->item_id)) return false;
    
    return true;
}

cli_user_use_item_out::cli_user_use_item_out()
{
    this->init();
}

void cli_user_use_item_out::init()
{
    this->state = 0;
	this->item_vec.clear();
}

int cli_user_use_item_out::get_msg_len()
{
    return sizeof(this->state) + sizeof(uint32_t) + this->item_vec.size() * sizeof(package_item_t);
}

bool cli_user_use_item_out::read_from_buf(byte_array_t & ba)
{
    if (!ba.read_uint32(this->state)) return false;
    int vec_size = 0;
	if (!ba.read_int32(vec_size)) return false;
	this->item_vec.clear();
	for (int i = 0; i < vec_size; ++i) {
		package_item_t item;
		if (!ba.read_uint32(item.package_id)) return false;
		if (!ba.read_uint32(item.item_id)) return false;
		if (!ba.read_uint32(item.item_cnt)) return false;
		this->item_vec.push_back(item);	
	}
	return true;
}

bool cli_user_use_item_out::write_to_buf(byte_array_t & ba)
{
    if (!ba.write_uint32(this->state)) return false;
    int vec_size = this->item_vec.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; ++i) {
		if (!ba.write_uint32(this->item_vec[i].package_id)) return false;
		if (!ba.write_uint32(this->item_vec[i].item_id)) return false;
		if (!ba.write_uint32(this->item_vec[i].item_cnt)) return false;
	}
	return true;
}

cli_user_action_in::cli_user_action_in()
{
	this->init();
}

void cli_user_action_in::init()
{
	action_type = 0;
	community_id = 0;
	pos_x = 0;
	pos_y = 0;
}

int cli_user_action_in::get_msg_len()
{
	return sizeof(uint32_t) * 4;
}

bool cli_user_action_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(action_type)) return false;
	if (!ba.read_uint32(community_id)) return false;
	if (!ba.read_float(pos_x)) return false;
	if (!ba.read_float(pos_y)) return false;
	return true;
}

bool cli_user_action_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(action_type)) return false;
	if (!ba.write_uint32(community_id)) return false;
	if (!ba.write_float(pos_x)) return false;
	if (!ba.write_float(pos_y)) return false;
	return true;

}

cli_user_action_out::cli_user_action_out()
{
	this->init();
}

void cli_user_action_out::init()
{
	community_id = 0;
	action_type = 0;
	total_user_cnt = 0;
	user_pos_vec.clear();
}

int cli_user_action_out::get_msg_len()
{
	return sizeof(uint32_t) * 3 + sizeof(int32_t) + user_pos_vec.size() * sizeof(user_pos_info_t);
}

bool cli_user_action_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(community_id)) return false;
	if (!ba.read_uint32(action_type)) return false;
	if (!ba.read_uint32(total_user_cnt)) return false;
	
	int32_t vec_size = 0;
	if (!ba.read_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		user_pos_info_t pos_info;
		if (!ba.read_uint32(pos_info.userid)) return false;
		if (!ba.read_uint32(pos_info.unit_id)) return false;
		if (!ba.read_uint16(pos_info.equip_series)) return false;
		if (!ba.read_uint32(pos_info.level)) return false;
		if (!ba.read_uint32(pos_info.vip_level)) return false;
		if (!ba.read_buf(pos_info.nick, sizeof(pos_info.nick))) return false;
		if (!ba.read_float(pos_info.pos_x)) return false;
		if (!ba.read_float(pos_info.pos_y)) return false;
		user_pos_vec.push_back(pos_info);
	}
	return true;
}

bool cli_user_action_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(community_id)) { return false; }	
	if (!ba.write_uint32(action_type)) { return false; }	
	if (!ba.write_uint32(total_user_cnt)) { return false; }	

	int32_t vec_size = user_pos_vec.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		if (!ba.write_uint32(user_pos_vec[i].userid)) return false;
		if (!ba.write_uint32(user_pos_vec[i].unit_id)) return false;
		if (!ba.write_uint16(user_pos_vec[i].equip_series)) return false;
		if (!ba.write_uint32(user_pos_vec[i].level)) return false;
		if (!ba.write_uint32(user_pos_vec[i].vip_level)) return false;
		if (!ba.write_buf(user_pos_vec[i].nick, sizeof(user_pos_vec[i].nick))) return false;
		if (!ba.write_float(user_pos_vec[i].pos_x)) return false;
		if (!ba.write_float(user_pos_vec[i].pos_y)) return false;
	}
	
	return true;
}

//  PROTO_USER_SWAP_CMD             = 506,  //兑换或随机获取物品
cli_user_swap_in::cli_user_swap_in()
{
    this->init();
}

void cli_user_swap_in::init()
{
    this->swap_id = 0;
}

int cli_user_swap_in::get_msg_len()
{
    return sizeof(this->swap_id);
}

bool cli_user_swap_in::read_from_buf(byte_array_t & ba)
{
    if (!ba.read_uint32(this->swap_id)) return false;
    return true;
}

bool cli_user_swap_in::write_to_buf(byte_array_t & ba)
{
    if (!ba.write_uint32(this->swap_id)) return false;
    return true;
}

cli_user_swap_out::cli_user_swap_out()
{
    this->init();
}

cli_user_swap_out::~cli_user_swap_out()
{
    this->init();
}

void cli_user_swap_out::init()
{
    state = 0;
    this->item_vec.clear();
}

int cli_user_swap_out::get_msg_len()
{
    return sizeof(state) + sizeof(uint32_t) + this->item_vec.size() * sizeof(package_item_t);
}

bool cli_user_swap_out::read_from_buf(byte_array_t & ba)
{
    if (!ba.read_uint32(state)) return false;
    int vec_size = 0;
	if (!ba.read_int32(vec_size)) return false;
	this->item_vec.clear();
	for (int i = 0; i < vec_size; ++i) {
		package_item_t item;
		if (!ba.read_uint32(item.package_id)) return false;
		if (!ba.read_uint32(item.item_id)) return false;
		if (!ba.read_uint32(item.item_cnt)) return false;
		this->item_vec.push_back(item);	
	}
    return true;
}

bool cli_user_swap_out::write_to_buf(byte_array_t & ba)
{
    if (!ba.write_uint32(state)) return false;
    int vec_size = this->item_vec.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; ++i) {
		if (!ba.write_uint32(this->item_vec[i].package_id)) return false;
		if (!ba.write_uint32(this->item_vec[i].item_id)) return false;
		if (!ba.write_uint32(this->item_vec[i].item_cnt)) return false;
	}
    return true;
}

// PROTO_USER_BUY_ITEM_CMD			= 507,	//yxb购买物品
cli_user_buy_item_in::cli_user_buy_item_in()
{
	this->init();
}

void cli_user_buy_item_in::init()
{
	item_id = 0;
	item_cnt = 0;
}

int cli_user_buy_item_in::get_msg_len()
{
	return sizeof(uint32_t) * 2;
}

bool cli_user_buy_item_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(item_id)) return false;
	if (!ba.read_uint32(item_cnt)) return false;
	return true;
}

bool cli_user_buy_item_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(item_id)) return false;
	if (!ba.write_uint32(item_cnt)) return false;
	return true;
}

// PROTO_USER_ADJUST_PACKAGE_CMD    = 509,  //整理背包
cli_user_adjust_package_out::cli_user_adjust_package_out()
{
	this->init();
}

cli_user_adjust_package_out::~cli_user_adjust_package_out()
{
	this->init();
}

void cli_user_adjust_package_out::init()
{
	this->package_type = 0;
	this->item_vec.clear();
	this->equipment_vec.clear();
}

int cli_user_adjust_package_out::get_msg_len()
{
	int len = sizeof(this->package_type);
	if (this->package_type == 4) {
		len += sizeof(uint32_t) + equipment_vec.size() * sizeof(equipment_vec);
	} else {
		len += sizeof(uint32_t) + item_vec.size() * sizeof(item_vec);
	}
	return len;
}

bool cli_user_adjust_package_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(this->package_type)) return false;
	int32_t vec_size = 0;
	if (!ba.read_int32(vec_size)) return false;
	if (this->package_type == 4) {
		this->equipment_vec.clear();
		for (int i = 0; i < vec_size; i++) {
			package_equipment_t equipment;
			if (!equipment.read_from_buf(ba)) return false;
			this->equipment_vec.push_back(equipment);
		}
	} else {
		this->item_vec.clear();
		for (int i = 0; i < vec_size; i++) {
			package_item_t item;
			if (!ba.read_uint32(item.package_id)) return false;
			if (!ba.read_uint32(item.item_id)) return false;
			if (!ba.read_uint32(item.item_cnt)) return false;
			this->item_vec.push_back(item);
		}
	}
	return true;
}

bool cli_user_adjust_package_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(this->package_type)) return false;
	if (this->package_type == 4) {
		int32_t vec_size = this->equipment_vec.size();
		if (!ba.write_int32(vec_size)) return false;
		for (int i = 0; i < vec_size; i++) {
			if (!equipment_vec[i].write_to_buf(ba)) return false;
		}
	} else {
		int32_t vec_size = this->item_vec.size();
		if (!ba.write_int32(vec_size)) return false;
		for (int i = 0; i < vec_size; i++) {
			if (!ba.write_uint32(this->item_vec[i].package_id)) return false;
			if (!ba.write_uint32(this->item_vec[i].item_id)) return false;
			if (!ba.write_uint32(this->item_vec[i].item_cnt)) return false;
		}
	}
	return true;
}

cli_user_sell_item_in::cli_user_sell_item_in()
{
	this->init();
}

void cli_user_sell_item_in::init()
{
	package_id = 0;
	item_id = 0;
	item_cnt = 0;
}

int cli_user_sell_item_in::get_msg_len()
{
	return sizeof(uint32_t) * 3;
}

bool cli_user_sell_item_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(package_id)) return false;
	if (!ba.read_uint32(item_id)) return false;
	if (!ba.read_uint32(item_cnt)) return false;
	return true;
}

bool cli_user_sell_item_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(package_id)) return false;
	if (!ba.write_uint32(item_id)) return false;
	if (!ba.write_uint32(item_cnt)) return false;
	return true;
}

cli_user_sell_item_out::cli_user_sell_item_out()
{
	this->init();
}

void cli_user_sell_item_out::init()
{
	package_id = 0;
	item_id = 0;
	item_cnt = 0;
	add_yxb = 0;
}

int cli_user_sell_item_out::get_msg_len()
{
	return sizeof(uint32_t) * 4;
}

bool cli_user_sell_item_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(package_id)) return false;
	if (!ba.read_uint32(item_id)) return false;
	if (!ba.read_uint32(item_cnt)) return false;
	if (!ba.read_uint32(add_yxb)) return false;
	return true;
}
    
bool cli_user_sell_item_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(package_id)) return false;
	if (!ba.write_uint32(item_id)) return false;
	if (!ba.write_uint32(item_cnt)) return false;
	if (!ba.write_uint32(add_yxb)) return false;
	return true;
}

//PROTO_USER_TRAIN_EQUIPMENT_CMD  = 513,	//装备洗练
cli_user_train_equipment_in::cli_user_train_equipment_in()
{
	this->init();
}

void cli_user_train_equipment_in::init()
{
	train_type = 0;	//0 确认培养 1普通培养 2指定培养
	package_id = 0;
	equipment_id = 0;
}

int cli_user_train_equipment_in::get_msg_len()
{
	return sizeof(uint32_t) * 3;
}

bool cli_user_train_equipment_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(train_type)) return false;
	if (!ba.read_uint32(package_id)) return false;
	if (!ba.read_uint32(equipment_id)) return false;
	return true;
}

bool cli_user_train_equipment_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(train_type)) return false;
	if (!ba.write_uint32(package_id)) return false;
	if (!ba.write_uint32(equipment_id)) return false;
	return true;
}

cli_user_train_equipment_out::cli_user_train_equipment_out()
{
	this->init();
}

void cli_user_train_equipment_out::init()
{
	memset(&equipment, 0, sizeof(equipment));
}

int cli_user_train_equipment_out::get_msg_len()
{
	return sizeof(equipment);
}

bool cli_user_train_equipment_out::read_from_buf(byte_array_t & ba)
{
	if (!equipment.read_from_buf(ba)) return false;
	return true;
}

bool cli_user_train_equipment_out::write_to_buf(byte_array_t & ba)
{
	if (!equipment.write_to_buf(ba)) return false;
	return true;
}

//	PROTO_USER_CHANGE_EQUIPMENT_CMD = 514,	//用户换装(穿上,替换,脱下)
cli_user_change_equipment_in::cli_user_change_equipment_in()
{
    this->init();
}

void cli_user_change_equipment_in::init()
{
    this->unit_id = 0;
    this->package_id = 0;
	this->equipment_id = 0;
    this->is_wear = 0;
}

int cli_user_change_equipment_in::get_msg_len()
{
    return sizeof(this->unit_id) + sizeof(this->equipment_id) + sizeof(this->package_id) + sizeof(this->is_wear);
}

bool cli_user_change_equipment_in::read_from_buf(byte_array_t & ba)
{    
    if (!ba.read_uint32(this->is_wear)) return false;
    if (!ba.read_uint32(this->unit_id)) return false;
    if (!ba.read_uint32(this->package_id)) return false;
    if (!ba.read_uint32(this->equipment_id)) return false;
    return true;
}

bool cli_user_change_equipment_in::write_to_buf(byte_array_t & ba)
{
    if (!ba.write_uint32(this->is_wear)) return false;
    if (!ba.write_uint32(this->unit_id)) return false;
    if (!ba.write_uint32(this->package_id)) return false;
    if (!ba.write_uint32(this->equipment_id)) return false;
    return true;
}

cli_user_change_equipment_out::cli_user_change_equipment_out()
{
    this->init();
}
    
void cli_user_change_equipment_out::init()
{
	this->is_wear = 0;
    this->unit_id = 0;
	this->on_package_id = 0;
   	this->on_old_package_id = 0;
    this->off_package_id = 0;
   	this->off_old_package_id = 0;
}
    
int cli_user_change_equipment_out::get_msg_len()
{
	return 6 * sizeof(uint32_t);
}
    
bool cli_user_change_equipment_out::read_from_buf(byte_array_t & ba)
{
    if (!ba.read_uint32(this->is_wear)) return false;
    if (!ba.read_uint32(this->unit_id)) return false;
    if (!ba.read_uint32(this->on_package_id)) return false;
    if (!ba.read_uint32(this->on_old_package_id)) return false;
    if (!ba.read_uint32(this->off_package_id)) return false;
    if (!ba.read_uint32(this->off_old_package_id)) return false;
    return true;
}

bool cli_user_change_equipment_out::write_to_buf(byte_array_t & ba)
{
    if (!ba.write_uint32(this->is_wear)) return false;
    if (!ba.write_uint32(this->unit_id)) return false;
    if (!ba.write_uint32(this->on_package_id)) return false;
    if (!ba.write_uint32(this->on_old_package_id)) return false;
    if (!ba.write_uint32(this->off_package_id)) return false;
    if (!ba.write_uint32(this->off_old_package_id)) return false;
    return true;
}

//	PROTO_USER_STRENGTHEN_EQUIPMENT_CMD = 515,	//装备强化
cli_user_strengthen_equipment_in::cli_user_strengthen_equipment_in()
{
    this->init();
}

void cli_user_strengthen_equipment_in::init()
{
    is_teen = 0;
    this->package_id = 0;
    this->item_id = 0;
}

int cli_user_strengthen_equipment_in::get_msg_len()
{
    return sizeof(is_teen) + sizeof(this->package_id) + sizeof(this->item_id);
}

bool cli_user_strengthen_equipment_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(is_teen)) return false;
    if (!ba.read_uint32(this->package_id)) return false;
    if (!ba.read_uint32(this->item_id)) return false;
    return true;
}

bool cli_user_strengthen_equipment_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(is_teen)) return false;
    if (!ba.write_uint32(this->package_id)) return false;
    if (!ba.write_uint32(this->item_id)) return false;
    return true;
}

cli_user_strengthen_equipment_out::cli_user_strengthen_equipment_out()
{
    this->init();
}
    
void cli_user_strengthen_equipment_out::init()
{
    this->cost_yxb = 0;
	this->cd_time = 0;
	this->cd_state = 0;
}
    
int cli_user_strengthen_equipment_out::get_msg_len()
{
    return sizeof(this->cost_yxb) * 3;
}

bool cli_user_strengthen_equipment_out::read_from_buf(byte_array_t & ba)
{
    if (!ba.read_uint32(this->cost_yxb)) return false;
    if (!ba.read_uint32(this->cd_time)) return false;
    if (!ba.read_uint32(this->cd_state)) return false;
    return true;
}

bool cli_user_strengthen_equipment_out::write_to_buf(byte_array_t & ba)
{
    if (!ba.write_uint32(this->cost_yxb)) return false;
    if (!ba.write_uint32(this->cd_time)) return false;
    if (!ba.write_uint32(this->cd_state)) return false;
    return true;
}

//	PROTO_USER_UPGRADE_EQUIPMENT_CMD	= 516,	//装备进阶
cli_user_upgrade_equipment_in::cli_user_upgrade_equipment_in()
{
    this->init();
}

void cli_user_upgrade_equipment_in::init()
{
	this->cost_gold = 0;
    this->package_id = 0;
    this->item_id = 0;
}

int cli_user_upgrade_equipment_in::get_msg_len()
{
    return sizeof(this->cost_gold) + sizeof(this->package_id) + sizeof(this->item_id);
}

bool cli_user_upgrade_equipment_in::read_from_buf(byte_array_t & ba)
{
    if (!ba.read_uint32(this->cost_gold)) return false;
    if (!ba.read_uint32(this->package_id)) return false;
    if (!ba.read_uint32(this->item_id)) return false;
    return true;
}

bool cli_user_upgrade_equipment_in::write_to_buf(byte_array_t & ba)
{
    if (!ba.write_uint32(this->cost_gold)) return false;
    if (!ba.write_uint32(this->package_id)) return false;
    if (!ba.write_uint32(this->item_id)) return false;
    return true;
}

cli_user_upgrade_equipment_out::cli_user_upgrade_equipment_out()
{
    this->init();
}

void cli_user_upgrade_equipment_out::init()
{
	this->cost_gold = 0;
    this->package_id = 0;
    this->new_item_id = 0;
    this->new_item_level = 0;
}

int cli_user_upgrade_equipment_out::get_msg_len()
{
    return sizeof(uint32_t) * 4;
}

bool cli_user_upgrade_equipment_out::read_from_buf(byte_array_t & ba)
{
    if (!ba.read_uint32(this->cost_gold)) return false;
    if (!ba.read_uint32(this->package_id)) return false;
    if (!ba.read_uint32(this->new_item_id)) return false;
    if (!ba.read_uint32(this->new_item_level)) return false;
    return true;
}

bool cli_user_upgrade_equipment_out::write_to_buf(byte_array_t & ba)
{
    if (!ba.write_uint32(this->cost_gold)) return false;
    if (!ba.write_uint32(this->package_id)) return false;
    if (!ba.write_uint32(this->new_item_id)) return false;
    if (!ba.write_uint32(this->new_item_level)) return false;
    return true;
}

//PROTO_USER_PUNCH_EQUIPMENT_CMD	= 524,	//装备打孔
cli_user_punch_equipment_in::cli_user_punch_equipment_in()
{
    this->init();
}

void cli_user_punch_equipment_in::init()
{
    this->package_id = 0;
    this->item_id = 0;
    this->position = 0;
}

int cli_user_punch_equipment_in::get_msg_len()
{
    return sizeof(this->package_id) + sizeof(this->item_id) + sizeof(this->position);
}

bool cli_user_punch_equipment_in::read_from_buf(byte_array_t & ba)
{
    if (!ba.read_uint32(this->package_id)) return false;
    if (!ba.read_uint32(this->item_id)) return false;
    if (!ba.read_uint32(this->position)) return false;
    return true;
}

bool cli_user_punch_equipment_in::write_to_buf(byte_array_t & ba)
{
    if (!ba.write_uint32(this->package_id)) return false;
    if (!ba.write_uint32(this->item_id)) return false;
    if (!ba.write_uint32(this->position)) return false;
    return true;
}

//PROTO_USER_INLAY_GEM_CMD		= 525,	//装备镶嵌宝石
cli_user_inlay_gem_in::cli_user_inlay_gem_in()
{
    this->init();
}

void cli_user_inlay_gem_in::init()
{
    this->op_type = 0;
    this->package_id = 0;
    this->item_id = 0;
    this->position = 0;
    this->gem_id = 0;
}

int cli_user_inlay_gem_in::get_msg_len()
{
    return sizeof(op_type) + sizeof(package_id) + sizeof(item_id) + sizeof(position) + sizeof(gem_id);
}

bool cli_user_inlay_gem_in::read_from_buf(byte_array_t & ba)
{
    if (!ba.read_uint32(this->op_type)) return false;
    if (!ba.read_uint32(this->package_id)) return false;
    if (!ba.read_uint32(this->item_id)) return false;
    if (!ba.read_uint32(this->position)) return false;
    if (!ba.read_uint32(this->gem_id)) return false;
    return true;
}

bool cli_user_inlay_gem_in::write_to_buf(byte_array_t & ba)
{
    if (!ba.write_uint32(this->op_type)) return false;
    if (!ba.write_uint32(this->package_id)) return false;
    if (!ba.write_uint32(this->item_id)) return false;
    if (!ba.write_uint32(this->position)) return false;
    if (!ba.write_uint32(this->gem_id)) return false;
    return true;
}

//PROTO_USER_COMPOSE_GEM_CMD		= 526,	//宝石合成
cli_user_compose_gem_out::cli_user_compose_gem_out()
{
    this->init();
}

void cli_user_compose_gem_out::init()
{
    this->del_gem_id = 0;
    this->del_gem_cnt = 0;
    this->del_rune_id = 0;
    this->del_rune_cnt = 0;
}

int cli_user_compose_gem_out::get_msg_len()
{
    return sizeof(del_gem_id) + sizeof(del_gem_cnt) + sizeof(del_rune_id) + sizeof(del_rune_cnt);
}

bool cli_user_compose_gem_out::read_from_buf(byte_array_t & ba)
{
    if (!ba.read_uint32(this->del_gem_id)) return false;
    if (!ba.read_uint32(this->del_gem_cnt)) return false;
    if (!ba.read_uint32(this->del_rune_id)) return false;
    if (!ba.read_uint32(this->del_rune_cnt)) return false;
    return true;
}

bool cli_user_compose_gem_out::write_to_buf(byte_array_t & ba)
{
    if (!ba.write_uint32(this->del_gem_id)) return false;
    if (!ba.write_uint32(this->del_gem_cnt)) return false;
    if (!ba.write_uint32(this->del_rune_id)) return false;
    if (!ba.write_uint32(this->del_rune_cnt)) return false;
    return true;
}

//PROTO_USER_TRAIN_BASE_ATTR_CMD = 518,    //培养
cli_user_train_base_attr_in::cli_user_train_base_attr_in()
{
    this->init();
}

void cli_user_train_base_attr_in::init()
{
	unit_id = 0;
    train_type = 0;
}

int cli_user_train_base_attr_in::get_msg_len()
{
    return sizeof(unit_id) + sizeof(train_type);
}

bool cli_user_train_base_attr_in::read_from_buf(byte_array_t & ba)
{
    if (!ba.read_uint32(this->unit_id)) return false;
    if (!ba.read_uint32(this->train_type)) return false;
    return true;
}

bool cli_user_train_base_attr_in::write_to_buf(byte_array_t & ba)
{
    if (!ba.write_uint32(this->unit_id)) return false;
    if (!ba.write_uint32(this->train_type)) return false;
    return true;
}

cli_user_train_base_attr_out::cli_user_train_base_attr_out()
{
    this->init();
}

void cli_user_train_base_attr_out::init()
{
	unit_id = 0;
    train_hp = 0;                //培养增加的生命值
    train_out_attack = 0;        //培养增加的攻击力
    train_out_def = 0;           //培养增加的防御力
    train_in_attack = 0;         //培养增加的内攻
    train_in_def = 0;            //培养增加的内防
	train_toughness = 0;             //培养增加的内力
}

int cli_user_train_base_attr_out::get_msg_len()
{
    return sizeof(unit_id) + sizeof(int) * 6;
}

bool cli_user_train_base_attr_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(unit_id)) return false;
    if (!ba.read_int32(train_hp)) return false;
    if (!ba.read_int32(train_out_attack)) return false;
    if (!ba.read_int32(train_out_def)) return false;
    if (!ba.read_int32(train_in_attack)) return false;
    if (!ba.read_int32(train_in_def)) return false;
    if (!ba.read_int32(train_toughness)) return false;
    return true;
}

bool cli_user_train_base_attr_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(unit_id)) return false;
    if (!ba.write_uint32(train_hp)) return false;
    if (!ba.write_uint32(train_out_attack)) return false;
    if (!ba.write_uint32(train_out_def)) return false;
    if (!ba.write_uint32(train_in_attack)) return false;
    if (!ba.write_uint32(train_in_def)) return false;
    if (!ba.write_uint32(train_toughness)) return false;
    return true;
}

//	PROTO_USER_GET_SKILL_CMD		= 520,	//用户拉取所有技能
cli_user_get_skill_in::cli_user_get_skill_in()
{
    this->init();
}

void cli_user_get_skill_in::init()
{
    this->userid = 0;
}

int cli_user_get_skill_in::get_msg_len()
{
    return sizeof(this->userid);
}


bool cli_user_get_skill_in::read_from_buf(byte_array_t & ba)
{
    if (!ba.read_uint32(this->userid)) return false;
    return true;
}

bool cli_user_get_skill_in::write_to_buf(byte_array_t & ba)
{
    if (!ba.write_uint32(this->userid)) return false;
    return true;
}

 
cli_user_get_skill_out::cli_user_get_skill_out()
{
    this->init();
}

cli_user_get_skill_out::~cli_user_get_skill_out()
{
    this->init();
}

void cli_user_get_skill_out::init()
{
    this->skill_vec.clear();
}

int cli_user_get_skill_out::get_msg_len()
{
    return sizeof(uint32_t) + this->skill_vec.size() * sizeof(user_skill_t);
}

bool cli_user_get_skill_out::read_from_buf(byte_array_t & ba)
{
    int32_t skill_size = 0;
    if (!ba.read_int32(skill_size)) return false;
    for (int i = 0; i < skill_size; i++) {
        user_skill_t skill;
        if (!ba.read_uint32(skill.skill_id)) return false;
        if (!ba.read_uint32(skill.skill_lvl)) return false;
        if (!ba.read_uint32(skill.wear_seat)) return false;
        this->skill_vec.push_back(skill);
    }
    return true;
}

bool cli_user_get_skill_out::write_to_buf(byte_array_t & ba)
{
    int32_t skill_size = this->skill_vec.size();
    if (!ba.write_int32(skill_size)) return false;
    for (int i = 0; i < skill_size; i++) {
        if (!ba.write_uint32(this->skill_vec[i].skill_id)) return false;
        if (!ba.write_uint32(this->skill_vec[i].skill_lvl)) return false;
        if (!ba.write_uint32(this->skill_vec[i].wear_seat)) return false;
    }
    return true;
}

//	PROTO_USER_UPGRADE_SKILL_CMD	= 521,	//技能升级
cli_user_upgrade_skill_in::cli_user_upgrade_skill_in()
{
    this->init();
}

void cli_user_upgrade_skill_in::init()
{
    this->skill_id = 0;
    this->skill_level = 0;
}

int cli_user_upgrade_skill_in::get_msg_len()
{
    return sizeof(this->skill_id) + sizeof(this->skill_level);
}

bool cli_user_upgrade_skill_in::read_from_buf(byte_array_t & ba)
{
    if (!ba.read_uint32(this->skill_id)) return false;
    if (!ba.read_uint32(this->skill_level)) return false;
    return true;
}

bool cli_user_upgrade_skill_in::write_to_buf(byte_array_t & ba)
{
    if (!ba.write_uint32(this->skill_id)) return false;
    if (!ba.write_uint32(this->skill_level)) return false;
    return true;
}

cli_user_upgrade_skill_out::cli_user_upgrade_skill_out()
{
    this->init();
}
    
void cli_user_upgrade_skill_out::init()
{
    this->skill_id = 0;
    this->skill_level = 0;
    this->cost_yxb = 0;
    this->cost_power = 0;
}
    
int cli_user_upgrade_skill_out::get_msg_len()
{
    return sizeof(this->skill_id) * 4;
}

bool cli_user_upgrade_skill_out::read_from_buf(byte_array_t & ba)
{
    if (!ba.read_uint32(this->skill_id)) return false;
    if (!ba.read_uint32(this->skill_level)) return false;
    if (!ba.read_uint32(this->cost_yxb)) return false;
    if (!ba.read_uint32(this->cost_power)) return false;
    return true;
}

bool cli_user_upgrade_skill_out::write_to_buf(byte_array_t & ba)
{
    if (!ba.write_uint32(this->skill_id)) return false;
    if (!ba.write_uint32(this->skill_level)) return false;
    if (!ba.write_uint32(this->cost_yxb)) return false;
    if (!ba.write_uint32(this->cost_power)) return false;
    return true;
}

//	PROTO_USER_CHANGE_SKILL_CMD		= 522,	//用户装载技能(装载,替换,卸下)
cli_user_change_skill_in::cli_user_change_skill_in()
{
    this->init();
}

void cli_user_change_skill_in::init()
{
    this->skill_id = 0;
    this->wear_seat = 0;
}

int cli_user_change_skill_in::get_msg_len()
{
    return sizeof(this->skill_id) + sizeof(this->wear_seat);
}

bool cli_user_change_skill_in::read_from_buf(byte_array_t & ba)
{
    if (!ba.read_uint32(this->skill_id)) return false;
    if (!ba.read_uint32(this->wear_seat)) return false;
    return true;
}

bool cli_user_change_skill_in::write_to_buf(byte_array_t & ba)
{
    if (!ba.write_uint32(this->skill_id)) return false;
    if (!ba.write_uint32(this->wear_seat)) return false;
    return true;
}

cli_user_change_skill_out::cli_user_change_skill_out()
{
    this->init();
}

void cli_user_change_skill_out::init()
{
	memset(this->skills, 0, sizeof(skills));
}

int cli_user_change_skill_out::get_msg_len()
{
    return sizeof(skills);
}

bool cli_user_change_skill_out::read_from_buf(byte_array_t & ba)
{
    if (!ba.read_buf((char*)this->skills, sizeof(this->skills))) return false;
    return true;
}

bool cli_user_change_skill_out::write_to_buf(byte_array_t & ba)
{
    if (!ba.write_buf((char*)this->skills, sizeof(this->skills))) return false;
    return true;
}

//PROTO_USER_UPGRADE_FORCE_CMD  	= 530,  //练气
cli_user_upgrade_force_in::cli_user_upgrade_force_in()
{
	this->init();
}

void cli_user_upgrade_force_in::init()
{
	type = 0;
	cost_value = 0;
}

int cli_user_upgrade_force_in::get_msg_len()
{
	return sizeof(uint32_t) * 2;
}

bool cli_user_upgrade_force_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(type)) return false;
	if (!ba.read_uint32(cost_value)) return false;
	return true;
}

bool cli_user_upgrade_force_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(type)) return false;
	if (!ba.write_uint32(cost_value)) return false;
	return true;
}

cli_user_upgrade_force_out::cli_user_upgrade_force_out()
{
	this->init();
}

void cli_user_upgrade_force_out::init()
{
	is_crit = 0;
	force_exp = 0;
	type = 0;
	cost_value = 0;
	is_finished = 0;
}

int cli_user_upgrade_force_out::get_msg_len()
{
	return sizeof(uint32_t) * 5;
}

bool cli_user_upgrade_force_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(is_crit)) return false;
	if (!ba.read_uint32(force_exp)) return false;
	if (!ba.read_uint32(type)) return false;
	if (!ba.read_uint32(cost_value)) return false;
	if (!ba.read_uint32(is_finished)) return false;
	return true;
}

bool cli_user_upgrade_force_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(is_crit)) return false;
	if (!ba.write_uint32(force_exp)) return false;
	if (!ba.write_uint32(type)) return false;
	if (!ba.write_uint32(cost_value)) return false;
	if (!ba.write_uint32(is_finished)) return false;
	return true;
}

cli_user_upgrade_nerve_in::cli_user_upgrade_nerve_in()
{
	this->init();
}

void cli_user_upgrade_nerve_in::init()
{
	is_batch = 0;
	nerve_id = 0;
	nerve_type = 0;//1普通 2元宝
}

int cli_user_upgrade_nerve_in::get_msg_len()
{
	return sizeof(uint32_t) * 3;
}

bool cli_user_upgrade_nerve_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(is_batch)) return false;
	if (!ba.read_uint32(nerve_id)) return false;
	if (!ba.read_uint32(nerve_type)) return false;
	return true;
}

bool cli_user_upgrade_nerve_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(is_batch)) return false;
	if (!ba.write_uint32(nerve_id)) return false;
	if (!ba.write_uint32(nerve_type)) return false;
	return true;
}

cli_user_upgrade_nerve_out::cli_user_upgrade_nerve_out()
{
	this->init();
}

void cli_user_upgrade_nerve_out::init()
{
	upgrade_succ = 0;
	cost_air = 0;
	cost_gold = 0;
	cd_type = 0;
	cd_time = 0;
	cd_state = 0;
	add_level = 0;
	inject_times = 0;
}

int cli_user_upgrade_nerve_out::get_msg_len()
{
	return sizeof(uint32_t) * 8;
}

bool cli_user_upgrade_nerve_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(upgrade_succ)) return false;
	if (!ba.read_uint32(cost_air)) return false;
	if (!ba.read_uint32(cost_gold)) return false;
	if (!ba.read_uint32(cd_type)) return false;
	if (!ba.read_uint32(cd_time)) return false;
	if (!ba.read_uint32(cd_state)) return false;
	if (!ba.read_uint32(add_level)) return false;
	if (!ba.read_uint32(inject_times)) return false;
	return true;
}

bool cli_user_upgrade_nerve_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(upgrade_succ)) return false;
	if (!ba.write_uint32(cost_air)) return false;
	if (!ba.write_uint32(cost_gold)) return false;
	if (!ba.write_uint32(cd_type)) return false;
	if (!ba.write_uint32(cd_time)) return false;
	if (!ba.write_uint32(cd_state)) return false;
	if (!ba.write_uint32(add_level)) return false;
	if (!ba.write_uint32(inject_times)) return false;
	return true;

}

//PROTO_USER_GET_COOLDOWN_CMD = 532, //得到某类型的CD时间
cli_user_get_cooldown_out::cli_user_get_cooldown_out()
{
	this->init();
}

void cli_user_get_cooldown_out::init()
{
	cd_type = 0;
	cd_time = 0;
	cd_state = 0;
}

int cli_user_get_cooldown_out::get_msg_len()
{
	return sizeof(cd_type) + sizeof(cd_time) + sizeof(cd_state);
}

bool cli_user_get_cooldown_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(cd_type)) return false;
	if (!ba.read_uint32(cd_time)) return false;
	if (!ba.read_uint32(cd_state)) return false;
	return true;
}

bool cli_user_get_cooldown_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(cd_type)) return false;
	if (!ba.write_uint32(cd_time)) return false;
	if (!ba.write_uint32(cd_state)) return false;
	return true;
}

//PROTO_USER_CLEAR_COOLDOWN_CMD = 533, //清楚某类型的CD时间
cli_user_clear_cooldown_in::cli_user_clear_cooldown_in()
{
	this->init();
}

void cli_user_clear_cooldown_in::init()
{
	cd_type = 0;
	cost_gold = 0;
}

int cli_user_clear_cooldown_in::get_msg_len()
{
	return sizeof(cd_type) + sizeof(cost_gold);
}

bool cli_user_clear_cooldown_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(cd_type)) return false;
	if (!ba.read_uint32(cost_gold)) return false;
	return true;
}

bool cli_user_clear_cooldown_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(cd_type)) return false;
	if (!ba.write_uint32(cost_gold)) return false;
	return true;
}

//PROTO_USER_GET_TYPE_EVENT_CMD	= 534,	//得到某事件的完成次数
cli_user_get_daily_event_out::cli_user_get_daily_event_out()
{
	this->init();
}

void cli_user_get_daily_event_out::init()
{
	this->event_type = 0;
	this->event_count = 0;
}

int cli_user_get_daily_event_out::get_msg_len()
{
	return sizeof(event_type) + sizeof(event_count);
}

bool cli_user_get_daily_event_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(event_type)) return false;
	if (!ba.read_uint32(event_count)) return false;
	return true;
}

bool cli_user_get_daily_event_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(event_type)) return false;
	if (!ba.write_uint32(event_count)) return false;
	return true;
}

//PROTO_USER_GET_MISSION_CMD      = 540,  //拉去关卡信息
cli_user_get_mission_out::cli_user_get_mission_out()
{
	this->init();
}

void cli_user_get_mission_out::init()
{
	is_reseted = 0;
	mission_vec.clear();	
}

int cli_user_get_mission_out::get_msg_len()
{
	return sizeof(is_reseted) + sizeof(uint32_t) + mission_vec.size() * sizeof(user_mission_t); 
}

bool cli_user_get_mission_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(is_reseted)) return false;
	int32_t vec_size = 0;
	if (!ba.read_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		user_mission_t mission;
		if (!ba.read_uint32(mission.mission_id)) return false;
		if (!ba.read_uint32(mission.score)) return false;
		if (!ba.read_uint32(mission.value)) return false;
		mission_vec.push_back(mission);
	}
	return true;
}

bool cli_user_get_mission_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(is_reseted)) return false;
	int32_t vec_size = mission_vec.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		if (!ba.write_uint32(mission_vec[i].mission_id)) return false;
		if (!ba.write_uint32(mission_vec[i].score)) return false;
		if (!ba.write_uint32(mission_vec[i].value)) return false;
	}
	return true;
}

//PROTO_USER_BUY_ENERY_CMD        = 541,  //黄金购买体力
cli_user_buy_enery_out::cli_user_buy_enery_out()
{
	this->init();
}

void cli_user_buy_enery_out::init()
{
	cost_gold = 0;//消耗的黄金
	cur_enery = 0;//当前的总体力
	add_enery = 0;//购买增加的体力
	enery_counter = 0;//倒计时
}

int cli_user_buy_enery_out::get_msg_len()
{
	return sizeof(uint32_t) * 4;
}

bool cli_user_buy_enery_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(cost_gold)) return false;
	if (!ba.read_uint32(cur_enery)) return false;
	if (!ba.read_uint32(add_enery)) return false;
	if (!ba.read_uint32(enery_counter)) return false;
	return true;
}

bool cli_user_buy_enery_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(cost_gold)) return false;
	if (!ba.write_uint32(cur_enery)) return false;
	if (!ba.write_uint32(add_enery)) return false;
	if (!ba.write_uint32(enery_counter)) return false;
	return true;
}

//PROTO_USER_RECOVER_ENERY_CMD    = 542,  //恢复体力
cli_user_recover_enery_out::cli_user_recover_enery_out()
{
	this->init();
}

void cli_user_recover_enery_out::init()
{
	cur_enery = 0;//当前的总体力
	enery_max = 0;
	enery_counter = 0;
}

int cli_user_recover_enery_out::get_msg_len()
{
	return sizeof(cur_enery) + sizeof(enery_max) + sizeof(enery_counter);
}

bool cli_user_recover_enery_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(cur_enery)) return false;
	if (!ba.read_uint32(enery_max)) return false;
	if (!ba.read_uint32(enery_counter)) return false;
	return true;
}

bool cli_user_recover_enery_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(cur_enery)) return false;
	if (!ba.write_uint32(enery_max)) return false;
	if (!ba.write_uint32(enery_counter)) return false;
	return true;
}

//PROTO_USER_START_AUTO_BATTLE_CMD= 543,  //开始扫荡
cli_user_start_auto_battle_in::cli_user_start_auto_battle_in()
{
	this->init();
}

void cli_user_start_auto_battle_in::init()
{
	map_id = 0;
	battle_times = 0;
}

int cli_user_start_auto_battle_in::get_msg_len()
{
	return sizeof(map_id) + sizeof(battle_times);
}

bool cli_user_start_auto_battle_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(map_id)) return false;
	if (!ba.read_uint32(battle_times)) return false;
	return true;
}

bool cli_user_start_auto_battle_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(map_id)) return false;
	if (!ba.write_uint32(battle_times)) return false;
	return true;
}

cli_user_start_auto_battle_out::cli_user_start_auto_battle_out()
{
	this->init();
}

void cli_user_start_auto_battle_out::init()
{
	wait_flag = 0;
	prize_vec.clear();
}

int cli_user_start_auto_battle_out::get_msg_len()
{
	int len = sizeof(wait_flag);
	for (int i = 0; i < (int)prize_vec.size(); i++) {
		len += prize_vec[i].get_msg_len();
	}
	return len;
}

bool cli_user_start_auto_battle_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(wait_flag)) return false;

	int prize_size = 0;
	if (!ba.read_int32(prize_size)) return false;
	for (int i = 0; i < prize_size; ++i) {
		auto_battle_t prize;
		prize.read_from_buf(ba);
		prize_vec.push_back(prize);
	}
	return true;
}

bool cli_user_start_auto_battle_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(wait_flag)) return false;

	int prize_size = prize_vec.size();
	if (!ba.write_int32(prize_size)) return false;
	for (int i = 0; i < prize_size; ++i) {
		prize_vec[i].write_to_buf(ba);
	}
	return true;
}

//PROTO_USER_STOP_AUTO_BATTLE_CMD = 544,  //结束扫荡

//PROTO_USER_GET_ROUNDS_BATTLE_CMD = 545,	//得到当前扫荡结果
cli_user_get_rounds_battle_out::cli_user_get_rounds_battle_out()
{
	this->init();
}

void cli_user_get_rounds_battle_out::init()
{
	is_elite = 0;
	map_id = 0;
	battle_counter = 0;//倒计时
	battle_times = 0;//剩余战斗次数
	cur_round = 0;
	prize_vec.clear();
}

int cli_user_get_rounds_battle_out::get_msg_len()
{
	int len = sizeof(uint32_t) * 5;
	len += sizeof(uint32_t);
	for (int i = 0; i < (int)prize_vec.size(); i++) {
		len += sizeof(uint32_t) * 1;
		len += sizeof(uint32_t) + prize_vec[i].item_vec.size() * sizeof(item_unit_t);
		len += sizeof(uint32_t) + prize_vec[i].drop_vec.size() * sizeof(item_unit_t);
	}
	return len;
}

bool cli_user_get_rounds_battle_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(is_elite)) return false;
	if (!ba.read_uint32(map_id)) return false;
	if (!ba.read_uint32(battle_counter)) return false;
	if (!ba.read_uint32(battle_times)) return false;
	if (!ba.read_uint32(cur_round)) return false;
	
	int prize_size = 0;
	if (!ba.read_int32(prize_size)) return false;
	for (int i = 0; i < prize_size; i++) {
        auto_battle_t prize;
        prize.read_from_buf(ba);
        prize_vec.push_back(prize);
	}
	return true;
}

bool cli_user_get_rounds_battle_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(is_elite)) return false;
	if (!ba.write_uint32(map_id)) return false;
	if (!ba.write_uint32(battle_counter)) return false;
	if (!ba.write_uint32(battle_times)) return false;
	if (!ba.write_uint32(cur_round)) return false;
	
	int prize_size = prize_vec.size();
	if (!ba.write_int32(prize_size)) return false;
	for (int i = 0; i < prize_size; i++) {
		if (!prize_vec[i].write_to_buf(ba)) return false;
	}
	return true;
}

//PROTO_USER_SPEED_AUTO_BATTLE_CMD= 546,  //加速扫荡
cli_user_speed_auto_battle_out::cli_user_speed_auto_battle_out()
{
	this->init();
}

void cli_user_speed_auto_battle_out::init()
{
	is_package_full = 0;
	cost_gold = 0;
	map_id = 0;
	battle_counter = 0;
	battle_times = 0;
	cur_round = 0;
	prize_vec.clear();
}

int cli_user_speed_auto_battle_out::get_msg_len()
{
	int len = sizeof(uint32_t) * 6;
	len += sizeof(uint32_t);
	for (int i = 0; i < (int)prize_vec.size(); i++) {
		len += sizeof(uint32_t) * 1;
		len += sizeof(uint32_t) + prize_vec[i].item_vec.size() * sizeof(item_unit_t);
		len += sizeof(uint32_t) + prize_vec[i].drop_vec.size() * sizeof(item_unit_t);
	}
	return len;
}

bool cli_user_speed_auto_battle_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(is_package_full)) return false;
	if (!ba.read_uint32(cost_gold)) return false;
	if (!ba.read_uint32(map_id)) return false;
	if (!ba.read_uint32(battle_counter)) return false;
	if (!ba.read_uint32(battle_times)) return false;
	if (!ba.read_uint32(cur_round)) return false;
	
	int prize_size = 0;
	if (!ba.read_int32(prize_size)) return false;
	for (int i = 0; i < prize_size; i++) {
        auto_battle_t prize;
        prize.read_from_buf(ba);
        prize_vec.push_back(prize);
	}
	return true;
}

bool cli_user_speed_auto_battle_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(is_package_full)) return false;
	if (!ba.write_uint32(cost_gold)) return false;
	if (!ba.write_uint32(map_id)) return false;
	if (!ba.write_uint32(battle_counter)) return false;
	if (!ba.write_uint32(battle_times)) return false;
	if (!ba.write_uint32(cur_round)) return false;
	
	int prize_size = prize_vec.size();
	if (!ba.write_int32(prize_size)) return false;
	for (int i = 0; i < prize_size; i++) {
		if (!prize_vec[i].write_to_buf(ba)) return false;
	}
	return true;
}

//PROTO_USER_BATTLE_START_CMD		= 547,	//进入关卡
cli_user_battle_start_out::cli_user_battle_start_out()
{
	this->init();
}

void cli_user_battle_start_out::init()
{
	start_time = 0;
	battle_key = 0;
	hp_item_cnt = 0;
	item_vec.clear();
	thief_start_time = 0;
	thief_interval_time = 0;
	thief_item_vec.clear();
}

int cli_user_battle_start_out::get_msg_len()
{
	return sizeof(start_time) + sizeof(battle_key) + sizeof(hp_item_cnt) + sizeof(uint32_t) + item_vec.size() * sizeof(item_unit_t)
		+ sizeof(thief_start_time) + sizeof(thief_interval_time) + sizeof(uint32_t) + thief_item_vec.size() * sizeof(item_unit_t);
}

bool cli_user_battle_start_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(start_time)) return false;
	if (!ba.read_uint32(battle_key)) return false;
	if (!ba.read_uint32(hp_item_cnt)) return false;
	int32_t vec_size = 0;
	if (!ba.read_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		item_unit_t item;
		if (!ba.read_uint32(item.item_id)) return false;
		if (!ba.read_uint32(item.item_cnt)) return false;
		this->item_vec.push_back(item);
	}
	if (!ba.read_uint32(thief_start_time)) return false;
	if (!ba.read_uint32(thief_interval_time)) return false;
	int32_t thief_vec_size = 0;
	if (!ba.read_int32(thief_vec_size)) return false;
	for (int i = 0; i < thief_vec_size; i++) {
		item_unit_t item;
		if (!ba.read_uint32(item.item_id)) return false;
		if (!ba.read_uint32(item.item_cnt)) return false;
		this->thief_item_vec.push_back(item);
	}

	return true;
}

bool cli_user_battle_start_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(start_time)) return false;
	if (!ba.write_uint32(battle_key)) return false;
	if (!ba.write_uint32(hp_item_cnt)) return false;
	int32_t vec_size = item_vec.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		if (!ba.write_uint32(item_vec[i].item_id)) return false;
		if (!ba.write_uint32(item_vec[i].item_cnt)) return false;
	}
	if (!ba.write_uint32(thief_start_time)) return false;
	if (!ba.write_uint32(thief_interval_time)) return false;
	int32_t thief_vec_size = thief_item_vec.size();
	if (!ba.write_int32(thief_vec_size)) return false;
	for (int i = 0; i < thief_vec_size; i++) {
		if (!ba.write_uint32(thief_item_vec[i].item_id)) return false;
		if (!ba.write_uint32(thief_item_vec[i].item_cnt)) return false;
	}
	return true;
}

//PROTO_USER_BATTLE_OVER_CMD		= 548,	//关卡战斗结束
cli_user_battle_over_in::cli_user_battle_over_in()
{
	this->init();
}

void cli_user_battle_over_in::init()
{
	map_id = 0;
	over_reason = 0;
	memset(kill_monster, 0, sizeof(kill_monster));
	add_yxb = 0;
	add_exp = 0;
	add_gold = 0;
	max_combo = 0;
	be_hit_count = 0;
	hp_remain_rate = 0;
	kill_vec.clear();
	born_info.clear();
	item_vec.clear();
	thief_item_vec.clear();
}

int cli_user_battle_over_in::get_msg_len()
{
	int len = 0;
	len = sizeof(map_id) + sizeof(over_reason) + sizeof(kill_monster) + sizeof(add_yxb) + sizeof(add_exp)
		+ sizeof(add_gold) + sizeof(used_item) + sizeof(hp_item_cnt) + sizeof(max_combo) + sizeof(be_hit_count) + sizeof(hp_remain_rate);
	len += sizeof(uint32_t) + kill_vec.size() * sizeof(kill_monster_info_t);
	len += sizeof(uint32_t) + born_info.size() * sizeof(born_monster_info_t);
	len += sizeof(uint32_t) + item_vec.size() * sizeof(item_unit_t);
	len += sizeof(uint32_t) + thief_item_vec.size() * sizeof(item_unit_t);
	return len;
}

bool cli_user_battle_over_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(map_id)) return false;
	if (!ba.read_uint32(over_reason)) return false;
	if (!ba.read_buf((char*)kill_monster, sizeof(kill_monster))) return false;
	if (!ba.read_uint32(add_yxb)) return false;
	if (!ba.read_uint32(add_exp)) return false;
	if (!ba.read_uint32(add_gold)) return false;
	if (!ba.read_uint32(max_combo)) return false;
	if (!ba.read_uint32(be_hit_count)) return false;
	if (!ba.read_uint32(hp_remain_rate)) return false;

	if (!ba.read_buf((char*)used_item, sizeof(used_item))) return false;
	if (!ba.read_uint32(hp_item_cnt)) return false;
	
	int vec_size = 0;
	if (!ba.read_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		kill_monster_info_t kill_info;
		if (!ba.read_uint16(kill_info.monster_id)) return false;
		if (!ba.read_uint16(kill_info.kill_cnt)) return false;
		kill_vec.push_back(kill_info);
	}
	
	if (!ba.read_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		born_monster_info_t info;
		if (!ba.read_uint16(info.born_second)) return false;
		if (!ba.read_uint16(info.monster_id)) return false;
		born_info.push_back(info);
	}

	if (!ba.read_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		item_unit_t item;
		if (!ba.read_uint32(item.item_id)) return false;
		if (!ba.read_uint32(item.item_cnt)) return false;
		item_vec.push_back(item);	
	}

	if (!ba.read_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		item_unit_t item;
		if (!ba.read_uint32(item.item_id)) return false;
		if (!ba.read_uint32(item.item_cnt)) return false;
		thief_item_vec.push_back(item);	
	}

	return true;
}

bool cli_user_battle_over_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(map_id)) return false;
	if (!ba.write_uint32(over_reason)) return false;
	if (!ba.write_buf((char*)kill_monster, sizeof(kill_monster))) return false;
	if (!ba.write_uint32(add_yxb)) return false;
	if (!ba.write_uint32(add_exp)) return false;
	if (!ba.write_uint32(add_gold)) return false;
	if (!ba.write_uint32(max_combo)) return false;
	if (!ba.write_uint32(be_hit_count)) return false;
	if (!ba.write_uint32(hp_remain_rate)) return false;

	if (!ba.write_buf((char*)used_item, sizeof(used_item))) return false;
	if (!ba.write_uint32(hp_item_cnt)) return false;
	
	int vec_size = kill_vec.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		if (!ba.write_uint16(kill_vec[i].monster_id)) return false;
		if (!ba.write_uint16(kill_vec[i].kill_cnt)) return false;
	}
	
	vec_size = born_info.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		if (!ba.write_uint16(born_info[i].born_second)) return false;
		if (!ba.write_uint16(born_info[i].monster_id)) return false;
	}

	vec_size = item_vec.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		if (!ba.write_uint32(item_vec[i].item_id)) return false;
		if (!ba.write_uint32(item_vec[i].item_cnt)) return false;
	}

	vec_size = thief_item_vec.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		if (!ba.write_uint32(thief_item_vec[i].item_id)) return false;
		if (!ba.write_uint32(thief_item_vec[i].item_cnt)) return false;
	}

	return true;
}

cli_user_battle_over_out::cli_user_battle_over_out()
{
	this->init();
}

void cli_user_battle_over_out::init()
{
	map_type = 0;
	first_pass = 0;
	score = 0;
	first_gold = 0;
	first_prize.clear();
	card_vec.clear();
	pass_prize.clear();
}

int cli_user_battle_over_out::get_msg_len()
{
	int len = 0;
	len = sizeof(map_type) + sizeof(first_pass) + sizeof(score) + sizeof(first_gold);
	len += sizeof(uint32_t) + first_prize.size() * sizeof(item_unit_t);
	len += sizeof(uint32_t) + card_vec.size() * sizeof(item_unit_t);
	len += sizeof(uint32_t) + pass_prize.size() * sizeof(item_unit_t);
	return len;
}

bool cli_user_battle_over_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(map_type)) return false;
	if (!ba.read_uint32(first_pass)) return false;
	if (!ba.read_uint32(score)) return false;
	if (!ba.read_uint32(first_gold)) return false;

	int vec_size = 0;
	if (!ba.read_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		item_unit_t item;
		if (!ba.read_uint32(item.item_id)) return false;
		if (!ba.read_uint32(item.item_cnt)) return false;
		first_prize.push_back(item);
	}

	if (!ba.read_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		item_unit_t item;
		if (!ba.read_uint32(item.item_id)) return false;
		if (!ba.read_uint32(item.item_cnt)) return false;
		card_vec.push_back(item);
	}
	
	if (!ba.read_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		item_unit_t item;
		if (!ba.read_uint32(item.item_id)) return false;
		if (!ba.read_uint32(item.item_cnt)) return false;
		pass_prize.push_back(item);
	}

	return true;
}

bool cli_user_battle_over_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(map_type)) return false;
	if (!ba.write_uint32(first_pass)) return false;
	if (!ba.write_uint32(score)) return false;
	if (!ba.write_uint32(first_gold)) return false;

	int vec_size = first_prize.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		if (!ba.write_uint32(first_prize[i].item_id)) return false;
		if (!ba.write_uint32(first_prize[i].item_cnt)) return false;
	}

	vec_size = card_vec.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		if (!ba.write_uint32(card_vec[i].item_id)) return false;
		if (!ba.write_uint32(card_vec[i].item_cnt)) return false;
	}
	
	vec_size = pass_prize.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		if (!ba.write_uint32(pass_prize[i].item_id)) return false;
		if (!ba.write_uint32(pass_prize[i].item_cnt)) return false;
	}
	return true;
}

//PROTO_USER_GET_BATTLE_PRIZE_CMD   = 549,  //翻牌获得奖励
cli_user_get_battle_prize_in::cli_user_get_battle_prize_in()
{
	this->init();
}

void cli_user_get_battle_prize_in::init()
{
	map_id = 0;
	cost_gold = 0;
	card_state = 0;
}

int cli_user_get_battle_prize_in::get_msg_len()
{
	return sizeof(uint32_t) * 3;
}

bool cli_user_get_battle_prize_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(map_id)) return false;
	if (!ba.read_uint32(cost_gold)) return false;
	if (!ba.read_uint32(card_state)) return false;
	return true;
}

bool cli_user_get_battle_prize_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(map_id)) return false;
	if (!ba.write_uint32(cost_gold)) return false;
	if (!ba.write_uint32(card_state)) return false;
	return true;
}

cli_user_get_battle_prize_out::cli_user_get_battle_prize_out()
{

}

void cli_user_get_battle_prize_out::init()
{
	memset(&card, 0, sizeof(card));
	gift_state = 0;
}

int cli_user_get_battle_prize_out::get_msg_len()
{
	return sizeof(card) + sizeof(gift_state);
}

bool cli_user_get_battle_prize_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_buf((char*)&card, sizeof(card))) return false;
	if (!ba.read_uint32(gift_state)) return false;
	return true;
}

bool cli_user_get_battle_prize_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_buf((const char*)&card, sizeof(card))) return false;
	if (!ba.write_uint32(gift_state)) return false;
	return true;
}

//PROTO_USER_BALLOT_CMD			= 550,	//藏经阁抽签(确定或抽签)
cli_user_ballot_out::cli_user_ballot_out()
{
	this->init();
}

void cli_user_ballot_out::init()
{
	mul = 0;
	cost_yxb = 0;
	cost_gold = 0;
	item_vec.clear();
}

int cli_user_ballot_out::get_msg_len()
{
	return sizeof(mul) + sizeof(cost_yxb) + sizeof(cost_gold) + sizeof(uint32_t) + item_vec.size() * sizeof(item_unit_t);
}

bool cli_user_ballot_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_float(mul)) return false;
	if (!ba.read_uint32(cost_yxb)) return false;
	if (!ba.read_uint32(cost_gold)) return false;

	int32_t vec_size = 0;
	if (!ba.read_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		item_unit_t item;
		if (!ba.read_uint32(item.item_id)) return false;
		if (!ba.read_uint32(item.item_cnt)) return false;
		this->item_vec.push_back(item);
	}
	return true;
}

bool cli_user_ballot_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_float(mul)) return false;
	if (!ba.write_uint32(cost_yxb)) return false;
	if (!ba.write_uint32(cost_gold)) return false;

	int32_t vec_size = item_vec.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		if (!ba.write_uint32(item_vec[i].item_id)) return false;
		if (!ba.write_uint32(item_vec[i].item_cnt)) return false;
	}
	return true;
}

//PROTO_USER_GET_ALL_BATTLE_PRIZE_CMD	= 551,	//获得所有老虎机的奖励
cli_user_get_all_battle_prize_out::cli_user_get_all_battle_prize_out()
{
	this->init();
}

void cli_user_get_all_battle_prize_out::init()
{
	cost_gold = 0;	//花费的黄金
	gift_state = 0;	//0放到背包中了 1放到个人礼包中了
	//prize_vec.clear();
}
	
int cli_user_get_all_battle_prize_out::get_msg_len()
{
	return sizeof(cost_gold) + sizeof(gift_state);
}

bool cli_user_get_all_battle_prize_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(cost_gold)) return false;
	if (!ba.read_uint32(gift_state)) return false;

	return true;
}

bool cli_user_get_all_battle_prize_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(cost_gold)) return false;
	if (!ba.write_uint32(gift_state)) return false;
	return true;
}

//PROTO_USER_USE_PARTNER_CARD_CMD= 556,	//使用弟子卡牌
cli_user_use_partner_card_in::cli_user_use_partner_card_in()
{
	this->init();
}

void cli_user_use_partner_card_in::init()
{
	package_id = 0;
	card_id = 0;
}

int  cli_user_use_partner_card_in::get_msg_len()
{
	return sizeof(card_id) + sizeof(package_id);
}

bool cli_user_use_partner_card_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(card_id)) return false;
	if (!ba.read_uint32(package_id)) return false;
	return true;
}

bool cli_user_use_partner_card_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(card_id)) return false;
	if (!ba.write_uint32(package_id)) return false;
	return true;
}

//PROTO_USER_INHERTIT_PARTNER_CMD = 558,	//弟子传承
cli_user_inherit_partner_in::cli_user_inherit_partner_in()
{
	this->init();
}

void cli_user_inherit_partner_in::init()
{
	partner_id = 0;		//传承者
	target_id = 0;			//继承者
	use_gold = 0;			//是否使用黄金
}

int  cli_user_inherit_partner_in::get_msg_len()
{
	return sizeof(partner_id) + sizeof(target_id) + sizeof(use_gold);
}

bool cli_user_inherit_partner_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(partner_id)) return false;
	if (!ba.read_uint32(target_id)) return false;
	if (!ba.read_uint32(use_gold)) return false;
	return true;
}

bool cli_user_inherit_partner_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(partner_id)) return false;
	if (!ba.write_uint32(target_id)) return false;
	if (!ba.write_uint32(use_gold)) return false;
	return true;
}

cli_user_inherit_partner_out::cli_user_inherit_partner_out()
{
	this->init();
}

void cli_user_inherit_partner_out::init()
{
	partner_id = 0;
	target_id = 0;
	cost_gold = 0;
	exp = 0;
    train_hp = 0;
    train_out_attack = 0;
    train_out_def = 0;
    train_in_attack = 0;
    train_in_def = 0;
	train_toughness = 0;
	quart = 0;
}
	
int  cli_user_inherit_partner_out::get_msg_len()
{
	return sizeof(partner_id) + sizeof(target_id) + sizeof(cost_gold) + sizeof(exp)
		+ sizeof(int) * 6 + sizeof(quart);
}

bool cli_user_inherit_partner_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(partner_id)) return false;
	if (!ba.read_uint32(target_id)) return false;
	if (!ba.read_uint32(cost_gold)) return false;
	if (!ba.read_uint32(exp)) return false;
	if (!ba.read_int32(train_hp)) return false;
	if (!ba.read_int32(train_out_attack)) return false;
	if (!ba.read_int32(train_out_def)) return false;
	if (!ba.read_int32(train_in_attack)) return false;
	if (!ba.read_int32(train_in_def)) return false;
	if (!ba.read_int32(train_toughness)) return false;
	if (!ba.read_uint32(quart)) return false;
	return true;
}

bool cli_user_inherit_partner_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(partner_id)) return false;
	if (!ba.write_uint32(target_id)) return false;
	if (!ba.write_uint32(cost_gold)) return false;
	if (!ba.write_uint32(exp)) return false;
	if (!ba.write_int32(train_hp)) return false;
	if (!ba.write_int32(train_out_attack)) return false;
	if (!ba.write_int32(train_out_def)) return false;
	if (!ba.write_int32(train_in_attack)) return false;
	if (!ba.write_int32(train_in_def)) return false;
	if (!ba.write_int32(train_toughness)) return false;
	if (!ba.write_uint32(quart)) return false;
	return true;
}

cli_user_upgrade_partner_skill_out::cli_user_upgrade_partner_skill_out()
{
	this->init();
}

void cli_user_upgrade_partner_skill_out::init()
{
	is_succ = 0;		//是否成功
	cost_vec.clear();
}

int  cli_user_upgrade_partner_skill_out::get_msg_len()
{
	return sizeof(is_succ) + cost_vec.size() * sizeof(item_unit_t);
}

bool cli_user_upgrade_partner_skill_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(is_succ)) return false;
	uint32_t vec_size = 0;
	if (!ba.read_uint32(vec_size)) return false;
	for (uint32_t i = 0; i < vec_size; i++) {
		item_unit_t one = {0};
		if (!ba.read_buf((char*)&one, sizeof(one))) return false;
		cost_vec.push_back(one);
	}
	return true;
}

bool cli_user_upgrade_partner_skill_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(is_succ)) return false;
	uint32_t vec_size = cost_vec.size();
	if (!ba.write_uint32(vec_size)) return false;
	for	(uint32_t i = 0; i < vec_size; i++) {
		if (!ba.write_buf((char*)&(cost_vec[i]), sizeof(cost_vec[i]))) return false;
	}
	return true;
}

//PROTO_USER_GET_JAR_LIST_CMD     = 564,  //得到寻宝的罐子的列表
cli_user_get_jar_list_out::cli_user_get_jar_list_out()
{
	this->init();
}

void cli_user_get_jar_list_out::init()
{
	memset(jar_level, 0, sizeof(jar_level));
}

int cli_user_get_jar_list_out::get_msg_len()
{
	return sizeof(jar_level);
}

bool cli_user_get_jar_list_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_buf((char*)this->jar_level, sizeof(this->jar_level))) return false;
	return true;
}

bool cli_user_get_jar_list_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_buf((char*)this->jar_level, sizeof(this->jar_level))) return false;
	return true;
}

//PROTO_USER_BREAK_JAR_CMD        = 566,  //砸罐子
cli_user_break_jar_out::cli_user_break_jar_out()
{
	this->init();
}

void cli_user_break_jar_out::init()
{
	memset(&prize, 0, sizeof(prize));
}

int cli_user_break_jar_out::get_msg_len()
{
	return sizeof(prize);
}

bool cli_user_break_jar_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_buf((char*)&prize, sizeof(prize))) return false;
	return true;
}

bool cli_user_break_jar_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_buf((char*)&prize, sizeof(prize))) return false;
	return true;
}

//PROTO_USER_BUY_HAMMER_CMD       = 567,  //购买用于砸罐子的锤子
cli_user_buy_hammer_out::cli_user_buy_hammer_out()
{
	this->init();
}

void cli_user_buy_hammer_out::init()
{
	cost_gold = 0;
}

int cli_user_buy_hammer_out::get_msg_len()
{
	return sizeof(cost_gold);
}

bool cli_user_buy_hammer_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(cost_gold)) return false;
	return true;
}

bool cli_user_buy_hammer_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(cost_gold)) return false;
	return true;
}

//PROTO_USER_SWAP_PARTNER_SEAT_CMD= 569,	//上阵侠客交换位置
cli_user_swap_partner_seat_in::cli_user_swap_partner_seat_in()
{
	this->init();
}

void cli_user_swap_partner_seat_in::init()
{
	seat_idx_1 = 0;
	seat_idx_2 = 0;
}

int cli_user_swap_partner_seat_in::get_msg_len()
{
	return sizeof(seat_idx_1) + sizeof(seat_idx_2);
}

bool cli_user_swap_partner_seat_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(seat_idx_1)) return false;
	if (!ba.read_uint32(seat_idx_2)) return false;
	return true;
}

bool cli_user_swap_partner_seat_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(seat_idx_1)) return false;
	if (!ba.write_uint32(seat_idx_2)) return false;
	return true;
}

//PROTO_USER_ENLIST_PARTNER_CMD   = 570,  //招募侠客
cli_user_enlist_partner_out::cli_user_enlist_partner_out()
{
	this->init();
}

void cli_user_enlist_partner_out::init()
{
	partner_id = 0;
	cost_money = 0;
	cost_gold = 0;
}

int cli_user_enlist_partner_out::get_msg_len()
{
	return sizeof(partner_id) + sizeof(cost_money) + sizeof(cost_gold);
}

bool cli_user_enlist_partner_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(partner_id)) return false;
	if (!ba.read_uint32(cost_money)) return false;
	if (!ba.read_uint32(cost_gold)) return false;
	return true;
}

bool cli_user_enlist_partner_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(partner_id)) return false;
	if (!ba.write_uint32(cost_money)) return false;
	if (!ba.write_uint32(cost_gold)) return false;
	return true;
}

//PROTO_USER_GET_PARTNER_LIST_CMD = 571,  //侠客列表
cli_user_get_partner_list_out::cli_user_get_partner_list_out()
{
	this->init();
}

void cli_user_get_partner_list_out::init()
{
	partner_vec.clear();
}

int cli_user_get_partner_list_out::get_msg_len()
{
	return sizeof(uint32_t) + partner_vec.size() * sizeof(partner_info_t);
}

bool cli_user_get_partner_list_out::read_from_buf(byte_array_t & ba)
{
	int32_t vec_size = 0;
	if (!ba.read_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		partner_info_t partner = {0};
		if (!ba.read_uint32(partner.partner_id)) return false;
		if (!ba.read_uint64(partner.exp)) return false;
		if (!ba.read_uint32(partner.level)) return false;
		if (!ba.read_uint32(partner.inherit_state))	return false;
		if (!ba.read_uint32(partner.qualify_value)) return false;
		if (!ba.read_uint32(partner.cur_seat)) return false;
		if (!ba.read_uint32(partner.skill_level)) return false;
		if (!ba.read_buf((char*)&(partner.equipment), sizeof(partner.equipment))) return false;     //使用的技能
		if (!ba.read_int32(partner.train_hp)) return false;
		if (!ba.read_int32(partner.train_out_attack)) return false;
		if (!ba.read_int32(partner.train_out_def)) return false;
		if (!ba.read_int32(partner.train_in_attack)) return false;
		if (!ba.read_int32(partner.train_in_def)) return false;
		if (!ba.read_int32(partner.train_toughness)) return false;
		if (!ba.read_buf((char*)partner.star_array, sizeof(partner.star_array))) return false;
		this->partner_vec.push_back(partner);
	}
	return true;
}

bool cli_user_get_partner_list_out::write_to_buf(byte_array_t & ba)
{
	int32_t vec_size = partner_vec.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		if (!ba.write_uint32(partner_vec[i].partner_id)) return false;
		if (!ba.write_uint64(partner_vec[i].exp)) return false;
		if (!ba.write_uint32(partner_vec[i].level)) return false;
		if (!ba.write_uint32(partner_vec[i].inherit_state)) return false;
		if (!ba.write_uint32(partner_vec[i].qualify_value)) return false;
		if (!ba.write_uint32(partner_vec[i].cur_seat)) return false;
		if (!ba.write_uint32(partner_vec[i].skill_level)) return false;
		if (!ba.write_buf((char*)&(partner_vec[i].equipment), sizeof(partner_vec[i].equipment))) return false;     //使用的技能
		if (!ba.write_int32(partner_vec[i].train_hp)) return false;
		if (!ba.write_int32(partner_vec[i].train_out_attack)) return false;
		if (!ba.write_int32(partner_vec[i].train_out_def)) return false;
		if (!ba.write_int32(partner_vec[i].train_in_attack)) return false;
		if (!ba.write_int32(partner_vec[i].train_in_def)) return false;
		if (!ba.write_int32(partner_vec[i].train_toughness)) return false;
		if (!ba.write_buf((char*)partner_vec[i].star_array, sizeof(partner_vec[i].star_array))) return false;
	}
	return true;
}

//PROTO_USER_ONPLAY_PARTNER_CMD   = 572,  //侠客上阵
cli_user_onplay_partner_in::cli_user_onplay_partner_in()
{
	this->init();
}

void cli_user_onplay_partner_in::init()
{
	partner_id = 0;
	seat_index = 0;
}

int cli_user_onplay_partner_in::get_msg_len()
{
	return sizeof(partner_id) + sizeof(seat_index);
}

bool cli_user_onplay_partner_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(partner_id)) return false;
	if (!ba.read_uint32(seat_index)) return false;
	return true;
}

bool cli_user_onplay_partner_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(partner_id)) return false;
	if (!ba.write_uint32(seat_index)) return false;
	return true;
}

cli_user_onplay_partner_out::cli_user_onplay_partner_out()
{
	this->init();
}

void cli_user_onplay_partner_out::init()
{
	on_partner_id = 0;
	on_seat_index = 0;
	off_partner_id = 0;
}

int cli_user_onplay_partner_out::get_msg_len()
{
	return sizeof(on_partner_id) + sizeof(on_seat_index) + sizeof(off_partner_id);
}

bool cli_user_onplay_partner_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(on_partner_id)) return false;
	if (!ba.read_uint32(on_seat_index)) return false;
	if (!ba.read_uint32(off_partner_id)) return false;

	return true;
}

bool cli_user_onplay_partner_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(on_partner_id)) return false;
	if (!ba.write_uint32(on_seat_index)) return false;
	if (!ba.write_uint32(off_partner_id)) return false;

	return true;
}

//PROTO_USER_TRAVEL_PARTNER_CMD   = 576,      //侠客游历
cli_user_travel_partner_in::cli_user_travel_partner_in()
{
	this->init();
}

void cli_user_travel_partner_in::init()
{
	partner_id = 0;
	travel_type = 0;
	is_ten_travel = 0;
}

int cli_user_travel_partner_in::get_msg_len()
{
	return sizeof(partner_id) + sizeof(travel_type) + sizeof(is_ten_travel);
}

bool cli_user_travel_partner_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(partner_id)) return false;
	if (!ba.read_uint32(travel_type)) return false;
	if (!ba.read_uint32(is_ten_travel)) return false;
	return true;
}

bool cli_user_travel_partner_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(partner_id)) return false;
	if (!ba.write_uint32(travel_type)) return false;
	if (!ba.write_uint32(is_ten_travel)) return false;
	return true;
}

cli_user_travel_partner_out::cli_user_travel_partner_out()
{
	this->init();
}

void cli_user_travel_partner_out::init()
{
	partner_id = 0;
	qualify_value = 0;
	cost_yxb = 0;
	cost_gold = 0;
}

int cli_user_travel_partner_out::get_msg_len()
{
	return sizeof(partner_id) + sizeof(qualify_value) + sizeof(cost_yxb) + sizeof(cost_gold);
}

bool cli_user_travel_partner_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(partner_id)) return false;
	if (!ba.read_uint32(qualify_value)) return false;
	if (!ba.read_uint32(cost_yxb)) return false;
	if (!ba.read_uint32(cost_gold)) return false;
	return true;
}

bool cli_user_travel_partner_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(partner_id)) return false;
	if (!ba.write_uint32(qualify_value)) return false;
	if (!ba.write_uint32(cost_yxb)) return false;
	if (!ba.write_uint32(cost_gold)) return false;
	return true;
}

//PROTO_USER_PARTNER_EAT_EXP_CMD	= 577,		//吃经验丹
cli_user_partner_eat_exp_in::cli_user_partner_eat_exp_in()
{
	this->init();
}

void cli_user_partner_eat_exp_in::init()
{
	unit_id = 0;
	item.item_id = 0;
	item.item_cnt = 0;
}
	
int cli_user_partner_eat_exp_in::get_msg_len()
{
	return sizeof(unit_id) + sizeof(item);
}

bool cli_user_partner_eat_exp_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(unit_id)) return false;
	if (!ba.read_uint32(item.item_id)) return false;
	if (!ba.read_uint32(item.item_cnt)) return false;
	
	return true;
}

bool cli_user_partner_eat_exp_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(unit_id)) return false;
	if (!ba.write_uint32(item.item_id)) return false;
	if (!ba.write_uint32(item.item_cnt)) return false;
	
	return true;
}

cli_user_partner_eat_exp_out::cli_user_partner_eat_exp_out()
{
	this->init();
}

void cli_user_partner_eat_exp_out::init()
{
	unit_id = 0;
	add_exp = 0;
}

int cli_user_partner_eat_exp_out::get_msg_len()
{
	return sizeof(unit_id) + sizeof(add_exp);
}

bool cli_user_partner_eat_exp_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(unit_id)) return false;
	if (!ba.read_uint32(add_exp)) return false;
	return true;
}

bool cli_user_partner_eat_exp_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(unit_id)) return false;
	if (!ba.write_uint32(add_exp)) return false;
	return true;
}

//PROTO_USER_RECALL_PARTNER_CMD	= 578,		//用户放逐和召回侠客
cli_user_recall_partner_in::cli_user_recall_partner_in()
{
	this->init();
}

void cli_user_recall_partner_in::init()
{
	partner_id = 0;
	recall = 0;		//0为放逐 1为召回 
}

int cli_user_recall_partner_in::get_msg_len()
{
	return sizeof(partner_id) + sizeof(recall);
}

bool cli_user_recall_partner_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(partner_id)) return false;
	if (!ba.read_uint32(recall)) return false;
	return true;
}

bool cli_user_recall_partner_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(partner_id)) return false;
	if (!ba.write_uint32(recall)) return false;
	return true;
}
	
cli_user_recall_partner_out::cli_user_recall_partner_out()
{
	this->init();
}

void cli_user_recall_partner_out::init()
{
	is_recall = 0;		//0为放逐 1为召回
	memset(&partner, 0, sizeof(partner));
}

int  cli_user_recall_partner_out::get_msg_len()
{
	return sizeof(is_recall) + sizeof(partner);
}

bool cli_user_recall_partner_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(is_recall)) return false;
	if (!ba.read_uint32(partner.partner_id)) return false;
	if (!ba.read_uint64(partner.exp)) return false;
	if (!ba.read_uint32(partner.level)) return false;
	if (!ba.read_uint32(partner.inherit_state)) return false;
	if (!ba.read_uint32(partner.qualify_value)) return false;
	if (!ba.read_uint32(partner.cur_seat)) return false;
	if (!ba.read_uint32(partner.skill_level)) return false;
	if (!ba.read_buf((char*)&(partner.equipment), sizeof(partner.equipment))) return false;     //使用的技能
	if (!ba.read_int32(partner.train_hp)) return false;
	if (!ba.read_int32(partner.train_out_attack)) return false;
	if (!ba.read_int32(partner.train_out_def)) return false;
	if (!ba.read_int32(partner.train_in_attack)) return false;
	if (!ba.read_int32(partner.train_in_def)) return false;
	if (!ba.read_int32(partner.train_toughness)) return false;
	return true;
}

bool cli_user_recall_partner_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(is_recall)) return false;
	if (!ba.write_uint32(partner.partner_id)) return false;
	if (!ba.write_uint64(partner.exp)) return false;
	if (!ba.write_uint32(partner.level)) return false;
	if (!ba.write_uint32(partner.inherit_state)) return false;
	if (!ba.write_uint32(partner.qualify_value)) return false;
	if (!ba.write_uint32(partner.cur_seat)) return false;
	if (!ba.write_uint32(partner.skill_level)) return false;
	if (!ba.write_buf((char*)&(partner.equipment), sizeof(partner.equipment))) return false;     //使用的技能
	if (!ba.write_int32(partner.train_hp)) return false;
	if (!ba.write_int32(partner.train_out_attack)) return false;
	if (!ba.write_int32(partner.train_out_def)) return false;
	if (!ba.write_int32(partner.train_in_attack)) return false;
	if (!ba.write_int32(partner.train_in_def)) return false;
	if (!ba.write_int32(partner.train_toughness)) return false;
	return true;
}

//PROTO_USER_GET_PRESTIGE_TITLE_CMD	= 579,	//得到声望挑战信息
cli_user_get_prestige_title_out::cli_user_get_prestige_title_out()
{
	this->init();
}

void cli_user_get_prestige_title_out::init()
{
	prestige_title = 0;
	is_get_prize = 0;
}

int cli_user_get_prestige_title_out::get_msg_len()
{
	return sizeof(prestige_title) + sizeof(is_get_prize);
}

bool cli_user_get_prestige_title_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(prestige_title)) return false;
	if (!ba.read_uint32(is_get_prize)) return false;
	
	return true;
}

bool cli_user_get_prestige_title_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(prestige_title)) return false;
	if (!ba.write_uint32(is_get_prize)) return false;
	
	return true;
}

//PROTO_USER_GET_PRESTIGE_PRIZE_CMD	= 580,	//领取
cli_user_get_prestige_prize_out::cli_user_get_prestige_prize_out()
{
	this->init();
}

void cli_user_get_prestige_prize_out::init()
{
	prize_vec.clear();
}

int cli_user_get_prestige_prize_out::get_msg_len()
{
	return sizeof(uint32_t) + prize_vec.size() * sizeof(item_unit_t);
}

bool cli_user_get_prestige_prize_out::read_from_buf(byte_array_t & ba)
{
	int32_t vec_size = 0;
	if (!ba.read_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		item_unit_t prize = {0};
		if (!ba.read_uint32(prize.item_id)) return false;
		if (!ba.read_uint32(prize.item_cnt)) return false;
		prize_vec.push_back(prize);
	}
	return true;
}

bool cli_user_get_prestige_prize_out::write_to_buf(byte_array_t & ba)
{
	int32_t vec_size = prize_vec.size();
	if (!ba.write_uint32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		if (!ba.write_uint32(prize_vec[i].item_id)) return false;
		if (!ba.write_uint32(prize_vec[i].item_cnt)) return false;
	}
	return true;
}

//PROTO_USER_PRESTIGE_LOTTERY_CMD	= 581,	//声望摇奖
cli_user_prestige_lottery_in::cli_user_prestige_lottery_in()
{
	this->init();
}

void cli_user_prestige_lottery_in::init()
{
	lottery_type = 0;
	is_gold = 0;
}

int cli_user_prestige_lottery_in::get_msg_len()
{
	return sizeof(lottery_type) + sizeof(is_gold);
}

bool cli_user_prestige_lottery_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(lottery_type)) return false;
	if (!ba.read_uint32(is_gold)) return false;

	return true;
}

bool cli_user_prestige_lottery_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(lottery_type)) return false;
	if (!ba.write_uint32(is_gold)) return false;
	
	return true;
}

cli_user_prestige_lottery_out::cli_user_prestige_lottery_out()
{
	this->init();
}

void cli_user_prestige_lottery_out::init()
{
	select_right = 0;
	cost_yxb = 0;
	cost_gold = 0;
	add_prestige = 0;
}

int cli_user_prestige_lottery_out::get_msg_len()
{
	return sizeof(select_right) + sizeof(cost_yxb) + sizeof(cost_gold) + sizeof(add_prestige);
}

bool cli_user_prestige_lottery_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(select_right)) return false;
	if (!ba.read_uint32(cost_yxb)) return false;
	if (!ba.read_uint32(cost_gold)) return false;
	if (!ba.read_uint32(add_prestige)) return false;
	return true;
}

bool cli_user_prestige_lottery_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(select_right)) return false;
	if (!ba.write_uint32(cost_yxb)) return false;
	if (!ba.write_uint32(cost_gold)) return false;
	if (!ba.write_uint32(add_prestige)) return false;
	return true;
}

//PROTO_USER_GET_TASK_LIST_CMD	= 585,	//得到任务列表
cli_user_get_task_list_out::cli_user_get_task_list_out()
{
	this->init();
}

void cli_user_get_task_list_out::init()
{
	task_vec.clear();
}

int cli_user_get_task_list_out::get_msg_len()
{
	return sizeof(int32_t) + task_vec.size() * sizeof(task_info_t);
}

bool cli_user_get_task_list_out::read_from_buf(byte_array_t & ba)
{
	int32_t vec_size = 0;
	if (!ba.read_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		task_info_t task;
		if (!ba.read_uint32(task.task_id)) return false;
		if (!ba.read_uint8(task.task_type)) return false;
		if (!ba.read_uint8(task.task_state)) return false;
		if (!ba.read_uint8(task.cur_step)) return false;
		if (!ba.read_uint8(task.max_step)) return false;
		task_vec.push_back(task);
	}
	return true;
}

bool cli_user_get_task_list_out::write_to_buf(byte_array_t & ba)
{
	int32_t vec_size = task_vec.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		if (!ba.write_uint32(task_vec[i].task_id)) return false;
		if (!ba.write_uint8(task_vec[i].task_type)) return false;
		if (!ba.write_uint8(task_vec[i].task_state)) return false;
		if (!ba.write_uint8(task_vec[i].cur_step)) return false;
		if (!ba.write_uint8(task_vec[i].max_step)) return false;
	}
	return true;
}

//PROTO_USER_GET_TASK_CMD			= 586,	//获得任务当前完成情况

//PROTO_USER_SET_TASK_CMD			= 588,	//设置任务
cli_user_set_task_in::cli_user_set_task_in()
{
	this->init();
}

void cli_user_set_task_in::init()
{
	task_id = 0;
	add_step = 0;
}
	
int cli_user_set_task_in::get_msg_len()
{
	return sizeof(task_id) + sizeof(add_step);
}

bool cli_user_set_task_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(task_id)) return false;
	if (!ba.read_uint32(add_step)) return false;
	return true;
}

bool cli_user_set_task_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(task_id)) return false;
	if (!ba.write_uint32(add_step)) return false;
	return true;
}

//PROTO_USER_FINISH_TASK_CMD		= 589,	//完成任务
cli_user_finish_task_in::cli_user_finish_task_in()
{
	this->init();
}

void cli_user_finish_task_in::init()
{
	task_id = 0;
	use_gold = 0;
}

int cli_user_finish_task_in::get_msg_len()
{
	return sizeof(task_id) + sizeof(use_gold);
}

bool cli_user_finish_task_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(task_id)) return false;
	if (!ba.read_uint32(use_gold)) return false;
	return true;
}

bool cli_user_finish_task_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(task_id)) return false;
	if (!ba.write_uint32(use_gold)) return false;
	return true;
}

cli_user_finish_task_out::cli_user_finish_task_out()
{
	this->init();
}

void cli_user_finish_task_out::init()
{
	task_id = 0;
	cost_gold = 0;
	prize_vec.clear();
}

int cli_user_finish_task_out::get_msg_len()
{
	return sizeof(task_id) + sizeof(cost_gold) + sizeof(int32_t) + sizeof(item_unit_t) * prize_vec.size();
}

bool cli_user_finish_task_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(task_id)) return false;
	if (!ba.read_uint32(cost_gold)) return false;
	int vec_size = 0;
	if (!ba.read_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		item_unit_t item;
		if (!ba.read_uint32(item.item_id)) return false;
		if (!ba.read_uint32(item.item_cnt)) return false;
		prize_vec.push_back(item);
	}
	return true;
}

bool cli_user_finish_task_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(task_id)) return false;
	if (!ba.write_uint32(cost_gold)) return false;
	int vec_size = prize_vec.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		if (!ba.write_uint32(prize_vec[i].item_id)) return false;
		if (!ba.write_uint32(prize_vec[i].item_cnt)) return false;
	}
	return true;
}

//PROTO_USER_GET_AWARD_TASK_CMD		= 591,	//点开悬赏面板时
cli_user_get_award_task_out::cli_user_get_award_task_out()
{
	this->init();
}

void cli_user_get_award_task_out::init()
{
	daily_count = 0;
	memset(scroll_count, 0, sizeof(scroll_count));
	award_counter = 0;
	memset(award_task, 0, sizeof(award_task));
}

int cli_user_get_award_task_out::get_msg_len()
{
	int len = sizeof(daily_count) + sizeof(scroll_count);
	len += sizeof(award_counter) + 4 * (sizeof(award_task[0].task_id) + sizeof(award_task[0].cur_step) + sizeof(award_task[0].max_step) + sizeof(award_task[0].task_star) + sizeof(award_task[0].task_state));
	return len;
}

bool cli_user_get_award_task_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(daily_count)) return false;
	if (!ba.read_buf((char*)scroll_count, sizeof(scroll_count))) return false;
	if (!ba.read_uint32(award_counter)) return false;
	for (int i = 0; i < 4; i++) {
		if (!ba.read_uint16(award_task[i].task_id)) return false;
		if (!ba.read_uint16(award_task[i].cur_step)) return false;
		if (!ba.read_uint16(award_task[i].max_step)) return false;
		if (!ba.read_uint8(award_task[i].task_star)) return false;
		if (!ba.read_uint8(award_task[i].task_state)) return false;
	}
	return true;
}

bool cli_user_get_award_task_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(daily_count)) return false;
	if (!ba.write_buf((char*)scroll_count, sizeof(scroll_count))) return false;
	if (!ba.write_uint32(award_counter)) return false;
	for (int i = 0; i < 4; i++) {
		if (!ba.write_uint16(award_task[i].task_id)) return false;
		if (!ba.write_uint16(award_task[i].cur_step)) return false;
		if (!ba.write_uint16(award_task[i].max_step)) return false;
		if (!ba.write_uint8(award_task[i].task_star)) return false;
		if (!ba.write_uint8(award_task[i].task_state)) return false;
	}
	return true;
}

//PROTO_USER_ACCEPT_AWARD_TASK_CMD 	= 592,	//接取悬赏任务

//PROTO_USER_UPGRADE_AWARD_TASK_CMD 	= 593,	//一件满星
cli_user_upgrade_award_task_out::cli_user_upgrade_award_task_out()
{
	this->init();
}

void cli_user_upgrade_award_task_out::init()
{
	task_id = 0;
	cost_gold = 0;
}

int cli_user_upgrade_award_task_out::get_msg_len()
{
	return sizeof(task_id) + sizeof(cost_gold);
}

bool cli_user_upgrade_award_task_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(task_id)) return false;
	if (!ba.read_uint32(cost_gold)) return false;
	return true;
}

bool cli_user_upgrade_award_task_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(task_id)) return false;
	if (!ba.write_uint32(cost_gold)) return false;
	return true;
}

//PROTO_USER_SET_AWARD_TASK_FINIS_CMD = 594,	//一键完成

//PROTO_USER_FINISH_AWARD_TASK_CMD	= 595,	//完成后提交任务
cli_user_finish_award_task_in::cli_user_finish_award_task_in()
{
	this->init();
}

void cli_user_finish_award_task_in::init()
{
	task_id = 0;
	item_id = 0;		//英雄帖类型
}

int cli_user_finish_award_task_in::get_msg_len()
{
	return sizeof(task_id) + sizeof(item_id);
}

bool cli_user_finish_award_task_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(task_id)) return false;
	if (!ba.read_uint32(item_id)) return false;
	return true;
}

bool cli_user_finish_award_task_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(task_id)) return false;
	if (!ba.write_uint32(item_id)) return false;
	return true;
}

cli_user_finish_award_task_out::cli_user_finish_award_task_out()
{
	this->init();
}

void cli_user_finish_award_task_out::init()
{
	task_id = 0;
	prize_vec.clear();
}

int cli_user_finish_award_task_out::get_msg_len()
{
	return sizeof(task_id) + sizeof(int32_t) + sizeof(item_unit_t) * prize_vec.size();
}

bool cli_user_finish_award_task_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(task_id)) return false;
	int vec_size = 0;
	if (!ba.read_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		item_unit_t item;
		if (!ba.read_uint32(item.item_id)) return false;
		if (!ba.read_uint32(item.item_cnt)) return false;
		prize_vec.push_back(item);
	}
	return true;
}

bool cli_user_finish_award_task_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(task_id)) return false;
	int vec_size = prize_vec.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		if (!ba.write_uint32(prize_vec[i].item_id)) return false;
		if (!ba.write_uint32(prize_vec[i].item_cnt)) return false;
	}
	return true;
}

//PROTO_USER_REFRESH_AWARD_TASK_CMD	= 596,	//刷新悬赏任务
cli_user_refresh_award_task_out::cli_user_refresh_award_task_out()
{
	this->init();
}

void cli_user_refresh_award_task_out::init()
{
	cost_yxb = 0;
	award_counter = 0;
	memset(award_task, 0, sizeof(award_task));
}

int cli_user_refresh_award_task_out::get_msg_len()
{
	return sizeof(cost_yxb) + sizeof(award_counter) + 4 * (sizeof(award_task[0].task_id) + sizeof(award_task[0].cur_step) + sizeof(award_task[0].max_step) + sizeof(award_task[0].task_star) + sizeof(award_task[0].task_state));
}

bool cli_user_refresh_award_task_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(cost_yxb)) return false;
	if (!ba.read_uint32(award_counter)) return false;
	for (int i = 0; i < 4; i++) {
		if (!ba.read_uint16(award_task[i].task_id)) return false;
		if (!ba.read_uint16(award_task[i].cur_step)) return false;
		if (!ba.read_uint16(award_task[i].max_step)) return false;
		if (!ba.read_uint8(award_task[i].task_star)) return false;
		if (!ba.read_uint8(award_task[i].task_state)) return false;
	}
	return true;
}

bool cli_user_refresh_award_task_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(cost_yxb)) return false;
	if (!ba.write_uint32(award_counter)) return false;
	for (int i = 0; i < 4; i++) {
		if (!ba.write_uint16(award_task[i].task_id)) return false;
		if (!ba.write_uint16(award_task[i].cur_step)) return false;
		if (!ba.write_uint16(award_task[i].max_step)) return false;
		if (!ba.write_uint8(award_task[i].task_star)) return false;
		if (!ba.write_uint8(award_task[i].task_state)) return false;
	}
	return true;
}

//PROTO_USER_GET_ATTR_ITEM_CMD	= 600,	//得到itemid类型为0的属性值
cli_user_get_attr_item_out::cli_user_get_attr_item_out()
{
	this->init();
}

void cli_user_get_attr_item_out::init()
{
	yxb = 0;
	exp = 0;
	gold = 0;
	power_value = 0;
	nerve_value = 0;
	enery = 0;
	enery_counter = 0;
	star_exp = 0;
	package_cnt = 0;
	max_package_size = 0;
}

int cli_user_get_attr_item_out::get_msg_len()
{
	return sizeof(uint32_t) * 10;
}

bool cli_user_get_attr_item_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(yxb)) return false;
	if (!ba.read_uint32(exp)) return false;
	if (!ba.read_uint32(gold)) return false;
	if (!ba.read_uint32(power_value)) return false;
	if (!ba.read_uint32(nerve_value)) return false;
	if (!ba.read_uint32(enery)) return false;
	if (!ba.read_uint32(enery_counter)) return false;
	if (!ba.read_uint32(star_exp)) return false;
	if (!ba.read_uint32(package_cnt)) return false;
	if (!ba.read_uint32(max_package_size)) return false;
	return true;
}

bool cli_user_get_attr_item_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(yxb)) return false;
	if (!ba.write_uint32(exp)) return false;
	if (!ba.write_uint32(gold)) return false;
	if (!ba.write_uint32(power_value)) return false;
	if (!ba.write_uint32(nerve_value)) return false;
	if (!ba.write_uint32(enery)) return false;
	if (!ba.write_uint32(enery_counter)) return false;
	if (!ba.write_uint32(star_exp)) return false;
	if (!ba.write_uint32(package_cnt)) return false;
	if (!ba.write_uint32(max_package_size)) return false;
	return true;
}

//PROTO_USER_GET_MAGIC_STORE_CMD	= 610,	//获得神秘商店
cli_user_get_magic_store_out::cli_user_get_magic_store_out()
{
	this->init();
}

void cli_user_get_magic_store_out::init()
{
	this->left_second = 0;
	this->sell_vec.clear();
}

int cli_user_get_magic_store_out::get_msg_len()
{
	return sizeof(left_second) + sizeof(uint32_t) + sell_vec.size() * sizeof(sell_item_t);
}

bool cli_user_get_magic_store_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(left_second)) return false;
	int vec_size = 0;
	if (!ba.read_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		sell_item_t item;
		if (!ba.read_uint32(item.item_id)) return false;
		if (!ba.read_uint32(item.item_cnt)) return false;
		if (!ba.read_uint32(item.price)) return false;
		if (!ba.read_uint32(item.is_sell)) return false;
		this->sell_vec.push_back(item);
	}
	return true;
}

bool cli_user_get_magic_store_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(left_second)) return false;
	int vec_size = (int)sell_vec.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		if (!ba.write_uint32(sell_vec[i].item_id)) return false;
		if (!ba.write_uint32(sell_vec[i].item_cnt)) return false;
		if (!ba.write_uint32(sell_vec[i].price)) return false;
		if (!ba.write_uint32(sell_vec[i].is_sell)) return false;
	}
	return true;
}

//PROTO_USER_BUY_MAGIC_ITEM_CMD	= 611,	//购买神秘商店中的物品
cli_user_buy_magic_item_in::cli_user_buy_magic_item_in()
{
	this->init();
}

void cli_user_buy_magic_item_in::init()
{
	item_index = 0;
	cost_yxb = 0;
}

int cli_user_buy_magic_item_in::get_msg_len()
{
	return sizeof(item_index) + sizeof(cost_yxb);
}

bool cli_user_buy_magic_item_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(item_index)) return false;
	if (!ba.read_uint32(cost_yxb)) return false;
	return true;
}

bool cli_user_buy_magic_item_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(item_index)) return false;
	if (!ba.write_uint32(cost_yxb)) return false;
	return true;
}

cli_user_buy_magic_item_out::cli_user_buy_magic_item_out()
{
	this->init();
}

void cli_user_buy_magic_item_out::init()
{
	item.item_id = 0;
	item.item_cnt = 0;
	cost_yxb = 0;
}

int cli_user_buy_magic_item_out::get_msg_len()
{
	return sizeof(item) + sizeof(cost_yxb);
}

bool cli_user_buy_magic_item_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(item.item_id)) return false;
	if (!ba.read_uint32(item.item_cnt)) return false;
	if (!ba.read_uint32(cost_yxb)) return false;
	return true;
}

bool cli_user_buy_magic_item_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(item.item_id)) return false;
	if (!ba.write_uint32(item.item_cnt)) return false;
	if (!ba.write_uint32(cost_yxb)) return false;
	return true;
}

//PROTO_USER_FLUSH_MAGIC_STORE_CMD= 612,	//刷新神秘商店
cli_user_flush_magic_store_out::cli_user_flush_magic_store_out()
{
	this->init();
}

void cli_user_flush_magic_store_out::init()
{
	cost_gold = 0;
	left_second = 0;
	sell_vec.clear();
}

int cli_user_flush_magic_store_out::get_msg_len()
{
	return sizeof(cost_gold) + sizeof(left_second) + sizeof(uint32_t) + sell_vec.size() + sizeof(sell_item_t);
}

bool cli_user_flush_magic_store_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(cost_gold)) return false;
	if (!ba.read_uint32(left_second)) return false;
	int vec_size = 0;
	if (!ba.read_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		sell_item_t item;
		if (!ba.read_uint32(item.item_id)) return false;
		if (!ba.read_uint32(item.item_cnt)) return false;
		if (!ba.read_uint32(item.price)) return false;
		if (!ba.read_uint32(item.is_sell)) return false;
		this->sell_vec.push_back(item);
	}
	return true;
}

bool cli_user_flush_magic_store_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(cost_gold)) return false;
	if (!ba.write_uint32(left_second)) return false;
	int vec_size = (int)sell_vec.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		if (!ba.write_uint32(sell_vec[i].item_id)) return false;
		if (!ba.write_uint32(sell_vec[i].item_cnt)) return false;
		if (!ba.write_uint32(sell_vec[i].price)) return false;
		if (!ba.write_uint32(sell_vec[i].is_sell)) return false;
	}
	return true;
}

//PROTO_USER_GET_PLATFORM_BONUS_CMD = 620,	//领取奖励平台的奖励
cli_user_get_platform_bonus_in::cli_user_get_platform_bonus_in()
{
	this->init();
}

void cli_user_get_platform_bonus_in::init()
{
	bonus_type = 0;
	bonus_index = 0;
}

int cli_user_get_platform_bonus_in::get_msg_len()
{
	return sizeof(bonus_type) + sizeof(bonus_index);
}

bool cli_user_get_platform_bonus_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(bonus_type)) return false;
	if (!ba.read_uint32(bonus_index)) return false;
	return true;
}

bool cli_user_get_platform_bonus_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(bonus_type)) return false;
	if (!ba.write_uint32(bonus_index)) return false;
	return true;
}

cli_user_get_platform_bonus_out::cli_user_get_platform_bonus_out()
{
	this->init();
}

void cli_user_get_platform_bonus_out::init()
{
	bonus_type = 0;
	swap_id = 0;
	rand_idx = 0;
	add_bonus_vec.clear();
}

int cli_user_get_platform_bonus_out::get_msg_len()
{
	return sizeof(bonus_type) + sizeof(swap_id) + sizeof(rand_idx)
			+ sizeof(int32_t) + sizeof(item_unit_t) * add_bonus_vec.size();;
}

bool cli_user_get_platform_bonus_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(bonus_type)) return false;
	if (!ba.read_uint32(swap_id)) return false;
	if (!ba.read_uint32(rand_idx)) return false;
	
	int vec_size = 0;
	if (!ba.read_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		item_unit_t item;
		if (!ba.read_uint32(item.item_id)) return false;
		if (!ba.read_uint32(item.item_cnt)) return false;
		add_bonus_vec.push_back(item);
	}
	return true;
}

bool cli_user_get_platform_bonus_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(bonus_type)) return false;
	if (!ba.write_uint32(swap_id)) return false;
	if (!ba.write_uint32(rand_idx)) return false;
	
	int vec_size = add_bonus_vec.size();
	if (!ba.write_uint32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		if (!ba.write_uint32(add_bonus_vec[i].item_id)) return false;
		if (!ba.write_uint32(add_bonus_vec[i].item_cnt)) return false;
	}
	return true;
}

//PROTO_USER_GET_TESTING_RANK_CMD	= 622,	//得到封测等级和战力排行
cli_user_get_testing_rank_out::cli_user_get_testing_rank_out()
{
	this->init();
}

void cli_user_get_testing_rank_out::init()
{
	rank_level.clear();
	rank_fight.clear();
}

int  cli_user_get_testing_rank_out::get_msg_len()
{
	return sizeof(int32_t) * 2 + sizeof(rank_info_t) * (rank_level.size() + rank_fight.size());
}

bool cli_user_get_testing_rank_out::read_from_buf(byte_array_t & ba)
{
	int vec_size = 0;
	if (!ba.read_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		rank_info_t rank;
		if (!ba.read_uint32(rank.rank_id)) return false;
		if (!ba.read_uint32(rank.userid)) return false;
		if (!ba.read_uint32(rank.role_type)) return false;
		if (!ba.read_buf(rank.nick,32)) return false;
		if (!ba.read_uint32(rank.faction)) return false;
		if (!ba.read_uint32(rank.value)) return false;
		rank_level.push_back(rank);
	}

	if (!ba.read_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		rank_info_t rank;
		if (!ba.read_uint32(rank.rank_id)) return false;
		if (!ba.read_uint32(rank.userid)) return false;
		if (!ba.read_uint32(rank.role_type)) return false;
		if (!ba.read_buf(rank.nick,32)) return false;
		if (!ba.read_uint32(rank.faction)) return false;
		if (!ba.read_uint32(rank.value)) return false;
		rank_fight.push_back(rank);
	}
	return true;
}

bool cli_user_get_testing_rank_out::write_to_buf(byte_array_t & ba)
{
	int vec_size = rank_level.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		if (!ba.write_buf((char*)&(rank_level[i]), sizeof(rank_info_t))) return false;
	}

	if (!ba.read_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		if (!ba.write_buf((char*)&(rank_level[i]), sizeof(rank_info_t))) return false;
	}
	return true;
}

//PROTO_USER_CHEAT_ITEM_CMD		= 1000,	//用户改变数量
cli_user_cheat_item_in::cli_user_cheat_item_in()
{
	this->init();
}

void cli_user_cheat_item_in::init()
{
	this->item_id = 0;
	this->item_cnt = 0;
}

int cli_user_cheat_item_in::get_msg_len()
{
	return sizeof(item_id) + sizeof(item_cnt);
}

bool cli_user_cheat_item_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(item_id)) return false;
	if (!ba.read_int32(item_cnt)) return false;
	return true;
}

bool cli_user_cheat_item_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(item_id)) return false;
	if (!ba.write_int32(item_cnt)) return false;
	return true;
}

//PROTO_USER_CHAT_CMD          = 630,//客户端发送聊天消息到服务器
cli_user_chat_in::cli_user_chat_in()
{
	this->init();
}

void cli_user_chat_in::init()
{
	this->channel_id = 0;
	this->target_id = 0;
	this->mlen = 0;
	memset(this->msg, 0, sizeof(this->msg));
}

int cli_user_chat_in::get_msg_len()
{
	return sizeof(channel_id) + sizeof(target_id) + sizeof(mlen) + mlen;
}

bool cli_user_chat_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(channel_id)) return false;
	if (!ba.read_uint32(target_id)) return false;
	if (!ba.read_uint32(mlen)) return false;
	if (!ba.read_buf(msg, mlen)) return false;
	return true;
}

bool cli_user_chat_in::write_to_buf(byte_array_t & ba)
{
    if (!ba.write_uint32(channel_id)) return false;
    if (!ba.write_uint32(target_id)) return false;
    if (!ba.write_uint32(mlen)) return false;
	if (!ba.write_buf(msg, mlen)) return false;
    return true;
}

cli_user_chat_out::cli_user_chat_out()
{
	this->init();
}

void cli_user_chat_out::init()
{
	this->userid = 0;
	memset(this->nick, 0, sizeof(this->nick));
	this->level = 0;
	this->unit_id = 0;
	this->chattime = 0;
	this->vip_level = 0;
	this->channel_id = 0;
	this->target_id = 0;
	this->mlen = 0;
	memset(this->msg, 0, sizeof(this->msg));
}

int cli_user_chat_out::get_msg_len()
{
	return sizeof(userid)+sizeof(nick)+sizeof(level)+sizeof(unit_id)\
		+sizeof(chattime)\
		+sizeof(vip_level)+sizeof(channel_id) + sizeof(target_id) + sizeof(mlen)+mlen;
}

bool cli_user_chat_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(userid)) return false;
	if (!ba.read_buf((char*)nick, 32)) return false;
	if (!ba.read_uint32(level)) return false;
	if (!ba.read_uint32(unit_id)) return false;
	if (!ba.read_uint32(chattime)) return false;
	if (!ba.read_uint32(vip_level)) return false;
	if (!ba.read_uint32(channel_id)) return false;
	if (!ba.read_uint32(target_id)) return false;
	if (!ba.read_uint32(mlen)) return false;
	if (!ba.read_buf(msg, mlen)) return false;
	return true;
}

bool cli_user_chat_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(userid)) return false;
	if (!ba.write_buf((char*)nick, 32)) return false;
	if (!ba.write_uint32(level)) return false;
	if (!ba.write_uint32(unit_id)) return false;
	if (!ba.write_uint32(chattime)) return false;
	if (!ba.write_uint32(vip_level)) return false;
	if (!ba.write_uint32(channel_id)) return false;
	if (!ba.write_uint32(target_id)) return false;
	if (!ba.write_uint32(mlen)) return false;
	if (!ba.write_buf(msg, mlen)) return false;
	return true;
}

//PROTO_USER_GET_USER_ONLINE_STATUS_CMD	= 639,	//查询玩家在线状态
cli_user_get_user_online_status_in::cli_user_get_user_online_status_in()
{
	this->init();
}

void cli_user_get_user_online_status_in::init()
{
	this->uid_vec.clear();
}

int cli_user_get_user_online_status_in::get_msg_len()
{
	return sizeof(uint32_t) + uid_vec.size() * sizeof(uint32_t);
}

bool cli_user_get_user_online_status_in::read_from_buf(byte_array_t & ba)
{
	int vec_size = 0;
	if (!ba.read_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		uint32_t uid;
		if (!ba.read_uint32(uid)) return false;
		this->uid_vec.push_back(uid);
	}
	return true;
}

bool cli_user_get_user_online_status_in::write_to_buf(byte_array_t & ba)
{
	int vec_size = (int)uid_vec.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		if (!ba.write_uint32(uid_vec[i])) return false;
	}
	return true;
}

cli_user_get_user_online_status_out::cli_user_get_user_online_status_out()
{
	this->init();
}

void cli_user_get_user_online_status_out::init()
{
	this->info_list.clear();
}

int cli_user_get_user_online_status_out::get_msg_len()
{
	return sizeof(uint32_t) + info_list.size() * ((usr_online_status_t*)0)->get_msg_len();
}

bool cli_user_get_user_online_status_out::read_from_buf(byte_array_t & ba)
{
	int vec_size = 0;
	if (!ba.read_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		usr_online_status_t info;
		if (!info.read_from_buf(ba)) return false;
		this->info_list.push_back(info);
	}
	return true;
}

bool cli_user_get_user_online_status_out::write_to_buf(byte_array_t & ba)
{
	int vec_size = (int)info_list.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		if (!info_list[i].write_to_buf(ba)) return false;
	}
	return true;
}

//PROTO_USER_GET_OTHER_USER_CMD	= 640,	//得到其他用户信息
cli_user_get_other_user_out::cli_user_get_other_user_out()
{
	this->init();
}

void cli_user_get_other_user_out::init()
{
	userid = 0;
	memset(nick, 0, sizeof(nick));
	unit_id = 0;
	vip_level = 0;
	level = 0;
	force_exp = 0;		//练气经验
	nerve_level = 0;	//经脉等级
	nerve_value = 0;	//真气值
	power_value = 0;	//内功值（战斗获得）
	prestige_value = 0;//声望

    train_hp = 0;                //培养增加的生命值
    train_out_attack = 0;        //培养增加的攻击力
    train_out_def = 0;           //培养增加的防御力
    train_in_attack = 0;         //培养增加的内攻
    train_in_def = 0;            //培养增加的内防
	train_toughness = 0;         //培养增加的内力

	memset(skills, 0, sizeof(skills));
	memset(passive_skills, 0, sizeof(passive_skills));
	memset(star_array, 0, sizeof(star_array));
	equipment_vec.clear();//已穿戴的装备
	partner_vec.clear();
}

int cli_user_get_other_user_out::get_msg_len()
{
	return sizeof(this->userid) + sizeof(this->nick) + sizeof(unit_id) + sizeof(vip_level) + sizeof(this->level)
		+ sizeof(force_exp) + sizeof(nerve_level) + sizeof(nerve_value) + sizeof(power_value) + sizeof(prestige_value)
        + sizeof(int) * 6
		+ sizeof(skills)
		+ sizeof(passive_skills)
		+ sizeof(star_array)
		+ sizeof(uint32_t) + this->equipment_vec.size() * sizeof(package_equipment_t)
		+ sizeof(uint32_t) + this->partner_vec.size() * sizeof(partner_info_t);
		;
}

bool cli_user_get_other_user_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(this->userid)) return false;
	if (!ba.read_buf(this->nick, sizeof(this->nick))) return false;
	if (!ba.read_uint32(this->unit_id)) return false;
	if (!ba.read_uint32(this->vip_level)) return false;
	if (!ba.read_uint32(this->level)) return false;
	if (!ba.read_uint32(this->force_exp)) return false;
	if (!ba.read_uint32(this->nerve_level)) return false;
	if (!ba.read_uint32(this->nerve_value)) return false;
	if (!ba.read_uint32(this->power_value)) return false;
	if (!ba.read_uint32(this->prestige_value)) return false;

    if (!ba.read_int32(this->train_hp)) return false;
    if (!ba.read_int32(this->train_out_attack)) return false;
    if (!ba.read_int32(this->train_out_def)) return false;
    if (!ba.read_int32(this->train_in_attack)) return false;
    if (!ba.read_int32(this->train_in_def)) return false;
    if (!ba.read_int32(this->train_toughness)) return false;
	
    if (!ba.read_buf((char*)this->skills, sizeof(this->skills))) return false;

	for (int i = 0; i < 4; i++) {
		if (!ba.read_uint16(this->passive_skills[i])) return false;
	}
	if (!ba.read_buf((char*)this->star_array, sizeof(this->star_array))) return false;
	
	int vec_size = 0;
	if (!ba.read_int32(vec_size)) return false;
	this->equipment_vec.clear();
	for (int i = 0; i < vec_size; ++i) {
		package_equipment_t equipment;
		if (!equipment.read_from_buf(ba)) return false;
		this->equipment_vec.push_back(equipment);
	}

	vec_size = 0;
	if (!ba.read_int32(vec_size)) return false;
	this->partner_vec.clear();
	for (int i = 0; i < vec_size; ++i) {
		partner_info_t partner;
		if (!ba.read_uint32(partner.partner_id)) return false;
		if (!ba.read_uint64(partner.exp)) return false;
		if (!ba.read_uint32(partner.level)) return false;
		if (!ba.read_uint32(partner.inherit_state)) return false;
		if (!ba.read_uint32(partner.qualify_value)) return false;
		if (!ba.read_uint32(partner.cur_seat)) return false;
		if (!ba.read_uint32(partner.skill_level)) return false;
		if (!ba.read_buf((char*)&(partner.equipment), sizeof(partner.equipment))) return false;     //使用的技能
		if (!ba.read_int32(partner.train_hp)) return false;
		if (!ba.read_int32(partner.train_out_attack)) return false;
		if (!ba.read_int32(partner.train_out_def)) return false;
		if (!ba.read_int32(partner.train_in_attack)) return false;
		if (!ba.read_int32(partner.train_in_def)) return false;
		if (!ba.read_int32(partner.train_toughness)) return false;
		if (!ba.read_buf((char*)partner.star_array, sizeof(partner.star_array))) return false;
		this->partner_vec.push_back(partner);
	}

	return true;
}

bool cli_user_get_other_user_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(this->userid)) return false;
	if (!ba.write_buf(this->nick, sizeof(this->nick))) return false;
	if (!ba.write_uint32(this->unit_id)) return false;
	if (!ba.write_uint32(this->vip_level)) return false;
	if (!ba.write_uint32(this->level)) return false;
	if (!ba.write_uint32(this->force_exp)) return false;
	if (!ba.write_uint32(this->nerve_level)) return false;
	if (!ba.write_uint32(this->nerve_value)) return false;
	if (!ba.write_uint32(this->power_value)) return false;
	if (!ba.write_uint32(this->prestige_value)) return false;

	if (!ba.write_uint32(this->train_hp)) return false;
    if (!ba.write_uint32(this->train_out_attack)) return false;
    if (!ba.write_uint32(this->train_out_def)) return false;
    if (!ba.write_uint32(this->train_in_attack)) return false;
    if (!ba.write_uint32(this->train_in_def)) return false;
    if (!ba.write_uint32(this->train_toughness)) return false;
	
    if (!ba.write_buf((char*)this->skills, sizeof(this->skills))) return false;

	for (int i = 0; i < 4; ++i) {
		if (!ba.write_uint16(this->passive_skills[i])) return false;
	}
	if (!ba.write_buf((char*)this->star_array, sizeof(this->star_array))) return false;

	int vec_size = this->equipment_vec.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; ++i) {
		if (!this->equipment_vec[i].write_to_buf(ba)) return false;
	}
	
	vec_size = this->partner_vec.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; ++i) {
		if (!ba.write_uint32(partner_vec[i].partner_id)) return false;
		if (!ba.write_uint64(partner_vec[i].exp)) return false;
		if (!ba.write_uint32(partner_vec[i].level)) return false;
		if (!ba.write_uint32(partner_vec[i].inherit_state)) return false;
		if (!ba.write_uint32(partner_vec[i].qualify_value)) return false;
		if (!ba.write_uint32(partner_vec[i].cur_seat)) return false;
		if (!ba.write_uint32(partner_vec[i].skill_level)) return false;
		if (!ba.write_buf((char*)&(partner_vec[i].equipment), sizeof(partner_vec[i].equipment))) return false;     //使用的技能
		if (!ba.write_int32(partner_vec[i].train_hp)) return false;
		if (!ba.write_int32(partner_vec[i].train_out_attack)) return false;
		if (!ba.write_int32(partner_vec[i].train_out_def)) return false;
		if (!ba.write_int32(partner_vec[i].train_in_attack)) return false;
		if (!ba.write_int32(partner_vec[i].train_in_def)) return false;
		if (!ba.write_int32(partner_vec[i].train_toughness)) return false;
		if (!ba.write_buf((char*)partner_vec[i].star_array, sizeof(partner_vec[i].star_array))) return false;
	}
	return true;
}

//PROTO_USER_GET_ARENA_INFO_CMD	= 641,	//得到竞技场信息
cli_user_get_arena_info_out::cli_user_get_arena_info_out()
{
	this->init();
}

void cli_user_get_arena_info_out::init()
{
	daily_cnt = 0;		//今日挑战剩余次数
	arena_cd = 0;
	can_get_prize = 0;	//是否可以领取奖励
	memset(top_rank, 0, sizeof(top_rank));
}

int cli_user_get_arena_info_out::get_msg_len()
{
	return sizeof(daily_cnt) + sizeof(arena_cd) + sizeof(can_get_prize) + sizeof(top_rank);
}

bool cli_user_get_arena_info_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(daily_cnt)) return false;
	if (!ba.read_uint32(arena_cd)) return false;
	if (!ba.read_uint32(can_get_prize)) return false;
	if (!ba.read_buf((char*)top_rank, sizeof(top_rank))) return false;
	return true;
}

bool cli_user_get_arena_info_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(daily_cnt)) return false;
	if (!ba.write_uint32(arena_cd)) return false;
	if (!ba.write_uint32(can_get_prize)) return false;
	if (!ba.write_buf((char*)top_rank, sizeof(top_rank))) return false;
	return true;
}

//PROTO_USER_GET_ARENA_TOP_100_CMD= 642,	//得到竞技场top100
cli_user_get_arena_top_100_out::cli_user_get_arena_top_100_out()
{
	this->init();
}

void cli_user_get_arena_top_100_out::init()
{
	cur_page = 0;
	memset(top_rank, 0, sizeof(top_rank));
}

int cli_user_get_arena_top_100_out::get_msg_len()
{
	return sizeof(cur_page) + sizeof(top_rank);
}

bool cli_user_get_arena_top_100_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(cur_page)) return false;
	if (!ba.read_buf((char*)top_rank, sizeof(top_rank))) return false;
	return true;
}

bool cli_user_get_arena_top_100_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(cur_page)) return false;
	if (!ba.write_buf((char*)top_rank, sizeof(top_rank))) return false;
	return true;
}

//PROTO_USER_GET_ARENA_FILE_CMD	= 643,	//得到竞技场战报
cli_user_get_arena_file_out::cli_user_get_arena_file_out()
{

}

void cli_user_get_arena_file_out::init()
{
	arena_file_vec.clear();
}

int cli_user_get_arena_file_out::get_msg_len()
{
	return sizeof(int32_t) + sizeof(user_arena_file_info_t) * arena_file_vec.size();
}

bool cli_user_get_arena_file_out::read_from_buf(byte_array_t & ba)
{
	int32_t size = 0;
	if (!ba.read_int32(size)) return false;
	for (int i = 0; i < size; i++) {
		user_arena_file_info_t file_info;
		if (!ba.read_uint32(file_info.type)) return false;
		if (!ba.read_uint32(file_info.battle_id)) return false;
		if (!ba.read_buf(file_info.battle_nick, 32)) return false;
		if (!ba.read_uint32(file_info.battle_time)) return false;
		if (!ba.read_uint32(file_info.battle_result)) return false;
		if (!ba.read_uint32(file_info.new_rank)) return false;
		arena_file_vec.push_back(file_info);
	}
	return true;
}

bool cli_user_get_arena_file_out::write_to_buf(byte_array_t & ba)
{
	int32_t size = arena_file_vec.size();
	if (!ba.write_int32(size)) return false;
	for (int i = 0; i < size; i++) {
		if (!ba.write_uint32(arena_file_vec[i].type)) return false;
		if (!ba.write_uint32(arena_file_vec[i].battle_id)) return false;
		if (!ba.write_buf(arena_file_vec[i].battle_nick, 32)) return false;
		if (!ba.write_uint32(arena_file_vec[i].battle_time)) return false;
		if (!ba.write_uint32(arena_file_vec[i].battle_result)) return false;
		if (!ba.write_uint32(arena_file_vec[i].new_rank)) return false;
	}
	return true;
}

//PROTO_USER_WORSHIP_TOP_USER_CMD	= 644,	//膜拜	
cli_user_worship_top_user_out::cli_user_worship_top_user_out()
{
	this->init();
}

void cli_user_worship_top_user_out::init()
{
	cost_yxb = 0;
	cost_gold = 0;
	add_prestige = 0;	//增加声望
}

int cli_user_worship_top_user_out::get_msg_len()
{
	return sizeof(cost_yxb) + sizeof(cost_gold) + sizeof(add_prestige);
}

bool cli_user_worship_top_user_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(cost_yxb)) return false;
	if (!ba.read_uint32(cost_gold)) return false;
	if (!ba.read_uint32(add_prestige)) return false;
	return true;
}

bool cli_user_worship_top_user_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(cost_yxb)) return false;
	if (!ba.write_uint32(cost_gold)) return false;
	if (!ba.write_uint32(add_prestige)) return false;
	return true;
}

//PROTO_USER_START_ARENA_BATTLE_CMD = 645,//开始竞技场战斗
cli_user_start_arena_battle_out::cli_user_start_arena_battle_out()
{
	this->init();
}

void cli_user_start_arena_battle_out::init()
{
	userid = 0;
	unit_id = 0;
	cost_gold = 0;
	memset(nick, 0, sizeof(nick));
	level = 0;             //等级
	life = 0;             	//生命值，许翔：这儿记录的是最大生命值
	out_attack = 0;        //攻击力
	out_def = 0;           //防御力
	in_attack = 0;         //内攻
	in_def = 0;            //内防
	crit = 0;             //暴击
	toughness = 0;        //韧性
	recover_hp = 0;        //回血
	reduce_hurt = 0;       //减伤
	add_hurt = 0;          //伤害加深
	suck_hp = 0;           //吸血

	memset(skills, 0, sizeof(skills));
	memset(battle_session, 0, sizeof(battle_session));
}

int cli_user_start_arena_battle_out::get_msg_len()
{
	return sizeof(userid) + sizeof(unit_id) + sizeof(cost_gold) + sizeof(nick) + sizeof(level) + sizeof(life) 
		+ sizeof(out_attack) + sizeof(out_def) + sizeof(in_attack) + sizeof(in_def) + sizeof(crit) 
		+ sizeof(toughness) + sizeof(recover_hp) + sizeof(reduce_hurt) + sizeof(add_hurt) + sizeof(suck_hp)
		+ sizeof(skills) + sizeof(battle_session);
}

bool cli_user_start_arena_battle_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(userid)) return false;
	if (!ba.read_uint32(unit_id)) return false;
	if (!ba.read_uint32(cost_gold)) return false;
	if (!ba.read_buf(nick, 32)) return false;
	if (!ba.read_uint32(level)) return false;
	if (!ba.read_uint32(life)) return false;
	if (!ba.read_float(out_attack)) return false;
	if (!ba.read_float(out_def)) return false;
	if (!ba.read_float(in_attack)) return false;
	if (!ba.read_float(in_def)) return false;
	if (!ba.read_float(crit)) return false;
	if (!ba.read_float(toughness)) return false;
	if (!ba.read_float(recover_hp)) return false;
	if (!ba.read_float(reduce_hurt)) return false;
	if (!ba.read_float(add_hurt)) return false;
	if (!ba.read_float(suck_hp)) return false;
    if (!ba.read_buf((char*)this->skills, sizeof(this->skills))) return false;
	if (!ba.read_buf((char*)battle_session, sizeof(battle_session))) return false;
	return true;
}

bool cli_user_start_arena_battle_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(userid)) return false;
	if (!ba.write_uint32(unit_id)) return false;
	if (!ba.write_uint32(cost_gold)) return false;
	if (!ba.write_buf(nick, 32)) return false;
	if (!ba.write_uint32(level)) return false;
	if (!ba.write_uint32(life)) return false;
	if (!ba.write_float(out_attack)) return false;
	if (!ba.write_float(out_def)) return false;
	if (!ba.write_float(in_attack)) return false;
	if (!ba.write_float(in_def)) return false;
	if (!ba.write_float(crit)) return false;
	if (!ba.write_float(toughness)) return false;
	if (!ba.write_float(recover_hp)) return false;
	if (!ba.write_float(reduce_hurt)) return false;
	if (!ba.write_float(add_hurt)) return false;
	if (!ba.write_float(suck_hp)) return false;
    if (!ba.write_buf((char*)this->skills, sizeof(this->skills))) return false;
	if (!ba.write_buf((char*)battle_session, sizeof(battle_session))) return false;
	return true;
}

//PROTO_USER_START_ARENA_BATTLE_CMD = 646,//开始竞技场战斗
cli_user_over_arena_battle_in::cli_user_over_arena_battle_in()
{
	this->init();
}

void cli_user_over_arena_battle_in::init()
{
	battle_result = 0;		//0失败 1胜利
	target_id = 0;
	memset(battle_session, 0, 32);
}

int cli_user_over_arena_battle_in::get_msg_len()
{
	return sizeof(battle_result) + sizeof(target_id) + sizeof(battle_session);
}

bool cli_user_over_arena_battle_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(battle_result)) return false;
	if (!ba.read_uint32(target_id)) return false;
	if (!ba.read_buf((char*)battle_session, sizeof(battle_session))) return false;

	return true;
}

bool cli_user_over_arena_battle_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(battle_result)) return false;
	if (!ba.write_uint32(target_id)) return false;
	if (!ba.write_buf((char*)battle_session, sizeof(battle_session))) return false;
	
	return true;
}

cli_user_over_arena_battle_out::cli_user_over_arena_battle_out()
{
	this->init();
}

void cli_user_over_arena_battle_out::init()
{
	battle_result = 0;
	prize_vec.clear();
}

int cli_user_over_arena_battle_out::get_msg_len()
{
	return sizeof(battle_result) + sizeof(int32_t) + sizeof(item_unit_t) * prize_vec.size();
}

bool cli_user_over_arena_battle_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(battle_result)) return false;

	int vec_size = 0;
	if (!ba.read_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		item_unit_t item;
		if (!ba.read_uint32(item.item_id)) return false;
		if (!ba.read_uint32(item.item_cnt)) return false;
		prize_vec.push_back(item);
	}

	return true;
}

bool cli_user_over_arena_battle_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(battle_result)) return false;

	int vec_size = prize_vec.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		if (!ba.write_uint32(prize_vec[i].item_id)) return false;
		if (!ba.write_uint32(prize_vec[i].item_cnt)) return false;
	}
	return true;
}

//PROTO_USER_GET_ARENA_RANK_PRIZE_CMD	= 647,//竞技场领取奖励
cli_user_get_arena_rank_prize_out::cli_user_get_arena_rank_prize_out()
{
	this->init();
}

void cli_user_get_arena_rank_prize_out::init()
{
	prize_yxb = 0;
	prize_prestige = 0;
}

int cli_user_get_arena_rank_prize_out::get_msg_len()
{
	return sizeof(prize_yxb) + sizeof(prize_prestige);
}

bool cli_user_get_arena_rank_prize_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(prize_yxb)) return false;
	if (!ba.read_uint32(prize_prestige)) return false;
	return true;
}

bool cli_user_get_arena_rank_prize_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(prize_yxb)) return false;
	if (!ba.write_uint32(prize_prestige)) return false;
	return true;
}

//PROTO_USER_GET_WORSHIP_INFO_CMD	= 648,	//得到膜拜信息
cli_user_get_worship_info_out::cli_user_get_worship_info_out()
{
	this->init();
}

void cli_user_get_worship_info_out::init()
{
	worship_field = 0;		//worship_field & 0x01 (第一名已完成)
	memset(top_rank, 0, sizeof(top_rank));
}
	
int cli_user_get_worship_info_out::get_msg_len()
{
	return sizeof(worship_field) + sizeof(top_rank);
}

bool cli_user_get_worship_info_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(worship_field)) return false;
	if (!ba.read_buf((char*)top_rank, sizeof(top_rank))) return false;
	return true;
}

bool cli_user_get_worship_info_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(worship_field)) return false;
	if (!ba.write_buf((char*)top_rank, sizeof(top_rank))) return false;
	return true;
}

//PROTO_USER_SHOP_INFO			= 650,//拉取商城数据,传递参数 为1代表是道具 为2代表是礼包
cli_user_shop_info_in::cli_user_shop_info_in()
{
	this->init();
}

void cli_user_shop_info_in::init()
{
	shop_type = 0;
	page_no = 0;
}

int cli_user_shop_info_in::get_msg_len()
{
	return sizeof(shop_type) + sizeof(page_no);
}

bool cli_user_shop_info_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(shop_type)) return false;
	if (!ba.read_uint32(page_no)) return false;

	return true;
}

bool cli_user_shop_info_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(shop_type)) return false;
	if (!ba.write_uint32(page_no)) return false;

	return true;
}


//PROTO_USER_SHOP_INFO			= 650,//拉取商城数据,传递参数 为1代表是道具 为2代表是礼包
cli_user_shop_info_out::cli_user_shop_info_out()
{
	this->init();
}

void cli_user_shop_info_out::init()
{
	shop_type = 0;
	page_no = 0;
	record_num = 0;
	shop_vec.clear();
}

int cli_user_shop_info_out::get_msg_len()
{
	uint32_t len = sizeof(shop_type) + sizeof(page_no) + sizeof(record_num);
	len += sizeof(uint32_t);
	
	for (uint32_t i = 0; i < shop_vec.size(); i++) {
		len += sizeof(shop_vec[i].product_id) + sizeof(shop_vec[i].title) + sizeof(shop_vec[i].des);
		len += sizeof(shop_vec[i].flag1) + sizeof(shop_vec[i].show_price);
	   	len	+= sizeof(shop_vec[i].now_price) + sizeof(shop_vec[i].vip_price) + sizeof(shop_vec[i].vip_level);

		len += sizeof(shop_vec[i].times) + sizeof(shop_vec[i].second);

		len += sizeof(uint32_t);
		len += shop_vec[i].item_vec.size() * sizeof(item_unit_t);
	}
	return len;
}

bool cli_user_shop_info_out::read_from_buf(byte_array_t & ba)
{
	uint32_t vec_size = 0;
	if (!ba.read_uint32(shop_type)) return false;
	if (!ba.read_uint32(page_no)) return false;
	if (!ba.read_uint32(record_num)) return false;
	if (!ba.read_uint32(vec_size)) return false;
		
	for (uint32_t i = 0; i < vec_size; i++) {
		shop_item_info_t info;
		info.item_vec.clear();

		memset(info.title,0,sizeof(info.title));
		memset(info.des,0,sizeof(info.des));		
		if (!ba.read_uint32(info.product_id)) return false;
		if (!ba.read_buf(info.title,sizeof(info.title))) return false;
		if (!ba.read_buf(info.des,sizeof(info.des))) return false;

		if (!ba.read_uint32(info.flag1)) return false;

		if (!ba.read_uint32(info.show_price)) return false;
		if (!ba.read_uint32(info.now_price)) return false;
		if (!ba.read_uint32(info.vip_price)) return false;
		if (!ba.read_uint32(info.vip_level)) return false;

		if (!ba.read_int32(info.times)) return false;
		if (!ba.read_uint32(info.second)) return false;

		uint32_t vec_size2 = 0;
		if (!ba.read_uint32(vec_size2)) return false;

		for (uint32_t j =0; j < vec_size2; j++) {
			item_unit_t item_info;
			if (!ba.read_uint32(item_info.item_id)) return false;
			if (!ba.read_uint32(item_info.item_cnt)) return false;
			info.item_vec.push_back(item_info);
		}
		shop_vec.push_back(info);
	}


	return true;
}

bool cli_user_shop_info_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(shop_type)) return false;
	if (!ba.write_uint32(page_no)) return false;
	if (!ba.write_uint32(record_num)) return false;
	if (!ba.write_uint32(shop_vec.size())) return false;

	for (uint32_t i = 0; i < shop_vec.size(); i++) {
		if (!ba.write_uint32(shop_vec[i].product_id)) return false;
		if (!ba.write_buf(shop_vec[i].title,sizeof(shop_vec[i].title))) return false;
		if (!ba.write_buf(shop_vec[i].des,sizeof(shop_vec[i].des))) return false;

		if (!ba.write_uint32(shop_vec[i].flag1)) return false;

		if (!ba.write_uint32(shop_vec[i].show_price)) return false;
		if (!ba.write_uint32(shop_vec[i].now_price)) return false;
		if (!ba.write_uint32(shop_vec[i].vip_price)) return false;
		if (!ba.write_uint32(shop_vec[i].vip_level)) return false;

		if (!ba.write_int32(shop_vec[i].times)) return false;
		if (!ba.write_uint32(shop_vec[i].second)) return false;

		uint32_t vec_size2 = shop_vec[i].item_vec.size();
		if (!ba.write_uint32(vec_size2)) return false;

		for (uint32_t j =0; j < vec_size2; j++) {
			if (!ba.write_uint32(shop_vec[i].item_vec[j].item_id)) return false;
			if (!ba.write_uint32(shop_vec[i].item_vec[j].item_cnt)) return false;
		}
	}
	return true;
}

///PROTO_USER_SHOP_BUY				= 651,//购买商城物品
cli_user_shop_buy_in::cli_user_shop_buy_in()
{
	this->init();
}

void cli_user_shop_buy_in::init()
{
	product_id = 0;
	product_cnt = 0;
}

int cli_user_shop_buy_in::get_msg_len()
{
	return sizeof(product_id) + sizeof(product_cnt);
}

bool cli_user_shop_buy_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(product_id)) return false;
	if (!ba.read_uint32(product_cnt)) return false;

	return true;
}

bool cli_user_shop_buy_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(product_id)) return false;
	if (!ba.write_uint32(product_cnt)) return false;

	return true;
}

cli_user_shop_buy_out::cli_user_shop_buy_out()
{
	this->init();
}

void cli_user_shop_buy_out::init()
{
	product_id = 0;
	product_cnt = 0;
	gold_cnt = 0;
	move_to_gift = 0;
	item_vec.clear();
}

int cli_user_shop_buy_out::get_msg_len()
{
	uint32_t len = sizeof(product_id) + sizeof(product_cnt) + sizeof(gold_cnt) + sizeof(move_to_gift);
	len += sizeof(uint32_t) + item_vec.size()*sizeof(item_unit_t);

	return len;
}

bool cli_user_shop_buy_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(product_id)) return false;
	if (!ba.read_uint32(product_cnt)) return false;
	if (!ba.read_uint32(gold_cnt)) return false;
	if (!ba.read_uint32(move_to_gift)) return false;
	
	uint32_t vec_size = 0;
	if (!ba.read_uint32(vec_size)) return false;

	for (uint32_t i = 0; i < vec_size; i++) {
		item_unit_t item = {0};
		if (!ba.read_uint32(item.item_id)) return false;
		if (!ba.read_uint32(item.item_cnt)) return false;
		item_vec.push_back(item);
	}

	return true;
}

bool cli_user_shop_buy_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(product_id)) return false;
	if (!ba.write_uint32(product_cnt)) return false;
	if (!ba.write_uint32(gold_cnt)) return false;
	if (!ba.write_uint32(move_to_gift)) return false;

	uint32_t vec_size = item_vec.size();
	if (!ba.write_uint32(vec_size)) return false;

	for (uint32_t i = 0; i < vec_size; i++) {
		if (!ba.write_uint32(item_vec[i].item_id)) return false;
		if (!ba.write_uint32(item_vec[i].item_cnt)) return false;
	}

	return true;
}

//PROTO_USER_RAND_INFO_CMD		= 653,//排行榜数据
cli_user_rank_info_in::cli_user_rank_info_in()
{
	this->init();
}

void cli_user_rank_info_in::init()
{
	rank_type = 0;
}

int cli_user_rank_info_in::get_msg_len()
{
	return sizeof(rank_type);
}

bool cli_user_rank_info_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(rank_type)) return false;

	return true;
}

bool cli_user_rank_info_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(rank_type)) return false;

	return true;
}

cli_user_rank_info_out::cli_user_rank_info_out()
{
	this->init();
}

void cli_user_rank_info_out::init()
{
	rank_type = 0;
	rank_vec.clear();
}

int cli_user_rank_info_out::get_msg_len()
{
	uint32_t len = sizeof(rank_type);
	len += sizeof(uint32_t) + rank_vec.size()*sizeof(rank_info_t);

	return len;
}

bool cli_user_rank_info_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(rank_type)) return false;
	
	uint32_t vec_size = 0;
	if (!ba.read_uint32(vec_size)) return false;

	for (uint32_t i = 0; i < vec_size; i++) {
		rank_info_t rank = {0};
		if (!ba.read_uint32(rank.rank_id)) return false;
		if (!ba.read_uint32(rank.userid)) return false;
		if (!ba.read_uint32(rank.role_type)) return false;
		if (!ba.read_buf(rank.nick,32)) return false;
		if (!ba.read_uint32(rank.faction)) return false;
		if (!ba.read_uint32(rank.value)) return false;
		rank_vec.push_back(rank);
	}

	return true;
}

bool cli_user_rank_info_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(rank_type)) return false;
	
	uint32_t vec_size = rank_vec.size();
	if (!ba.write_uint32(vec_size)) return false;

	for (uint32_t i = 0; i < vec_size; i++) {
		if (!ba.write_uint32(rank_vec[i].rank_id)) return false;
		if (!ba.write_uint32(rank_vec[i].userid)) return false;
		if (!ba.write_uint32(rank_vec[i].role_type)) return false;
		if (!ba.write_buf(rank_vec[i].nick,32)) return false;
		if (!ba.write_uint32(rank_vec[i].faction)) return false;
		if (!ba.write_uint32(rank_vec[i].value)) return false;
	}

	return true;
}

//PROTO_USER_PRIVATE_GIFT = 654 个人礼包  type 1代表商城背包不足时过来的物品
cli_user_gift_box_info_in::cli_user_gift_box_info_in()
{
	this->init();
}

void cli_user_gift_box_info_in::init()
{
	type = 0;
}

int cli_user_gift_box_info_in::get_msg_len()
{
	return sizeof(type);
}

bool cli_user_gift_box_info_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(type)) return false;

	return true;
}

bool cli_user_gift_box_info_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(type)) return false;

	return true;
}

cli_user_gift_box_info_out::cli_user_gift_box_info_out()
{
	this->init();
}

void cli_user_gift_box_info_out::init()
{
	type = 0;
	box_vec.clear();
}

int cli_user_gift_box_info_out::get_msg_len()
{
	uint32_t len = sizeof(type);
	len += sizeof(uint32_t);

	for (uint32_t i = 0;i<box_vec.size();i++) {
		len += sizeof(box_vec[i].id) + sizeof(box_vec[i].gift_id) + sizeof(box_vec[i].insert_time) + sizeof(box_vec[i].title) + sizeof(box_vec[i].des);

		len += sizeof(uint32_t);
		for (uint32_t j = 0;j<box_vec[i].items_vec.size();j++) {
			len += sizeof(box_vec[i].items_vec[j].item_id) + sizeof(box_vec[i].items_vec[j].item_cnt);
		}
	}
	return len;
}

bool cli_user_gift_box_info_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(type)) return false;
	
	uint32_t vec_size = 0;
	if (!ba.read_uint32(vec_size)) return false;

	for (uint32_t i = 0; i < vec_size; i++) {
		gift_box_t box = {0};
		if (!ba.read_uint32(box.id)) return false;
		if (!ba.read_uint32(box.gift_id)) return false;
		if (!ba.read_uint32(box.insert_time)) return false;
		if (!ba.read_buf(box.title,20)) return false;
		if (!ba.read_buf(box.des,40)) return false;
		
		uint32_t vec_vec_size = 0;
		if (!ba.read_uint32(vec_vec_size)) return false;
		for (uint32_t j = 0;j<vec_vec_size;j++) {
			item_unit_t item = {0};
			if (!ba.read_uint32(item.item_id)) return false;
			if (!ba.read_uint32(item.item_cnt)) return false;
			box.items_vec.push_back(item);
		}
		box_vec.push_back(box);
	}

	return true;
}

bool cli_user_gift_box_info_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(type)) return false;
	
	uint32_t vec_size = box_vec.size();
	if (!ba.write_uint32(vec_size)) return false;

	for (uint32_t i = 0; i < vec_size; i++) {
		if (!ba.write_uint32(box_vec[i].id)) return false;
		if (!ba.write_uint32(box_vec[i].gift_id)) return false;
		if (!ba.write_uint32(box_vec[i].insert_time)) return false;
		if (!ba.write_buf(box_vec[i].title,20)) return false;
		if (!ba.write_buf(box_vec[i].des,40)) return false;

		uint32_t vec_vec_size = box_vec[i].items_vec.size();
		if (!ba.write_uint32(vec_vec_size)) return false;
		for (uint32_t j = 0;j<vec_vec_size;j++) {
			if (!ba.write_uint32(box_vec[i].items_vec[j].item_id)) return false;
			if (!ba.write_uint32(box_vec[i].items_vec[j].item_cnt)) return false;
		}
	}

	return true;
}

//PROTO_USER_PRIVATE_GIFT_GET = 655 个人礼包领取  type 1代表商城背包不足时过来的物品 id为后端传说给前端的ID
cli_user_gift_box_get_in::cli_user_gift_box_get_in()
{
	this->init();
}

void cli_user_gift_box_get_in::init()
{
	type = 0;
	id = 0;
}

int cli_user_gift_box_get_in::get_msg_len()
{
	return sizeof(type) + sizeof(id);
}

bool cli_user_gift_box_get_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(type)) return false;
	if (!ba.read_uint32(id)) return false;

	return true;
}

bool cli_user_gift_box_get_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(type)) return false;
	if (!ba.write_uint32(id)) return false;

	return true;
}

cli_user_gift_box_get_out::cli_user_gift_box_get_out()
{
	this->init();
}

void cli_user_gift_box_get_out::init()
{
	type = 0;
	id = 0;
	result = 0;
	items_vec.clear();
}

int cli_user_gift_box_get_out::get_msg_len()
{
	uint32_t len = sizeof(type) + sizeof(id) + sizeof(result);
	len += sizeof(uint32_t);

	len += items_vec.size() * sizeof(item_unit_t);
	return len;
}

bool cli_user_gift_box_get_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(type)) return false;
	if (!ba.read_uint32(id)) return false;
	if (!ba.read_uint32(result)) return false;
	
	uint32_t vec_size = 0;
	if (!ba.read_uint32(vec_size)) return false;

	for (uint32_t i = 0; i < vec_size; i++) {
		item_unit_t item = {0};
		if (!ba.read_uint32(item.item_id)) return false;
		if (!ba.read_uint32(item.item_cnt)) return false;

		items_vec.push_back(item);
	}

	return true;
}

bool cli_user_gift_box_get_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(type)) return false;
	if (!ba.write_uint32(id)) return false;
	if (!ba.write_uint32(result)) return false;

	uint32_t vec_size = items_vec.size();
	if (!ba.write_uint32(vec_size)) return false;

	for (uint32_t i = 0; i < vec_size; i++) {
		if (!ba.write_uint32(items_vec[i].item_id)) return false;
		if (!ba.write_uint32(items_vec[i].item_cnt)) return false;
	}

	return true;
}

//PROTO_USER_AUTO_BOSS_BATTLE_CMD = 658, //BOSS扫荡
cli_user_auto_boss_battle_out::cli_user_auto_boss_battle_out()
{
	this->init();
}

void cli_user_auto_boss_battle_out::init()
{
	wait_flag = 0;
	boss_vec.clear();
}

int cli_user_auto_boss_battle_out::get_msg_len()
{
	int len = sizeof(wait_flag);
	for (int i = 0; i < (int)boss_vec.size(); i++) {
		len += boss_vec[i].get_msg_len();
	}
	return len;
}

bool cli_user_auto_boss_battle_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(wait_flag)) return false;

	int prize_size = 0;
	if (!ba.read_int32(prize_size)) return false;
	for (int i = 0; i < prize_size; ++i) {
		auto_battle_t prize;
		prize.read_from_buf(ba);
		boss_vec.push_back(prize);
	}
	return true;
}

bool cli_user_auto_boss_battle_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(wait_flag)) return false;

	int prize_size = boss_vec.size();
	if (!ba.write_int32(prize_size)) return false;
	for (int i = 0; i < prize_size; ++i) {
		boss_vec[i].write_to_buf(ba);
	}
	return true;
}

//PROTO_USER_GET_MAX_MISSION_CMD = 659，//拉取开放的最大MISSION ID
cli_user_get_max_mission_out::cli_user_get_max_mission_out()
{
	this->init();	
}

void cli_user_get_max_mission_out::init()
{
	maxid_vec.clear();
}

int  cli_user_get_max_mission_out::get_msg_len()
{
	return maxid_vec.size() * sizeof(uint32_t) + sizeof(uint32_t);
}

bool cli_user_get_max_mission_out::read_from_buf(byte_array_t & ba)
{
	uint32_t vec_size = 0;
	if (!ba.read_uint32(vec_size)) return false;
	for (uint32_t i =0; i< vec_size; i++) {
		uint32_t maxid = 0;
		if (!ba.read_uint32(maxid)) return false;
		maxid_vec.push_back(maxid);
	}
	return true;
}

bool cli_user_get_max_mission_out::write_to_buf(byte_array_t & ba)
{
	uint32_t vec_size = maxid_vec.size();
	if (!ba.write_uint32(vec_size)) return false;
	for (uint32_t i =0; i < vec_size; i++) {
		if (!ba.write_uint32(maxid_vec[i])) return false;
	}
	return true;
}

//PROTO_USER_GET_VILLA_INFO_CMD	= 660,	//得到山庄信息
cli_user_get_villa_info_out::cli_user_get_villa_info_out()
{
	this->init();
}

void cli_user_get_villa_info_out::init()
{
	land_sum = 0;	//总共几块土地
	plant_vec.clear();
	land_vec.clear();	//只返回有种植的土地
};

int  cli_user_get_villa_info_out::get_msg_len()
{
	return sizeof(land_sum) + sizeof(int32_t) + sizeof(item_unit_t) * plant_vec.size();
}

bool cli_user_get_villa_info_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(land_sum)) return false;
	int32_t vec_size = 0;
	if (!ba.read_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		item_unit_t item;
		if (!ba.read_uint32(item.item_id)) return false;
		if (!ba.read_uint32(item.item_cnt)) return false;
		plant_vec.push_back(item);
	}

	if (!ba.read_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		villa_land_info_t land_info;
		if (!ba.read_uint32(land_info.land_id)) return false;
		if (!ba.read_uint32(land_info.plant_id)) return false;
		if (!ba.read_uint32(land_info.left_time)) return false;
		land_vec.push_back(land_info);
	}
	return true;
}

bool cli_user_get_villa_info_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(land_sum)) return false;
	int32_t vec_size = plant_vec.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		if (!ba.write_uint32(plant_vec[i].item_id)) return false;
		if (!ba.write_uint32(plant_vec[i].item_cnt)) return false;
	}

	vec_size = land_vec.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		if (!ba.write_uint32(land_vec[i].land_id)) return false;
		if (!ba.write_uint32(land_vec[i].plant_id)) return false;
		if (!ba.write_uint32(land_vec[i].left_time)) return false;
	}
	return true;
}


//PROTO_USER_PLANT_CMD			= 661,	//种植种子
cli_user_plant_in::cli_user_plant_in()
{
	this->init();
}

void cli_user_plant_in::init()
{
	plant_id = 0;		//种子ID
	land_id = 0;		//土地ID
}

int  cli_user_plant_in::get_msg_len()
{
	return sizeof(plant_id) + sizeof(land_id);
}

bool cli_user_plant_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(plant_id)) return false;
	if (!ba.read_uint32(land_id)) return false;
	return true;
}

bool cli_user_plant_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(plant_id)) return false;
	if (!ba.write_uint32(land_id)) return false;
	return true;
}

cli_user_plant_out::cli_user_plant_out()
{
	this->init();
}

void cli_user_plant_out::init()
{
	memset(&land_info, 0, sizeof(land_info));
}

int  cli_user_plant_out::get_msg_len()
{
	return sizeof(land_info);
}

bool cli_user_plant_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(land_info.land_id)) return false;
	if (!ba.read_uint32(land_info.plant_id)) return false;
	if (!ba.read_uint32(land_info.left_time)) return false;
	return true;
}

bool cli_user_plant_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(land_info.land_id)) return false;
	if (!ba.write_uint32(land_info.plant_id)) return false;
	if (!ba.write_uint32(land_info.left_time)) return false;
	return true;
}

//PROTO_USER_SPEED_PLANT_CMD		= 662,	//加速植物
cli_user_speed_plant_in::cli_user_speed_plant_in()
{
	this->init();
}

void cli_user_speed_plant_in::init()
{
	land_id = 0;
	plant_id = 0;
	cost_gold = 0;
}

int  cli_user_speed_plant_in::get_msg_len()
{
	return sizeof(land_id) + sizeof(plant_id) + sizeof(cost_gold);
}

bool cli_user_speed_plant_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(land_id)) return false;
	if (!ba.read_uint32(plant_id)) return false;
	if (!ba.read_uint32(cost_gold)) return false;
	return true;
}

bool cli_user_speed_plant_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(land_id)) return false;
	if (!ba.write_uint32(plant_id)) return false;
	if (!ba.write_uint32(cost_gold)) return false;
	return true;
}

cli_user_speed_plant_out::cli_user_speed_plant_out()
{
	this->init();
}

void cli_user_speed_plant_out::init()
{
	land_id = 0;
	cost_gold = 0;
}

int  cli_user_speed_plant_out::get_msg_len()
{
	return sizeof(land_id) + sizeof(cost_gold);
}

bool cli_user_speed_plant_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(land_id)) return false;
	if (!ba.read_uint32(cost_gold)) return false;
	return true;
}

bool cli_user_speed_plant_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(land_id)) return false;
	if (!ba.write_uint32(cost_gold)) return false;
	return true;
}

//PROTO_USER_HAVEST_PLANT_CMD		= 663,	//收获果实
cli_user_havest_plant_in::cli_user_havest_plant_in()
{
	this->init();
}

void cli_user_havest_plant_in::init()
{
	land_id = 0;
	plant_id = 0;
}

int  cli_user_havest_plant_in::get_msg_len()
{
	return sizeof(land_id) + sizeof(plant_id);
}

bool cli_user_havest_plant_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(land_id)) return false;
	if (!ba.read_uint32(plant_id)) return false;
	return true;
}

bool cli_user_havest_plant_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(land_id)) return false;
	if (!ba.write_uint32(plant_id)) return false;
	return true;
}

cli_user_havest_plant_out::cli_user_havest_plant_out()
{
	this->init();
}

void cli_user_havest_plant_out::init()
{
	land_id = 0;
	memset(&prize, 0, sizeof(prize));		//种植获得的奖励
}

int  cli_user_havest_plant_out::get_msg_len()
{
	return sizeof(land_id) + sizeof(prize);
}

bool cli_user_havest_plant_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(land_id)) return false;
	if (!ba.read_uint32(prize.item_id)) return false;
	if (!ba.read_uint32(prize.item_cnt)) return false;
	return true;
}

bool cli_user_havest_plant_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(land_id)) return false;
	if (!ba.write_uint32(prize.item_id)) return false;
	if (!ba.write_uint32(prize.item_cnt)) return false;
	return true;
}

//PROTO_USER_EXPAND_LAND_CMD		= 664,	//扩展土地
cli_user_expand_land_out::cli_user_expand_land_out()
{
	this->init();
}

void cli_user_expand_land_out::init()
{
	land_id = 0;
	cost_yxb  = 0;
}

int  cli_user_expand_land_out::get_msg_len()
{
	return sizeof(land_id) + sizeof(cost_yxb);
}

bool cli_user_expand_land_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(land_id)) return false;
	if (!ba.read_uint32(cost_yxb)) return false;
	return true;
}

bool cli_user_expand_land_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(land_id)) return false;
	if (!ba.write_uint32(cost_yxb)) return false;
	return true;
}

//PROTO_USER_SHAKE_MONEY_CMD		= 665,	//摇钱树
cli_user_shake_money_out::cli_user_shake_money_out()
{
	this->init();	
}

void cli_user_shake_money_out::init()
{
	cost_gold = 0;		//消耗黄金
	add_yxb = 0;		//获得银两
	is_mul = 0;		//本次金子是否点亮
}

int  cli_user_shake_money_out::get_msg_len()
{
	return sizeof(cost_gold) + sizeof(add_yxb) + sizeof(is_mul);
}

bool cli_user_shake_money_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(cost_gold)) return false;
	if (!ba.read_uint32(add_yxb)) return false;
	if (!ba.read_uint32(is_mul)) return false;
	return true;
}

bool cli_user_shake_money_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(cost_gold)) return false;
	if (!ba.write_uint32(add_yxb)) return false;
	if (!ba.write_uint32(is_mul)) return false;
	return true;
}

//PROTO_USER_GET_VILLA_STATE_CMD	= 676,	//得到山庄状态
cli_user_get_villa_state_out::cli_user_get_villa_state_out()
{
	this->init();
}

void cli_user_get_villa_state_out::init()
{
	plant_state = 0;
}

int  cli_user_get_villa_state_out::get_msg_len()
{
	return sizeof(plant_state);
}

bool cli_user_get_villa_state_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(plant_state)) return false;
	return true;
}

bool cli_user_get_villa_state_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(plant_state)) return false;
	return true;
}

////PROTO_USER_ACTIVITY_DICE_CMD					= 666, //活动摇色子
cli_user_activity_dice_out::cli_user_activity_dice_out()
{
	this->init();	
}

void cli_user_activity_dice_out::init()
{
	enery = 0;
	cost_gold = 0;
	cost_yxb = 0;
}

int  cli_user_activity_dice_out::get_msg_len()
{
	return sizeof(enery) + sizeof(cost_gold) + sizeof(cost_yxb);
}

bool cli_user_activity_dice_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(enery)) return false;
	if (!ba.read_uint32(cost_gold)) return false;
	if (!ba.read_uint32(cost_yxb)) return false;
	return true;
}

bool cli_user_activity_dice_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(enery)) return false;
	if (!ba.write_uint32(cost_gold)) return false;
	if (!ba.write_uint32(cost_yxb)) return false;
	return true;
}

//PROTO_USER_TOWER_CMD					= 667, //爬塔拉取信息
cli_user_tower_out::cli_user_tower_out()
{
	this->init();	
}

void cli_user_tower_out::init()
{
	cur_pos = 0;
	max_pos = 0;
	reseted_cnt = 0;
}

int  cli_user_tower_out::get_msg_len()
{
	return 3 * sizeof(uint32_t);
}

bool cli_user_tower_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(cur_pos)) return false;
	if (!ba.read_uint32(max_pos)) return false;
	if (!ba.read_uint32(reseted_cnt)) return false;
	return true;
}

bool cli_user_tower_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(cur_pos)) return false;
	if (!ba.write_uint32(max_pos)) return false;
	if (!ba.write_uint32(reseted_cnt)) return false;
	return true;
}

//PROTO_USER_AUTO_TOWERBATTLE_CMD = 668,//爬塔自动挑战

//PROTO_USER_GET_FRIEND_LIST_CMD  = 670, //拉取好友信息
cli_user_get_friend_list_out::cli_user_get_friend_list_out()
{
	this->init();
}

void cli_user_get_friend_list_out::init()
{
	friend_vec.clear();
}

int  cli_user_get_friend_list_out::get_msg_len()
{
	uint32_t len = sizeof(int32_t) + friend_vec.size() * sizeof(friend_info_t) + sizeof(uint32_t) * friend_vec.size();
	
	for (uint32_t i =0; i < friend_vec.size(); i++) {
		len += friend_vec[i].offline_vec.size() * sizeof(offline_msg_t);
	}
	return len;
}

bool cli_user_get_friend_list_out::read_from_buf(byte_array_t & ba)
{
	int32_t vec_size = 0;
	if (!ba.read_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		friend_info_t info;
		if (!ba.read_uint32(info.type)) return false;
		if (!ba.read_uint32(info.friend_id)) return false;
		if (!ba.read_uint16(info.svr_id)) return false;
		if (!ba.read_uint32(info.unit_id)) return false;
		if (!ba.read_buf(info.friend_nick, 32)) return false;
		if (!ba.read_uint32(info.level)) return false;
		if (!ba.read_uint32(info.time)) return false;
		uint32_t msg_size = 0;
		if (!ba.read_uint32(msg_size)) return false;
		for (uint32_t j =0; j<msg_size; j++) {
			offline_msg_t msg;
			if (!ba.read_uint32(msg.time)) return false;
			if (!ba.read_buf(msg.msg,255)) return false;
			info.offline_vec.push_back(msg);
		}
		//if (!ba.read_uint32(info.offline_cnt)) return false;
		friend_vec.push_back(info);
	}
	return true;
}

bool cli_user_get_friend_list_out::write_to_buf(byte_array_t & ba)
{
	int32_t vec_size = friend_vec.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		if (!ba.write_uint32(friend_vec[i].type)) return false;
		if (!ba.write_uint32(friend_vec[i].friend_id)) return false;
		if (!ba.write_uint16(friend_vec[i].svr_id)) return false;
		if (!ba.write_uint32(friend_vec[i].unit_id)) return false;
		if (!ba.write_buf(friend_vec[i].friend_nick, 32)) return false;
		if (!ba.write_uint32(friend_vec[i].level)) return false;
		if (!ba.write_uint32(friend_vec[i].time)) return false;
		uint32_t offline_size = friend_vec[i].offline_vec.size();
		if (!ba.write_uint32(offline_size)) return false;
		for (uint32_t j =0; j < offline_size; j++) {
			if (!ba.write_uint32(friend_vec[i].offline_vec[j].time)) return false;
			if (!ba.write_buf(friend_vec[i].offline_vec[j].msg,255)) return false;
		}
		//if (!ba.write_uint32(friend_vec[i].offline_cnt)) return false;
	}
	return true;
}

//PROTO_USER_ADD_FRIEND_CMD       = 671, //用户添加好友
cli_user_add_friend_in::cli_user_add_friend_in()
{
	this->init();
}

void cli_user_add_friend_in::init()
{
	memset(friend_nick, 0, sizeof(friend_nick));
	friend_id = 0;
	svr_id = 0;
	is_black = 0;
}

int cli_user_add_friend_in::get_msg_len()
{
	return sizeof(friend_nick) + sizeof(friend_id) + sizeof(svr_id) + sizeof(is_black);
}

bool cli_user_add_friend_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_buf(friend_nick, sizeof(friend_nick))) return false;
	if (!ba.read_uint32(friend_id)) return false;
	if (!ba.read_uint32(svr_id)) return false;
	if (!ba.read_uint32(is_black)) return false;
	return true;
}

bool cli_user_add_friend_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_buf(friend_nick, sizeof(friend_nick))) return false;
	if (!ba.write_uint32(friend_id)) return false;
	if (!ba.write_uint32(svr_id)) return false;
	if (!ba.write_uint32(is_black)) return false;
	return true;
}

//PROTO_USER_DEAL_FRIEND_REQUEST_CMD = 672,//用户处理好友请求
cli_user_deal_friend_reqeust_in::cli_user_deal_friend_reqeust_in()
{
	this->init();
}

void cli_user_deal_friend_reqeust_in::init()
{
	friend_vec.clear();
	opt = 0;
}
	
int  cli_user_deal_friend_reqeust_in::get_msg_len()
{
	return sizeof(uint32_t) + sizeof(opt) + sizeof(uint32_t) * friend_vec.size();
}

bool cli_user_deal_friend_reqeust_in::read_from_buf(byte_array_t & ba)
{
	uint32_t size = 0;
	if (!ba.read_uint32(size)) return false;
	if (!ba.read_uint32(opt)) return false;
	for (uint32_t i = 0; i < size; i++) {
		uint32_t friend_id = 0;
		if (!ba.read_uint32(friend_id)) return false;
		friend_vec.push_back(friend_id);
	}
	return true;
}

bool cli_user_deal_friend_reqeust_in::write_to_buf(byte_array_t & ba)
{
	uint32_t size = friend_vec.size();
	if (!ba.write_uint32(size)) return false;
	if (!ba.write_uint32(opt)) return false;
	for (uint32_t i = 0; i < size; i++) {
		if (!ba.write_uint32(friend_vec[i])) return false;		
	}
	return true;
}

//PROTO_USER_DEL_FRIEND_CMD       = 673, //用户删除好友

//PROTO_USER_RECV_FRIEND_REQUEST_CMD = 674,//用户主动收到好友请求

//PROTO_USER_FIND_USER_FROM_NICK_CMD = 675,//通过nick找到用户基本信息
cli_user_find_user_from_nick_in::cli_user_find_user_from_nick_in()
{
	this->init();
}

void cli_user_find_user_from_nick_in::init()
{
	memset(friend_nick, 0, sizeof(friend_nick));
}

int  cli_user_find_user_from_nick_in::get_msg_len()
{
	return sizeof(friend_nick);
}

bool cli_user_find_user_from_nick_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_buf(friend_nick, sizeof(friend_nick))) return false;
	return true;
}

bool cli_user_find_user_from_nick_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_buf(friend_nick, sizeof(friend_nick))) return false;
	return true;
}

cli_user_find_user_from_nick_out::cli_user_find_user_from_nick_out()
{
	this->init();
}

void cli_user_find_user_from_nick_out::init()
{
	userid = 0;
	svr_id = 0;
	unit_id = 0;
	memset(nick, 0, sizeof(nick));
	level = 0;
}

int  cli_user_find_user_from_nick_out::get_msg_len()
{
	return sizeof(userid) + sizeof(svr_id) + sizeof(unit_id) + sizeof(nick) + sizeof(level);
}

bool cli_user_find_user_from_nick_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(userid)) return false;
	if (!ba.read_uint16(svr_id)) return false;
	if (!ba.read_uint32(unit_id)) return false;
	if (!ba.read_buf(nick, sizeof(nick))) return false;
	if (!ba.read_uint32(level)) return false;
	return true;
}

bool cli_user_find_user_from_nick_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(userid)) return false;
	if (!ba.write_uint16(svr_id)) return false;
	if (!ba.write_uint32(unit_id)) return false;
	if (!ba.write_buf(nick, sizeof(nick))) return false;
	if (!ba.write_uint32(level)) return false;
	return true;
}

//PROTO_USER_GET_STAR_LIST_CMD    = 677,  //得到占星列表
cli_user_get_star_list_out::cli_user_get_star_list_out()
{
	this->init();
}

void cli_user_get_star_list_out::init()
{
	astrologer_state = 0;
	star_chip_cnt = 0;
	star_vec.clear();
}
	
int  cli_user_get_star_list_out::get_msg_len()
{
	return sizeof(astrologer_state) + sizeof(star_chip_cnt) + sizeof(int32_t) + sizeof(star_ston_info_t) * star_vec.size();
}

bool cli_user_get_star_list_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(astrologer_state)) return false;
	if (!ba.read_uint32(star_chip_cnt)) return false;
	int32_t vec_size = 0;
	if (!ba.read_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		star_ston_info_t star_info;
		if (!ba.read_uint32(star_info.unique_id)) return false;
		if (!ba.read_uint32(star_info.star_id)) return false;
		if (!ba.read_uint32(star_info.seat_id)) return false;
		if (!ba.read_uint32(star_info.level)) return false;
		star_vec.push_back(star_info);
	}
	return true;
}

bool cli_user_get_star_list_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(astrologer_state)) return false;
	if (!ba.write_uint32(star_chip_cnt)) return false;
	int32_t vec_size = star_vec.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		if (!ba.write_uint32(star_vec[i].unique_id)) return false;
		if (!ba.write_uint32(star_vec[i].star_id)) return false;
		if (!ba.write_uint32(star_vec[i].seat_id)) return false;
		if (!ba.write_uint32(star_vec[i].level)) return false;
	}
	return true;
}

//PROTO_USER_ASTROLOGY_CMD        = 678,  //占星
cli_user_astrology_in::cli_user_astrology_in()
{
	this->init();
}

void cli_user_astrology_in::init()
{
	astrology_type = 0;
	astrologer_id = 0;
}

int  cli_user_astrology_in::get_msg_len()
{
	return sizeof(astrology_type) + sizeof(astrologer_id);
}

bool cli_user_astrology_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(astrology_type)) return false;
	if (!ba.read_uint32(astrologer_id)) return false;
	return true;
}

bool cli_user_astrology_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(astrology_type)) return false;
	if (!ba.write_uint32(astrologer_id)) return false;
	return true;
}

cli_user_astrology_out::cli_user_astrology_out()
{
	this->init();
}

void cli_user_astrology_out::init()
{
	astrologer_state = 0;
	cost_yxb = 0;
	star_vec.clear();
	astrology_vec.clear();
}
	
int  cli_user_astrology_out::get_msg_len()
{
	return sizeof(astrologer_state) + sizeof(cost_yxb) + sizeof(int32_t) + star_vec.size() * sizeof(uint32_t) 
		+ astrology_vec.size() * sizeof(uint32_t);
}

bool cli_user_astrology_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(astrologer_state)) return false;
	if (!ba.read_uint32(cost_yxb)) return false;
	int32_t vec_size = 0;
	if (!ba.read_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		star_ston_info_t star_info;
		if (!ba.read_uint32(star_info.unique_id)) return false;
		if (!ba.read_uint32(star_info.star_id)) return false;
		if (!ba.read_uint32(star_info.seat_id)) return false;
		if (!ba.read_uint32(star_info.level)) return false;
		star_vec.push_back(star_info);
	}
	
	for (int i = 0; i < vec_size; i++) {
		uint32_t astrology_id;
		if (!ba.read_uint32(astrology_id)) return false;
		astrology_vec.push_back(astrology_id);
	}
	
	return true;
}

bool cli_user_astrology_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(astrologer_state)) return false;
	if (!ba.write_uint32(cost_yxb)) return false;
	int vec_size = star_vec.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; i++) {
		if (!ba.write_uint32(star_vec[i].unique_id)) return false;
		if (!ba.write_uint32(star_vec[i].star_id)) return false;
		if (!ba.write_uint32(star_vec[i].seat_id)) return false;
		if (!ba.write_uint32(star_vec[i].level)) return false;
	}
	
	for (int i = 0; i < vec_size; i++) {
		if (!ba.write_uint32(astrology_vec[i])) return false;
	}
	return true;
}

//PROTO_USER_TRANSFORM_STAR_CMD   = 679,  //转化单个星石
cli_user_transform_star_in::cli_user_transform_star_in()
{
	this->init();
}

void cli_user_transform_star_in::init()
{
	unique_id = 0;
}

int  cli_user_transform_star_in::get_msg_len()
{
	return sizeof(unique_id);
}

bool cli_user_transform_star_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(unique_id)) return false;
	
	return true;
}

bool cli_user_transform_star_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(unique_id)) return false;
	
	return true;
}

//PROTO_USER_AUTO_TRANSFORM_STAR_CMD  = 680,  //一键转化星石
	
//PROTO_USER_EQUIP_STAR_CMD       = 681,  //装备或卸下星石
cli_user_equip_star_in::cli_user_equip_star_in()
{
	this->init();
}

void cli_user_equip_star_in::init()
{
	equip_type = 0;
	unique_id = 0;
	seat_id = 0;
}

int  cli_user_equip_star_in::get_msg_len()
{
	return sizeof(equip_type) + sizeof(unique_id) + sizeof(seat_id);
}

bool cli_user_equip_star_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(equip_type)) return false;
	if (!ba.read_uint32(unique_id)) return false;
	if (!ba.read_uint32(seat_id)) return false;
	return true;
}

bool cli_user_equip_star_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(equip_type)) return false;
	if (!ba.write_uint32(unique_id)) return false;
	if (!ba.write_uint32(seat_id)) return false;
	return true;
}

cli_user_equip_star_out::cli_user_equip_star_out()
{
	this->init();
}

void cli_user_equip_star_out::init()
{
	equip_type = 0;
	unique_id = 0;
	seat_id = 0;	
}

int  cli_user_equip_star_out::get_msg_len()
{
	return sizeof(equip_type) + sizeof(unique_id) + sizeof(seat_id);
}

bool cli_user_equip_star_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(equip_type)) return false;
	if (!ba.read_uint32(unique_id)) return false;
	if (!ba.read_uint32(seat_id)) return false;
	return true;
}

bool cli_user_equip_star_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(equip_type)) return false;
	if (!ba.write_uint32(unique_id)) return false;
	if (!ba.write_uint32(seat_id)) return false;
	return true;
}

//PROTO_USER_UPGRADE_STAR_CMD     = 682,  //升级星石
cli_user_upgrade_star_in::cli_user_upgrade_star_in()
{
	this->init();
}

void cli_user_upgrade_star_in::init()
{
	unique_id = 0;
}

int  cli_user_upgrade_star_in::get_msg_len()
{
	return sizeof(unique_id);
}

bool cli_user_upgrade_star_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(unique_id)) return false;
	return true;
}

bool cli_user_upgrade_star_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(unique_id)) return false;
	return true;
}

cli_user_upgrade_star_out::cli_user_upgrade_star_out()
{
	this->init();
}

void cli_user_upgrade_star_out::init()
{
	cost_star_exp = 0;
	unique_id = 0;
}

int  cli_user_upgrade_star_out::get_msg_len()
{
	return sizeof(cost_star_exp) + sizeof(unique_id);
}

bool cli_user_upgrade_star_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(cost_star_exp)) return false;
	if (!ba.read_uint32(unique_id)) return false;
	return true;
}

bool cli_user_upgrade_star_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(cost_star_exp)) return false;
	if (!ba.write_uint32(unique_id)) return false;
	return true;
}

//PROTO_USER_CALL_ASTROLOGER_CMD	= 683,	//召唤占星师
cli_user_call_astrologer_out::cli_user_call_astrologer_out()
{
	this->init();
}

void cli_user_call_astrologer_out::init()
{
	astrologer_state = 0;
	cost_gold = 0;
}

int  cli_user_call_astrologer_out::get_msg_len()
{
	return sizeof(astrologer_state) + sizeof(cost_gold);
}

bool cli_user_call_astrologer_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(astrologer_state)) return false;
	if (!ba.read_uint32(cost_gold)) return false;
	return true;
}

bool cli_user_call_astrologer_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(astrologer_state)) return false;
	if (!ba.write_uint32(cost_gold)) return false;
	return true;
}

//PROTO_USER_CHOOSE_HORSE_CMD = 684, //选马了
cli_user_choose_horse_out :: cli_user_choose_horse_out()
{
	this->init();
}

void cli_user_choose_horse_out::init() 
{
	horse_quality = 0;
	refreshed_cnt = 0;
	escort_time = 0;
	keap_time = 0;
}

int cli_user_choose_horse_out::get_msg_len()
{
	return sizeof(horse_quality) + sizeof(refreshed_cnt) 
		+ sizeof(escort_time) + sizeof(keap_time);
}

bool cli_user_choose_horse_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(horse_quality)) return false;
	if (!ba.read_uint32(refreshed_cnt)) return false;
	if (!ba.read_uint32(escort_time)) return false;
	if (!ba.read_uint32(keap_time)) return false;
	return true;
}

bool cli_user_choose_horse_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(horse_quality)) return false;
	if (!ba.write_uint32(refreshed_cnt)) return false;
	if (!ba.write_uint32(escort_time)) return false;
	if (!ba.write_uint32(keap_time)) return false;
	return true;
}

//PROTO_USER_REFRESH_HORSE_CMD = 685, //刷马
cli_user_refresh_horse_out :: cli_user_refresh_horse_out()
{
	this->init();
}

void cli_user_refresh_horse_out::init() 
{
	horse_quality = 0;
	refreshed_cnt = 0;
	cost_gold = 0;
}

int cli_user_refresh_horse_out::get_msg_len()
{
	return sizeof(horse_quality) + sizeof(refreshed_cnt) + sizeof(cost_gold);
}

bool cli_user_refresh_horse_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(horse_quality)) return false;
	if (!ba.read_uint32(refreshed_cnt)) return false;
	if (!ba.read_uint32(cost_gold)) return false;
	return true;
}

bool cli_user_refresh_horse_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(horse_quality)) return false;
	if (!ba.write_uint32(refreshed_cnt)) return false;
	if (!ba.write_uint32(cost_gold)) return false;
	return true;
}

//PROTO_USER_GET_ESCORT_INFO_CMD = 686, //押镖运信息
cli_user_get_escort_info_out::cli_user_get_escort_info_out()
{
	this->init();
}

void cli_user_get_escort_info_out::init()
{
	finish_sec = 0;
	escort_time = 0;
	keap_time = 0;
	horse_quality = 0;
	keaped_time = 0;
	memset(gift, 0, sizeof(gift));
	is_new = 0;
	escort_vec.clear();
}

int cli_user_get_escort_info_out::get_msg_len()
{
	return sizeof(finish_sec) + sizeof(escort_time) + sizeof(keap_time) + sizeof(horse_quality) + sizeof(keaped_time) + sizeof(gift) + sizeof(uint32_t) + escort_vec.size() * sizeof(uint32_t);
}

bool cli_user_get_escort_info_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(finish_sec)) return false;
	if (!ba.read_uint32(escort_time)) return false;
	if (!ba.read_uint32(keap_time)) return false;
	if (!ba.read_uint32(horse_quality)) return false;
	if (!ba.read_uint32(keaped_time)) return false;
	if (!ba.read_buf((char*)gift, sizeof(gift))) return false;
	if (!ba.read_uint32(is_new)) return false;
	uint32_t vec_size = 0;
	if (!ba.read_uint32(vec_size)) return false;
	for(uint32_t i = 0; i < vec_size; i++) {
		escort_info _escort_info = {0};
		if (!ba.read_buf((char*)&_escort_info, sizeof(_escort_info))) return false;
		escort_vec.push_back(_escort_info);
	}
	return true;
}

bool cli_user_get_escort_info_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(finish_sec)) return false;
	if (!ba.write_uint32(escort_time)) return false;
	if (!ba.write_uint32(keap_time)) return false;
	if (!ba.write_uint32(horse_quality)) return false;
	if (!ba.write_uint32(keaped_time)) return false;
	if (!ba.write_buf((char*)gift, sizeof(gift))) return false;
	if (!ba.write_uint32(is_new)) return false;
	uint32_t vec_size = escort_vec.size();
	if (!ba.write_uint32(vec_size)) return false;
	for(uint32_t i = 0; i < vec_size; i++) {
		if (!ba.write_buf((char*)&escort_vec[i], sizeof(escort_vec[i]))) return false;
	}
	return true;
}

//PROTO_USER_ESCORT_COMBAT_INFO_CMD = 687, //战报信息
cli_user_escort_combat_info_out::cli_user_escort_combat_info_out()
{
	this->init();
}

void cli_user_escort_combat_info_out::init()
{
	escort_combat_vec.clear();
}

int cli_user_escort_combat_info_out::get_msg_len()
{
	return sizeof(uint32_t) + sizeof(user_escort_file_info_t) * escort_combat_vec.size();
}

bool cli_user_escort_combat_info_out::read_from_buf(byte_array_t & ba)
{
	uint32_t vec_size = 0;
	if (!ba.read_uint32(vec_size)) return false;
	for(uint32_t i = 0; i < vec_size; i++) {
		user_escort_file_info_t _escort_file_info = {0};
		if (!ba.read_buf((char*)&_escort_file_info, sizeof(_escort_file_info))) return false;
		escort_combat_vec.push_back(_escort_file_info);
	}
	return true;
}

bool cli_user_escort_combat_info_out::write_to_buf(byte_array_t & ba)
{
	uint32_t vec_size = escort_combat_vec.size();
	if (!ba.write_uint32(vec_size)) return false;
	for(uint32_t i = 0; i < vec_size; i++) {
		if (!ba.write_buf((char*)&escort_combat_vec[i], sizeof(escort_combat_vec[i]))) return false;
	}
	return true;
}
//PROTO_USER_OVER_KEAP_HORSE_CMD = 689, //结束劫持白龙马
cli_user_over_keap_horse_out::cli_user_over_keap_horse_out()
{
	this->init();
}

void cli_user_over_keap_horse_out::init()
{
	battle_result = 0;
	silver_cnt = 0;
}

int cli_user_over_keap_horse_out::get_msg_len()
{
	return sizeof(battle_result) + sizeof(silver_cnt);
}

bool cli_user_over_keap_horse_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(battle_result)) return false;
	if (!ba.read_uint32(silver_cnt)) return false;
	return true;
}

bool cli_user_over_keap_horse_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(battle_result)) return false;
	if (!ba.write_uint32(silver_cnt)) return false;
	return true;
}

//PROTO_USER_GET_ESCORT_USER_INFO_CMD = 690, //拉取押镖玩家信息
cli_user_get_escort_user_info_out::cli_user_get_escort_user_info_out()
{
	this->init();
}

void cli_user_get_escort_user_info_out::init()
{
	memset(&escort_user, 0, sizeof(escort_user));
}

int cli_user_get_escort_user_info_out::get_msg_len()
{
	return sizeof(escort_user);
}

bool cli_user_get_escort_user_info_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_buf((char*)&escort_user, sizeof(escort_user))) return false;
	return true;
}

bool cli_user_get_escort_user_info_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_buf((char*)&escort_user, sizeof(escort_user))) return false;
	return true;
}

//PROTO_USER_ESCORT_GIFT_CMD = 692, //领押镖奖励
cli_user_escort_gift_out::cli_user_escort_gift_out()
{
	this->init();
}

void cli_user_escort_gift_out::init()
{
	is_moved = 0;
	memset(gift, 0, sizeof(gift));
}

int cli_user_escort_gift_out::get_msg_len()
{
	return sizeof(is_moved) + sizeof(gift);
}

bool cli_user_escort_gift_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(is_moved)) return false;
	if (!ba.read_buf((char*)gift, sizeof(gift))) return false;
	return true;
}

bool cli_user_escort_gift_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(is_moved)) return false;
	if (!ba.write_buf((char*)gift, sizeof(gift))) return false;
	return true;
}

//PROTO_USER_ESCORT_EXISTS_GIFT_CMD = 693, //是否存在押镖奖励
cli_user_escort_exists_gift_out::cli_user_escort_exists_gift_out()
{
	this->init();
}

void cli_user_escort_exists_gift_out::init()
{
	is_exist = 0;
	memset(gift, 0, sizeof(gift));
	escort_time = 0;
	keap_time = 0;
}

int cli_user_escort_exists_gift_out::get_msg_len()
{
	return sizeof(is_exist) + sizeof(gift) + sizeof(escort_time) + sizeof(keap_time);
}

bool cli_user_escort_exists_gift_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(is_exist)) return false;
	if (!ba.read_buf((char*)gift, sizeof(gift))) return false;
	if (!ba.read_uint32(escort_time)) return false;
	if (!ba.read_uint32(keap_time)) return false;
	return true;
}

bool cli_user_escort_exists_gift_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(is_exist)) return false;
	if (!ba.write_buf((char*)gift, sizeof(gift))) return false;
	if (!ba.write_uint32(escort_time)) return false;
	if (!ba.write_uint32(keap_time)) return false;
	return true;
}

//PROTO_USER_INVITE_INFO_CMD = 694, //好友邀请信息
cli_user_invite_info_out::cli_user_invite_info_out()
{
	this->init();
}

void cli_user_invite_info_out::init()
{
	invite_code = 0;
	friend_num = 0;
}

int cli_user_invite_info_out::get_msg_len()
{
	return sizeof(invite_code) + sizeof(friend_num);
}

bool cli_user_invite_info_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(invite_code)) return false;
	if (!ba.read_uint32(friend_num)) return false;
	return true;
}

bool cli_user_invite_info_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(invite_code)) return false;
	if (!ba.write_uint32(friend_num)) return false;
	return true;
}

//PROTO_USER_GET_WORLD_BOSS_INFO_CMD = 695, //拉取世界boss信息
cli_user_get_world_boss_info_out::cli_user_get_world_boss_info_out()
{
	this->init();
}

void cli_user_get_world_boss_info_out::init()
{
	boss_hp_max = 0;
	boss_hp = 0;
    boss_out_atk = 0;
    boss_in_atk = 0;
    boss_out_def = 0;
    boss_in_def = 0;
    boss_add_hurt = 0;
	my_hurt_hp = 0;
	reward_yxb = 0;
	reward_nerve = 0;
	invigorate = 0;
	time_counter = 0;
	users.clear();
}

int cli_user_get_world_boss_info_out::get_msg_len()
{
	int len = sizeof(boss_hp_max) + sizeof(boss_hp) + sizeof(boss_out_atk) + sizeof(boss_in_atk);
	len += sizeof(boss_out_def) + sizeof(boss_in_def) + sizeof(boss_add_hurt);
	len += sizeof(my_hurt_hp) + sizeof(reward_yxb) + sizeof(reward_nerve) + sizeof(invigorate) + sizeof(time_counter);
	len += sizeof(uint32_t) + sizeof(simpl_rank_info_t) * users.size();
	return len;
}

bool cli_user_get_world_boss_info_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(boss_hp_max)) return false;
	if (!ba.read_uint32(boss_hp)) return false;
	if (!ba.read_uint32(boss_out_atk)) return false;
	if (!ba.read_uint32(boss_in_atk)) return false;
	if (!ba.read_uint32(boss_out_def)) return false;
	if (!ba.read_uint32(boss_in_def)) return false;
	if (!ba.read_uint32(boss_add_hurt)) return false;

	if (!ba.read_uint32(my_hurt_hp)) return false;
	if (!ba.read_uint32(reward_yxb)) return false;
	if (!ba.read_uint32(reward_nerve)) return false;
	if (!ba.read_uint32(invigorate)) return false;
	if (!ba.read_int32(time_counter)) return false;
	uint32_t vec_size = 0;
	if (!ba.read_uint32(vec_size)) return false;
	for (uint32_t i = 0; i < vec_size; ++i) {
		simpl_rank_info_t user;
		if (!user.read_from_buf(ba)) return false;
		users.push_back(user);
	}
	return true;
}

bool cli_user_get_world_boss_info_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(boss_hp_max)) return false;
	if (!ba.write_uint32(boss_hp)) return false;
	if (!ba.write_uint32(boss_out_atk)) return false;
	if (!ba.write_uint32(boss_in_atk)) return false;
	if (!ba.write_uint32(boss_out_def)) return false;
	if (!ba.write_uint32(boss_in_def)) return false;
	if (!ba.write_uint32(boss_add_hurt)) return false;

	if (!ba.write_uint32(my_hurt_hp)) return false;
	if (!ba.write_uint32(reward_yxb)) return false;
	if (!ba.write_uint32(reward_nerve)) return false;
	if (!ba.write_uint32(invigorate)) return false;
	if (!ba.write_int32(time_counter)) return false;
	uint32_t vec_size = users.size();
	if (!ba.write_uint32(vec_size)) return false;
	for (uint32_t i = 0; i < vec_size; ++i) {
		if (!users[i].write_to_buf(ba)) return false;
	}
	return true;
}

//PROTO_USER_WORLD_BOSS_BTL_START_CMD = 696, //开始单局世界boss
//CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_world_boss_btl_start_in, uint32_t, cost_gold, uint32);//立即复活消耗的黄金
cli_user_world_boss_btl_start_out::cli_user_world_boss_btl_start_out()
{
	this->init();
}

void cli_user_world_boss_btl_start_out::init()
{
	need_btl = 0;
	boss_hp = 0;
	hurt_hp = 0;
	reward_yxb = 0;
	reward_nerve = 0;
	users.clear();
}

int cli_user_world_boss_btl_start_out::get_msg_len()
{
	int len = sizeof(need_btl) + sizeof(boss_hp) + sizeof(hurt_hp) + sizeof(reward_yxb) + sizeof(reward_nerve);
	len += sizeof(uint32_t) + sizeof(simpl_rank_info_t) * users.size();
	return len;
}

bool cli_user_world_boss_btl_start_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(need_btl)) return false;
	if (!ba.read_uint32(boss_hp)) return false;
	if (!ba.read_uint32(hurt_hp)) return false;
	if (!ba.read_uint32(reward_yxb)) return false;
	if (!ba.read_uint32(reward_nerve)) return false;
	uint32_t vec_size = 0;
	if (!ba.read_uint32(vec_size)) return false;
	for (uint32_t i = 0; i < vec_size; ++i) {
		simpl_rank_info_t user;
		if (!user.read_from_buf(ba)) return false;
		users.push_back(user);
	}

	return true;
}

bool cli_user_world_boss_btl_start_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(need_btl)) return false;
	if (!ba.write_uint32(boss_hp)) return false;
	if (!ba.write_uint32(hurt_hp)) return false;
	if (!ba.write_uint32(reward_yxb)) return false;
	if (!ba.write_uint32(reward_nerve)) return false;
	uint32_t vec_size = users.size();
	if (!ba.write_uint32(vec_size)) return false;
	for (uint32_t i = 0; i < vec_size; ++i) {
		if (!users[i].write_to_buf(ba)) return false;
	}

	return true;
}

//PROTO_USER_WORLD_BOSS_BTL_OVER_CMD = 697, //单局世界boss结束
//CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_world_boss_btl_over_in, uint32_t, hurt_hp, uint32);//本局打掉的boss血量
cli_user_world_boss_btl_over_out::cli_user_world_boss_btl_over_out()
{
	this->init();
}

void cli_user_world_boss_btl_over_out::init()
{
	boss_hp = 0;
	hurt_hp = 0;
	reward_yxb = 0;
	reward_nerve = 0;
	users.clear();
}

int cli_user_world_boss_btl_over_out::get_msg_len()
{
	int len = sizeof(boss_hp) + sizeof(hurt_hp) + sizeof(reward_yxb) + sizeof(reward_nerve);
	len += sizeof(uint32_t) + sizeof(simpl_rank_info_t) * users.size();
	return len;
}

bool cli_user_world_boss_btl_over_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(boss_hp)) return false;
	if (!ba.read_uint32(hurt_hp)) return false;
	if (!ba.read_uint32(reward_yxb)) return false;
	if (!ba.read_uint32(reward_nerve)) return false;
	uint32_t vec_size = 0;
	if (!ba.read_uint32(vec_size)) return false;
	for (uint32_t i = 0; i < vec_size; ++i) {
		simpl_rank_info_t user;
		if (!user.read_from_buf(ba)) return false;
		users.push_back(user);
	}

	return true;
}

bool cli_user_world_boss_btl_over_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(boss_hp)) return false;
	if (!ba.write_uint32(hurt_hp)) return false;
	if (!ba.write_uint32(reward_yxb)) return false;
	if (!ba.write_uint32(reward_nerve)) return false;
	uint32_t vec_size = users.size();
	if (!ba.write_uint32(vec_size)) return false;
	for (uint32_t i = 0; i < vec_size; ++i) {
		if (!users[i].write_to_buf(ba)) return false;
	}

	return true;
}

//PROTO_USER_WORLD_BOSS_INVIGORATE_CMD = 698, //世界boss鼓舞
//CREATE_CMESSAGE_WITH_ONE_MEMBER(cli_user_world_boss_invigorate_in, uint32_t, cost_gold, uint32);//鼓舞消耗的黄金
cli_user_world_boss_invigorate_out::cli_user_world_boss_invigorate_out()
{
	this->init();
}

void cli_user_world_boss_invigorate_out::init()
{
	cost_gold = 0;
	invigorate = 0;
}

int cli_user_world_boss_invigorate_out::get_msg_len()
{
	return sizeof(cost_gold) + sizeof(invigorate);
}

bool cli_user_world_boss_invigorate_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(cost_gold)) return false;
	if (!ba.read_uint32(invigorate)) return false;
	return true;
}

bool cli_user_world_boss_invigorate_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(cost_gold)) return false;
	if (!ba.write_uint32(invigorate)) return false;
	return true;
}

//PROTO_USER_WORLD_BOSS_OVER_NOTI_CMD = 699, //世界boss结束通知
cli_user_world_boss_over_noti_out::cli_user_world_boss_over_noti_out()
{
	this->init();
}

void cli_user_world_boss_over_noti_out::init()
{
	boss_hp_max = 0;
	boss_hp = 0;
	my_rank = 0;
	my_hurt_hp = 0;
	reward_yxb = 0;
	reward_nerve = 0;
	memset(&fatal_killer, 0, sizeof(fatal_killer));
	memset(&top_killers, 0, sizeof(top_killers));
}

int cli_user_world_boss_over_noti_out::get_msg_len()
{
	int len = sizeof(boss_hp_max) + sizeof (boss_hp) + sizeof(my_rank);
	len += sizeof(my_hurt_hp) + sizeof(reward_yxb) + sizeof(reward_nerve);
	len += sizeof(fatal_killer) +  sizeof(top_killers);
	return len;
}

bool cli_user_world_boss_over_noti_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(boss_hp_max)) return false;
	if (!ba.read_uint32(boss_hp)) return false;
	if (!ba.read_uint32(my_rank)) return false;
	if (!ba.read_uint32(my_hurt_hp)) return false;
	if (!ba.read_uint32(reward_yxb)) return false;
	if (!ba.read_uint32(reward_nerve)) return false;

	if (!fatal_killer.read_from_buf(ba)) return false;
	for (int i = 0; i < 3; ++i) {
		if (!top_killers[i].read_from_buf(ba)) return false;
	}
	return true;
}

bool cli_user_world_boss_over_noti_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(boss_hp_max)) return false;
	if (!ba.write_uint32(boss_hp)) return false;
	if (!ba.write_uint32(my_rank)) return false;
	if (!ba.write_uint32(my_hurt_hp)) return false;
	if (!ba.write_uint32(reward_yxb)) return false;
	if (!ba.write_uint32(reward_nerve)) return false;

	if (!fatal_killer.write_to_buf(ba)) return false;
	for (int i = 0; i < 3; ++i) {
		if (!top_killers[i].write_to_buf(ba)) return false;
	}
	return true;
}

//PROTO_USER_GET_ACTIVITY_INFO_CMD = 701, //获取活动信息
cli_user_get_activity_info_in::cli_user_get_activity_info_in()
{
	this->init();
}

void cli_user_get_activity_info_in::init()
{
	id_vec.clear();
}

int  cli_user_get_activity_info_in::get_msg_len()
{
	return 0;
}

bool cli_user_get_activity_info_in::read_from_buf(byte_array_t & ba)
{
	int vec_size = 0;
	if (!ba.read_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; ++i) {
		uint32_t activiy_id;
		if (!ba.read_uint32(activiy_id)) return false;
		id_vec.push_back(activiy_id);
	}
	return true;
}

bool cli_user_get_activity_info_in::write_to_buf(byte_array_t & ba)
{
	int vec_size = id_vec.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; ++i) {
		if (!ba.write_uint32(id_vec[i])) return false;
	}
	return true;
}

cli_user_get_activity_info_out::cli_user_get_activity_info_out()
{
	this->init();
}

void cli_user_get_activity_info_out::init()
{
	activity_vec.clear();
}

int  cli_user_get_activity_info_out::get_msg_len()
{
	return activity_vec.size() * 12;
}

bool cli_user_get_activity_info_out::read_from_buf(byte_array_t & ba)
{
	int vec_size = 0;
	if (!ba.read_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; ++i) {
		activity_time_t item;
		if (!item.read_from_buf(ba)) return false;
		activity_vec.push_back(item);
	}
	return true;
}

bool cli_user_get_activity_info_out::write_to_buf(byte_array_t & ba)
{
	int vec_size = activity_vec.size();
	if (!ba.write_int32(vec_size)) return false;
	for (int i = 0; i < vec_size; ++i) {
		if (!activity_vec[i].write_to_buf(ba)) return false;
	}
	return true;
}

//PROTO_USER_PARTNER_FRIENDVAL_TRAIN_CMD = 702, //弟子好友度培养
cli_user_partner_friendval_train_in::cli_user_partner_friendval_train_in()
{
	this->init();
}

void cli_user_partner_friendval_train_in::init()
{
	partner_id = 0;
	type = 0;
}

int  cli_user_partner_friendval_train_in::get_msg_len()
{
	return sizeof(partner_id) + sizeof(type);
}

bool cli_user_partner_friendval_train_in::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(partner_id)) return false;
	if (!ba.read_uint32(type)) return false;
	return true;
}

bool cli_user_partner_friendval_train_in::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(partner_id)) return false;
	if (!ba.write_uint32(type)) return false;
	return true;
}

cli_user_partner_friendval_train_out::cli_user_partner_friendval_train_out()
{
	this->init();
}

void cli_user_partner_friendval_train_out::init()
{
	partner_id = 0;
	cost_yxb = 0;
	add_friend_val = 0;
}

int  cli_user_partner_friendval_train_out::get_msg_len()
{
	return sizeof(partner_id) + sizeof(cost_yxb) + sizeof(add_friend_val);
}

bool cli_user_partner_friendval_train_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(partner_id)) return false;
	if (!ba.read_uint32(cost_yxb)) return false;
	if (!ba.read_uint32(add_friend_val)) return false;
	return true;
}

bool cli_user_partner_friendval_train_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(partner_id)) return false;
	if (!ba.write_uint32(cost_yxb)) return false;
	if (!ba.write_uint32(add_friend_val)) return false;
	return true;
}


//PROTO_USER_PARTNER_UNHAD_LIST_CMD = 703, //拉取没有获得的弟子列表及友好度
void partner_unhad_info_t::init()
{
	partner_id = 0;
	friend_val = 0;
	cur_state = 0;
}

bool partner_unhad_info_t::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(partner_id)) return false;
	if (!ba.read_uint32(friend_val)) return false;
	if (!ba.read_uint32(cur_state)) return false;
	return true;
}

bool partner_unhad_info_t::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(partner_id)) return false;
	if (!ba.write_uint32(friend_val)) return false;
	if (!ba.write_uint32(cur_state)) return false;
	return true;
}

cli_user_partner_unhad_list_out::cli_user_partner_unhad_list_out()
{
	this->init();
}

void cli_user_partner_unhad_list_out::init()
{
	unhad_vec.clear();
}

int  cli_user_partner_unhad_list_out::get_msg_len()
{
	return sizeof(uint32_t) + unhad_vec.size() * sizeof(partner_unhad_info_t);
}

bool cli_user_partner_unhad_list_out::read_from_buf(byte_array_t & ba)
{
	uint32_t vec_size = 0;
	if (!ba.read_uint32(vec_size)) return false;
	for (uint32_t i = 0; i < vec_size; i++) {
		partner_unhad_info_t one = {0};
		if (!one.read_from_buf(ba)) return false;
		unhad_vec.push_back(one);
	}
	return true;
}

bool cli_user_partner_unhad_list_out::write_to_buf(byte_array_t & ba)
{
	uint32_t vec_size = unhad_vec.size();
	if (!ba.write_uint32(vec_size)) return false;
	for (uint32_t i = 0; i < vec_size; i++) {
		if (!unhad_vec[i].write_to_buf(ba)) return false;
	}
	return true;
}

//PROTO_USER_SYS_MSG_NOTI_CMD = 710, //系统消息通知
void sys_noti_info_t::init()
{
	msg_type = 0;
	num = 0;
}

bool sys_noti_info_t::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(msg_type)) return false;
	if (!ba.read_uint32(num)) return false;
	return true;
}

bool sys_noti_info_t::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(msg_type)) return false;
	if (!ba.write_uint32(num)) return false;
	return true;
}

void cli_user_sys_msg_noti_out::init()
{
	msg_list.clear();
}

int cli_user_sys_msg_noti_out::get_msg_len()
{
	return msg_list.size() * (4 + 4);
}

bool cli_user_sys_msg_noti_out::read_from_buf(byte_array_t & ba)
{
	uint32_t vec_size = 0;
	if (!ba.read_uint32(vec_size)) return false;
	for (uint32_t i = 0; i < vec_size; i++) {
		sys_noti_info_t item;
		if (!item.read_from_buf(ba)) return false;
		msg_list.push_back(item);
	}
	return true;
}

bool cli_user_sys_msg_noti_out::write_to_buf(byte_array_t & ba)
{
	uint32_t vec_size = msg_list.size();
	if (!ba.write_uint32(vec_size)) return false;
	for (uint32_t i = 0; i < vec_size; i++) {
		if (!msg_list[i].write_to_buf(ba)) return false;
	}
	return true;
}

//PROTO_USER_GET_ORDER_BILL_NO_CMD	= 800,	//获取充值订单号
cli_user_get_order_bill_no_out::cli_user_get_order_bill_no_out()
{
	this->init();
}

void cli_user_get_order_bill_no_out::init()
{
	memset(bill_no, 0, sizeof(bill_no));
}

int  cli_user_get_order_bill_no_out::get_msg_len()
{
	return sizeof(bill_no);
}

bool cli_user_get_order_bill_no_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_buf(bill_no, sizeof(bill_no))) return false;
	return true;
}

bool cli_user_get_order_bill_no_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_buf(bill_no, sizeof(bill_no))) return false;
	return true;
}

//PROTO_USER_NOTIFY_BUY_GOLD_CMD		= 801,	//充值结果通知
cli_user_notify_buy_gold_out::cli_user_notify_buy_gold_out()
{
	this->init();
}

void cli_user_notify_buy_gold_out::init()
{
	buy_gold_total = 0;
	buy_gold = 0;
	is_first_buy = 0;
}

int  cli_user_notify_buy_gold_out::get_msg_len()
{
	return sizeof(buy_gold_total) + sizeof(buy_gold) + sizeof(is_first_buy);
}

bool cli_user_notify_buy_gold_out::read_from_buf(byte_array_t & ba)
{
	if (!ba.read_uint32(buy_gold_total)) return false;
	if (!ba.read_uint32(buy_gold)) return false;
	if (!ba.read_uint32(is_first_buy)) return false;
	return true;
}

bool cli_user_notify_buy_gold_out::write_to_buf(byte_array_t & ba)
{
	if (!ba.write_uint32(buy_gold_total)) return false;
	if (!ba.write_uint32(buy_gold)) return false;
	if (!ba.write_uint32(is_first_buy)) return false;
	return true;
}
