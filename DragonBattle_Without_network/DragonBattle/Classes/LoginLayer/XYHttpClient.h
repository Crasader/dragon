//
//  XYHttpClient.h
//  XY
//
//  Created by long on 12-12-27.
//
//

#ifndef __XY__XYHttpClient__
#define __XY__XYHttpClient__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC_EXT;
USING_NS_CC;

class XYHttpClient : public CCObject
{
public:
    XYHttpClient(void);
    ~XYHttpClient(void);
    static XYHttpClient *getInstance();
    
    void send(CCHttpRequest* request);
    void httpRequestCompleted(CCNode* pSender,void* data);
};

#endif
