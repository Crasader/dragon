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
