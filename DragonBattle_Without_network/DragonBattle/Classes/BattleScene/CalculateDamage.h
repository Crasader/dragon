//
//  CalculateDamage.h
//  DragonBattle
//
//  Created by zeng xiaolong on 13-11-2.
//
//

#ifndef __DragonBattle__CalculateDamage__
#define __DragonBattle__CalculateDamage__
#include "BattleUnitConf.h"
class CalculateDamage
{
public:
    static float getNormalAttackHurt(int attackBuffSkill,int underAttackBuffSkill,UnitBaseConf*Attacked,UnitBaseConf* pUnderAttacked,float fOutAttackCorrect, float fInAttackCorrect,bool &bCrit);//普通攻击伤害 
    static float getSkillAttackHurt(int attackskill,int attackBuffSkill,int underAttackBuffSkill,UnitBaseConf*Attacked,UnitBaseConf* pUnderAttacked,float fOutAttackCorrect, float fInAttackCorrect,bool &bCrit);//主角技能伤害
    
    static float getThrowAttackHurt(int attackskill,int attackBuffSkill,int underAttackBuffSkill,UnitBaseConf*Attacked,UnitBaseConf* pUnderAttacked,float fOutAttackCorrect, float fInAttackCorrect,bool &bCrit);//抛出物伤害
private:
    static float getOutAttackHurt(int attackBuffSkill,int underAttackBuffSkill,UnitBaseConf*Attacked,UnitBaseConf* pUnderAttacked,float fOutAttackCorrect);//外攻伤害
    static float getInAttackHurt(int attackBuffSkill,int underAttackBuffSkill,UnitBaseConf*Attacked,UnitBaseConf* pUnderAttacked,float fInAttackCorrect);//内攻伤害
};

#endif /* defined(__DragonBattle__CalculateDamage__) */
