//
//  BattleMapUnits.h
//  DragonBattle
//  地表层 所有物品（地图中功能道具 掉落物品等 怪物 BOSS）
//  Created by 小敦 王 on 13-4-22.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_BattleMapUnits_h
#define DragonBattle_BattleMapUnits_h

#include "cocos2d.h"
#include "BattleLayer.h"
#include "UnitContainer.h"
#include "MonsterContainer.h"
#include "TrapContainer.h"
#include "ThrowSprite.h"
#include "MapObstacle.h"
#include "ComboControl.h"
#include <vector>
#include "MovieClip.h"
#include "BattleMap.h"
#include "HeroSprite.h"
//地图地表层 
enum kLandBgOrder 
{
    kFarLayerOrder = 0,
    kMidLayerOrder,
    kBackgroudOrder,    //背景
    kBlackMaskOrder,    //黑色遮罩
    kBoxUnitOrder,      //盒子等物品 
    kMonsterOrder,  //小怪
    kThiefOrder,  //小怪
    kBossOrder,     //BOSS
    kPartnerOrder,  //侠客层
    kRoleOrder,     //主角
    kAttactEffectOrder, //攻击特效
    kHurtEffectOrder,   //受击特效
    kThrowUnitOrder,
    kHitTipOrder,
    kNearLayerOrder,
    kLandBgOrderMax,
};

enum NodeInLayerTag
{
    MapSharkActionTag = 101,
};

typedef std::vector<ThrowSprite*> ThrowSpriteVec;
typedef std::vector<MapObstacle*> MapObstacleVec;
typedef std::vector<MonsterUnitConf*> FollowMonsterInfoVec;

class BattleSubBGNode;
class DropItem;

class BattleMapUnits : public cocos2d::CCLayer,public MovieOver
{
public:
    BattleMapUnits();
    ~BattleMapUnits();
    
    bool init(MissionConf_t* pMissionConf);
    
    // implement the "static node()" method manually
    static BattleMapUnits* create(MissionConf_t* pMissionConf);
    
    virtual void onExit();
    
    // 地图长
    float getMapLenght() { return m_fMapLength; }
    
    // 战斗场景背景
    void  createBg();
    
    void onMovieOver(bool isOver);

    // 添加角色sprite,进入游戏时候设定
    void  addSelf(HeroSprite* pSprite);
    
    void createPartner(float dt);
    
    // 每帧操作 重新定位地表层坐标
    void  battleMapUpdate(float dt);
    
    // monster死亡后倒地动画结束
    void removeBattleUnit(BattleUnitSprite* pUnit);
    
    // monster死亡后加到角色上的经验值
    void addSelfExp(int nID, int nDropExp);
    
    //关卡结束后，所有掉落物飞向人物头像
    void collectAllItems();
    
    // 加上掉落的物品
    void addDropItem(std::vector<itemDrop_t>* pDropItemVec, CCPoint position, bool bIsThief);
    void addDropItem(int nItemId, int nItemCount, CCPoint position, bool bIsThief);
    void setBattleState(kBattleState state)
    {
        m_nBattleState = state;
    }
    kBattleState getBattleState() { return m_nBattleState; }     //当前战斗场景的状态
    
    // 地图震动
    void mapShark();
    
    // 判断是否胜利
    void checkWin(BattleUnitSprite* pSprite);
    
    kObstacleDirection checkDirection(cocos2d::CCRect unitRect, float moveX, float moveY, float* pCorrectX, float* pCorrectY);
    
    //得到离某个点最近的怪物或可攻击的障碍物
    BattleUnitSprite* getNearestMonster(BattleUnitSprite* unitSprite,float fdistance,bool bInScreen);
    
    // get Hero
    BattleUnitSprite* getHero() const { return m_pSelf; }
    
    // 添加抛出物
    void addThrowSprite(AttackInfo_t* pAttackInfo, int pSkillConfIDLV,
                        UnitBaseConf* m_pUnitBaseConf,
                        ThrowAttackConfig_t* throwAttackConfig,
                        BattleUnitSprite* pUnitSprite, int nDir,CCPoint pt);
    void removeThrowSprite(ThrowSprite* pThrow);
    
    // 用于回调，判断是否刷怪
    void showGoForwardsTip(float dt);
    
    // 得到关卡结算：银两、经验、道具列表
    inline std::map<int, int>* getDropItemMap() { return &m_dropItemMap; }
    inline int getExp() { return m_nGetExp; }
    inline int getYxb() { return m_nGetYxb; }
    inline void addYxb(int nYxb) { m_nGetYxb += nYxb; }
    inline int getGold() { return m_nGetGold; }
    
    void setConfig(std::vector<item_unit_t>* pItemVec, std::vector<item_unit_t>* pThiefItemVec);
    
    void battleWin(float dt);
    void showWinPanel();
    void showLosePanel(float dt);
    void onExitGame();
    MissionConf_t*      m_pMissionConf; //关卡信息
    //获取杀死的怪物信息，传入怪物数组中
    void getKillMonsters(uint32_t* pMonster);
    
    void limitExp();
    BattleUnitSprite* createEmeryHero(UnitBaseConf *info,uint32_t *skill);
    BattleUnitSprite* createWorldBoss(UnitBaseConf *info);
    int getBeHitCount(){return m_pSelf->getBeHitCount();}
private:
    void updateComboControlPos();
    void updateGoForwardsTipPos();
    void heroAndLandbgUndate(float dt);
    
