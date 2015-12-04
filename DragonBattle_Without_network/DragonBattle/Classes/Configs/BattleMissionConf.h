//
//  BattleMission.h
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-16.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_BattleMission_h
#define DragonBattle_BattleMission_h

#include "BattleUnitConf.h"
#include "MapUnitConf.h"

#include "XmlEncrypt.h"

#include <map>
#include <vector>
#include <string>
#include "MonsterConf.h"
#include "ItemConf.h"


/* 关卡ID通用规则: XYYZZ
 * X(万位 代表关卡类型: 0 主线, 1 精英, 2 爬塔, 3 困难, 9 特殊活动) (注：趣味关卡属于主线)
 * YY(千位百位, 代表社区id, 01-99) (注：爬塔没有社区id)
 * ZZ(末两位，代表在社区内的编号,01-99)
 */
#define MISSION_CMT_BASE_NUM 100	//关卡社区基数
#define MISSION_TYPE_BASE_NUM  10000 //关卡类型基数
enum MissionType {
	MAIN_MISSION = 0,//主线关卡
	ELITE_MISSION = 1,//精英关卡
	TOWER_MISSION = 2,//爬塔关卡
	HARD_MISSION = 3,//困难关卡
	SPECIAL_MISSION = 9,//特殊关卡，如世界boss活动关卡,新手关卡等
};

//由精英关卡id得到对应的普通关卡id,如10103-->103
#define GetNormalMissionOfElite(eliteMissionID) ((eliteMissionID) - MISSION_TYPE_BASE_NUM) 

#define MinMainMissionId (MAIN_MISSION * MISSION_TYPE_BASE_NUM + MISSION_CMT_BASE_NUM)	//精英关卡id>100
#define MaxMainMissionId ((MAIN_MISSION + 1) * MISSION_TYPE_BASE_NUM)//精英关卡id<10000
#define MinEliteMissionId (ELITE_MISSION * MISSION_TYPE_BASE_NUM + MISSION_CMT_BASE_NUM)	//精英关卡id>10100
#define MaxEliteMissionId ((ELITE_MISSION + 1) * MISSION_TYPE_BASE_NUM)//精英关卡id<20000
#define MinHardMissionId (HARD_MISSION * MISSION_TYPE_BASE_NUM + MISSION_CMT_BASE_NUM)	//精英关卡id>30100
#define MaxHardMissionId ((HARD_MISSION + 1) * MISSION_TYPE_BASE_NUM)//精英关卡id<40000
#define MinHardMission 20000	//爬塔关卡id>20000
#define MaxHardMission 30000	//爬塔关卡id<30000

//是否是主线关卡(含趣味奖励关卡)
#define IsMainMission(missionId) ((missionId) > MinMainMissionId && (missionId) < MaxMainMissionId)
//是否是精英关卡
#define IsEliteMission(missionId) ((missionId) > MinEliteMissionId && (missionId) < MaxEliteMissionId)
//是否是困难关卡
#define IsHardMission(missionId) ((missionId) > MinHardMissionId && (missionId) < MaxHardMissionId)
//是否是爬塔关卡
#define IsTowerMission(missionId) ((missionId) > MinTowerMissionId && (missionId) < MaxTowerMissionId)

// 战斗场景中远景 中景 地面 近景的图片信息
struct MissionBG_t
{
    MissionBG_t()
    {
        isFlipX = false;
        fBgPosX = 0;
    }
    std::string strBgName;
    bool        isFlipX;
    float       fBgPosX;
    float       fBgPosY;
};
typedef std::vector<MissionBG_t*> MissionBGVec;

// 获胜条件
struct WinCondition_t
{
    uint32_t     nWinType;           //2杀怪数 4杀boss 
    uint32_t     nKillCnt;           //杀死的数量
};

struct ScoreRule_t
{
	uint32_t	nMaxCombo;			//最大连击数
	uint32_t	nBeHitCount;		//受击次数
	uint32_t	nBattleTime;		//战斗时间(秒)
};

