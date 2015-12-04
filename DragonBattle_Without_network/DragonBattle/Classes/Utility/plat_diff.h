//
//  plat_diff.h
//  DragonBattle
//
//  Created by 小敦 王 on 13-5-9.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_plat_diff_h
#define DragonBattle_plat_diff_h

#include "cocos2d.h"

#undef DEBUG_LOG 
#define DEBUG_LOG  CCLOG

#undef ERROR_LOG  
#define ERROR_LOG  CCLOG

#undef ERROR_RETURN    
#define ERROR_RETURN(X, Y) \
do { \
    ERROR_LOG X; \
    return Y; \
} while (0)

#undef INFO_LOG
#define INFO_LOG(fmt, args...)

#undef BOOT_LOG
#define BOOT_LOG(OK, fmt, args...) \
do { \
return OK; \
} while (0)

#endif
