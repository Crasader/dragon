//
//  StartLoading.h
//  ydsg
//
//  Created by xiaolong zeng on 12-6-26.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#include "cocos2d.h"
USING_NS_CC;
@interface StartLoading : NSObject

+(void)startLoadConfig:(NSObject*)nsobj;
+(void)checkUpdate;
+(StartLoading *)sharedInstance;
@end
