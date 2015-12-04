#ifndef XYHTTPUNIPAY_H
#define XYHTTPUNIPAY_H

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class XYHttpUniPay : public CCObject
{
private:
    XYHttpUniPay();
public:
    ~XYHttpUniPay();

public:
    static XYHttpUniPay* getInstance();

    void requestUniPay(const char* item_id, int item_cnt);
    void responseUniPay(CCNode* pSender, CCHttpResponse* data);
private:

public:
    static XYHttpUniPay* g_sUniPayInstance;
};

#endif // XYHTTPUNIPAY_H
