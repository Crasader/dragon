#ifndef DragonBattle_UtilityAction_h
#define DragonBattle_UtilityAction_h

#include "cocos2d.h"

namespace cocos2d 
{
    class CCShark : public CCActionInterval
    {
    public:
        /** initializes the action */
        bool initWithDuration(float duration, const CCPoint& position,int sharktimes,float high,bool bIsSlows);
        
        virtual CCObject* copyWithZone(CCZone* pZone);
        virtual void startWithTarget(CCNode *pTarget);
        virtual void update(float dt);
        
    public:
        /** creates the action */
        static CCShark* create(float duration, const CCPoint& position, int sharktimes, float high, bool bIsSlows);
        
    protected:
        CCPoint m_endPosition;
        CCPoint m_startPosition;
        CCPoint m_delta;
        int     m_sharktimes;
        float   m_high;
        bool    m_isSlow;
    };
}

#endif
