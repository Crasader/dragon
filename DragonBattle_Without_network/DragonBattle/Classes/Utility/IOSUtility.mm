//
//  IOSUtility.mm
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-24.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "IOSUtility.h"

#import <SystemConfiguration/SCNetworkReachability.h>
#import "AppController.h"

#include <arpa/inet.h>

// get current time
double getCurTime()
{
    return CFAbsoluteTimeGetCurrent();
}

bool isDeviceNetConnect()
{
    // Create zero addy
    struct sockaddr_in zeroAddress;
    bzero(&zeroAddress, sizeof(zeroAddress));
    zeroAddress.sin_len = sizeof(zeroAddress);
    zeroAddress.sin_family = AF_INET;
    
    // Recover reachability flags
    SCNetworkReachabilityRef defaultRouteReachability = SCNetworkReachabilityCreateWithAddress(NULL, (struct sockaddr *)&zeroAddress);
    SCNetworkReachabilityFlags flags;
    
    BOOL didRetrieveFlags = SCNetworkReachabilityGetFlags(defaultRouteReachability, &flags);
    CFRelease(defaultRouteReachability);
    
    if (!didRetrieveFlags)
    {
        return NO;
    }
    
    BOOL isReachable = flags & kSCNetworkFlagsReachable;
    BOOL needsConnection = flags & kSCNetworkFlagsConnectionRequired;
    return (isReachable && !needsConnection) ? TRUE : FALSE;
}

void setMulitTouch(bool isMulitTouch)
{
    UIApplication *App=[UIApplication sharedApplication];
    AppController *Delegate=(AppController *)App.delegate;
    if (isMulitTouch)
    {
        [Delegate.viewController.view setMultipleTouchEnabled:YES];
    }
    else
    {
        [Delegate.viewController.view setMultipleTouchEnabled:NO];
    }
}