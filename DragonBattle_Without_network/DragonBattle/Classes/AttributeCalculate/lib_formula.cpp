#ifdef WONPEE_SERVER
#include "./Configs/GameData.h"
#include "global_data.hpp"
#include "user_partner.hpp"
extern "C" {
#include <libwonpee/log.h>
}
#else
#include "GameData.h"
#endif

#include "lib_formula.h"
#include <math.h>

#ifndef EPSILON
#define EPSILON 0.000001
#endif

namespace LIB_FORMULA
{
enum {
	SKILL_INCR_FIGHT_VAULE = 15, //主动技能每提升一级增加的战力
};

/*
* @brief 得到角色type类型的属性值(角色属性=角色基础属性+培养属性+装备属性+经脉+技能+练气)
*/
#ifdef WONPEE_SERVER
int get_role_attr(player_t* p, uint32_t type)
{
	const RoleAttrConf_t* pRoleConf = GameData::getRoleAttrConf(p->level, p->unit_id);
	if (!pRoleConf) {
		return -1;
	}

	uint32_t nForceLevel = GameData::getRoleForceLevel(p->force_value);
	const RoleForceConf_t* pForce = GameData::getRoleForceSum(nForceLevel);
	const RoleNerveConf_t* pNerve = GameData::getRoleNerve(p->nerve_level);
	float value = 0;

	//TODO:RoleNerveConf_t的nAttrValue属性与equip_attr_type不完全一致(内攻外防是反的)
	//TODO:与前端HeroData::refreshBaseAttr是一致的
	//TODO:另竞技场，运镖劫持也是一致的，但与配表的表述及EquipAttributeType_t::OUT_DEFENCE,IN_ATTACK是反的
	// 基本属性 + 培养属性 + 练气
	// + 经脉	
	switch (type) {
		case HP:
			value = (pRoleConf->nLife + p->train_attr.train_hp + pForce->nLife + pNerve->nAttrValue[0]);
			break;
		case OUT_ATTACK:
			value = (pRoleConf->nOutAttack + p->train_attr.train_out_attack + pForce->nOutAttack + pNerve->nAttrValue[1]);
			break;
		case IN_ATTACK:
			value = (pRoleConf->nInAttack + p->train_attr.train_in_attack + pForce->nInAttack + pNerve->nAttrValue[2]);
			break;
		case OUT_DEFENCE:
			value = (pRoleConf->nOutDef + p->train_attr.train_out_def + pForce->nOutDef + pNerve->nAttrValue[3]);
			break;
		case IN_DEFENCE:
			value = (pRoleConf->nInDef + p->train_attr.train_in_def + pForce->nInDef + pNerve->nAttrValue[4]);
			break;
		case TOUGHNESS:
			value = (pRoleConf->nToughness + p->train_attr.train_toughness + pForce->nToughness + pNerve->nAttrValue[5]);
			break;
		case CRIT:
			value = pRoleConf->nCrit + pForce->nCrit + pNerve->nAttrValue[6];
			break;
		case REDUCE_HURT:
			value = pRoleConf->nReduceHurt + pForce->nReduceHurt + pNerve->nAttrValue[8];
			break;
		case ADD_HURT:
			value = pRoleConf->nAddHurt + pForce->nAddHurt + pNerve->nAttrValue[9];
			break;
		case RECOVER_HP:
			value = pRoleConf->nRecoverHP + pForce->nRecoverHP + pNerve->nAttrValue[7];
			break;
		default:
			break;
	}

	//装备(包括宝石镶嵌)
	for (int i = 0; i < 8; i++) {
		if (p->equipment[i].package_id != 0 && p->equipment[i].equipment_id != 0) {
			value += get_equipment_attr(&(p->equipment[i]), type);
		}
	}

	//被动技能
	for (uint32_t i = 0; i < sizeof(p->passive_skills) / sizeof(p->passive_skills[0]); ++i) {
		if (p->passive_skills[i] != 0) {
			uint32_t skill_id = GET_SKILL_ID(p->passive_skills[i]);
			uint32_t skill_lv = GET_SKILL_LEVEL(p->passive_skills[i]);
			value += get_role_passive_skill_attr(skill_id, skill_lv, type);
		} else {
			break;
		}
	}

	//星石
	for (int i = 0; i < 4; i++) {
		if (p->star_ston[i].star_id != 0) {
			value += get_star_ston_attr(p->star_ston + i, type);
		}
	}
	return (int)value;
}

/*
* @brief 计算战力值
*/
int cal_role_fight_value(player_t* p)
{
	p->fight_value = (int)(get_role_attr(p, HP) * 0.5
					+ (get_role_attr(p, OUT_ATTACK) + get_role_attr(p, IN_ATTACK)) * 3
					+ (get_role_attr(p, OUT_DEFENCE) + get_role_attr(p, IN_DEFENCE)) * 2
					+ get_role_attr(p, CRIT) * 1.5
					+ get_role_attr(p, TOUGHNESS) * 1
					+ get_role_attr(p, REDUCE_HURT) * 3
					+ get_role_attr(p, ADD_HURT) * 5
					+ get_role_attr(p, RECOVER_HP) * 3);
	//主动技能,额外增加战力
	for (uint32_t i = 0; i < sizeof(p->skill) / sizeof(p->skill[0]); ++i) {
		if (p->skill[i] != 0) {
			p->fight_value += SKILL_INCR_FIGHT_VAULE * GET_SKILL_LEVEL(p->skill[i]);
		}
	}

	DEBUG_LOG("get user fight value:[%d hp:%d out_atk:%d in_atk:%d out_def:%d in_def:%d crit:%d ness:%d red:%d hurt:%d rec:%d] fight:%d", p->userid, get_role_attr(p, HP), get_role_attr(p, OUT_ATTACK), get_role_attr(p, IN_ATTACK), get_role_attr(p, OUT_DEFENCE), get_role_attr(p, IN_DEFENCE), get_role_attr(p, CRIT), get_role_attr(p, TOUGHNESS), get_role_attr(p, REDUCE_HURT), get_role_attr(p, ADD_HURT), get_role_attr(p, RECOVER_HP), p->fight_value);

	update_global_user_change_data(p, false);
	return p->fight_value;
}

/*
 * @brief 计算弟子战力值
 */
int cal_partner_fight_value(partner_t* partner)
{
	const PartnerConf_t *partnerConf = GameData::getPartnerConf(partner->partner_id);
	if (!partnerConf) {
		ERROR_LOG("invalid partner %u", partner->partner_id);
		return 0;
	}
    RoleAttrConf_t pRoleAttr;//基本属性
    GameData::getPartnerAttrConf(partner->level, partner->partner_id, &pRoleAttr);

	uint32_t attrs[EQUIP_ATTR_MAX + 1] = {0};
	//基本属性 + 培养属性
	attrs[HP] += pRoleAttr.nLife + partner->train_attr.train_hp;
	attrs[OUT_ATTACK] += pRoleAttr.nOutAttack + partner->train_attr.train_out_attack;
	attrs[IN_ATTACK] += pRoleAttr.nInAttack + partner->train_attr.train_in_attack;
	attrs[OUT_DEFENCE] += pRoleAttr.nOutDef + partner->train_attr.train_out_def;
	attrs[IN_DEFENCE] += pRoleAttr.nInDef + partner->train_attr.train_in_def;
	attrs[TOUGHNESS] += pRoleAttr.nToughness + partner->train_attr.train_toughness;
	attrs[CRIT] += pRoleAttr.nCrit;
	attrs[REDUCE_HURT] += pRoleAttr.nReduceHurt;
	attrs[ADD_HURT] += pRoleAttr.nAddHurt;
	attrs[RECOVER_HP] += pRoleAttr.nRecoverHP;

	//装备(包括宝石镶嵌)
	for (int i = 0; i < 8; i++) {
		if (partner->equipment[i].package_id != 0 && partner->equipment[i].equipment_id != 0) {
			get_equipment_attrs(&(partner->equipment[i]), attrs);
		}
	}

	//星石
	for (int i = 0; i < 4; i++) {
		if (partner->star_ston[i].star_id != 0) {
			get_star_ston_attrs(partner->star_ston + i, attrs);
		}
	}

	uint32_t fight_value = attrs[HP] * 2
					+ (attrs[OUT_ATTACK] + attrs[IN_ATTACK]) * 3
					+ (attrs[OUT_DEFENCE] + attrs[IN_DEFENCE]) * 2
					+ uint32_t(attrs[CRIT] * 1.5)
					+ attrs[TOUGHNESS] * 1
					+ attrs[REDUCE_HURT] * 5
					+ attrs[ADD_HURT] * 5
					+ attrs[RECOVER_HP] * 3;

	fight_value += 10 * GameData::getPartnerSkillHurt(partnerConf->nSkillID, partner->skill_level);
	return fight_value;
}

#else
int get_role_attr(uint32_t attr_type)
{
	return 0;
}
#endif

/*
* @brief 得到装备的属性值(装备属性=强化等级的属性+洗练出的属性)
*/
int get_equipment_attr(package_equipment_t* equipment, uint32_t type)
{
	int value = get_equipment_base_attr(equipment->equipment_id, equipment->equipment_level, type)
				+ get_equipment_attach_attr(equipment, type)
				+ get_equipment_gem_attr(equipment, type);

	return value;
}

/*
* @brief 根据强化等级得到装备属性
* @param equipment_id:装备ID
* 	      equipment_level:装备等级
* 	      type:属性类型
*/
int get_equipment_base_attr(uint32_t equipment_id, uint32_t equipment_level, uint32_t type)
{
	float value = 0;
	const item_t* itm = GameData::getItem(equipment_id);
	if (!itm) {
		return -1;
	}
	uint32_t equip_state = getEquipmentState(itm);
	switch (type) {
	case HP:
		if (fabs(itm->u.equipment.nBaseHp) < EPSILON) {
			value = 0;
		} else {
			value = itm->u.equipment.nBaseHp * equip_state + itm->u.equipment.nBaseHp * equipment_level * itm->u.equipment.fEquipFac + 85 * itm->u.equipment.fEquipFac;
		}
		break;
	case OUT_ATTACK:
		if (fabs(itm->u.equipment.fBaseAtk) < EPSILON) {
			value = 0;
		} else {
			value = itm->u.equipment.fBaseAtk * equip_state + itm->u.equipment.fBaseAtk * equipment_level * itm->u.equipment.fEquipFac + 45 * itm->u.equipment.fEquipFac;
		}
		break;
	case IN_ATTACK:
		if (fabs(itm->u.equipment.fBaseInnerAtk) < EPSILON) {
			value = 0;
		} else {
			value = itm->u.equipment.fBaseInnerAtk * equip_state + itm->u.equipment.fBaseInnerAtk * equipment_level * itm->u.equipment.fEquipFac + 45 * itm->u.equipment.fEquipFac;
		}
		break;
	case OUT_DEFENCE:
		if (fabs(itm->u.equipment.fBaseDef) < EPSILON) {
			value = 0;
		} else {
			value = itm->u.equipment.fBaseDef * equip_state + itm->u.equipment.fBaseDef * equipment_level * itm->u.equipment.fEquipFac + 10 * itm->u.equipment.fEquipFac;
		}
		break;
	case IN_DEFENCE:
		if (fabs(itm->u.equipment.fBaseInnerDef) < EPSILON) {
			value = 0;
		} else {
			value = itm->u.equipment.fBaseInnerDef * equip_state + itm->u.equipment.fBaseInnerDef * equipment_level * itm->u.equipment.fEquipFac + 10 * itm->u.equipment.fEquipFac;
		}
		break;
	case TOUGHNESS:
		if (fabs(itm->u.equipment.fBaseDefCrit) < EPSILON) {
			value = 0;
		} else {
			value = itm->u.equipment.fBaseDefCrit * equip_state + itm->u.equipment.fBaseDefCrit * equipment_level * itm->u.equipment.fEquipFac + 10 * itm->u.equipment.fEquipFac;
		}
		break;
	case CRIT:
		if (fabs(itm->u.equipment.fBaseCrit) < EPSILON) {
			value = 0;
		} else {
			value = itm->u.equipment.fBaseCrit * equip_state + itm->u.equipment.fBaseCrit * equip_state + itm->u.equipment.fBaseCrit * equipment_level * itm->u.equipment.fEquipFac + 10 * itm->u.equipment.fEquipFac;
		}
		break;
	default :
		break;
	}
	return (int)value;
}

/*
* @brief 装备洗练的属性
* @param equipment:装备
* 		  type:属性类型
* @return 装备洗练出来的属性值
*/
int get_equipment_attach_attr(package_equipment_t* equipment, uint32_t type)
{
	for (int i = 0; i < 4; i++) {
		if (equipment->attach_attr[i].attr_type != 0) {
			if (equipment->attach_attr[i].attr_type == type) {
				return equipment->attach_attr[i].attr_value;
			}
		} else {
			break;
		}
	}
	return 0;
}

/*
* @brief 装备镶嵌宝石的属性
* @param equipment:装备
* 		  type:属性类型
* @return 装备镶嵌宝石的属性值
*/
int get_equipment_gem_attr(package_equipment_t* equipment, uint32_t type)
{
	uint32_t value = 0;
	for (int i = 0; i < EQUIP_GEM_NUM; i++) {
		if (equipment->gems[i] <= 1) {
			continue;
		}
		uint32_t gem_id = equipment->gems[i];
		const item_t* itm = GameData::getItem(gem_id);
		if (itm && getEquipGemType(itm->u.goods.nItemType) == type) {
			value += itm->u.goods.nEffect;
		}
	}
	return value;
}

/*
 * @brief 得到星石的属性
 * @param star:占星石信息
 * 		  type:属性类型
 * @return 星石的属性值
 */
int get_star_ston_attr(star_ston_info_t* star, uint32_t type)
{
	const item_t* pStar = GameData::getItem(star->star_id);
	if (!pStar || !isKindOfItem(pStar->nID, kStarItemKind)) {
		return 0;
	}
	
	int value = 0;
	
	switch (type) {
		case HP:
			value = pStar->u.star.nBaseHp * star->level;
			break;
		case OUT_ATTACK:
			value = pStar->u.star.nBaseAtk * star->level;
			break;
		case IN_ATTACK:
			value = pStar->u.star.nBaseInnerAtk * star->level;
			break;
		case OUT_DEFENCE:
			value = pStar->u.star.nBaseDef * star->level;
			break;
		case IN_DEFENCE:
			value = pStar->u.star.nBaseInnerDef * star->level;
			break;
		case TOUGHNESS:
			value = pStar->u.star.nBaseDefCrit * star->level;
			break;
		case CRIT:
			value = pStar->u.star.nBaseCrit * star->level;
			break;
		case REDUCE_HURT:
			value = pStar->u.star.nReduceHurt * star->level;
			break;
		case ADD_HURT:
			value = pStar->u.star.nAddHurt * star->level;
			break;
		case RECOVER_HP:
			break;
		default:
			break;
	}
	return value;
}


/*
* @brief 得到装备的各项属性值(装备属性=强化等级的属性+洗练出的属性)
*/
void get_equipment_attrs(package_equipment_t* equipment, uint32_t attrs[])
{
	get_equipment_base_attrs(equipment->equipment_id, equipment->equipment_level, attrs);
	get_equipment_attach_attrs(equipment, attrs);
	get_equipment_gem_attrs(equipment, attrs);
}

/*
* @brief 根据强化等级得到装备属性
* @param equipment_id:装备ID
* 	      level:装备等级
* 	      attrs:属性值
*/
void get_equipment_base_attrs(uint32_t equipment_id, uint32_t level, uint32_t attrs[])
{
	const item_t* itm = GameData::getItem(equipment_id);
	if (!itm) {
		return ;
	}
	uint32_t state = getEquipmentState(itm);//装备阶级
	const equipmentItem_t& equip = itm->u.equipment;
	if (equip.nBaseHp > 0) {
		attrs[HP] += uint32_t(equip.nBaseHp * state + equip.nBaseHp * level * equip.fEquipFac + 85 * equip.fEquipFac);
	}
	if (fabs(equip.fBaseAtk) > EPSILON) {
		attrs[OUT_ATTACK] += uint32_t(equip.fBaseAtk * state + equip.fBaseAtk * level * equip.fEquipFac + 45 * equip.fEquipFac);
	}
	if (fabs(equip.fBaseInnerAtk) > EPSILON) {
		attrs[IN_ATTACK] += uint32_t(equip.fBaseInnerAtk * state + equip.fBaseInnerAtk * level * equip.fEquipFac + 45 * equip.fEquipFac);
	}
	if (fabs(equip.fBaseDef) > EPSILON) {
		attrs[OUT_DEFENCE] += uint32_t(equip.fBaseDef * state + equip.fBaseDef * level * equip.fEquipFac + 10 * equip.fEquipFac);
	}
	if (fabs(equip.fBaseInnerDef) > EPSILON) {
		attrs[IN_DEFENCE] += uint32_t(equip.fBaseInnerDef * state + equip.fBaseInnerDef * level * equip.fEquipFac + 10 * equip.fEquipFac);
	}
	if (fabs(equip.fBaseDefCrit) > EPSILON) {
		attrs[TOUGHNESS] += uint32_t(equip.fBaseDefCrit * state + equip.fBaseDefCrit * level * equip.fEquipFac + 10 * equip.fEquipFac);
	}
	if (fabs(equip.fBaseCrit) > EPSILON) {
		attrs[CRIT] += uint32_t(equip.fBaseCrit * state + equip.fBaseCrit * state + equip.fBaseCrit * level * equip.fEquipFac + 10 * equip.fEquipFac);
	}
}

/*
* @brief 装备洗练的属性
* @param equipment:装备
* 		  attrs:属性值
*/
void get_equipment_attach_attrs(package_equipment_t* equipment, uint32_t attrs[])
{
	for (int i = 0; i < 4; i++) {
		uint32_t type = equipment->attach_attr[i].attr_type;
		if (type != 0 && type < EQUIP_ATTR_MAX) {
			attrs[type] += equipment->attach_attr[i].attr_value;
		} else {
			break;
		}
	}
	return ;
}

/*
* @brief 装备镶嵌宝石的属性
* @param equipment:装备
* 		  attrs:属性类型
*/
void get_equipment_gem_attrs(package_equipment_t* equipment, uint32_t attrs[])
{
	for (int i = 0; i < EQUIP_GEM_NUM; i++) {
		if (equipment->gems[i] <= 1) {
			continue;
		}
		uint32_t gem_id = equipment->gems[i];
		const item_t* itm = GameData::getItem(gem_id);
		if (!itm) {
			continue;
		}
		uint32_t type = getEquipGemType(itm->u.goods.nItemType);
		if (type != 0 && type < EQUIP_ATTR_MAX) {
			attrs[type] += itm->u.goods.nEffect;
		}
	}
	return ;
}

/*
 * @brief 得到星石的各项属性
 * @param star:占星石信息
 * @param attrs:各项属性信息加成值
 */
void get_star_ston_attrs(star_ston_info_t* star, uint32_t* attrs)
{
	const item_t* pStar = GameData::getItem(star->star_id);
	if (!pStar || !isKindOfItem(pStar->nID, kStarItemKind)) {
		return ;
	}
	attrs[HP] += pStar->u.star.nBaseHp * star->level;	
	attrs[OUT_ATTACK] += pStar->u.star.nBaseAtk * star->level;	
	attrs[IN_ATTACK] += pStar->u.star.nBaseInnerAtk * star->level;	
	attrs[OUT_DEFENCE] += pStar->u.star.nBaseDef * star->level;	
	attrs[IN_DEFENCE] += pStar->u.star.nBaseInnerDef * star->level;	
	attrs[TOUGHNESS] += pStar->u.star.nBaseDefCrit * star->level;	
	attrs[CRIT] += pStar->u.star.nBaseCrit * star->level;	
	attrs[REDUCE_HURT] += pStar->u.star.nReduceHurt * star->level;	
	attrs[ADD_HURT] += pStar->u.star.nAddHurt * star->level;	
	//attrs[RECOVER_HP] += 0;	
	return ;
}

/*
* @brief 被动技能(心法)的属性
* @param skill_id:技能id
*		  level:技能等级
* 		  type:属性类型
* @return 被动技能(心法)的属性值
*/
int get_role_passive_skill_attr(uint32_t skill_id, uint32_t level, uint32_t type)
{
	if (!isPassiveSkillID(skill_id)) {//不是被动技能
		return 0;
	}
	const SkillLevel_t* pConf = GameData::getSkillConf(skill_id, level);
	if (!pConf) {
		return 0;
	}
	int value = 0;
	switch (type) {
		case HP:
			value = pConf->skillLvBase.nHPValue;
			break;
		case OUT_ATTACK:
			value = pConf->skillLvBase.nOutAttackValue;
			break;
		case IN_ATTACK:
			value = pConf->skillLvBase.nInAttackValue;
			break;
		case OUT_DEFENCE:
			value = pConf->skillLvBase.nOutDefValue;
			break;
		case IN_DEFENCE:
			value = pConf->skillLvBase.nInDefValue;
			break;
		case TOUGHNESS:
			value = pConf->skillLvBase.nToughness;
			break;
		case CRIT:
			value = pConf->skillLvBase.nCrit;
			break;
		case REDUCE_HURT:
			value = pConf->skillLvBase.nReducehurtValue;
			break;
		case ADD_HURT:
			value = pConf->skillLvBase.nAddhurtValue;
			break;
		case RECOVER_HP:
			//value = pConf->skillLvBase.;
			break;
		default:
			break;
	}

	return value;
}

/*
* @brief 得到弟子最大的培养属性 (目前没用，写到GameData中计算了)
* @param attr_type:属性类型，在ItemConf.h中定义
* 		  partner_id:弟子ID
* 		  level:弟子等级
* @return 能培养出来的最大值
*/
float get_partner_max_train_attr(uint32_t attr_type, uint32_t partner_id, uint32_t level)
{
	const float fac_config[3][6] = {
		{0, 0.1, 0.1, 0.05, 0.05, 0.44},
		{0, 0.06, 0.06, 0.04, 0.04, 0.5},
		{0, 0.05, 0.05, 0.04, 0.04, 0.28},
	};

	float value = 0;
	const PartnerConf_t* pPartnerConf = GameData::getPartnerConf(partner_id);
	if (!pPartnerConf || pPartnerConf->nProfession == 0 || pPartnerConf->nProfession > 3) {
		return value;
	}

	const RoleAttrConf_t* pRole = GameData::getRoleAttrConf(level, pPartnerConf->nProfession);
	if (!pRole) {
		return value;
	}

	switch (attr_type) {
	case HP:
		value = pRole->nLife * 12 * 0.6 * pPartnerConf->fPartnerFac;
		break;
	case OUT_ATTACK:
		value = (pRole->nOutAttack * 20 + level * 1.0 / fac_config[pPartnerConf->nProfession - 1][1])
					* 0.8 * 1.2 * pPartnerConf->fPartnerFac;
		break;
	case IN_ATTACK:
		value = (pRole->nInAttack * 20 + level * 1.0 / fac_config[pPartnerConf->nProfession - 1][2])
					* 0.8 * 1.2 * pPartnerConf->fPartnerFac;
		break;
	case OUT_DEFENCE:
		value = (pRole->nOutDef * 20 + level * 1.0 / fac_config[pPartnerConf->nProfession - 1][3])
					* 1.8 * 0.8 * pPartnerConf->fPartnerFac;
		break;
	case IN_DEFENCE:
		value = (pRole->nInDef * 20 + level * 1.0 / fac_config[pPartnerConf->nProfession - 1][4])
					* 1.8 * 0.8 * pPartnerConf->fPartnerFac;
		break;
	case TOUGHNESS:
		value = (pRole->nToughness * 20 + level * 1.0 / fac_config[pPartnerConf->nProfession - 1][5])
					* 5 * 2 * pPartnerConf->fPartnerFac;
		break;
	default:
		break;
	}

	return value;
}

/*
* @brief 得到角色最大的培养属性
* @param type:属性类型，在ItemConf.h中定义
* 		  unit_id:角色类型（1 2 3 或弟子ID）
* 		  unit_level:角色等级
* @return 能培养出来的最大值
*/
float get_role_max_train_attr(uint32_t attr_type, uint32_t unit_id, uint32_t unit_level)
{
	float value = 0;
	const RoleAttrConf_t* role = NULL;
	if (unit_id <= 3) {
		role = GameData::getRoleAttrConf(unit_level, unit_id);
	} else {
		RoleAttrConf_t partnerConf;
		role = GameData::getPartnerAttrConf(unit_level, unit_id, &partnerConf);
	}

	if (role == NULL) {
		return 0;
	}
	
	switch (attr_type) {
	case HP:
		value = role->roleTrainAttr.fMaxLife;
		break;
	case OUT_ATTACK:
		value = role->roleTrainAttr.fMaxOutAttack;
		break;
	case IN_ATTACK:
		value = role->roleTrainAttr.fMaxInAttack;
		break;
	case OUT_DEFENCE:
		value = role->roleTrainAttr.fMaxOutDef;
		break;
	case IN_DEFENCE:
		value = role->roleTrainAttr.fMaxInDef;
		break;
	case TOUGHNESS:
		value = role->roleTrainAttr.fMaxToughness;
		break;
	default:
		break;
	}
	return value;
}

/*
* @brief 计算角色当前属性的培养值
* @param train_type:培养类型
* 		  cur_attr:当前属性值
* 		  max_attr:当前类型最大可培养值
* 		  role_level:角色等级
* 		  is_asc:是否增加属性
* @return 培养的值
*/
int get_role_train_attr_value(uint32_t train_type, uint32_t attr_type, uint32_t cur_attr, float max_attr, uint32_t role_level, bool is_asc)
{
	if (train_type < 1 || train_type > 5) {
		return 0;
	}

	int value = 0;
	int judge_value[][2] = { {10, 20}, {20, 35}, {30, 50}, {40, 65}, {100, 100}, };
	int asc_factor[] = {50, 50, 50, 50, 100};

	if (train_type == 5) {
		is_asc = true;
	}
	if (!is_asc) {
		if (cur_attr < max_attr * judge_value[train_type - 1][0] / 100) {
			is_asc = true;
		} else if (cur_attr < max_attr * judge_value[train_type - 1][1] / 100){
			int rand_asc_value = rand() % 100 + 1;
			is_asc = (rand_asc_value >= asc_factor[train_type - 1]);
		} else {
			is_asc = false;
		}
	}

	if (!is_asc) {
		int factor[] = {50, 20, 10, 7};//千分之几
		int tmp_value = cur_attr * factor[train_type - 1] / 1000;
		if (tmp_value == 0) {
			tmp_value = 1;
		}
		value = -(rand() % tmp_value + 1);
	} else {
		int add_value[][6] = {
			{10, 4, 2, 4, 2, 2},
			{16, 6, 4, 6, 4, 2},
			{24, 8, 6, 8, 6, 2},
			{32,10, 8,10, 8, 4},
			{32,10, 8,10, 8, 4}
		};
		int tmp_value = add_value[train_type - 1][attr_type - 1];
		if (train_type == 5) {
			value = tmp_value;
		} else {
			value = (rand() % tmp_value + 1);
		}
	}

	return (int)value;
}

/*
* @brief 计算练气是否暴击
* @param is_gold:是否使用黄金
* @return 是否暴击(0不暴击，1暴击)
*/
int is_role_upgrade_force_crit(uint32_t is_gold)
{
	int is_crit = 0;
	if (is_gold) {
		is_crit = ((rand() % 100) < 85) ? 1 : 0;
	} else {
		is_crit = ((rand() % 100) < 25) ? 1 : 0;
	}
	return is_crit;
}

/*
* @brief 计算得到装备强化说需要的yxb花销
* @param equipment_kind:装备阶级
* 		  level:装备等级
* @return 装备强化所需要的yxb
*/
int get_equipment_strengthen_cost(uint32_t equipment_state, uint32_t level)
{
	return uint32_t(500 * pow(level, 1.3 + equipment_state*0.04)) + 500;
}

/*
* @brief 计算装备能够洗练出来的属性个数
* @param equipment_id：装备ID
* @return 属性个数
*/
int get_equipment_train_attr_count(uint32_t equipment_id)
{
	int state = equipment_id % 100;
	if (state <= 2) {
		return 1;
	} else if (state <= 5) {
		return 2;
	} else if (state <= 9) {
		return 3;
	} else {
		return 4;
	}

	return 0;
}

/*
* @brief 计算能洗练出的最大属性值
* @param euqipment_id:装备ID
* 		  attr_type:属性类型
* @return 洗练的最大属性值
*/
int get_equipment_train_max_attr(uint32_t equipment_id, uint32_t attr_type)
{
	float value = 0;
	int state = equipment_id % 100;
	switch (attr_type) {
		case HP:
			//生命上限=INT(100*(装备阶级^1.22)/0.8)；
			value = 100 * pow(state, 1.22) / 0.8;
			break;
		case OUT_ATTACK:
			//外攻上限=INT(23*(装备阶级^1.23)/1.2)；
			value = 23 * pow(state, 1.23) / 1.2;
			break;
		case IN_ATTACK:
			//内攻上限=INT(23*(装备阶级^1.23)/1.2)；
			value = 23 * pow(state, 1.23) / 1.2;
			break;
		case OUT_DEFENCE:
			//外防上限=INT(10*(装备阶级^1.15)/0.4)；
			value = 10 * pow(state, 1.15) / 0.4;
			break;
		case IN_DEFENCE:
			//内防上限=INT(10*(装备阶级^1.15)/8)；
			value = 10 * pow(state, 1.15) / 8;
			break;
		case TOUGHNESS:
			//韧性上限=INT(8*(装备阶级^1.23)/0.5)；
			value = 8 * pow(state, 1.23) / 0.5;
			break;
		case CRIT:
			//暴击上限=INT(8*(装备阶级^1.23)/2)；
			value = 8 * pow(state, 1.23) / 2;
			break;
		case REDUCE_HURT:
			//减伤上限=INT(8*(装备阶级^0.8)/0.5)；
			value = 8 * pow(state, 0.8) / 0.5;
			break;
		//break;
		case ADD_HURT:
			//增强伤害上限=INT(8*(装备阶级^0.9)/0.5)；
			value = 8 * pow(state, 0.9) / 0.5;
			break;
		case RECOVER_HP:
			//回血上限=INT(8*(装备阶级^0.8)/1.2)；
			value = 8 * pow(state, 0.8) / 1.2;
			break;
		default:
			break;
	}
	return (int)value;
}

/*
* @brief 根据cd类型得到cd时间
* @param cd_type:cd类型
* 		  factor:传入的系数
* @return 得到CD时间
*/
int get_cooldown_time(uint32_t cd_type, uint32_t factor)
{
	switch (cd_type) {
		case STRENGTHEN_EQUIPMENT_CD:
			return (factor * 2 - 1) * 10;
		default:
			return 100;	
	}
}

/*
* @brief 弟子游历成功率判断
* @param partner_qualify:弟子资质
* 		  travel_type: 游历类型
*/
int is_partner_travel_succ(uint32_t partner_qualify, uint32_t travel_type)
{
	int is_succ = 0;
	if (partner_qualify > 30 || travel_type > 3) {
		return is_succ;
	}

	const uint16_t travel_rate[4][31] = {
		{800, 600, 400, 200, 100, 50, 10, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1000, 800, 600, 400, 300, 250, 210, 200, 200, 200, 200, 100, 100, 100, 100, 100, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50},
		{1000, 1000, 1000, 900, 800, 700, 600, 500, 400, 300, 300, 300, 300, 300, 300, 300, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
		{1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 900, 800, 700, 600, 600, 600, 600, 600, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300},
	};

	uint16_t rand_val = rand() % 1000 + 1;
	if (rand_val <= travel_rate[travel_type][partner_qualify]) {
		is_succ = 1;
	}
	return is_succ;
}

/*
* @brief 得到弟子一次银两游历的消耗
	公式：当次银两花费=当日银两提升次数*（弟子资质等级+1）*2000
* @param qualify:弟子资质
* 		  travel_count: 游历次数
* @return 返回弟子消耗的银两
*/
int get_partner_travel_cost(uint32_t qualify, uint32_t travel_count)
{
	return travel_count * (qualify + 1) * 2000;
}

/*
* @brief 竞技场排名奖励
*/
int get_arena_rank_prize(int32_t rank_id, uint32_t& prize_yxb, uint32_t& prize_prestige)
{
	if (rank_id <= 50) {
		prize_yxb = 505000 - (rank_id * 5000);
		prize_prestige = 301 - rank_id;
	} else if (rank_id <= 100)
    {
		prize_yxb = 300000 - (rank_id * 1000);
		prize_prestige = 301 - rank_id;
	} else if (rank_id < 2000)
    {
		prize_yxb = 210000 - (rank_id * 100);
		prize_prestige = ((301 - rank_id) > 20) ? (301 - rank_id) : 20;
	} else {
		prize_yxb = 10000;
		prize_prestige = 20;
	}

	return 0;
}

const uint32_t g_exchange_partner_list[] = { 60054, 60055};
const uint32_t* get_exchange_partner_list(uint32_t& exchange_count)
{
	exchange_count = sizeof(g_exchange_partner_list) / sizeof(g_exchange_partner_list[0]);
	return g_exchange_partner_list;
}

uint32_t get_expand_land_cost_yxb(uint32_t land_id)
{
	const uint32_t g_expand_cost[] = {120000, 250000, 450000, 735000, 1120000, 1620000, 2250000};
	return g_expand_cost[land_id - 3];
}

int cal_partner_inherit_qualify(uint32_t partner_id, uint32_t inherit_id, uint32_t cur_qualify, float factor, uint32_t* add_qualify)
{
	const uint32_t g_quarty_value[] = {10, 20, 40, 60};//资质点数
	const PartnerConf_t* pConfig1 = GameData::getPartnerConf(partner_id);
	const PartnerConf_t* pConfig2 = GameData::getPartnerConf(inherit_id);
	
	uint32_t inherit_qualify = g_quarty_value[pConfig1->nQuarty - 2] * cur_qualify;
	*add_qualify = (int)(inherit_qualify * factor / g_quarty_value[pConfig2->nQuarty - 2]);
	return *add_qualify;
}

uint32_t cal_buy_hammer_cost_gold(uint32_t buy_times)
{
	return 5 + (buy_times + 1) * 5;
}

uint32_t cal_compose_gem_req_rune_num(uint32_t new_gem_lv)
{
    static uint32_t req_rune_nums[10 - 1] = {
        1, 2, 3, 4, 5, 7, 8, 9, 10
    };
	return (new_gem_lv >= 2 && new_gem_lv <= 10) ? req_rune_nums[new_gem_lv - 2] : 999999; 
}

/*
 * @brief 增加活跃度
 * @return -1 类型错误，0 活跃度无实际增加，1 活跃度有增加
 */
int add_user_active_degree(uint32_t task_type, int count, uint8_t* active_degree)
{
	if (task_type == 0 || task_type > 60 || count == 0) {
		return -1;
	}
	uint8_t& state = active_degree[task_type - 1];
	const DegreeItem_t* pConf = GameData::getActiveDegreeItem(task_type);
	if (!pConf) {
		return -1;
	}
	if (state >= pConf->maxPoints) {
		return 0;
	} else {
		state += pConf->points * count;
		if (state > pConf->maxPoints) {
			state = pConf->maxPoints;
		}
		return 1;
	}
}

/*
 * @brief 计算活跃度
 */
uint32_t user_cal_active_degree(uint8_t* active_degree)
{
	uint32_t degree = 0;
	for (int i = 0; i < LIVENESS_MAX_TYPE - 1; i++) {
		if (active_degree[i] != 0) {
			degree += active_degree[i];
		}
	}
	return degree;
}

}
