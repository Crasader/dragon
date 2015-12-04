//
//  PlatformLogin.m
//  ydsg
//
//  Created by xiaolong zeng on 12-6-26.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "PlatformLogin.h"
#import <NdComPlatform/NDComPlatform.h>
#import <NdComPlatform/NdComPlatformAPIResponse.h>
#import <NdComPlatform/NdCPNotifications.h>
#include "NWStatistical.h"
@implementation PlatformLogin
@synthesize AutoLoginFlag;
static PlatformLogin *singlePlatformLogin;

+(PlatformLogin*)sharedInstance
{
	if (singlePlatformLogin==nil) 
    {
		singlePlatformLogin =[[PlatformLogin alloc] init];
        [[NSNotificationCenter defaultCenter] addObserver:singlePlatformLogin
                                                 selector:@selector(SNSLoginResult:)
                                                     name:(NSString *)kNdCPLoginNotification
                                                   object:nil];
	}
	return singlePlatformLogin;
}

+(bool)islogin
{
    return [[NdComPlatform defaultPlatform] isLogined];
}

+(NSString*)PlatformId
{
    return [[NdComPlatform defaultPlatform]loginUin];
}

+(NSString*)PlatformNickname
{
    return [[NdComPlatform defaultPlatform]nickName];
}

+(void)login
{
    [PlatformLogin sharedInstance];
    if ([[NdComPlatform defaultPlatform] isLogined]) 
    {
        CCString *strlog = new CCString("1");
        CCNotificationCenter::sharedNotificationCenter()->postNotification(LOGIN_CALLBACK_NOTE,strlog);
        strlog->autorelease();
    }
    else
    {
        [[NdComPlatform defaultPlatform] NdLogin:0];
    }
}

+(void)loginout
{
    [[NdComPlatform defaultPlatform] NdLogout:1];
}

+(void)pLatformStop
{
    [[NdComPlatform defaultPlatform] NdPause];
}

- (void)SNSLoginResult:(NSNotification *)notify
{
    NSDictionary *dict = [notify userInfo];
    BOOL success = [[dict objectForKey:@"result"] boolValue];
    NdGuestAccountStatus* guestStatus = (NdGuestAccountStatus*)[dict
                                                                objectForKey:@"NdGuestAccountStatus"]; //登录成功后处理
    if([[NdComPlatform defaultPlatform] isLogined] && success) 
    { //也可以通过[[NdComPlatform defaultPlatform] getCurrentLoginState]判断是否游客登录状态
        if (guestStatus)
        {
            if ([guestStatus isGuestLogined]) 
            {   
                //游客账号登录成功; 
                
            }
            else if ([guestStatus isGuestRegistered])
            { 
                    //游客成功注册为普通账号
            } 
        }
        else
        {
            CCString *strlog = new CCString("1");
            CCNotificationCenter::sharedNotificationCenter()->postNotification(LOGIN_CALLBACK_NOTE,strlog);
            strlog->autorelease();
        } 
    }
    else //登录失败处理和相应提示
    {
        int error = [[dict objectForKey:@"error"] intValue];
        NSString* strTip = [NSString stringWithFormat:@"登录失败, error=%d",error];
        switch (error)
        {
            case ND_COM_PLATFORM_ERROR_USER_CANCEL://用户取消登录
                if (([[NdComPlatform defaultPlatform] getCurrentLoginState] == ND_LOGIN_STATE_GUEST_LOGIN))
                {
                    strTip = @"当前仍处于游客登录状态";
                } 
                else 
                {
                    strTip = @"用户未登录"; 
                }
                
                CCNotificationCenter::sharedNotificationCenter()->postNotification(LOGOUT_CALLBACK_NOTE, CCString::create("2"));

                break;
            case ND_COM_PLATFORM_ERROR_APP_KEY_INVALID://appId未授权接入, 或appKey 无效
            case ND_COM_PLATFORM_ERROR_CLIENT_APP_ID_INVALID://无效的应用ID
            case ND_COM_PLATFORM_ERROR_HAS_ASSOCIATE_91: //有关联的91账号,不能以游客方式登录
            {
                CCString *strlog = new CCString("0");
                CCNotificationCenter::sharedNotificationCenter()->postNotification(LOGIN_CALLBACK_NOTE, strlog);
                strlog->autorelease();
            }
                break;
            default:
                break;
        }
    }
}

+(int)platformType
{
    return 2;
}

+(void)setToolBarHidden:(BOOL)isHidden
{
    if (isHidden)
    {
        [[NdComPlatform defaultPlatform] NdHideToolBar];
    }
    else
    {
        [[NdComPlatform defaultPlatform] NdShowToolBar:NdToolBarAtTopLeft];
    }
    
}
+(void)shareWithContent:(NSString*)strcontent
{

}
@end
