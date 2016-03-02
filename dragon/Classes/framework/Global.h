//
//  Global.h
//  dragon
//
//  Created by xingchong on 2/24/16.
//
//

#ifndef __dragon__Global__
#define __dragon__Global__

#include <stdio.h>
#include "CsvConfig.h"


class Global
{
public:
    
    enum class CollideDirection
    {
        NONE    = 0x0000,
        UP      = 0x0001,
        DOWN    = 0x0010,
        LEFT    = 0x0100,
        RIGHT   = 0x1000
    };
    
    enum class RoleState
    {
        ACTION_NULL,
        STAND_BY,
        RUNNING,
        
        JUMP_BY,
        JUMP_IN_AIR,
        JUMP_DOWN,
        
        HIT_NORMAL,
        HIT_HEAD,
        HIT_BELLY,
        HIT_BACK,
        
        HIT_FLY_BEGIN,
        HIT_FLY_INAIR,
        HIT_FLY_OVER,
        
        LIE_DOWN,
        VERTI_GO,
        BE_HIT,
        
        ATTACK_BEGIN,
        ATTACK_VALID,
        ATTACK_OVER,
        
        INAIR_ATTACK_BEGIN,
        INAIR_ATTACK_VALID,
        INAIR_ATTACK_OVER,
        
        SKILL_ATTACK_BEGIN,
        SKILL_ATTACK_VALID,
        SKILL_ATTACK_OVER,
        
        DEATH,
        WIN,
        LOSE
    };
    
public:
    static Global* getInstance();
    
public:
    Global();
    ~Global();
    
    bool init();
    
public:
    CsvConfig *_csvConfig;
    
};

#endif /* defined(__dragon__Global__) */
