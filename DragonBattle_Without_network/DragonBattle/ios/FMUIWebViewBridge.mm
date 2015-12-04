//
//  FMUIWebViewBridge.cpp
//  WebViewDemo
//
//  Created by Yanghui Liu on 12-6-5.
//  Copyright (c) 2012年 FMinutes company. All rights reserved.
//

#import "FMUIWebViewBridge.h"
#import "EAGLView.h"

@implementation FMUIWebViewBridge

- (id)init
{
    self = [super init];
    if (self) {
        // init code here.
    }
    return self;
}

- (void)dealloc
{
    [mWebView release];
    [mView release];
    
    [super dealloc];
}



-(void) setLayerWebView : (FMLayerWebView*) iLayerWebView URLString:(const char*) urlString Left:(float) left Top:(float) top Width:(float) width Height:(float) height ScreenWidth:(float) screenWidth ScreenHeight:(float) screenHeight
{
    UIScreen *MainScreen = [UIScreen mainScreen];
    CGSize Size = [MainScreen bounds].size;

    mLayerWebView = iLayerWebView;
    mView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, Size.height, Size.width)];
    
    // create webView
    mWebView = [[UIWebView alloc] initWithFrame:CGRectMake(left * Size.height / screenWidth, (screenHeight - top - height) * Size.width / screenHeight, width * Size.height / screenWidth, height * Size.width / screenHeight)];
    mWebView.scalesPageToFit = YES;
    mWebView.dataDetectorTypes = UIDataDetectorTypeNone;
    mWebView.delegate = self;
    mWebView.backgroundColor = [UIColor blackColor];
    [(UIScrollView *)[[mWebView subviews] objectAtIndex:0] setBounces:NO];
    
    NSString *urlBase = [NSString stringWithCString:urlString encoding:NSUTF8StringEncoding];
    
    [mWebView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:urlBase ]]];
    [mWebView setUserInteractionEnabled:NO]; //don't let the user scroll while things are
    
    //[mToolbar release];
    
    // add the webView to the view
    [mView addSubview:mWebView];
    [[EAGLView sharedEGLView] addSubview:mView];
}

- (void)webViewDidStartLoad:(UIWebView *)thisWebView
{
    
}

- (void)webViewDidFinishLoad:(UIWebView *)thisWebView
{
    [mWebView setUserInteractionEnabled:YES];
    mLayerWebView->webViewDidFinishLoad();
}

- (void)webView:(UIWebView *)thisWebView didFailLoadWithError:(NSError *)error
{
    if ([error code] != -999 && error != NULL)
    { //error -999 happens when the user clicks on something before it's done loading.
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Network Error" message:@"Unable to load the page. Please keep network connection."
        delegate:self cancelButtonTitle:nil otherButtonTitles:@"OK", nil];
        
        [alert show];
        [alert release];
    }
}

-(void) backClicked:(id)sender
{
    mWebView.delegate = nil; //keep the webview from firing off any extra messages
    //remove items from the Superview...just to make sure they're gone
    [mWebView removeFromSuperview];
    [mView removeFromSuperview];
}

@end