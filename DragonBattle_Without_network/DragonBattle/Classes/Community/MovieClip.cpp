//
//  MovieClip.cpp
//  DragonBattle
//
//  Created by zeng xiaolong on 13-9-11.
//
//

#include "MovieClip.h"
#include "UserData.h"
#include "OnlineManager.h"
#include "UnitResource.h"
MovieClip* MovieClip::create(int taskID,kClipState clipState,MovieOver *Delegate)
{ 
    MovieClip *pRet = new MovieClip();
    if (pRet && pRet->init(taskID,clipState,Delegate))
    { 
        pRet->autorelease();
        return pRet;
    }
    else
    { 
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

MovieClip::MovieClip()
{
    m_pDelegate= NULL;
}

MovieClip::~MovieClip()
{

}

bool MovieClip::init(int taskID,kClipState clipState,MovieOver *Delegate)
{
    if (!CCLayer::init())
    {
        return false;
    }
    setTouchEnabled(true);
    m_pDelegate = Delegate;
    if (m_pDelegate)
    {
        m_pDelegate->onMovieOver(false);
    }
    m_taskID = taskID;
    const TaskConfig_t* taskconf = GameData::getTaskConf(taskID);
    sprBg = CCSprite::create("public/task_dialog.png");
    sprBg->setAnchorPoint(CCPointZero);
    sprBg->setScaleX(SCREEN_WIDTH/sprBg->getContentSize().width);
    sprBg->setScaleY(SCREEN_HEIGTH/sprBg->getContentSize().height);
    addChild(sprBg);
    nodebg = CCNode::create();
    addChild(nodebg);
    stageTalkVec = taskconf->stageTalk.stageTalkVec;
    switch (clipState)
    {
        case kAllClip:
            m_num = stageTalkVec.size()-1;
            m_index = 0;
            break;
        case kFrontClip:
            m_num = taskconf->stageTalk.nOverIndex-1;
            m_index = 0;
            break;
        case kBehindClip:
            m_index = taskconf->stageTalk.nOverIndex;
            m_num = stageTalkVec.size()-1;
            break;
        default:
            break;
    }
    play();
    return true;
}

string& replace_all(string& str,const string& old_value,const string& new_value)
{
    while(true)
    {
        string::size_type   pos(0);
        if((pos=str.find(old_value))!=string::npos)
            str.replace(pos,old_value.length(),new_value);
        else   break;
    }
    return  str;
}

void MovieClip::play()
{
    nodebg->removeAllChildren();
    char str[512] = {0};
    if (stageTalkVec.empty())
    {
        return ;
    }
    RoleTalk_t talk = stageTalkVec.at(m_index);
    string name = gUserData->nick;
    contentLeght = 0;
    wordCount = 0;
    strtalk = talk.roleTalk;
    int length = strtalk.length();
    int i = 0;
    while(i < length)
    {
        char ch = strtalk[i];
        //重点在这里
        //中文在ASCII码中是-127~0
        if (ch > -127 && ch< 0)
        {
            //这里为什么+＝3呢
            //因为一个中文占3个字节
            i+=3;
        }
        else
        {
            i++;
        }
        contentLeght++;
    }
    
    strtalk = replace_all(strtalk,"【】",name);
    pLabChat = CCLabelTTF::create(" ", FONTNAME, 22, CCSize(480, 130), kCCTextAlignmentLeft);
    pLabChat->setAnchorPoint(ccp(0.5,1));
    pLabChat->setPosition(ccp(SCREEN_WIDTH/2,140));
    nodebg->addChild(pLabChat);
    
    CCLabelTTF *pLabName = CCLabelTTF::create("", FONTNAME, 22);
    pLabName->setAnchorPoint(CCPointZero);
    CCSprite *npc = NULL;
    if (talk.nRoleID != 0)
    {
        const TaskConfig_t* taskconf = GameData::getTaskConf(m_taskID);
        if (taskconf->nFinishType == 2)
        {
            MonsterUnitConf*conf = GameData::getMonsterConf(talk.nRoleID);
            sprintf(str, "bust/bust_%d.png", conf->nResourceID%100);
            npc = CCSprite::create(str);
            sprintf(str,"%s：",conf->unitName);
        }
        else
        {
            CommunityNPC_t *pNpc =GameData::getNpcConf(taskconf->stageTalk.nNpcID);
            CmtNPCResource_t * pNpcResource= GameData::getNpcResourceConf(pNpc->nResourceID);
            sprintf(str, "bust/%s", pNpcResource->bustImage.c_str());
            npc = CCSprite::create(str);
            sprintf(str,"%s：",pNpc->strNPCName.c_str());
        }
        
        npc->setAnchorPoint(ccp(0,0));
        npc->setPosition(ccp(0,0));
    }
    else
    {
        UnitResource_t*resource = UnitResource::sharedInstance()->getUnitResConf(gUserData->m_heroData->nUnitID);
        sprintf(str, "bust/bust_%d.png", resource->ID%100);
        npc = CCSprite::create(str);
        npc->setFlipX(true);
        npc->setAnchorPoint(ccp(1,0));
        npc->setPosition(ccp(SCREEN_WIDTH,20));
        sprintf(str,"%s：",gUserData->nick);
    }
    nodebg->addChild(npc);
    pLabName->setString(str);
    pLabName->setColor(FONT_COLOR_GOLD);
    pLabName->setPositionY(130);
    pLabChat->addChild(pLabName);
    
    schedule(schedule_selector(MovieClip::logic), 1.f / 30.f);
    m_index++;
}

void MovieClip::logic(float cTime)
{
    wordCount++;
    pLabChat->setString(getContentByLength(wordCount).c_str());
    if (wordCount >= contentLeght)
    {
        if (m_index <= m_num)
        {
            CCSprite* end2 = CCSprite::create("public/talk_end2.png");
            end2->setPosition(ccp(sprBg->getContentSize().width - 250, 40));
            pLabChat->getParent()->addChild(end2, 10);
            
            CCSprite* end1 = CCSprite::create("public/talk_end1.png");
            end1->setPosition(ccp(sprBg->getContentSize().width - 250, 40));
            pLabChat->getParent()->addChild(end1, 10);
            end1->runAction(CCRepeatForever::create(CCSequence::create(CCMoveBy::create(0.3, ccp(0, -5)), CCMoveBy::create(0.3, ccp(0, 5)), NULL)));
        }
        
        unschedule(schedule_selector(MovieClip::logic));
    }
}

int MovieClip::getContentLength()
{
    return contentLeght;
}

//返回所需长度的字符串
std::string MovieClip::getContentByLength(int length)
{
    if (length >= contentLeght)
    {
        return strtalk;
    }
    int i = 0;
    int index = 0;
    while(index < length)
    {
        char ch = strtalk[i];
        //这里上面说过了
        if (ch > -127 && ch < 0)
        {
            i+=3;
        }
        else
        {
            i++;
        }
        index++;
    }
    
    //截取strng
    std::string str = strtalk.substr(0, i);
    return str;
}

void MovieClip::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, -128, true);
}

bool MovieClip::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (wordCount < contentLeght)
    {
        wordCount = contentLeght;
        logic(0);
    }
    else
    {
        if (m_index > m_num)
        {
            if (m_pDelegate)
            {
                m_pDelegate->onMovieOver(true);
            }
            const TaskConfig_t* taskconf = GameData::getTaskConf(m_taskID);
            if (taskconf->nFinishType == 4)
            {
                OnlineManager::sharedManager()->userSetTask(m_taskID,1);
            }
            removeFromParent();
        }
        else
        {
            play();
        }
    }
    return true;
}