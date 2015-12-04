//
//  Lock.h
//  DragonBattle
//
//  Created by 白明江 on 12/6/13.
//
//

#ifndef __DragonBattle__Lock__
#define __DragonBattle__Lock__

#include <iostream>
#include <pthread.h>
#include "cocos2d.h"

USING_NS_CC;

class XYLock {
public:
    XYLock();
    ~XYLock();
    void lock();
    void unlock();
private:
    void *m_pCriticalSection;
};

class XYSmartLock {
public:
    XYSmartLock(XYLock *pLock): m_pLock(pLock) { m_pLock->lock(); }
    ~XYSmartLock() { m_pLock->unlock(); }
private:
    XYLock *m_pLock;
};



#endif /* defined(__DragonBattle__Lock__) */
