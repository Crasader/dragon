//
//  BattleUnitSprite.h
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-18.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_BattleUnitSprite_h
#define DragonBattle_BattleUnitSprite_h

#include "cocos2d.h"
#include "Resources.h"
#include "GameData.h"
#include <time.h>
#include "NotificeDefine.h"
USING_NS_CC;
class BattleMapUnits;
class MapObject;
class ThrowSprite;

class BattleUnitSprite : public CCNode
{
public:
    BattleUnitSprite();
    virtual ~BattleUnitSprite();
    
    //static BattleUnitSprite* create(const UnitBaseConf* pInto);
    
    
    // 初始化
    virtual bool init(UnitBaseConf *info);

    //sprite Action
    virtual void goForward();   //前进
    virtual void goBack();      //后退
    virtual void stopRunning(); 
    
    //跳跃
    virtual bool moveJump();
    virtual bool roll();
    //回调
    virtual void jumpUpDone();
    
    //外部调用
    //许翔：增加招式连击，暂时用不到
    virtual void comboBegin(int nCombo);     //地面连击攻击起招
    virtual void attackBegin();     //地面攻击起招
    virtual void statueAttackBegin(bool isToRight);     //打斗
    virtual void statueAttackValid(){}
    //用来回调
    virtual void attackValid();     //地面攻击有效
    virtual void attackValidEnd();  //地面攻击有效结束
    virtual void resetStandby();    //地面攻击收招
    virtual void getupEnd();        //起身后动作
    virtual void attackOverEnd();
    virtual void addcombo() {}
    virtual bool skillAttack(int index);
    virtual void onDie(bool* pIsToRight) { this->unscheduleAllSelectors();}
    virtual void updateShadow(BattleMapUnits* pMapUnitLayer);
    
    void setUnderAttackIndex(uint32_t nIndex) { m_iUnderAttackIndex = nIndex; }
    
    void underAttack(BattleUnitSprite* pUnit, AttackInfo_t* pAttack, float fHurt, bool isCrit);
    void underAttack(bool isToRight, AttackInfo_t* pAttack, float fHurt, bool isCrit);
    void changeUnderAttackStatue(bool isToRight);
    virtual void onBeHit(bool bIsToRight);
    
    virtual bool underOjbectAttack(MapObject* pObject);
    virtual void attackValidRole(){}//打到人
    virtual void unAttackValidRole(){}//被人打到
    // 用来回调，倒地结束
    virtual void resetFallOver();
    //角色死亡
    virtual void unitDead();
    
    void forceMoveBy(CCPoint moveBy) { m_forceMoveBy = moveBy; }
    
    kRoleActionType getCurAction() { return m_nCurAction; }
    void  setCurAction(kRoleActionType act){m_nCurAction = act;}
    kObstacleDirection getCurObstacleDirection() { return m_nCurObstacleDirection; }
    
    const UnitResource_t* getUnitResource() { return m_pUintInfo; }
    UnitBaseConf*   getUnitBaseConf() { return &m_unitConf; }
    
    AttackInfo_t*   getCurAttack() { return m_pCurAttack; }
    const SkillConf_t* getCurSkillConf() { return GameData::getSkillConf(GET_SKILL_ID(m_nAttackSkill)); }
    
    CCPoint getUnitOffPos();
    CCSize getUnitSize();
    cocos2d::CCPoint getScreenPosition();
    cocos2d::CCPoint getFootPosition();
    // 得到单位中心点的坐标（可能与偏中心点不一致）
    cocos2d::CCPoint getCenterPosition();
    // 设置单位中心点
    void setCenterPosition(cocos2d::CCPoint pos);
    
    // 当前招式的攻击区域，相对sprite
    cocos2d::CCRect getLocalAttackRect();
    // 当前招式的攻击区域，相对layer
    cocos2d::CCRect getAttackRect();
    // 受击区域，相对于sprite
    cocos2d::CCRect getLocalUnitRect();
    // 受击区域，相对于layer
    cocos2d::CCRect getUnitRect();
    bool isFlipX();
    //阴影
    CCSprite* m_pSprShadow;
    // 是否被pUnit攻击
    bool hasUnderAttacked(BattleUnitSprite* pUnit);
    
    // pUnit攻击结束，从buff中删除
    void clearUnserAttacked(BattleUnitSprite* pUnit);
    
    CC_SYNTHESIZE(int, m_nHurtHp, HurtHp);
    CC_PROPERTY(int, m_nHp, Hp);
    virtual void addHp(int hp);
    void        resetHp() { m_nHp = m_unitConf.getMaxHP(); }
    void        reset();
    bool        canUnderAttack();//状态是否能被攻击
    int         getLV() { return m_nLV; }
    int         nRecoverHpTime;//20秒回一次血
    
    int         m_nBuffSkill;       //当前buff技能
    int         m_nBuffTime;
    int         m_nAttackSkill;    //当前攻击技能
    int         getCurDirection() { return m_nCurDirection; }
    int         getUnitType() { return m_unitConf.nUnitType;}
    
