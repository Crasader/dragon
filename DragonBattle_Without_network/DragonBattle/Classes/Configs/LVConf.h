//
//  LVConf.h
//  DragonBattle
//
//  Created by long on 13-5-27.
//
//

#ifndef __DragonBattle__LVConf__
#define __DragonBattle__LVConf__

#include <iostream>
#include "XmlEncrypt.h"

#define MAX_LEVEL   300

class LVConf
{
public:
	LVConf();
	~LVConf();

	bool loadFromFile(const char* xmlFile, bool bEncrypt = false);
	void unload();

	// 通过总经验得到等级
	int getLevelFromExp(uint64_t exp);

	int getLVLimit();
	uint64_t getExpLimit();
	uint64_t getLVExp(int lv);
	uint64_t getNextLvExp(uint64_t nExp);
	float getExpPercent(uint64_t nExp);//currentpresent of currentlv

private:    
	uint64_t m_exp[MAX_LEVEL + 1];    //每一等级所需要的总经验
	uint32_t m_nCurMaxLevel;    //当前开放的最大等级
};

#endif

