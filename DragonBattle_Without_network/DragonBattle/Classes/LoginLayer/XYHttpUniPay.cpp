#include "XYHttpUniPay.h"
#include "XYTopLayer.h"
#include "XYHttpBridge.h"
#include "UserData.h"
#include "UserDataEx.h"
#include "TimerManager.h"
#include "DictGenerator.h"
#include "XYHttpBridge.h"

#include "md5.h"

#include "native_jni.h"

XYHttpUniPay* XYHttpUniPay::g_sUniPayInstance = NULL;

XYHttpUniPay::XYHttpUniPay()
{

}

XYHttpUniPay::~XYHttpUniPay()
{

}

XYHttpUniPay* XYHttpUniPay::getInstance()
{
    if (g_sUniPayInstance == NULL)
    {
        g_sUniPayInstance = new XYHttpUniPay;
    }

    return g_sUniPayInstance;
}

void XYHttpUniPay::requestUniPay(const char* item_id, int item_cnt)
{
    const char* info = getUserInfoForPayment();

    cocos2d::CCLog("apple...%s", info);

    DictGenerator generator;
    CCDictionary* infoDict = generator.dictionaryFromJSON(info);

//MD5校验信息:ckcode=md5(md5(userid.zone_id.item_id.item_cnt.unixtime.pf.pfkey.openid.openkey.pay_token.秘钥).秘钥)

    int user_id = UserData::sharedInstance()->userid;
    int zone_id = UserData::sharedInstance()->server_id;
    long unixtime = UserDataEx::shareInstance()->server_time;

    const char* pf = infoDict->valueForKey("pf")->getCString();
    const char* pfkey = infoDict->valueForKey("pfkey")->getCString();
    const char* openid = infoDict->valueForKey("openid")->getCString();
    const char* openkey = infoDict->valueForKey("openkey")->getCString();
    const char* pay_token = infoDict->valueForKey("pay_token")->getCString();


    const char* format = "%d%d%s%d%d%s%s%s%s%s%s";
    CCString* checkStr1 = CCString::createWithFormat(format,
                                                    user_id,
                                                    zone_id,
                                                    item_id,
                                                    item_cnt,
                                                    unixtime,
                                                    pf,
                                                    pfkey,
                                                    openid,
                                                    openkey,
                                                    pay_token,
                                                    MD5KEY);

    cocos2d::CCLog("apple1...%s", checkStr1->getCString());

    CCString * checkStr2 = CCString::createWithFormat("%s%s", MD5(checkStr1->m_sString).toString().c_str(), MD5KEY);

    cocos2d::CCLog("apple2...%s", checkStr2->getCString());

    std::string ckcode = MD5(checkStr2->m_sString).toString();

    infoDict->setObject(CCString::create(item_id), "item_id");
    infoDict->setObject(CCString::createWithFormat("%d", item_cnt), "item_cnt");
    infoDict->setObject(CCString::createWithFormat("%d", user_id), "userid");
    infoDict->setObject(CCString::createWithFormat("%d", zone_id), "zone_id");
    infoDict->setObject(CCString::createWithFormat("%d", unixtime), "time");
    infoDict->setObject(CCString::create(ckcode), "ckcode");

    //http://xiaobaichi/index.php?userid=%d&zone_id=%d&item_id=%s&item_cnt=%d&pf=%s&pfkey=%s&openid=%s&openkey=%s&pay_token=%s&time=%d&ckcode=%s
    CCString* pUrlStr = CCString::create(UNIPAYMENT_URL);
    pUrlStr->m_sString += "&";
    pUrlStr->m_sString += XYHttpBridge::getInstance()->getPostFields(infoDict);

    cocos2d::CCLog("apple3...%s", pUrlStr->getCString());

    CCHttpRequest* request = new CCHttpRequest();
    request->setUrl(pUrlStr->getCString());
    request->setRequestType(CCHttpRequest::kHttpGet);

    request->setResponseCallback(this, httpresponse_selector(XYHttpUniPay::responseUniPay));

    CCHttpClient::getInstance()->send(request);
    CC_SAFE_RELEASE_NULL(request);
}

void XYHttpUniPay::responseUniPay(CCNode* pSender, CCHttpResponse* data)
{
    CCHttpResponse* response = (CCHttpResponse*)data;

    if (response && response->isSucceed())
    {
        CCLog("response succeed!");
        std::string strContents;
        strContents.assign(response->getResponseData()->begin(), response->getResponseData()->end());
        cocos2d::CCLog("%s", strContents.c_str());
    }

    int result = 0;
    CCDictionary* pDict = XYHttpBridge::parseRequest(&result, response);
    CC_SAFE_RETAIN(pDict);

    cocos2d::CCLog("apple...responseUniPay result %d", result);

    if (result == 0)
    {
        cocos2d::CCLog("apple...responseUniPay ok1 %s", pDict->valueForKey("url_params")->getCString());
        if (pDict)
        {
            notifyToJava(CMD_UNIPAY_REFRESH, NULL);
        }
    }
    else
    {

    }
}

