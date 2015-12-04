//
//  XYTouchSprite.cpp
//  DragonBattle
//
//  Created by 何 永翔 on 13-12-4.
//
//

#include "XYTouchSprite.h"
#include "CRUtils.h"

using namespace cocos2d;

XYTouchSprite::XYTouchSprite(void):CCSprite()
{
    m_isEnabled = true;
    
    m_iPriority = 0;
    
    m_pTarget = NULL;
    m_pTouchBegan = NULL;
    m_pTouchEnded = NULL;
    m_pTouchRectLayer = NULL;
}

XYTouchSprite::~XYTouchSprite(void)
{
    
}

bool XYTouchSprite::initWithFile(const char * pFile, bool bIsHorizontal, int nPriority)
{
    m_isHorizontal = bIsHorizontal;
    m_strPic = pFile;
    m_iPriority = nPriority;
    if (pFile == NULL)
    {
        if(!CCSprite::init())
        {
            return false;
        }
    }
    CCSpriteFrame* FileFrame = CRUtils::initSpriteFrameWithName(pFile);
    if(!CCSprite::initWithSpriteFrame(FileFrame))
    {
        return false;
    }
    m_TouchRect = ((CCMenuItem*)this)->rect();
    m_TouchRect.origin = CCPointZero;
    return true;
}

void XYTouchSprite::setProirity(int nPriority)
{
    m_iPriority = nPriority;
    if (getParent())
    {
        CCDirector::sharedDirector()->getTouchDispatcher()->setPriority(m_iPriority,this);
    }
}

int XYTouchSprite::getProirity()
{
    return m_iPriority;
}

void XYTouchSprite::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, m_iPriority, false);
    CCSprite::onEnter();
}

void XYTouchSprite::onExit()
{
    //移除监听
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCSprite::onExit();
}

XYTouchSprite *  XYTouchSprite::create(const char * pNormalFile, bool bIsHorizontal,int nPriority)
{
    XYTouchSprite *pBtn = new XYTouchSprite();
	if (pBtn && pBtn->initWithFile(pNormalFile, bIsHorizontal, nPriority))
    {
	    pBtn->autorelease();
        return pBtn;
    }
    CC_SAFE_DELETE(pBtn);
	return NULL;
}

bool XYTouchSprite::ccTouchBegan(CCTouch* pTouch, CCEvent* event)
{
    for (CCNode *c = this; c != NULL; c = c->getParent())
    {
        if (c->isVisible() == false)
        {
            return false;
        }
    }
    m_moveVec =CCPointZero;
    if (m_isEnabled)
    {
        CCPoint touchLocation = pTouch->getLocation();
        m_startPoi = touchLocation;
        touchLocation = this->convertToNodeSpace(touchLocation);
        CCRect r = m_TouchRect;
        
        if (r.containsPoint(touchLocation))
        {
            //            CCLayerColor *layer = CCLayerColor::create(ccc4(0, 0, 0, 128), m_TouchRect.size.width, m_TouchRect.size.height);
            //            layer->setPosition(ccp(m_TouchRect.origin.x, m_TouchRect.origin.y));
            //            addChild(layer);
            touchBegin();
            return true;
        }
    }
    return false;
}

void XYTouchSprite::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchLocation = pTouch->getLocation();
    m_moveVec = ccpSub(touchLocation, m_startPoi);
}

void XYTouchSprite::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_isEnabled)
    {
        CCPoint touchLocation = pTouch->getLocationInView();
        touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
        
        CCPoint local = this->convertToNodeSpace(touchLocation);
        //        m_TouchRect = ((CCMenuItem*)this)->rect();
        //        m_TouchRect.origin = CCPointZero;
        CCRect r = m_TouchRect;
        if (r.containsPoint(local))
        {
            if (m_isHorizontal)
            {
                if (m_moveVec.x > 35)
                {
                    touchEnd(true);
                }
                else if (m_moveVec.x < -35)
                {
                    touchEnd(false);
                }
            }
            else
            {
                if (m_moveVec.y > 35)
                {
                    touchEnd(true);
                }
                else if (m_moveVec.y < -35)
                {
                    touchEnd(false);
                }
            }
        }
    }
    m_moveVec = CCPointZero;
}

void XYTouchSprite::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void XYTouchSprite::touchBegin()
{
    if(m_pTarget && m_pTouchBegan)
    {
        (m_pTarget->*m_pTouchBegan)(this);
    }
}

void XYTouchSprite::touchEnd(bool bIsPositive)
{
    if(m_pTarget && m_pTouchEnded)
    {
        (m_pTarget->*m_pTouchEnded)(this, &bIsPositive);
    }
}
