//
//  Lock.cpp
//  DragonBattle
//
//  Created by 白明江 on 12/6/13.
//
//

#include "Lock.h"


XYLock::XYLock() {
	m_pCriticalSection = new pthread_mutex_t;
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init((pthread_mutex_t *)m_pCriticalSection, &attr);
}

XYLock::~XYLock() {
	pthread_mutex_destroy((pthread_mutex_t *)m_pCriticalSection);
	delete (pthread_mutex_t *)m_pCriticalSection;
}

void XYLock::lock() {
	//CCLOG("XYLock::lock threadID(%ld)", pthread_self());
	pthread_mutex_lock((pthread_mutex_t *)m_pCriticalSection);
	//CCLOG("XYLock::lock end threadID(%ld)", pthread_self());
}

void XYLock::unlock() {
	//CCLOG("XYLock::unlock threadID(%ld)", pthread_self());
	pthread_mutex_unlock((pthread_mutex_t *)m_pCriticalSection);
	//CCLOG("XYLock::unlock end threadID(%ld)", pthread_self());
}