//
//  DragonBattleAppController.h
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-15.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//

@class RootViewController;
#import "RootViewController.h"
@interface AppController : NSObject <UIAccelerometerDelegate, UIAlertViewDelegate, UITextFieldDelegate,UIApplicationDelegate> {
    UIWindow *window;
    RootViewController    *viewController;
}
@property(retain)RootViewController    *viewController;
@end

