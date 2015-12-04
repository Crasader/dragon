//
//  AudioResource.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-19.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "AudioResource.h"
#include "SimpleAudioEngine.h"
#include "GlobalDefine.h"
#include "UserData.h"

#define BG_MUSIC        GET_KEY_WITH_USER(gUserData->userid, "bg_music")
#define SOUND_EFFECT    GET_KEY_WITH_USER(gUserData->userid, "sound_effect")

static AudioResource* g_sAudioResourceInstance = NULL;

AudioResource::AudioResource() {
    m_pBgMusicFile = NULL;
    
    m_bBgMuscOpen = true;
    m_bSoundEffectOpen = true;
    
    
    // 读取声音记录
    m_bBgMuscOpen = CCUserDefault::sharedUserDefault()->getBoolForKey(BG_MUSIC, true);
    m_bSoundEffectOpen = CCUserDefault::sharedUserDefault()->getBoolForKey(SOUND_EFFECT, true);
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.8f);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(1.f);

    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AudioResource::playClickSound), "PLAY_CLICK_SOUND_NOTE", NULL);
}


AudioResource::~AudioResource() {
    CC_SAFE_RELEASE_NULL(m_pBgMusicFile);
    
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

AudioResource* AudioResource::shareAudioResource() {
    if (g_sAudioResourceInstance == NULL) {
        g_sAudioResourceInstance = new AudioResource;
    }
    
    return g_sAudioResourceInstance;
}

void AudioResource::setIsBgMusicOpen(bool var) {
    m_bBgMuscOpen = var;
    if (m_bBgMuscOpen) {
        playBgSound(m_pBgMusicFile->getCString());
    } else {
        stopBgSound();
    }
    
    CCUserDefault::sharedUserDefault()->setBoolForKey( BG_MUSIC, var);
}

bool AudioResource::getIsBgMusicOpen() {
    return m_bBgMuscOpen;
}

void AudioResource::setIsSoundEffectOpen(bool var) {
    m_bSoundEffectOpen = var;
    // 记录到本地
    CCUserDefault::sharedUserDefault()->setBoolForKey(SOUND_EFFECT, var);
}

bool AudioResource::getIsSoundEffectOpen() {
    return m_bSoundEffectOpen;
}

void AudioResource::playClickSound(CCObject* pObj)
{
    this->playEffect("music/click.mp3");
}

int AudioResource::playBgSound(const char* file) {
    int ret = 0;
    do {
        CC_BREAK_IF(file == NULL);
        CC_BREAK_IF(!m_bBgMuscOpen);
        
        setBgMusicFile(CCString::create(file));
        
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(m_pBgMusicFile->getCString(), true);
        ret = 1;
    } while (0);
    
    return ret;
}

void AudioResource::stopBgSound(){
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
}

int AudioResource::playEffect(const char*  file, bool loop){
    int ret = 1;
    do {
        CC_BREAK_IF(file == NULL);
        CC_BREAK_IF(!m_bSoundEffectOpen);
        
        
        ret = CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(file, loop);
        
    } while (false);
    
    return ret;
}
void AudioResource::stopEffect(int soundId) {
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
}