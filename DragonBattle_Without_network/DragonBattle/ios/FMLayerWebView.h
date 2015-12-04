//
//  FMLayerWebView.h
//  WebViewDemo
//
//  Created by Yanghui Liu on 12-6-5.
//  Copyright (c) 2012年 FMinutes company. All rights reserved.
//

#ifndef WebViewDemo_FMLayerWebView_h
#define WebViewDemo_FMLayerWebView_h

#include "CCCommon.h"
#include "cocos2d.h"

USING_NS_CC;

class FMLayerWebView : public CCLayer
{
public:
    FMLayerWebView();
    ~FMLayerWebView();
    virtual bool init();
    CREATE_FUNC(FMLayerWebView);
    
    void webViewDidFinishLoad();
    void onBackbuttonClick();
    
    void setUrl(const char* pUrl, float left, float top, float width, float height, float screenWidth, float screenHeight);
    
private:
    int mWebViewLoadCounter;
};

#endif