//
//  ThrowResource.h
//  DragonBattle
//
//  Created by 永翔 何 on 13-5-6.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_ThrowResource_h
#define DragonBattle_ThrowResource_h

#include "cocos2d.h"
#include "AnimationInfo.h"
#include "XmlEncrypt.h"

// 抛出物抛出方式
enum kThrowMode
{
    kThrowNull = 0, 
    kThrowSequence,             //间隔抛出多个，如果只有一个抛出物，也选择这个类型
    kThrowMultiOnceTime,        //同时抛出多个
};

struct ThrowResource_t
{
    uint32_t nID;
    cocos2d::CCPoint ptCollideStart;    //碰撞偏移量
    cocos2d::CCPoint ptExplodeStart;    //爆炸偏移量
    cocos2d::CCSize  contentSize;       //碰撞区域
    cocos2d::CCSize  explodeSize;       //爆炸大小
    
    
    int             nAttackCount;        // 每次攻击，播放伤害特效的次数
    float           fHitRecover;         //硬直时间
    int             nUnderAttackIndex;  //受招的动画类型
    float           fScale;             //抛出物放大倍数
    
    AttackEffectInfo_t* pCreateAction;  //抛储物创建动画
    AttackEffectInfo_t* pMoveAction;    //移动
    AttackEffectInfo_t* pAttackAction;  //攻击
};

typedef std::map<uint32_t, ThrowResource_t*> ThrowResourceMap;

class ThrowResource
{
public:
    ThrowResource();
    ~ThrowResource();
    
    static ThrowResource* sharedInstance();
    static void purgeInstance();
    
    bool initAllThrowResource();
    
    cocos2d::CCAnimation* createAnimation(AttackEffectInfo_t* pInfo);
    cocos2d::CCAnimate* createAnimate(AttackEffectInfo_t* pInfo);
    
    ThrowResource_t* getConf(uint32_t unitID);
private:
    AttackEffectInfo_t* parseAction(xmlNodePtr nodeChl);
    
    static ThrowResource* pSharedInstance;
    
    ThrowResourceMap m_throwResourceMap;   //角色资源
};

#endif
