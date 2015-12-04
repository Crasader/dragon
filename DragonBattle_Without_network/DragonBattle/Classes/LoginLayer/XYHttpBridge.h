//
//  XYHttpBridge.h
//  XY
//
//  Created by long on 12-12-29.
//
//

#ifndef __XY__XYHttpBridge__
#define __XY__XYHttpBridge__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "HttpRequest.h"

USING_NS_CC;
USING_NS_CC_EXT;

class XYHttpBridge:public cocos2d::CCObject
{
public:
    XYHttpBridge(void);
    ~XYHttpBridge(void);
    
    static XYHttpBridge *getInstance();
    void sendEncryData(cocos2d::CCDictionary *dicdata, CCObject*pTarget, SEL_HttpResponse callfun);
    void checkHaveBulletin(CCObject*pTarget, SEL_HttpResponse callfun);
    void sendData(cocos2d::CCDictionary *dicdata, CCObject*pTarget, SEL_HttpResponse callfun);    
    std::string getMd5str(cocos2d::CCDictionary *dicdata);
    static CCDictionary* parseRequest(int* result, cocos2d::extension::CCHttpResponse* response);
    std::string getPostFields(cocos2d::CCDictionary *dicdata);
    std::string m_URL;
};

#endif
