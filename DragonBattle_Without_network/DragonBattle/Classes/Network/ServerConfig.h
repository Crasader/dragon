//
//  ServerConfig.h
//  iPetLand
//
//  Created by 小敦 王 on 12-11-23.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//


#ifndef ServerConfig_h
#define ServerConfig_h
//#define SERVER_RELEASE                //发布版注释打印信息
#define INTRANET_ENVIRONMENT          1 //内网环境
#define INTERNET_TEST_ENVIRONMENT     2 //外网测试环境
#define INTERNET_ENVIRONMENT          3 //外网正常环境
#define NET_ENVIRONMENT    INTERNET_ENVIRONMENT

//---------------------- 内网环境配置 -------------------------
#if (NET_ENVIRONMENT == INTRANET_ENVIRONMENT)
//PHP登录服务器
#define SERVER_LOGIN        "http://192.168.1.210:8088/index.php?c=login&"
#define BULLETIN_URL        "http://game.21dot.com/index.php?c=index&m=recordinfo"
#define TEST_ONLINE_IP      (char*)"192.168.1.210"
#define TEST_ONLINE_PORT    5200
#define UNIPAYMENT_URL      "http://192.168.1.210:8089/index.php?c=index&m=request"
#endif

//---------------------- 外网测试环境配置 -------------------------
#if (NET_ENVIRONMENT == INTERNET_TEST_ENVIRONMENT)
#define SERVER_LOGIN        "http://42.121.57.238:82/login_svr/login.php?"
#define BULLETIN_URL        "http://game.21dot.com/index.php?c=index&m=recordinfo"
#define TEST_ONLINE_IP      (char*)"42.121.57.238"
#define TEST_ONLINE_PORT    5200
#define UNIPAYMENT_URL      "http://xiaobaichi/index.php"
#endif

//---------------------- 外网正常环境配置 -------------------------
#if (NET_ENVIRONMENT == INTERNET_ENVIRONMENT)
#define SERVER_LOGIN        "http://115.29.172.150/login_svr/login.php?"
#define BULLETIN_URL        "http://game.21dot.com/index.php?c=index&m=recordinfo"
#define UNIPAYMENT_URL      "http://xiaobaichi/index.php"
#endif

#define MAX_RECV_BUFFER  (1024 * 256)
#define HTTP_TIME_OUT_SEC   5
#define MD5KEY "Jt9ic$Io4rm@cx1!5UhwQg0"

#define LOGIN_SOCKET_TIMEOUT    15
#define ONLINE_SOCKET_TIMEOUT   15
#define ONLINE_KEEP_ALIVE_SEC   30      //客户端心跳包间隔
#define ONLINE_ARRIVAL_TIME     200000
#define KSEQ  10001 //协议包中的序列号

//#define NONETWORK

#endif
