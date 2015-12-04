//
//  CheckResLayer.cpp
//  DragonBattle
//
//  Created by 白明江 on 12/11/13.
//
//

#include "CheckResLayer.h"
#include "GlobalDefine.h"

#define VERSION_URL ("http://192.168.1.210/test/version.php?package_name=action")
#define PACKAGE_URL ("http://192.168.1.210/test/action.dat")

CheckResLayer::CheckResLayer()
{
    m_pProgressLabel = NULL;
    m_pResourceManager = NULL;
    m_pProgressTimer = NULL;
    
    m_bCanTouch = false;
    
    m_pResourceManager = new NetResourceManager("",
                                                "",
                                                "");
    m_pResourceManager->setDelegate(this);
    m_pResourceManager->setConnectionTimeout(3);
}

CheckResLayer::~CheckResLayer()
{
    CC_SAFE_DELETE(m_pResourceManager);
    
    m_pProgressLabel = NULL;
    m_pResourceManager = NULL;
    m_pProgressTimer = NULL;
}

void CheckResLayer::loadView()
{
    this->setPrority(-128);
    
    CCLayerColor* pBack = CCLayerColor::create(ccc4(0, 0, 0, 255), SCREEN_WIDTH, SCREEN_HEIGTH);
    this->addChild(pBack);
    
    char str[128] = {0};
    sprintf(str, "CCBCommunityResource/progress_bg_%d.jpg", rand() % 3);
    CCSprite* progressImg = CCSprite::create(str);
    pBack->addChild(progressImg);
    progressImg->setPosition(ccp(0, - SCREEN_HEIGTH / 2 + SCREEN_HEIGTH * 0.443));
    
    CCSize size = pBack->getContentSize();
    float scale = SCREEN_WIDTH / size.width > SCREEN_HEIGTH / size.height ? (SCREEN_WIDTH / size.width) : (SCREEN_HEIGTH / size.height);
    pBack->setScaleX(scale);
    pBack->setScaleY(scale);
    
    CCSprite* pTitle = CCSprite::create("CCBLoginResource/game_icon.png");
    this->addChild(pTitle);
    pTitle->setPosition(ccp(SCREEN_WIDTH / 2 - 200, SCREEN_HEIGTH / 2 - 100));
    
    CCSprite* pSlot = CCSprite::create("CCBCommunityResource/progress_slot.png");
    this->addChild(pSlot);
    pSlot->setPosition(ccp(0, - SCREEN_HEIGTH / 2 + 100));
    
    m_pProgressTimer = CCProgressTimer::create(CCSprite::create("CCBCommunityResource/progress_bar.png"));
    this->addChild(m_pProgressTimer);
    m_pProgressTimer->setPosition(ccp(0, - SCREEN_HEIGTH / 2 + 100));
    
    m_pProgressTimer->setPercentage(0);
    m_pProgressTimer->setType(kCCProgressTimerTypeBar);
    m_pProgressTimer->setMidpoint(ccp(0,0));
    m_pProgressTimer->setBarChangeRate(ccp(1, 0));
    m_pProgressTimer->setVisible(false);
    
    m_pProgressLabel = CCLabelTTF::create("", "Arial", 22);
    m_pProgressLabel->setPosition(ccp(0, - SCREEN_HEIGTH / 2 + 50));
    addChild(m_pProgressLabel);
    
    createDownloadedDir();
}

void CheckResLayer::createDownloadedDir()
{
    pathToSave = CCFileUtils::sharedFileUtils()->getWritablePath();
    
    cocos2d::CCLog("apple....CheckResLayer::createDownloadedDir = %s", pathToSave.c_str());
    
    // Create the folder if it doesn't exist
    DIR *pDir = NULL;
    
    pDir = opendir (pathToSave.c_str());
    if (!pDir)
    {
        mkdir(pathToSave.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
    }
}

void CheckResLayer::updateResource()
{
    m_pResourceManager->setPackageUrl(PACKAGE_URL);
    m_pResourceManager->setVersionFileUrl(VERSION_URL);
    m_pResourceManager->setStoragePath(pathToSave.c_str());
    
    if (m_pResourceManager->checkUpdate())
    {
        m_pResourceManager->update();
    }
    
}


void CheckResLayer::onEnter()
{
    CCLayer::onEnter();
    
    m_pProgressLabel->setString("");
    updateResource();
}

void CheckResLayer::onError(ErrorCode errorCode)
{
    if (errorCode == kNoNewVersion)
    {
        m_pProgressTimer->setPercentage(100);
        m_pProgressTimer->setVisible(true);
        m_pProgressLabel->setString("最新资源，点击继续！");
        m_bCanTouch = true;
    }
    
    if (errorCode == kNetwork)
    {
        m_pProgressLabel->setString("网络错误，请检查！");
    }

}

void CheckResLayer::onProgress(int percent)
{
    char progress[128] = {0};
    sprintf(progress, "更新资源%d%% ...", percent);
    m_pProgressLabel->setString(progress);
    
    m_pProgressTimer->setPercentage(percent);
    m_pProgressTimer->setVisible(true);
}

void CheckResLayer::onSuccess()
{
    m_pProgressLabel->setString("点击继续");
    m_bCanTouch = true;
}