#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>
#import <UIKit/UIKit.h>

#import "FMLayerWebView.h"

@interface FMUIWebViewBridge : NSObject<UIWebViewDelegate,UIAlertViewDelegate>
{
    FMLayerWebView * mLayerWebView;
    UIView    *mView;
    UIWebView *mWebView;
}

-(void) setLayerWebView : (FMLayerWebView*) iLayerWebView URLString:(const char*) urlString Left:(float) left Top:(float) top Width:(float) width Height:(float) height ScreenWidth:(float) screenWidth ScreenHeight:(float) screenHeight;
-(void) backClicked:(id)sender;

@end
