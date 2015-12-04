//
//  NWStatistical.cpp
//  ydsg
//
//  Created by xiaolong zeng on 12-6-18.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
#include "NWStatistical.h"
#include "PlatformLogin.h"
#include "AppController.h"
#include "Recharge.h"
static void static_platformLogin()
{
    [PlatformLogin login];
}

void NWPlatformAPI::platformLogin()
{
    static_platformLogin();
}

static bool static_islogin()
{
    return [PlatformLogin islogin];
}

bool NWPlatformAPI::isLogin()
{
    return static_islogin();
}

static void static_setToolBarHidden(bool isHidden)
{
    [PlatformLogin setToolBarHidden:isHidden];
}
void NWPlatformAPI::setToolBarHidden(bool isHidden)
{
    return static_setToolBarHidden(isHidden);
}

const char* NWPlatformAPI::LoginId()
{
    return [[PlatformLogin PlatformId] UTF8String];
}

const char* NWPlatformAPI::LoginNickname()
{
    return [[PlatformLogin PlatformNickname] UTF8String];
}
static void static_networkcheck()
{
    //[Reachability sharedInstance];
}
void NWPlatformAPI::networkcheck()
{
    static_networkcheck();
}

int NWPlatformAPI::platformLogintype()
{
    return [PlatformLogin platformType];
}

void NWPlatformAPI::platformLoginout()
{
    [PlatformLogin loginout];
}

void NWPlatformAPI::platformStop()
{
    [PlatformLogin pLatformStop];
}

static void static_recharge(const char*serial,int proid,const char*proName,const char* proDesc,int proPri )
{
    NSString *nsserial = [NSString stringWithUTF8String:serial];
    NSString *nsproid = [NSString stringWithFormat:@"%d",proid];
    NSString *nsproName = [NSString stringWithUTF8String:proName];
    NSString *nsproDesc = [NSString stringWithUTF8String:proDesc];
    [[Recharge sharedInstance] buyAysn:nsserial proId:nsproid proName:nsproName proDesc:nsproDesc proPri:proPri];
}

void NWPlatformAPI::recharge(const char*serial,int proid,const char*proName,const char *proDesc,int proPri)
{
    static_recharge(serial,proid,proName,proDesc,proPri);
}
