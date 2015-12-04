//
//  MonsterConf.h
//  DragonBattle
//
//  Created by zeng xiaolong on 13-8-2.
//
//

#ifndef __DragonBattle__MonsterConf__
#define __DragonBattle__MonsterConf__

#include "XmlEncrypt.h"
#include <map>

#include "BattleUnitConf.h"

typedef std::map<uint32_t, MonsterUnitConf*> MonsterMap;
class MonsterConf
{
public:
    MonsterConf();
    ~MonsterConf();
    
    MonsterUnitConf* getMonster(uint32_t monsterID);
    
    bool loadFromFile(char* filePath, bool bEncrypt = false);
    void unload();
    bool decodeDropItemArray(xmlNodePtr chl, char* field, std::vector<itemDrop_t>* pVec, bool hasRate);
private:
	MonsterMap  m_MonsterMap;
    uint32_t    m_nCurItemIdx;
};

#endif /* defined(__DragonBattle__MonsterConf__) */
