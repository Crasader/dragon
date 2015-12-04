//
//  UiResources.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-18.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "UiResources.h"
#include "Resources.h"

USING_NS_CC;

UIResources* UIResources::pSharedInstance = NULL;

UIResources::UIResources()
{
    m_pResourceMap.clear();
}

UIResources::~UIResources()
{
    if (!m_pResourceMap.empty())
    {
        UIResourceMap::iterator it;
        for (it = m_pResourceMap.begin(); it != m_pResourceMap.end(); it++)
        {
            delete it->second;
            it->second = NULL;
        }
    }
    m_pResourceMap.clear();
}

void UIResources::insertResouceInfo(uint32_t ID, const char* name, CCPoint point, CCPoint achPt)
{
    UIResourceInfo_t* pResInfo = new UIResourceInfo_t(ID, name, point, achPt);
    m_pResourceMap.insert(std::pair<uint32_t, UIResourceInfo_t*>(pResInfo->ID, pResInfo));
}

void UIResources::initUIResources()
{
#define INSERT_RESOUCE_FOR_IMG(_ID, _name, _pt, _achPt, _scale)\
{\
UIResourceInfo_t* pResInfo = new UIResourceInfo_t(_ID, _name, _pt, _achPt, _scale);\
m_pResourceMap.insert(std::pair<uint32_t, UIResourceInfo_t*>(pResInfo->ID, pResInfo));\
}
    
#define INSERT_RESOUCE_FOR_MENU(_ID, _name, _pt, _achPt, _scale)\
{\
UIResourceInfo_t* pResInfo = new UIResourceInfo_t(_ID, _name, _pt,  _achPt, _scale, kUIResForMenu);\
m_pResourceMap.insert(std::pair<uint32_t, UIResourceInfo_t*>(pResInfo->ID, pResInfo));\
}
    
    //#define LOGIN_BKG_ID             10001
    INSERT_RESOUCE_FOR_IMG(LOGIN_BKG_ID, "login_bkg.png", ccp(480, 320), ccp(0.5, 0.5), 1.0);
    
    INSERT_RESOUCE_FOR_MENU(UI_LEFT_ID, "BattleUI/left-auto.png", ccp(90, 90), ccp(0.5, 0.5), 1.0);
    INSERT_RESOUCE_FOR_MENU(UI_LEFT_SEL_ID, "BattleUI/left-auto.png", CCPoint(), ccp(0.5, 0.5), 1.0);
    INSERT_RESOUCE_FOR_MENU(UI_RIGHT_ID, "BattleUI/right-auto.png", ccp(250, 90), ccp(0.5, 0.5), 1.0);
    INSERT_RESOUCE_FOR_MENU(UI_RIGHT_SEL_ID, "BattleUI/right-auto.png", CCPoint(), ccp(0.5, 0.5), 1.0);
    INSERT_RESOUCE_FOR_MENU(UI_ATTACK_BTN_ID, "BattleUI/attack-auto.png", ccp(SCREEN_WIDTH - 100, 100), ccp(0.5, 0.5), 1.0);
    INSERT_RESOUCE_FOR_MENU(UI_ATTACK_BTN_SEL_ID, "BattleUI/attack-auto.png", CCPoint(), ccp(0.5, 0.5), 1.0);
    INSERT_RESOUCE_FOR_MENU(UI_JUMP_BTN_ID, "BattleUI/jump-auto.png", ccp(SCREEN_WIDTH - 78, 260), ccp(0.5, 0.5), 0.90);
    INSERT_RESOUCE_FOR_MENU(UI_JUMP_BTN_SEL_ID, "BattleUI/jump-auto.png", CCPoint(), ccp(0.5, 0.5), 0.90);
    INSERT_RESOUCE_FOR_MENU(UI_ROLL_BTN_ID, "BattleUI/rush-auto.png", ccp(SCREEN_WIDTH - 215, 212), ccp(0.5, 0.5), 0.85);
    INSERT_RESOUCE_FOR_MENU(UI_ROLL_BTN_SEL_ID, "BattleUI/rush-auto.png", CCPoint(), ccp(0.5, 0.5), 0.85);
    INSERT_RESOUCE_FOR_MENU(UI_SKILL_NULL_ID, "BattleUI/skill-null-auto.png", ccp(SCREEN_WIDTH - 255, 82), ccp(0.5, 0.5), 1.0);
    INSERT_RESOUCE_FOR_MENU(UI_SKILL_NULL_SEL_ID, "BattleUI/skill-null-hover.png", CCPoint(), ccp(0.5, 0.5), 1.0);
    INSERT_RESOUCE_FOR_MENU(UI_SKILL1_NULL_ID, "BattleUI/skill-null-auto.png", ccp(SCREEN_WIDTH - 385, 82), ccp(0.5, 0.5), 1.0);
    INSERT_RESOUCE_FOR_MENU(UI_SKILL1_NULL_SEL_ID, "BattleUI/skill-null-hover.png", CCPoint(), ccp(0.5, 0.5), 1.0);
    
    /*
     CCLOG("initUIResources size %d", m_pResourceMap.size());
     UIResourceMap::iterator it;
     for (it = m_pResourceMap.begin(); it != m_pResourceMap.end(); it++)
     {
     CCLOG("init ID:%d name:%s", it->first, it->second->strName.data());
     }*/
    
#undef INSERT_RESOUCE_FOR_IMG
#undef INSERT_RESOUCE_FOR_MENU
}

UIResources* UIResources::sharedInstance()
{
    if (!pSharedInstance)
    {
        pSharedInstance = new UIResources();
    }
    return pSharedInstance;
}

void UIResources::purgeInstance()
{
    if (pSharedInstance)
    {
        delete pSharedInstance;
        pSharedInstance = NULL;
    }
}

UIResourceInfo_t* UIResources::getUIResourceWithID(uint32_t ID)
{
    if (m_pResourceMap.empty())
    {
        return NULL;
    }
    
    UIResourceMap::iterator it = m_pResourceMap.find(ID);
    if (it == m_pResourceMap.end())
    {
        CCLOG("can not find resource ID");
        return NULL;
    }
    return it->second;
}

CCSprite* UIResources::createUISpriteWithID(uint32_t ID)
{
    UIResourceInfo_t* pRes = this->getUIResourceWithID(ID);
    CCSprite* pSprite = CCSprite::create(pRes->strName.c_str());
    if (pRes->nResType == kUIResForSprite)
    {
        pSprite->setPosition(pRes->pt);
        pSprite->setAnchorPoint(pRes->anchorPt);
        pSprite->setScale(pRes->fScale);
        pSprite->setFlipX(pRes->bFlipX);
        pSprite->setFlipY(pRes->bFilpY);
    }
    
    return pSprite;
}