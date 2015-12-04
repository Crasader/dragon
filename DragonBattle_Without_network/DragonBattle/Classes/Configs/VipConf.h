//
//  VipConf.h
//  DragonBattle
//
//  Created by zeng xiaolong on 13-10-21.
//
//

#ifndef __DragonBattle__VipConf__
#define __DragonBattle__VipConf__

#include "XmlEncrypt.h"
#include "ItemConf.h"
#include <map>
#include <vector>

#define VIPFUNCTIONCOUNT 30

#define VIPNEEDGOLD         0//vip所需消耗黄金
#define VIPGROWGIFT         1//VIP成长礼包
#define DAYVIPGIFT          2//每日可领取VIP礼包一次
#define DAYBUYENERY         3//每日可购买体力
#define DAYBUYHAMMERNUM     4//每日砸罐可额外购买锤子数量
#define BUYTASKNUM          5//悬赏任务黄金购买次数
#define GETJOURNEYNUM       6//可领取盘缠次数
#define GOLDEXCHANGEYXBNUM  7//黄金换银票次数
#define BUYSPORTSNUM        8//竞技场可额外购买竞技次数
#define RECHOOKEXP          9//每日最多可领取XX小时离线                                          挂机经验
#define GOLDSTRENGTHENTRAIN 10//开启5黄金加强培养
#define GOLDPLATINUMTRAIN   11//开启10黄金白金培养
#define GOLDDIAMONTRAIN     12//开启钻石培养
#define GOLDEXTREMETRAIN    13//开启200黄金至尊培养
#define REFRESHELITEMONSTER 14//每日精英怪副本50黄金刷新1次
#define CANGJINGGE20GOLD    15//藏经阁20黄金卜卦功能
#define CANGJINGGE100GOLD   16//藏经阁100黄金卜卦功能
#define VILLAUPGRADE        17//山庄可花银两升级土地，收益增加，cd时间减少
#define SHOPDISCOUNTS       18//商城礼包打折优惠
#define STRENGTHENLOCK      19//强化洗练开启黄金锁定功能
#define ACCELERATECLIMB     20//开启快速爬塔功能
#define ACCELERATERAID      21//开启扫荡免黄金快速完成功能
#define NERVEGOLD           22//开启经脉花费黄金失败不扣真气功能
#define MIDDLEWORSHIP       23//开启每日竞技场中级膜拜功能
#define HIGHWORSHIP         24//开启每日竞技场高级膜拜功能
#define INFORCEPRACTICE     25//开启炼气批量修炼功能

class VipConf
{
public:
    VipConf();
    ~VipConf();
    bool loadFromFile(char* filePath, bool bEncrypt = false);
    int getVipFunction(int viplv,int FunctionID);
    int getVipLvByCostGold(int cost_gold);
#ifndef WONPEE_SERVER
	char* getDes(int vipLv) { return des[vipLv]; }
    char des[13][2048];
#endif

private:
    int vipLvAndFunction[13][50];
};

#endif /* defined(__DragonBattle__VipConf__) */
