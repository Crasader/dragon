//
//  Recharge.m
//  ydsg
//
//  Created by xiaolong zeng on 12-6-28.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "Recharge.h"
#import <NdComPlatform/NDComPlatform.h>
#import <NdComPlatform/NdCPNotifications.h>
#import <NdComPlatform/NdComPlatformError.h>
@implementation Recharge
static Recharge *singleRecharge;
+(Recharge*)sharedInstance
{
	if (singleRecharge==nil) 
    {
		singleRecharge =[[Recharge alloc] init];
        [[NSNotificationCenter defaultCenter] addObserver:singleRecharge
                                                 selector:@selector(NdUniPayAsynResult:)
                                                     name:kNdCPBuyResultNotification
                                                   object:nil];
	}
	return singleRecharge;
}

- (void)buyAysn:(NSString*)cooOrderSerial proId:(NSString*)pid proName:(NSString*)pName proDesc:(NSString*)pDesc proPri:(int)pPri 
{
    NdBuyInfo *buyInfo = [[NdBuyInfo new] autorelease]; 
    buyInfo.cooOrderSerial = cooOrderSerial; //订单号必须唯一!
    buyInfo.productId = pid; //你的产品id
    buyInfo.productName = pName;
    buyInfo.productPrice = pPri; //价格不能为0! 
    buyInfo.productOrignalPrice = pPri; //原价不能为0!
    buyInfo.productCount = 1; //发起请求并检查返回值。注意!该返回值并不是交易结果!
    buyInfo.payDescription = pDesc;
    int res = [[NdComPlatform defaultPlatform] NdUniPayAsyn: buyInfo]; if (res < 0)
    {
        //输入参数有错!无法提交购买请求
    }
}

- (void)NdUniPayAsynResult:(NSNotification*)notify
{
    NSDictionary* dic = [notify userInfo];
    BOOL bSuccess = [[dic objectForKey:@"result"] boolValue]; NSString* str = bSuccess ? @"购买成功" : @"购买失败";
    if (!bSuccess)
    {
        //TODO: 购买失败处理
        NSString* strError = nil;
        int nErrorCode = [[dic objectForKey:@"error"] intValue]; 
        switch (nErrorCode) {
            case ND_COM_PLATFORM_ERROR_USER_CANCEL: strError = @"用户取消操作";
                break;
            case ND_COM_PLATFORM_ERROR_NETWORK_FAIL: strError = @"网络连接错误";
                break;
            case ND_COM_PLATFORM_ERROR_SERVER_RETURN_ERROR: strError = @"服务端处理失败";
                break;
            case ND_COM_PLATFORM_ERROR_ORDER_SERIAL_SUBMITTED: //!!!: 异步支付,用户进入充值界面了或者让好友代付了
                strError = @"支付订单已提交";
                break;
            default:
                strError = @"购买过程发生错误";
            break; }
        str = [str stringByAppendingFormat:@"\n%@", strError];
    }
    else
    {
        //NewWing::RechargeCallBack::shareRecharge()->rechargeCallback(true);
        //TODO: 购买成功处理
    }
    //本次购买的请求参数
    NdBuyInfo* buyInfo = (NdBuyInfo*)[dic objectForKey:@"buyInfo"];
    str = [str stringByAppendingFormat:@"\n<productId = %@, productCount = %d, cooOrderSerial = %@>",buyInfo.productId, buyInfo.productCount, buyInfo.cooOrderSerial];
    NSLog(@"NdUiPayAsynResult: %@", str);
}
@end
