//
//  NetUtil_IOS.mm
//  iPetLand
//
//  Created by 小敦 王 on 12-12-27.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "NetUtil_IOS.h"
#include "NetUtil_C.h"
#include "CMDProcessor.h"
#include "LogicManager.h"

void performFuncOnMainThread(int type)
{
    if (type == E_PROCESS_CMD)
    {
        [NetUtil_IOS performProcessCMDOnMainThread];
    }
    else if (type == E_CONNECT_ONLINE)
    {
        [NetUtil_IOS performConnectOnlineOnMainThread];
    }
    else if (type == E_CLOSE_ONLINE)
    {
        [NetUtil_IOS performColseOnlineOnMainThread];
    }
}

int gLoginRet;

void preformLoginRetOnMainThread(int ret, char* data)
{
    
}

void initAutoReleasePool()
{
    [[NetUtil_IOS instance] initAutoreLeasePool];
}

void releaseAutoReleasePool()
{
    [[NetUtil_IOS instance] releaseAutoReleasePool];
}

static NetUtil_IOS* netUtilInstance = nil;
@implementation NetUtil_IOS

+(NetUtil_IOS*) instance
{
	if(netUtilInstance == nil)
	{
		netUtilInstance = [[super allocWithZone:NULL] init];
	}
	return netUtilInstance;
}

// init
-(id) init
{
    if( (self = [super init]))
    {
    }
    
    return self;
}

-(void) initAutoreLeasePool
{
    pool = [[NSAutoreleasePool alloc] init];
}

-(void) releaseAutoReleasePool
{
    [pool release];
}

+(void) performProcessCMDOnMainThread
{
    [self performSelectorOnMainThread:@selector(processCMD) withObject:nil waitUntilDone:YES];
}

+(void) processCMD
{
    CMDProcessor::sharedInstance()->receiveCmd();
}

+(void) performConnectOnlineOnMainThread
{
    [self performSelectorOnMainThread:@selector(performConnectOnline) withObject:nil waitUntilDone:YES];
}

+(void) performConnectOnline
{
    LogicManager::shareLogicManager()->connectOnlineSuccess();
}

+(void) performColseOnlineOnMainThread
{
    [self performSelectorOnMainThread:@selector(performColseOnline) withObject:nil waitUntilDone:YES];
}

+(void) performColseOnline
{
    LogicManager::shareLogicManager()->setStopNetReason(E_SERVER_CLOSE);
    LogicManager::shareLogicManager()->onOnlineClosed();
}

@end