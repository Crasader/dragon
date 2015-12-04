
#include "XYButton.h"
#include "CRUtils.h"
#define LBL_TAG 20
#define IMAGE_TAG 21
USING_NS_CC;
XYButton::XYButton(void)
{
    m_iPriority = 0;
}

XYButton::~XYButton(void)
{

}

XYButton *  XYButton::create(const char * pNormalFile,int nPriority)
{
    XYButton *pBtn = new XYButton();
	if (pBtn && pBtn->initWithFile(pNormalFile,nPriority))
    {
	    pBtn->autorelease();
        return pBtn;
    }
    CC_SAFE_DELETE(pBtn);
	return NULL;
}

XYButton *  XYButton::createWithFileAndPriority(const char * pNormalFile, const char *pSelectFile,const char *pDisableFile,int nPriority)
{
    XYButton *pBtn = new XYButton();
	if (pBtn && pBtn->initWithFile(pNormalFile,pSelectFile,pDisableFile,nPriority))
    {
	    pBtn->autorelease();
        return pBtn;
    }
    CC_SAFE_DELETE(pBtn);
	return NULL;
}

void XYButton::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) 
{
    XYBaseTouchSprite::ccTouchMoved(pTouch,pEvent);
    if (m_isEnabled) 
    {
        CCPoint touchLocation = pTouch->getLocationInView();
        touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
        
        CCPoint local = this->convertToNodeSpace(touchLocation);
        CCRect r = m_TouchRect;
        r.setRect(0,0,r.size.width,r.size.height);

        if (!r.containsPoint(local))
        {
            if (m_pNormalFileFrame)
            {
                this->setDisplayFrame(m_pNormalFileFrame);
            }
        }
    }
}

void XYButton::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_pNormalFileFrame&&m_isEnabled)
    {
        this->setDisplayFrame(m_pNormalFileFrame);
    }
    XYBaseTouchSprite::ccTouchEnded(pTouch,pEvent);
}

void XYButton::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_isEnabled)
    {
        if (m_pNormalFileFrame)
        {
            this->setDisplayFrame(m_pNormalFileFrame);
        }
    }
}

void XYButton::touchBegin()
{
    if (m_pTarget)
    {
        XYBaseButton::touchBegin();
        if (m_pSelectFileFrame)
        {
            this->setDisplayFrame(m_pSelectFileFrame);
        }
        else
        {
            CCSpriteFrame* pSelectFileFrame = CRUtils::getlightSpriteFrame(m_strPic.c_str());
            this->setDisplayFrame(pSelectFileFrame);
        }
    }
}
