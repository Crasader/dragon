//
//  NetUtil_C.h
//  iPetLand
//
//  Created by 小敦 王 on 12-12-27.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef iPetLand_NetUtil_C_h
#define iPetLand_NetUtil_C_h

void initAutoReleasePool();
void releaseAutoReleasePool();

void performFuncOnMainThread(int type);

void preformLoginRetOnMainThread(int ret, char* data);

#endif
