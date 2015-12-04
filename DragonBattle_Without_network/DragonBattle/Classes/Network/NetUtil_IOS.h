//
//  NetUtil_IOS.h
//  iPetLand
//
//  Created by 小敦 王 on 12-12-27.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef iPetLand_NetUtil_IOS_h
#define iPetLand_NetUtil_IOS_h


#import <Foundation/Foundation.h>
#import <Foundation/NSAutoreleasePool.h>

@interface NetUtil_IOS : NSObject/* <UITextFieldDelegate>*/
{
    NSAutoreleasePool* pool;
}
+(NetUtil_IOS*) instance;

-(void) initAutoreLeasePool;
-(void) releaseAutoReleasePool;

+(void) performProcessCMDOnMainThread;
+(void) processCMD;

+(void) performConnectOnlineOnMainThread;
+(void) performConnectOnline;

+(void) performColseOnlineOnMainThread;
+(void) performColseOnline;
@end

#endif
