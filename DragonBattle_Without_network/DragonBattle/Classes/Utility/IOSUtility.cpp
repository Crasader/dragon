//
//  IOSUtility.mm
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-24.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "IOSUtility.h"


#include <arpa/inet.h>

// get current time
double getCurTime()
{
    return 0.f;//CFAbsoluteTimeGetCurrent();
}

bool isDeviceNetConnect()
{
    return true;
}

void setMulitTouch(bool isMulitTouch)
{
    return;
}