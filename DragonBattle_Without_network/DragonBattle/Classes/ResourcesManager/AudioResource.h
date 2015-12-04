//
//  AudioResource.h
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-19.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_AudioResource_h
#define DragonBattle_AudioResource_h

#include "cocos2d.h"


USING_NS_CC;

class AudioResource : public CCObject {
    
protected:
    CC_SYNTHESIZE_RETAIN(CCString*, m_pBgMusicFile, BgMusicFile);
    CC_PROPERTY(bool, m_bBgMuscOpen, IsBgMusicOpen);
    CC_PROPERTY(bool, m_bSoundEffectOpen, IsSoundEffectOpen);
    
private:
    AudioResource();
    
    void playClickSound(CCObject* pObj);
    
public:
    virtual ~AudioResource();
    static AudioResource* shareAudioResource();
    
    int playBgSound(const char* file);
    void stopBgSound();
    
    int playEffect(const char*  file, bool loop = false);
    void stopEffect(int soundId);
    
};

#endif
