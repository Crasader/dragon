//
//  PlatformLogin.h
//  ydsg
//
//  Created by xiaolong zeng on 12-6-26.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface PlatformLogin : NSObject
{
    BOOL AutoLoginFlag;
}
@property()BOOL AutoLoginFlag;
+(PlatformLogin *)sharedInstance;//单例模式
+(void)login;
+(bool)islogin;
+(void)loginout;
+(NSString*)PlatformId;
+(NSString*)PlatformNickname;
+(int)platformType;
+(void)shareWithContent:(NSString*)strcontent;
+(void)pLatformStop;
+(void)setToolBarHidden:(BOOL)isHidden;
@end