class MonsterUnit
{
public:
    MonsterUnit()
    {
        nMonsterID = 0;
        isRight = 1;
        fBasePosY =0;
        nBornType=0;
    }
    ~MonsterUnit(){}
    uint32_t nMonsterID;
    uint32_t isRight;           //出生位置 0左 1右 2随机
    float    fBasePosY;         //基础Y出生点
    uint8_t  nBornType;         //刷怪类型，0摆放刷怪 1自动刷怪 2跳跃刷怪 3固定位置刷怪
    cocos2d::CCPoint    pos;    //固定位置（仅nBornType=3有效）
    float    fBasePosX;         //摆放怪特有
};

typedef std::vector<MonsterUnit> MonsterBornInfoVec;
typedef std::vector<MonsterBornInfoVec> MonsterBornVec;

// 关卡阶段
struct MissionStage_t
{
    float   fClipScreenPosX;    //卡屏点
    float   fClipScreenLength;  //卡屏长度
    float   fRefreshTime;       //小怪刷新时间
    MonsterBornVec monsterBorn;
};

typedef std::vector<MissionStage_t> MissionStageVec;

struct Obstacle_t
{
    uint16_t            nResID;
    cocos2d::CCPoint    ptPosition;
    float               fWidth;
    float               fHeight;
};

typedef std::vector<Obstacle_t>  ObstacleVec;

typedef std::vector<TrapUnit> TrapUnitVec;

struct MonsterCountInfo_t
{
    uint32_t nMonsterID;
    uint32_t nMonsterCount;
};

// 战斗关卡信息
struct MissionConf_t
{
    uint32_t     nMissionId;      //关卡ID
    std::string  strMissionName;  //关卡名字
    std::string  strMissionDesc;
    float        fFarBgPosY;
    MissionBGVec farFileVec;    //远景素材
    float        fMidBgPosY;
    MissionBGVec midFileVec;    //中景素材
    float        fLandBgPosY;
    MissionBGVec landFileVec;   //地面素材
    MissionBGVec nearFileVec;   //近景素材

    int     nCommunityID;       //主城
    int     nMissionType;       //关卡类型
    int     bFancy;       		//是否是趣味关卡
	uint32_t nNeedLevel;		//进入等级
    float   fPassTime;          //通关时间
    float   fHorizonHeight;     //水平线高度
    float   fRolePosX;
    float   fRolePosY;          //地图中人物的初始Y坐标
    
    float   fMissionScale;
    float   fMissionPosX;       //关卡图标在地图中的位置
    float   fMissionPosY;
    int     nLine;
    float   fLinePosX;
    float   fLinePosY;
    int     bossID;
    int     nScenesResourceID;   //场景资源加载
    
    int		nFirstGiftID;		//首次过关对应的奖励库
	std::vector<ItemUnit_t>		costItemVec;
	std::vector<itemDrop_t>     passPrizeVec;    	//过关奖励
    std::vector<itemDrop_t>     passChangePrizeVec; //过关概率奖励
	std::vector<itemDrop_t>     firstPrizeVec;  	//首次过关奖励
    std::vector<itemDrop_t>     firstChangePrizeVec;//首次过关翻牌
    std::vector<itemDrop_t>     itemDropLimitVec;

    WinCondition_t      winCondition;
	ScoreRule_t			scoreRule;
    MissionStageVec     missionStageVec;    //卡屏
    
    ObstacleVec         obstacleVec;        //障碍物
    
    TrapUnitVec         trapUnitVec;        //陷阱 地刺 障碍物等
    
    MapUnitConfMap      mapUnitMap;         //陷阱 地刺 障碍物等（原先设计）
    
    //关卡中简单AI配置 刷怪时间、刷怪间隔、事件等
    
public:
    int getBornMonsterIndexInStage(const MissionStage_t* pStage,int nWaveCnt, int nBornCnt) const
    {
        int index = -1;
        if (pStage)
        {
            if(pStage->monsterBorn.size()>0)
            {
                if (pStage->monsterBorn[nWaveCnt].size() > (uint32_t)nBornCnt)
                {
                    return nBornCnt;
                }
            }
        }
        return index;
    }
    
