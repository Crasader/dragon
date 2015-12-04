//
//  MovieClip.h
//  DragonBattle
//
//  Created by zeng xiaolong on 13-9-11.
//
//

#ifndef __DragonBattle__MovieClip__
#define __DragonBattle__MovieClip__
#include "ViewDefine.h"
#include "GameData.h"
enum kClipState
{
    kAllClip,//所有剧情
    kFrontClip,//前面剧情
    kBehindClip,//后面剧情

};

class MovieOver
{
public:
    virtual void onMovieOver(bool isOver)=0;
};

class MovieClip:public CCLayer
{
public:
    static MovieClip *create(int taskID,kClipState clipState,MovieOver *Delegate);
    MovieOver *m_pDelegate;
protected:
    bool init(int taskID,kClipState clipState,MovieOver *Delegate);
    MovieClip();
    ~MovieClip();
    int getContentLength();
    std::string getContentByLength(int length);
    void logic(float cTime);
    
    CCSprite *sprBg;
    CCNode *nodebg;
    CCLabelTTF *pLabChat;
    
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void play();
    std::vector<RoleTalk_t> stageTalkVec;
    int m_num;//播放数量
    int m_index;
    int m_taskID;
    int contentLeght;
    int wordCount;
    std::string strtalk;
};

#endif /* defined(__DragonBattle__MovieClip__) */
