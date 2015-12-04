//
//  IOSUtility.h
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-24.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_IOSUtility_h
#define DragonBattle_IOSUtility_h

#include "cocos2d.h"


//~~~~~~~~~~~IOS Specific~~~~~~~~~~~~~~
void setMulitTouch(bool isMulitTouch);

// get current time
double getCurTime();

// check if net is connect
bool isDeviceNetConnect();

#endif
