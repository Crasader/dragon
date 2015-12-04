//
//  RoleAttrConf.h
//  DragonBattle
//
//  Created by 何 永翔 on 13-7-3.
//
//

#ifndef DragonBattle_RoleAttrConf_h
#define DragonBattle_RoleAttrConf_h

#include <map>
#include "XmlEncrypt.h"

struct RoleMaxTrainAttr_t
{
	float fMaxLife;		//培养HP上限
	float fMaxOutAttack;//培养外功上限
	float fMaxOutDef;	//培养外防上限
	float fMaxInAttack;	//培养内功上限
	float fMaxInDef;	//培养内防上限
	float fMaxToughness;//培养韧性上限
};

struct RoleAttrConf_t
{
    int nLife;
    int nOutAttack;
    int nOutDef;
    int nInAttack;
    int nInDef;
    int nCrit;
    int nToughness;
    int nReduceHurt;
    int nAddHurt;
    int nRecoverHP;
	RoleMaxTrainAttr_t roleTrainAttr;
};

typedef std::map<int, RoleAttrConf_t*> RoleAttrConfMap;

class RoleAttrConf
{
public:
    RoleAttrConf();
    ~RoleAttrConf();
    
    bool loadFromFile(const char* fileName, bool bEncrypt = false);
    void unload();
    
    const RoleAttrConf_t* getRoleAttrConf(int nLevel, int nRoleType);
    
private:
    int getMapKey(int nLevel, int nRoleType);
    
    RoleAttrConfMap m_roleAttrMap;
};

#endif