    int getWaveIndexInStage(const MissionStage_t* pStage,int nWaveCnt) const
    {
        int index = -1;
        if (pStage)
        {
            if (pStage->monsterBorn.size() > (uint32_t)nWaveCnt + 1)
            {
                return nWaveCnt + 1;
            }
        }
        return index;
    }
};

//typedef std::vector<MissionConf_t*> MissionConfVec;
typedef std::map<uint32_t/*mission_id*/, MissionConf_t*> MissionConfMap;
typedef std::vector<uint32_t> IDVec;
typedef std::map<uint32_t/*type+cmt_id*/, IDVec> MissionIDVecMap;

struct ThiefConf_t
{
	enum {
		RATE_LEN = 4,
	};
	int occurRates[RATE_LEN];			//小偷数量及出现概率
	std::vector<itemDrop_t>	dropChangeVec;	//小偷概率掉落
};

class BattleMissionConf
{
public:
    ~BattleMissionConf();
    
	bool loadFromFile(const char* fileName, bool bEncrypt = false);
    void unload();
	const MissionConf_t* getBattleMission(uint32_t missionId) {
		MissionConfMap::const_iterator it = m_MissionConfMap.find(missionId);
		return it != m_MissionConfMap.end() ? it->second : NULL;
	}

	const MissionConf_t* getNextMissionInRange(uint32_t missionID, uint32_t begID, uint32_t endID) {
		missionID = missionID >= begID ? missionID : begID; 
		MissionConfMap::const_iterator it = m_MissionConfMap.upper_bound(missionID);
		if (it != m_MissionConfMap.end() && it->first < endID) {
			return it->second;
		}
		return NULL;
	}
	const MissionConf_t* getNextMainMission(uint32_t missionId) {
		return getNextMissionInRange(missionId, MinMainMissionId, MaxMainMissionId);
	}
	const MissionConf_t* getNextHardMission(uint32_t missionId) {
		return getNextMissionInRange(missionId, MinHardMissionId, MaxHardMissionId);
	}
	const MissionConf_t* getNextEliteMission(uint32_t missionId) {
		return getNextMissionInRange(missionId, MinEliteMissionId, MaxEliteMissionId);
	}

	//根据类型和社区ID找关卡ID列表
	const IDVec& getCmtMissionIDVec(MissionType type, uint32_t nCmtMapID) {
		uint32_t type_cmt_id = type * MISSION_TYPE_BASE_NUM / MISSION_CMT_BASE_NUM + nCmtMapID;
		MissionIDVecMap::iterator it = m_missionIDvecMap.find(type_cmt_id);
		return it != m_missionIDvecMap.end() ? it->second : m_nullVec;
	}

    const ThiefConf_t* getBattleThiefConf() const { return &m_ThiefConf; }
 
    void getMonsterCountInfo(uint32_t nMissionID, MonsterCountInfo_t* pMonsterArray, uint32_t* pArrayCount);
    
private:
    bool loadObstacles(xmlNodePtr chl, MissionConf_t* pMission);
    bool loadTraps(xmlNodePtr chl, MissionConf_t* pMission);
    bool loadMissionBgScene(xmlNodePtr chl, float* pPosY, MissionBGVec* pFileVec);
    bool loadMissionStage(xmlNodePtr chl, MissionConf_t* pMission);
    bool decodeDropItemArray(xmlNodePtr chl, char* field, std::vector<itemDrop_t>* pVec, bool hasRate);
    
private:
    //MissionConfVec m_MissionConfVec;
	MissionConfMap m_MissionConfMap;
	MissionIDVecMap m_missionIDvecMap;
	IDVec m_nullVec;
	ThiefConf_t m_ThiefConf;
};

#endif
