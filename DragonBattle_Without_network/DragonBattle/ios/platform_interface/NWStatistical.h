//
//  NWStatistical.h
//  ydsg
//
//  Created by xiaolong zeng on 12-6-18.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ydsg_NWStatistical_h
#define ydsg_NWStatistical_h
#include "cocos2d.h"
USING_NS_CC;

#define LOGIN_CALLBACK_NOTE "LOGIN_CALLBACK_NOTE"
#define LOGOUT_CALLBACK_NOTE "LOGOUT_CALLBACK_NOTE"

    class NWPlatformAPI
    {
        public:
        NWPlatformAPI();
        ~NWPlatformAPI();
        static void platformLogin();
        static bool isLogin();
        static void platformStop();
        static void platformLoginout();
        static const char* LoginId();
        static const char* LoginNickname();
        static int platformLogintype();
        static void networkcheck();
        static void setToolBarHidden(bool isHidden);
        static void recharge(const char*serial,int proid,const char*proName,const char *proDesc,int proPri);
        static void share(const char*strDescc);
    };
#endif
