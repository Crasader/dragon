//
//  FMLayerWebView.mm
//  WebViewDemo
//
//  Created by Yanghui Liu on 12-6-5.
//  Copyright (c) 2012年 FMinutes company. All rights reserved.
//

#include "FMLayerWebView.h"
#include "FMUIWebViewBridge.h"

static FMUIWebViewBridge *g_FMUIWebViewBridge=nil;

FMLayerWebView::FMLayerWebView()
{
    
}
FMLayerWebView::~FMLayerWebView()
{
    [g_FMUIWebViewBridge release];
}

void FMLayerWebView::webViewDidFinishLoad()
{
    
}

void FMLayerWebView::onBackbuttonClick()
{
    [g_FMUIWebViewBridge backClicked : 0];
    this->removeFromParentAndCleanup(true);
}

bool FMLayerWebView::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    g_FMUIWebViewBridge = [[FMUIWebViewBridge alloc] init];
    
    return true;
}

void FMLayerWebView::setUrl(const char* pUrl, float left, float top, float width, float height, float screenWidth, float screenHeight)
{
    [g_FMUIWebViewBridge setLayerWebView : this URLString:pUrl Left:left Top:top Width:width Height:height ScreenWidth:screenWidth ScreenHeight:screenHeight];
}
