//
//  SpriteFrameManager.h
//  DragonBattle
//
//  Created by long on 13-5-29.
//
//

#ifndef __DragonBattle__SpriteFrameManager__
#define __DragonBattle__SpriteFrameManager__
#include "cocos2d.h"
USING_NS_CC;
typedef enum kPathType
{
    kCachePath = 1,
    kRelativePath,
}PathType;
class SpriteFrameManager
{
public:
    static SpriteFrameManager* sharedInstance();
    SpriteFrameManager();
    ~SpriteFrameManager();
    void addSpriteFrameWithFile(const char *pszPlist,PathType ktype);
};
#endif /* defined(__DragonBattle__SpriteFrameManager__) */
