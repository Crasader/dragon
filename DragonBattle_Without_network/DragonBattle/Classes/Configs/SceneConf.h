//
//  SceneConf.h
//  DragonBattle
//
//  Created by zeng xiaolong on 13-8-27.
//
//

#ifndef __DragonBattle__SceneConf__
#define __DragonBattle__SceneConf__

#include "BattleMissionConf.h"
#include "MapUnitConf.h"

#include <vector>
#include <string>
#include "MonsterConf.h"
#include "ItemConf.h"
#include "XmlEncrypt.h"

#include <vector>
#include <string>
#include "MonsterConf.h"
#include "ItemConf.h"

struct SceneConf_t
{
    uint32_t     nResourceID;   //场景id
    std::vector<std::string>  fileVec;//资源列表
    std::string  strMusic;
    float        fFarBgWidth;
    float        fFarBgPosY;
    MissionBGVec farFileVec;    //远景素材
    float        fMidBgWidth;
    float        fMidBgPosY;
    MissionBGVec midFileVec;    //中景素材
    float        fLandBgWidth;
    float        fLandBgPosY;
    MissionBGVec landFileVec;   //地面素材
    float        fNearBgWidth;
    MissionBGVec nearFileVec;   //近景素材
    bool         bMarquee;      //跑马灯场景
};

typedef std::map<uint32_t, SceneConf_t*>SceneMap;
class BattleSceneConf
{
public:
    BattleSceneConf();
    ~BattleSceneConf();
    
    bool loadFromFile(const char* fileName, bool bEncrypt = false);
    void unload();
    SceneConf_t* getSceneConf(uint32_t nID);
    
private:
    SceneMap m_SceneConfMap;
    bool loadMissionResource(xmlNodePtr chl,std::vector<std::string>* pFileVec);
    bool loadMissionBgScene(xmlNodePtr chl, float* pPosY, MissionBGVec* pFileVec);
};

#endif /* defined(__DragonBattle__SceneConf__) */
