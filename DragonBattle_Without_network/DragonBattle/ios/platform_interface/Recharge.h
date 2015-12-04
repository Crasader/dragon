//
//  Recharge.h
//  ydsg
//
//  Created by xiaolong zeng on 12-6-28.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
@interface Recharge : NSObject
- (void)buyAysn:(NSString*)cooOrderSerial proId:(NSString*)pid proName:(NSString*)pName proDesc:(NSString*)pDesc proPri:(int)pPri;
+(Recharge*)sharedInstance;
@end
