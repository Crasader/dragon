#include "UtilityAction.h"

using namespace cocos2d;

CCShark* CCShark::create(float duration, const CCPoint& position,int sharktimes,float high,bool bIsSlows)
{
	CCShark *pShark = new CCShark();
	pShark->initWithDuration(duration, position,sharktimes,high,bIsSlows);
	pShark->autorelease();
    
	return pShark;
}

bool CCShark::initWithDuration(float duration, const CCPoint& position,int sharktimes,float high,bool bIsSlows)
{
	if (CCActionInterval::initWithDuration(duration))
	{
		m_endPosition = position;
        m_sharktimes = sharktimes;
        m_high = high;
        m_isSlow = bIsSlows;
		return true;
	}
    
	return false;
}

CCObject* CCShark::copyWithZone(CCZone *pZone)
{
	CCZone* pNewZone = NULL;
	CCShark* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject) 
	{
		//in case of being called at sub class
		pCopy = (CCShark*)(pZone->m_pCopyObject);
	}
	else
	{
		pCopy = new CCShark();
		pZone = pNewZone = new CCZone(pCopy);
	}
    
	CCActionInterval::copyWithZone(pZone);
    
	pCopy->initWithDuration(m_fDuration, m_endPosition,m_sharktimes,m_high,m_isSlow);
    
	CC_SAFE_DELETE(pNewZone);
	return pCopy;
}

void CCShark::startWithTarget(CCNode *pTarget)
{
	CCActionInterval::startWithTarget(pTarget);
	m_startPosition = pTarget->getPosition();
	m_delta = ccpSub(m_endPosition, m_startPosition);
}

void CCShark::update(float dt)
{
	if (m_pTarget)
	{   
        if (m_isSlow) 
        {
            m_pTarget->setPosition(ccp(m_pTarget->getPosition().x,
                                       m_startPosition.y + m_high*sin(dt * (m_sharktimes - 0.5 * dt * m_sharktimes) * 6.28)/(m_high*dt+1)));
        }
        else
        {
            m_pTarget->setPosition(ccp(m_pTarget->getPosition().x,
                                       m_startPosition.y + m_high*sin(dt * m_sharktimes * 6.28)));
        }
        
        
	}
}