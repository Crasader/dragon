//
//  CommunityConf.h
//  DragonBattle
//
//  Created by zeng xiaolong on 13-6-3.
//
//

#ifndef __DragonBattle__CommunityConf__
#define __DragonBattle__CommunityConf__

#include "XmlEncrypt.h"

#include <vector>
#include <string>
#include <map>

#ifdef WONPEE_SERVER
#include "cocos2d_diff.h"
#else
#include "cocos2d.h"
#endif

#define BOSS_MAP_ID(map_id) ((map_id))
#define WORLD_BOSS_MAP_ID (99)


struct CmtNPCResource_t
{
    uint32_t     nResourceID;
    std::string  strplistFile;
    std::string  strFile;
    std::string strRoleFile;
    std::string headImage;
    std::string bustImage;
    uint16_t nStartFrame;   //开始帧
    uint16_t nFrameCount;   //总帧数
    cocos2d::CCSize     contentSize;
    cocos2d::CCPoint    offPos;
};

struct CommunityNPC_t
{
    uint32_t     nNPCId;
    uint32_t     nResourceID;
    std::string strNPCName;
    std::string strNPCPic;
    std::string strNPCSay;
    std::string strMusic;
    
    int secretShop;//没商店
    int shop;//0没商店
    float fPosX;
    float fPosY;
};

struct CmtBossMission
{
    uint32_t nBossMissionID;
    uint32_t nMissionID;
};

typedef std::map<uint32_t,CommunityNPC_t*>  CmtNPCMap;
typedef std::map<uint32_t,CmtNPCResource_t*>  CmtNPCResourceMap;
struct CommunitymapConf_t
{
    uint16_t     nMapId;
	uint32_t	 nNeedLevel;
    std::string strMapName;
    std::string         strMusic;       //背景音乐
    std::string         strEffect;      //粒子特效
    std::vector<std::string> strMapPic;
    std::vector<std::string> strNearMapPic;
    std::vector<std::string> strPlist;
    std::vector<int> npcVec;
    std::vector<int> missionVec;//TODO:DEL,可由missionID的规则直接查找
    std::vector<CmtBossMission*> bossMissionVec;//TODO:DEL,可由missionID规则查找
};

typedef std::vector<CommunitymapConf_t*>  CmtMapConfVec;
typedef std::map<uint32_t/*map_id*/, CommunitymapConf_t*>  CmtMapConfMap;

class CommunitymapConf
{
public:
    CommunitymapConf();
    ~CommunitymapConf();
    
    bool loadFromFile(const char* xmlFile, bool bEncrypt = false);
    void unload();
	const CommunitymapConf_t* getCmdMapConf(uint32_t nCmtMapID) {
		CmtMapConfMap::iterator it = m_CmtMapConfMap.find(nCmtMapID);
		return it != m_CmtMapConfMap.end() ? it->second : NULL;	
	}
	void getCmdMapConfByLevel(uint32_t nLevel, CmtMapConfVec& cmtVec);
    CmtNPCMap cmtNPCMap;
    CmtNPCResourceMap cmtNPCResourceMap;
    
private:
    bool loadCommunityBgScene(xmlNodePtr chl,std::vector<std::string> * pFileVec);
    bool loadPlist(xmlNodePtr chl, std::vector<std::string>* pStrPlist);
    bool loadNPC(xmlNodePtr chl,std::vector<int> *npcvec);
    bool loadMissions(xmlNodePtr chl,std::vector<int> *missionvec);
    bool loadBossPropArr(xmlNodePtr chl, char* field, std::vector<CmtBossMission*>* vec);
    //CmtMapConfVec m_CmtMapConfVec;
    CmtMapConfMap m_CmtMapConfMap;
};
#endif /* defined(__DragonBattle__CommunityConf__) */
