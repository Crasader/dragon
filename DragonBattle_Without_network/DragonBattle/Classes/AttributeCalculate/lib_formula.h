#ifndef LIB_FORMULA_H__
#define LIB_FORMULA_H__

#include <map>
#ifdef WONPEE_SERVER
#include "player.hpp"
struct partner_t;
#else
#include "online_cli_proto_cmd.h"
#endif

namespace LIB_FORMULA
{
enum {
    MAX_BUY_ENERGY_COST_GOLD = 200, //每次购买体力价格上限200黄金
};

/*
 * @brief 得到角色type类型的属性值(角色属性=角色基础属性+培养属性+装备属性+经脉+技能+练气)
 */
#ifdef WONPEE_SERVER
int get_role_attr(player_t* p, uint32_t attr_type);

/*
 * @brief 计算战力值
 */
int cal_role_fight_value(player_t* p);

/*
 * @brief 计算弟子战力值
 */
int cal_partner_fight_value(partner_t* partner);

#else
int get_role_attr(uint32_t attr_type);
#endif


/*
 * @brief 得到装备的属性值(装备属性=基础属性计算+强化等级的属性+洗练出的属性)
 */
int get_equipment_attr(package_equipment_t* equipment, uint32_t type);

/*
 * @brief 根据强化等级得到装备属性
 * @param equipment_id:装备ID
 * 	      equipment_level:装备等级
 * 	      type:属性类型
 */
int get_equipment_base_attr(uint32_t equipment_id, uint32_t equipment_level, uint32_t type);

/*
 * @brief 装备洗练的属性
 * @param equipment:装备
 * 		  type:属性类型
 * @return 装备洗练出来的属性值
 */
int get_equipment_attach_attr(package_equipment_t* equipment, uint32_t type);

/*
* @brief 装备镶嵌宝石的属性
* @param equipment:装备
* 		  type:属性类型
* @return 装备镶嵌宝石的属性值
*/
int get_equipment_gem_attr(package_equipment_t* equipment, uint32_t type);

/*
 * @brief 得到星石的属性
 * @param star:占星石信息
 * 		  type:属性类型
 * @return 星石的属性值
 */
int get_star_ston_attr(star_ston_info_t* star, uint32_t type);

/*
* @brief 被动技能(心法)的属性
* @param skill_id:技能id
*		  level:技能等级
* 		  type:属性类型
* @return 被动技能(心法)的属性值
*/
int get_role_passive_skill_attr(uint32_t skill_id, uint32_t level, uint32_t type);

/*
 * @brief 得到装备的各项属性值(装备属性=强化等级的属性+洗练出的属性)
 */
void get_equipment_attrs(package_equipment_t* equipment, uint32_t attrs[]);

/*
 * @brief 根据强化等级得到装备属性
 * @param equipment_id:装备ID
 * 	      level:装备等级
 * 	      attrs:属性值
 */
void get_equipment_base_attrs(uint32_t equipment_id, uint32_t level, uint32_t attrs[]);

/*
* @brief 装备洗练的属性
* @param equipment:装备
* 		  attrs:属性值
*/
void get_equipment_attach_attrs(package_equipment_t* equipment, uint32_t attrs[]);

/*
* @brief 装备镶嵌宝石的属性
* @param equipment:装备
* 		  attrs:属性类型
*/
void get_equipment_gem_attrs(package_equipment_t* equipment, uint32_t attrs[]);

/*
 * @brief 得到星石的各项属性
 * @param star:占星石信息
 * @param attrs:各项属性信息加成值
 */
void get_star_ston_attrs(star_ston_info_t* star, uint32_t* attrs);


/*
 * @brief 得到角色最大的培养属性
 * @param type:属性类型，在ItemConf.hpp中定义
 * 		  role_id:角色类型（1 2 3）
 * 		  role_level:角色等级
 * @return 培养上限
 */
float get_role_max_train_attr(uint32_t type, uint32_t role_id, uint32_t role_level);

/*
 * @brief 计算角色当前属性的培养值
 * @param train_type:培养类型
 * 		  attr_type:增加属性的类型
 * 		  cur_attr:当前属性值
 * 		  max_attr:当前类型最大可培养值
 * 		  role_level:角色等级
 * 		  is_asc:是否增加属性
 * @return 培养的值
 */
int get_role_train_attr_value(uint32_t train_type, uint32_t attr_type, uint32_t cur_attr, float max_attr, uint32_t role_level, bool is_asc);

/*
 * @brief 计算练气是否暴击
 * @param is_gold:是否使用黄金
 * @return 是否暴击(0不暴击，1暴击)
 */
int is_role_upgrade_force_crit(uint32_t is_gold);

/*
 * @brief 计算得到装备强化说需要的yxb花销
 * @param equipment_state:装备阶级
 * 		  level:装备等级
 * @return 装备强化所需要的yxb
 */
int get_equipment_strengthen_cost(uint32_t equipment_state, uint32_t level);

/*
 * @brief 计算得到出售装备的yxb价格
 * @param equipment_state:装备阶级
 * 		  level:装备等级
 * @return 装备出售的yxb价格
 */
inline int get_equipment_sell_price(uint32_t equipment_state, uint32_t level)
{
	return get_equipment_strengthen_cost(equipment_state, level) * 6;
}

/*
 * @brief 计算装备能够洗练出来的属性个数
 * @param equipment_id：装备ID
 * @return 属性个数
 */
int get_equipment_train_attr_count(uint32_t equipment_id);

/*
 * @brief 计算能洗练出的最大属性值
 * @param euqipment_id:装备ID
 * 		  attr_type:属性类型
 * @return 洗练的最大属性值
 */
int get_equipment_train_max_attr(uint32_t equipment_id, uint32_t attr_type);

/*
 * @brief 根据cd类型得到cd时间
 * @param cd_type:CD类型
 *        factor:传入的系数
 * @return cd时间,单位为秒
 */
int get_cooldown_time(uint32_t cd_type, uint32_t factor);

/*
 * @brief 弟子游历成功率判断
 * @param partner_qualify:弟子资质
 * 		  travel_type: 游历类型
 */
int is_partner_travel_succ(uint32_t partner_qualify, uint32_t travel_type);

/*
 * @brief 得到弟子一次银两游历的消耗
		公式：当次银两花费=当日银两提升次数*（弟子资质等级+1）*2000
 * @param qualify:弟子资质
 * 		  travel_count: 游历次数
 * @return 返回弟子消耗的银两
 */
int get_partner_travel_cost(uint32_t qualify, uint32_t travel_count);

/*
 * @brief 得到竞技场排行奖励
 */
int get_arena_rank_prize(int32_t rank_id, uint32_t& prize_yxb, uint32_t& prize_prestige);

const uint32_t* get_exchange_partner_list(uint32_t& exchange_count);

/*
 * @brief 扩展山庄种植土地的银两花费
 */
uint32_t get_expand_land_cost_yxb(uint32_t land_id);

/*
 * @brief 
 */
int cal_partner_inherit_qualify(uint32_t partner_id, uint32_t inherit_id, uint32_t cur_qualify, float factor, uint32_t* add_qualify);

/*
 * @brief 计算购买锤子的黄金花费 
 * @buy_times 当天已经购的次数
 */
uint32_t cal_buy_hammer_cost_gold(uint32_t buy_times);

/*
 * @brief 计算购买体力的黄金花费 
 * @buy_times 当天已经购的次数
 * @reference MAX_BUY_ENERGY_COST_GOLD 价格上限
 */
inline uint32_t cal_buy_energy_cost_gold(uint32_t buy_times) {
	uint32_t price = 15 + ((buy_times + 1) / 2 + 1) * 5;
	return price <= MAX_BUY_ENERGY_COST_GOLD ? price : MAX_BUY_ENERGY_COST_GOLD;
}

/*
 * @brief 计算弟子技能的伤害
 * @param addhurt 1级伤害
 *        lv           技能等级
 * @return 当前等级伤害
 */
inline int cal_partner_skill_hurt(int addhurt,int lv)
{
    return (int)(addhurt * (lv /4.0f + 0.75f));
}

inline uint32_t cal_bodyguard_get_yxb(uint32_t quarty,uint32_t lv)
{
    float s = quarty == 1?1:quarty == 2?1.2f:quarty == 3?1.5f:quarty == 4?2.0f:5.0f;
    return (uint32_t)(s*2000*lv+10000);
}
/*
 * @brief 计算合成宝石需要的符文数量
 * @param new_gem_lv 将合成的新宝石的等级
 * @return 合成宝石需要的符文数量
 */
uint32_t cal_compose_gem_req_rune_num(uint32_t new_gem_lv);

/*
 * @brief 增加活跃度
 * @return -1 类型错误，0 活跃度无实际增加，1 活跃度有增加
 */
int add_user_active_degree(uint32_t task_type, int count, uint8_t* active_degree);

/*
 * @brief 计算活跃度
 */
uint32_t user_cal_active_degree(uint8_t* p_active_state);
};


#endif
