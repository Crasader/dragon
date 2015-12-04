//
//  XYBaseButton.cpp
//  DragonBattle
//
//  Created by zeng xiaolong on 13-7-17.
//
//

#include "XYBaseButton.h"
#include "AudioResource.h"
#include "CRUtils.h"
#define LBL_TAG 20
#define IMAGE_TAG 21
USING_NS_CC;
XYBaseButton::XYBaseButton(void):XYBaseTouchSprite()
{
    m_pNormalFileFrame = NULL;
    m_pSelectFileFrame = NULL;
    m_pDisableFileFrame = NULL;
    m_iPriority = 0;
    m_sound = true;
}

XYBaseButton::~XYBaseButton(void)
{
    CC_SAFE_RELEASE_NULL(m_pSelectFileFrame);
    CC_SAFE_RELEASE_NULL(m_pDisableFileFrame);
    CC_SAFE_RELEASE_NULL(m_pNormalFileFrame);
}

bool XYBaseButton::initWithFile(const char * pFile,int nPriority)
{
    return initWithFile(pFile,NULL,NULL,nPriority);
}

bool XYBaseButton::initWithFile(const char * pNormalFile, const char *pSelectFile,const char *pDisableFile, int nPriority)
{
    if(!XYBaseTouchSprite::initWithFile(pNormalFile,nPriority))
    {
        return false;
    }
    CC_SAFE_RELEASE_NULL(m_pNormalFileFrame);
    m_pNormalFileFrame = CRUtils::initSpriteFrameWithName(pNormalFile);
    CC_SAFE_RETAIN(m_pNormalFileFrame);
    
    if (pSelectFile)
    {
        CC_SAFE_RELEASE_NULL(m_pSelectFileFrame);
        m_pSelectFileFrame = CRUtils::initSpriteFrameWithName(pSelectFile);
        CC_SAFE_RETAIN(m_pSelectFileFrame);
    }
    
    if (pDisableFile)
    {
        CC_SAFE_RELEASE_NULL(m_pDisableFileFrame);
        m_pDisableFileFrame = CRUtils::initSpriteFrameWithName(pDisableFile);
        CC_SAFE_RETAIN(m_pDisableFileFrame);
    }
    return true;
}

void XYBaseButton::setEnable(bool enable)
{
    if (m_isEnabled == enable)
    {
        return;
    }
    m_isEnabled = enable;
    if (m_isEnabled)
    {
        setDisplayFrame(m_pNormalFileFrame);
    }
    else
    {
        if(m_pDisableFileFrame)
        {
            setDisplayFrame(m_pDisableFileFrame);
        }
        else
        {
            CCSpriteFrame* pDisableFileFrame = CRUtils::getgaySpriteFrame(m_strPic.c_str());
            setDisplayFrame(pDisableFileFrame);
        }
    }
}

XYBaseButton *  XYBaseButton::create(const char * pNormalFile,int nPriority)
{
    XYBaseButton *pBtn = new XYBaseButton();
	if (pBtn && pBtn->initWithFile(pNormalFile,nPriority))
    {
	    pBtn->autorelease();
        return pBtn;
    }
    CC_SAFE_DELETE(pBtn);
	return NULL;
}

XYBaseButton *  XYBaseButton::createWithFileAndPriority(const char * pNormalFile, const char *pSelectFile,const char *pDisableFile,int nPriority)
{
    XYBaseButton *pBtn = new XYBaseButton();
	if (pBtn && pBtn->initWithFile(pNormalFile,pSelectFile,pDisableFile,nPriority))
    {
	    pBtn->autorelease();
        return pBtn;
    }
    CC_SAFE_DELETE(pBtn);
	return NULL;
}

void XYBaseButton::setLblColor(const ccColor3B& color)
{
    if (getChildByTag(LBL_TAG))
    {
        CCLabelTTF *plbl =(CCLabelTTF *)getChildByTag(LBL_TAG);
        plbl->setColor(color);
    }
}

CCLabelTTF * XYBaseButton::setLbl(const char *pFile,const char *fontName,float fontsize)
{
    if (getChildByTag(LBL_TAG))
    {
        CCLabelTTF *plbl =(CCLabelTTF *)getChildByTag(LBL_TAG);
        plbl->setString(pFile);
    }
    else
    {
        CCLabelTTF *plbl = CCLabelTTF::create(pFile, fontName, fontsize);
        plbl->setTag(LBL_TAG);
        plbl->setPosition(ccp(getContentSize().width/2, getContentSize().height/2));
        addChild(plbl,2);
    }
    return (CCLabelTTF*)getChildByTag(LBL_TAG);
}

void XYBaseButton::setLblPosition(float widthPercent,float heigthPercent)
{
    if (getChildByTag(LBL_TAG))
    {
        CCLabelTTF *plbl =(CCLabelTTF *)getChildByTag(LBL_TAG);
        plbl->setPosition(ccp(getContentSize().width*widthPercent, getContentSize().height*heigthPercent));
    }
    
}

void XYBaseButton::removeImage()
{
    if(getChildByTag(IMAGE_TAG))
    {
        getChildByTag(IMAGE_TAG)->removeFromParentAndCleanup(true);
    }
}