    bool isInterSect(cocos2d::CCRect unitRect, float moveX, float moveY, float* pCorrectX, float* pCorrectY, kObstacleDirection* pObstacleDirection);
    
    // 每帧计算场景中所有单位的情况
    void  mapUnitsUpdate(float dt);
    
    // 背景移动
    void  resetBackGroud();
    
    void showDropItemTip(DropItem* pItem);
    
    //创建障碍物
    void createObstacle();
    
    // 怪物更新
    void updateMonsters(float dt);
    
    void updateFriend(float dt);
    
    // 计算攻击
    void onUnitAttack(BattleUnitSprite* pUnit);
    void onThrowAttack();
    
    // 判断抛出物是否与地面碰撞
    bool isThrowSpriteCollideObstacle(ThrowSprite* pSprite, cocos2d::CCRect& explodeRect);

    // 重新设置卡屏点 bJustRight为true，只设置右边的点，等过了点后在卡左边点 false为两边的点都设置
    void resetClipStageBothSide(bool bBoth = true);
    
    // 判断是否到达出怪点
    void reachStageBornPos();
    
    // update 中场景出怪的AI
    void judgeClipAndBornMonster();
    
    bool judgeCreateMonster();
    void caculateMonsterAttr(UnitBaseConf *conf);
    // 在坐标点刷新怪
    void createMonster();
    
    // 创建随从怪，出生在屏幕左右
    void createFollowMonster();
    
    void updateThrowSpritePos();
    
    //检测是否捡到掉落物
    void onGetDropItem();
    //更新掉落物的位置
    void updateDropItemPos();
    
    int getDropItemCount(int nItemID, int nItemCount);
    
    void addToItemMap(DropItem* pItem);
    // 创建弟子

private:
    BattleMap*          m_pMapBg;          //地图背景移动层
    MissionStage_t*     m_pCurMissionStage;//当前刷怪信息
    std::vector<cocos2d::CCLabelTTF*> m_vName;
    bool                m_bShowGoForwardsTip;
    HeroSprite*         m_pSelf;
    PartnerSprite*      m_pPartner;
    uint32_t            m_nCurPartnerIdx;
    kBattleState        m_nBattleState;     //当前战斗场景的状态
public:
    UnitContainer       m_friendContainer;
    MonsterContainer    m_monsterContainer; //存放怪物的容器
    TrapContainer       m_trapContainer;    //存放陷阱的容器
    
    ThrowSpriteVec      m_throwSpriteVec;        //抛出物
    MapObstacleVec      m_mapObstacleVec;        //障碍物
    
    cocos2d::CCSprite*           m_pGoForwardsTip;
    ComboControl*                m_pComboControl;
    
    FollowMonsterInfoVec  m_deadMonsterVec; //死亡随从怪保存配置
    uint8_t  m_nCurMissionStageIdx;         //在第几个卡频点，从0开始，当前关卡的第几屏
    int      m_nCurWaveIdx;                 //当前屏的第几波怪物
    uint16_t m_nCurStageBornCnt;            //当前屏刷出的普通怪数量
    bool     m_bHasBronFristMonster;        //当前屏第一波摆放怪是否生成
    bool     m_bHasFristMonsterAIStart;     //第一波摆放怪AI是否开启
    int      m_nKillCnt;                    //杀死关键怪物的数量
    bool     m_bStopCreateMonster;          //胜利或失败不刷新怪
    
    int      m_nBornMonsterFrequency;       //怪物刷新的频率
    int      m_nBornFollowMonsterFrequency; //随从刷新时间
    
    bool     m_bClipBothSide;               //当前屏左右都卡屏
    bool     m_bPosOnRightClip;             //过一小关时，英雄是否走过右边的卡屏点进入下一屏
    bool     m_bGotBornPos;                 //是否到达刷怪点
    
    float    m_fLeftClipX;                  //左边的卡屏X坐标
    float    m_fRightClipX;                 //右边的卡屏X坐标
    
    float    m_fToY;                        //角色跳到不同的高度时，需要调整的目标Y坐标
    float    m_fFromY;                      //角色跳到不同的高度时，需要调整的起始Y坐标
    float    m_fDeltaY;                     //每帧Y坐标调整的值
    
    float    m_fMapLength;                  //地图长度
    int      m_fightTime;
    int getFightTime()
    {
        return m_fightTime/60;
    }
    CC_SYNTHESIZE(bool, m_isStop, isStop);  //暂停
    CCArray* m_pDropItemArray;              //掉落物精灵数组
public:
    std::map<int, int> m_dropItemMap;       //掉落物缓存
    std::map<int, int> m_thiefItemMap;      //拾取的小偷物品
    std::map<int, int> m_dieMonsterMap;     //死亡怪物记录
    int m_nGetExp;                          //经验缓存
    int m_nGetYxb;                          //掉落银两缓存
    int m_nGetGold;                          //掉落黄金缓存
    uint32_t m_nStartTime;                  //开始时间
    uint32_t m_nBattleKey;                  //加密Key
    std::vector<item_unit_t> m_ItemLimitVec;//掉落物限制向量
    std::vector<item_unit_t> m_ThiefItemVec;////小偷掉落物品向量
    
    //显示区域，测试用
public:
    void setAttacRectShow(bool bShow);
    void setMonsterCountTips();
    cocos2d::CCLabelTTF*         m_pMonsterCountTips;
};

#endif
