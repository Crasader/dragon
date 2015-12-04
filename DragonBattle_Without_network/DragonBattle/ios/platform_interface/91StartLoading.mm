//
//  StartLoading.m
//  ydsg
//
//  Created by xiaolong zeng on 12-6-26.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "StartLoading.h"
#import <NdComPlatform/NDComPlatform.h>
#import <NdComPlatform/NdComPlatformAPIResponse.h>
#import <NdComPlatform/NdCPNotifications.h>

@implementation StartLoading
static StartLoading *singleStartLoading;
+(StartLoading *)sharedInstance
{
    if (singleStartLoading==nil) 
    {
		singleStartLoading =[[StartLoading alloc] init];
	}
	return singleStartLoading;
}

+(void)startLoadConfig:(NSObject*)notifyer
{
    NdInitConfigure *cfg = [[[NdInitConfigure alloc] init] autorelease];
    cfg.appid =103387;
    cfg.appKey =@"984cb8611759b176c8c8bd2dfcedfd92afea42163dad2a9c";
    cfg.versionCheckLevel = ND_VERSION_CHECK_LEVEL_STRICT;
    cfg.orientation = UIInterfaceOrientationLandscapeRight;
    [[NdComPlatform defaultPlatform] NdInit:cfg];
    //[[NdComPlatform defaultPlatform] NdSetDebugMode:0];
    [[NSNotificationCenter defaultCenter] addObserver:notifyer
                                             selector:@selector(SNSInitResult:)
                                                 name:(NSString *)kNdCPInitDidFinishNotification object:nil];
    [[NdComPlatform defaultPlatform] NdSetScreenOrientation:UIInterfaceOrientationLandscapeRight];
    [[NdComPlatform defaultPlatform] NdSetAutoRotation:YES];

}

+(void)checkUpdate
{
    //[[NdComPlatform defaultPlatform] NdAppVersionUpdate:0 delegate:[StartLoading sharedInstance]];
}
@end