CCSprite* XYBaseButton::setImage(const char *pFile,int orderZ)
{
    if(getChildByTag(IMAGE_TAG))
    {
        getChildByTag(IMAGE_TAG)->removeFromParentAndCleanup(true);
    }
    CCSpriteFrame *FileFrame = CRUtils::initSpriteFrameWithName(pFile);
    CCSprite *image = CCSprite::createWithSpriteFrame(FileFrame);
    image->setTag(IMAGE_TAG);
    image->setPosition(ccp(getContentSize().width/2,getContentSize().height/2));
    addChild(image,orderZ);
    return image;
}

CCSprite* XYBaseButton::addImage(const char *pFile,int orderZ,int tag)
{
    CCSpriteFrame *FileFrame = CRUtils::initSpriteFrameWithName(pFile);
    CCSprite *image = CCSprite::createWithSpriteFrame(FileFrame);
    image->setTag(tag);
    image->setPosition(ccp(getContentSize().width/2,getContentSize().height/2));
    addChild(image,orderZ);
    return image;
}

void XYBaseButton::setImage(const char *pFile, int orderZ, float scale, float widthPercent, float heigthPercent)
{
    if(getChildByTag(IMAGE_TAG))
    {
        getChildByTag(IMAGE_TAG)->removeFromParentAndCleanup(true);
    }
    CCSpriteFrame *FileFrame = CRUtils::initSpriteFrameWithName(pFile);
    CCSprite *image = CCSprite::createWithSpriteFrame(FileFrame);
    image->setScale(scale);
    image->setTag(IMAGE_TAG);
    image->setPosition(ccp(getContentSize().width*widthPercent,getContentSize().height*heigthPercent));
    addChild(image,orderZ);
}

void XYBaseButton::addImage(const char *pFile, int orderZ, float scale, float widthPercent, float heigthPercent)
{
    CCSpriteFrame *FileFrame = CRUtils::initSpriteFrameWithName(pFile);
    CCSprite *image = CCSprite::createWithSpriteFrame(FileFrame);
    image->setScale(scale);
    image->setPosition(ccp(getContentSize().width*widthPercent,getContentSize().height*heigthPercent));
    addChild(image,orderZ);
}

void XYBaseButton::setGayImage(const char *pFile, int orderZ)
{
    if(getChildByTag(IMAGE_TAG))
    {
        getChildByTag(IMAGE_TAG)->removeFromParentAndCleanup(true);
    }
    CCSpriteFrame *frame =  CRUtils::getgaySpriteFrame(pFile);
    CCSprite *image = CCSprite::createWithSpriteFrame(frame);
    image->setTag(IMAGE_TAG);
    image->setPosition(ccp(getContentSize().width/2,getContentSize().height/2));
    addChild(image,orderZ);
}

void XYBaseButton::touchBegin()
{
    if(m_pTarget && m_pTouchBegan)
    {
        if (m_sound)
        {
            AudioResource::shareAudioResource()->playEffect("music/click.mp3");
        }
        
        (m_pTarget->*m_pTouchBegan)(this);
    }
}
void XYBaseButton::touchEnd()
{
    if(m_pTarget && m_pTouchEnded)
    {
        if (m_sound)
        {
            AudioResource::shareAudioResource()->playEffect("music/click.mp3");
        }
        (m_pTarget->*m_pTouchEnded)(this);
    }
}

void XYBaseButton::setButtonBgImage(CCSpriteFrame* normalImg, CCSpriteFrame* seleImg, CCSpriteFrame* disableImg)
{
    CC_SAFE_RELEASE_NULL(m_pNormalFileFrame);
    m_pNormalFileFrame = normalImg;
    CC_SAFE_RETAIN(m_pNormalFileFrame);
    
    CC_SAFE_RELEASE_NULL(m_pSelectFileFrame);
    m_pSelectFileFrame = seleImg;
    CC_SAFE_RETAIN(m_pSelectFileFrame);
    
    CC_SAFE_RELEASE_NULL(m_pDisableFileFrame);
    m_pDisableFileFrame = disableImg;
    CC_SAFE_RETAIN(m_pDisableFileFrame);
    
    if (m_pNormalFileFrame)
    {
        this->setDisplayFrame(m_pNormalFileFrame);
    }
}

void XYBaseButton::setButtonBgImage(const char * pNormalFile, const char *pSelectFile,const char *pDisableFile)
{
    CC_SAFE_RELEASE_NULL(m_pNormalFileFrame);
    m_pNormalFileFrame = CRUtils::initSpriteFrameWithName(pNormalFile);
    CC_SAFE_RETAIN(m_pNormalFileFrame);
    
    if (pSelectFile)
    {
        CC_SAFE_RELEASE_NULL(m_pSelectFileFrame);
        m_pSelectFileFrame = CRUtils::initSpriteFrameWithName(pSelectFile);
        CC_SAFE_RETAIN(m_pSelectFileFrame);
    }
    
    if (pDisableFile)
    {
        CC_SAFE_RELEASE_NULL(m_pDisableFileFrame);
        m_pDisableFileFrame = CRUtils::initSpriteFrameWithName(pDisableFile);
        CC_SAFE_RETAIN(m_pDisableFileFrame);
    }
    if (m_pNormalFileFrame)
    {
        this->setDisplayFrame(m_pNormalFileFrame);
    }
}