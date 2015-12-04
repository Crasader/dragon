//
//  AnimationInfo.h
//  DragonBattle
//
//  Created by 永翔 何 on 13-5-6.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_AnimationInfo_h
#define DragonBattle_AnimationInfo_h

#include "cocos2d.h"

struct AnimationInfo_t
{
    uint16_t nStartFrame;   //开始帧
    uint16_t nFrameCount;   //总帧数
    float fFrameTime;
    float fSelfSpeedX;      //招式本身的X速度
    float fSelfSpeedAddX;   //招式本身的X加速度
    float fSelfSpeedY;      //招式本身的Y速度
    float fSelfSpeedAddY;   //招式本身的Y加速度 
};

struct AttackEffectInfo_t : public AnimationInfo_t
{
    cocos2d::CCPoint ptOffect;
    std::string strFileHead;
    bool    isScale;
};

#endif
