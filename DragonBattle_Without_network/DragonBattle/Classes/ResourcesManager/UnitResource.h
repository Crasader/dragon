//
//  AnimateResouce.h
//  DragonBattle
//  角色 怪物资源 形象、动作配置
//  Created by 小敦 王 on 13-4-17.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_AnimateResouce_h
#define DragonBattle_AnimateResouce_h

#include "cocos2d.h"
#include <string>
#include <map>
#include "GlobalDefine.h"
#include "AnimationInfo.h"
#include "ThrowResource.h"
#include "XmlEncrypt.h"

// 玩家行为类型(动作类型) 
enum kAnimationType
{
    kAnimStandBy = 1,   //待机
    kAnimRun = 2,           //移动
    kAnimJumpUp = 3,        //
    kAnimJumping = 4,
    kAnimJumpDwon = 5,
    kAnimSkillOverDown = 6,
    kAnimAirHit = 7,
    kAnimGetUp = 19,
    kAnimRoll = 22,
    kAnimDrawWeapon,
    kAnimWin,
    kAnimLose,
};

enum kBeHitState
{
    kHeadBeHit  = 0,    //头部受击
    kBellyBeHit = 1,    //腹部受击
    kFlyState_1 = 2,    //被击飞状态1
    kFlyState_2 = 3,    //被击飞状态2
    kHitDownState = 4,  //被击倒
};

typedef std::map<kAnimationType, AnimationInfo_t*> AnimationInfoMap;
typedef std::vector<AnimationInfo_t*>  AnimationInfoVec;

// 普通攻击
struct AttackInfo_t
{
    AttackInfo_t()
    {
        attackAnimVec.clear();
        attactEffectVec.clear();
        attackHitVec.clear();
        nAttackOverType = 0;
        fAngrayTime = 0;
        nSharkType = -1;
        fScale = 1;
    }
    ~AttackInfo_t()
    {
    }
    
    float           fOutAttackCorrect;  //外攻修正
    float           fInAttackCorrect;   //内攻修正
    uint32_t        nAudioID;           //音效
    cocos2d::CCRect rcAttackRect;       //攻击区域
    float           fSpeedAddY;         //受击者y轴加速度
    float           hitRecover;         //硬直时间
    float           fMoveX;             //自身移动距离
    float           fSpeedX;            //X方向移动速度
    float           fSpeedY;            //Y方向移动速度
    float           fAngrayTime;        //霸体时间，如果为0，则没有触发霸体，如果为不为0，则为霸体效果时间
    int             nUnderAttackIndex;  //受招的动画类型
    int             nAttackOverType;    //受招类型，0允许接普通攻击和技能，1允许接技能，2不允许接任何招式
    int             isAngry;
    AnimationInfoVec attackAnimVec;    //普通攻击
    AnimationInfoVec attactEffectVec;  //普通攻击特效 (剑气)
    AnimationInfoVec attackHitVec;     //攻击到对方身上的特效
    
    kThrowMode         nThrowMode;     //抛出物类型
    int                nThrowintervalTime;//抛出物间隔时间 仅kThrowSequence有效
    std::vector<uint16_t>  throwVec;//抛出物
    
    std::string     music;          //音效
    int             nSharkType;     //0起招第一帧开始震动1有效真第一帧，2碰撞
    int             nAmplitude;     //振幅高度
    float           fSharkTime;     //时间
    int             nSharkCount;    //次数
    bool            isUniform;      //是否匀速0匀速1减速
    
    bool            isBlinkScreen;  //攻击播放时是否要闪屏
    
    bool            isFallDown;         //是否击倒
    
    float           fScale;         //特效缩放比例缩放比例
};
typedef std::vector<AttackInfo_t*> AttackInfoVec;

struct SkillInfo_t
{
    AttackInfoVec attackVec;
    std::string strSkillIconEmpty;
    std::string strSkillIconFull;
    std::vector<std::string> fileResourceVec;//资源文件
    uint32_t     nID;
};
typedef std::map<int,SkillInfo_t*> SkillInfoMap;

struct UnitResource_t
{
    uint32_t            ID;             //单位ID
    std::vector<std::string> fileResourceVec;  //资源文件
    std::string         strFile;        
    std::string         headImage;      //头像
    std::string         strRoleFile;
    std::string         strmusic;       //被打音效
    std::string         strDiemusic;    //死亡音效
    float               fMoveSpeed;
    float               fJumpSpeed;     //初始起跳速度
    float               fScale;         //放大比例
    cocos2d::CCSize     contentSize;    //(受击区域大小)
    cocos2d::CCPoint    offPos;         //(角色图片中心点与受击区域中心点的偏移)
    
    AnimationInfoMap    animMap;        //动作帧信息
    AttackInfoVec       attackVec;       //攻击招式
    SkillInfoMap        skillMap;
    AttackInfoVec       airAttack;      //空中攻击
    AnimationInfoVec    underAttacks;   //被攻击
};

// std::pair<ID,动画>
typedef std::map<uint32_t, UnitResource_t*>    UnitResourceMap;

class UnitResource
{
public:
    UnitResource();
    ~UnitResource();
    
    static UnitResource* sharedInstance();
    static void purgeInstance();
    
    bool loadUnitResouceFromFile();
    bool loadUnitResource(xmlNodePtr chl,std::vector<std::string>* pFileVec);
    bool loadUnitSkillResource(xmlNodePtr chl,std::vector<std::string>* pFileVec);
    cocos2d::CCAnimation*   createAnimation(uint32_t unitID, kAnimationType animType);
    cocos2d::CCAnimate*     createAnimate(uint32_t unitID, kAnimationType animType);
    cocos2d::CCAnimation*   createAnimation(uint32_t unitID, AnimationInfo_t* pInfo);
    cocos2d::CCAnimate*     createAnimate(uint32_t unitID, AnimationInfo_t* pInfo);
    
    cocos2d::CCAnimation*   createAnimation(AttackEffectInfo_t* pInfo);
    cocos2d::CCAnimate*     createAnimate(AttackEffectInfo_t* pInfo);
    
    cocos2d::CCSprite*      getUnitSprite(uint32_t unitID);
    UnitResource_t*         getUnitResConf(uint32_t unitID);

private:
    bool loadAttackAction(xmlNodePtr chl, AttackInfoVec* pAttackVec);

private:
    static UnitResource* pSharedInstance;
    
    UnitResourceMap m_unitResourceMap;   //角色资源
};

#endif