    bool        isAIActivated() { return m_bAIActivated; }
    void        setAIActivated(bool bVal) { m_bAIActivated = bVal; }
    void        setAngryTime(float fAngryTime) { m_fAngryTime = fAngryTime; }
    
    // 得到障碍方向
    kObstacleDirection getObstacleDirection(float moveX, float moveY, float* pCorrectX, float* pCorrectY);
    
    // 更新
    virtual void update();
    
    // AI
    virtual void runAIOnce(BattleUnitSprite* pOhter);
    virtual void setFlipX(bool bFlipX);
    
    // 设置受击动画索引
    virtual int getUnderAttackIndex();
    
    // 这个changeAction不需要用virtual
    bool changeAction(kRoleActionType action);
    // 抛出物体
    virtual void throwObject();
    
    // 伤害数值动画
    void underAttackHarmAction(int nHarm, bool isCrit = false, int nBonus = 0);
    
    // 被攻击特效
    void playUnderAttackEffect(AttackInfo_t* pAttack);
    
    //许翔：角色问互相攻击的行为一致，故添加该方法
    //     pAttackInfo应该可以不用传递，暂时先加上这个参数，以后再验证
    bool attack(BattleUnitSprite* pSprite, AttackInfo_t* pAttackInfo);
    int  getAttackDis(){return m_nAttackDistance;}
    void angryOver(float dt);
protected:
    void resetColor(float dt);
    virtual int correctDamage(AttackInfo_t* pAttack, float fHurt);
    virtual bool changeAction(kRoleActionType action, bool isToRight);
    bool changeAction(kRoleActionType action, kRoleDirectionType direction);
    static float calculateHurt(int attackBuffSkill,int underAttackBuffSkill,UnitBaseConf* pUnitConf,AttackInfo_t* pAttackInfo,int skillconfID,UnitBaseConf* pUnderAttackConf,bool &bCrit);
    
    // 计算每一帧单位移动的距离, 每一帧只计算一次
    cocos2d::CCPoint updateMovePt();
    
    // 增加招式连击
    virtual void addCombo(const AttackInfoVec* infoVec, int nCombo = 1);
    bool addSkillCombo(const SkillInfo_t* skill);
    void playAttack(AnimationInfo_t* animationInfo, int sharkType);
    void playAttackEffect();    //攻击特效,刀光剑影
    
    // 被地图中其他物体攻击特效
    void playOtherAttackedEffect(float dt);
    
    void updateAngryTime(float dt);
    void updateRecoverHp(float dt);
    float getAttackDistance();//获取攻击距离
    void resetAddSpeed(float dt);
protected:
    CCSprite *m_UnitSprite;
    int     m_nAttackDistance;  //攻击距离
    int     m_iSkillIndex;
    int     m_iSkillHitIdx;
    int     m_iUnderAttackIndex;
    float   m_fHitRecover;
    
    float   m_fSpeedX;  //被攻击后的X位移
    float   m_fSpeedY;  //被攻击后的Y速度
    float   m_fAddSpeedY;//y轴加速度
    float   m_fAddSpeedX;//被攻击后的X加速度
    
    UnitBaseConf    m_unitConf;
    const UnitResource_t* m_pUintInfo;
    //int             m_nExp;             //经验值
    int             m_nLV;              //等级
    
    // 当前行为
    kRoleActionType     m_nCurAction;
    kRoleDirectionType  m_nCurDirection;
    kObstacleDirection  m_nCurObstacleDirection;
    
    bool            m_bAIActivated;     //AI是否开启
    
    // 每一帧的坐标移动量
    cocos2d::CCPoint    m_movePt;       //速度
    cocos2d::CCPoint    m_moveAddPt;    //加速度
    cocos2d::CCPoint    m_forceMoveBy;  //强制更新位置
    
    int             m_nComboIdx;
    AttackInfoVec   m_pComboBuff;       //招式连击
    AttackInfo_t*   m_pCurAttack;       //当前出招
    float           m_fAngryTime;       //霸体时间
    int             m_nFlashTimes;      //闪烁次数
    std::map<BattleUnitSprite*, AttackInfo_t*> m_pCurHit;      //当前受到的攻击
protected:
    cocos2d::CCRepeatForever* m_pStand;
    cocos2d::CCRepeatForever* m_pGoForward;
    cocos2d::CCActionInterval* m_pRoll;
    cocos2d::CCActionInterval* m_pGetUp;
    cocos2d::CCActionInterval* m_pJumpUp;
    cocos2d::CCActionInterval* m_pJumping;
    cocos2d::CCActionInterval* m_pJumpDown;
    cocos2d::CCActionInterval* m_pSkillOverDown;
    cocos2d::CCArray* m_pUnderAttacks;
    CC_SYNTHESIZE(int,m_nBeHitCount, BeHitCount);
    CC_SYNTHESIZE(int,m_nCurrentBeHitCount, CurrentBeHitCount);
    // for test
public:
    uint32_t m_nCurFrame;
    void setAttacRectVisible(bool bShow);
    cocos2d::CCLayerColor* m_pSelfRectLayer;
    cocos2d::CCLayerColor* m_pAttackRectLayer;
    cocos2d::CCLayerColor* m_pBeHitRectLayer;
};

#endif
