//
//  SwapItemConf.h
//  DragonBattle
//  兑换配置表 和 随机配置表
//  Created by alexhe on 13-5-28.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_SwapItemConf_h
#define DragonBattle_SwapItemConf_h

#include "ItemConf.h"
#include "XmlEncrypt.h"
#include <string>
#include <vector>
#include <map>

typedef std::vector<ItemUnit_t> ItemUnitVec;

typedef struct {
    uint32_t nItemID;
    uint32_t nItemCount;
    float    fRandStart;
    float    fRandEnd;
} RandItemUint_t;

typedef std::vector<RandItemUint_t> RandItemOutVec;

typedef struct {
    uint32_t nEntryID;	//兑换ID
    uint8_t  nSwapType;	//兑换类型(1固定、2随机)
    uint8_t  nShowItemType;//UI显示类型(1.物品直接飞出2.弹出奖励面板)
 	uint8_t	 nCliReq;	//是否client主动请求swap协议
    uint32_t nMulSet;	//倍数限制
    uint32_t nDateStart;//开始日期
    uint32_t nDateEnd;	//结束日期
    uint32_t nLimitType;//限制类型
    uint32_t nLimitCount;//限制次数
    uint32_t nLevel;    //等级限制
    std::string desc;   //描述
    
    ItemUnitVec 	itemIn;
    ItemUnitVec     swapItemOut;
    RandItemOutVec  randItemOut;
} SwapItem_t;

typedef std::map<uint32_t, SwapItem_t*> SwapItemMap;

class SwapItemConfig
{
public:
    SwapItemConfig();
    ~SwapItemConfig();
    
    bool loadFromFile(const char* filePath, bool bEncrypt = false);
    void unload();
    
    const SwapItem_t* getSwapItem(uint32_t swapID) const;
    
private:
    SwapItemMap m_swapItemMap;
};

#endif
