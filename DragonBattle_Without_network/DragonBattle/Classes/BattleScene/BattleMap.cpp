//
//   BattleMap.cpp
//  DragonBattle
//
//  Created by zeng xiaolong on 13-12-3.
//
//

#include "BattleMap.h"
#include "AudioResource.h"
#include "BattleMapUnits.h"
BattleMap::BattleMap()
{
    m_bMarquee = false;
    m_pFarBg = NULL;
    m_pMidBg  = NULL;
    m_pLandBG = NULL;
    m_pNearBg = NULL;
    
    i = 0;
    directY = true;
}

BattleMap::~BattleMap()
{

}

BattleMap *BattleMap::createWithID(int MapConfID,BattleMapUnits *pMapUnits)
{
    BattleMap *pRet = new BattleMap();
    if (pRet && pRet->initWithID(MapConfID,pMapUnits))
    {
        pRet->autorelease();
        return pRet;
    }
    if (pRet)
    {
        delete pRet;
        pRet = NULL;
    }
    return NULL;
}

bool BattleMap::initWithID(int MapConfID,BattleMapUnits *pMapUnits)
{
    if (!CCNode::init())
    {
        return false;
    }
    m_pMapUnits = pMapUnits;
    
    SceneConf_t *sceneConf = GameData::getSceneConf(MapConfID);
    m_bMarquee = sceneConf->bMarquee;
    if(sceneConf->strMusic.length()>0)
    {
        char strmusic [64]= {0};
        sprintf(strmusic, "music/%s",sceneConf->strMusic.c_str());
        AudioResource::shareAudioResource()->playBgSound(strmusic);
    }
    m_pFarBg = BattleSubBGNode::createNearScene(&(sceneConf->farFileVec),sceneConf->fFarBgWidth);
    if (m_pFarBg)
    {
        m_pMapUnits->addChild(m_pFarBg, kFarLayerOrder);
        m_pFarBg->setPosition(ccp(0, sceneConf->fFarBgPosY));
    }
    
    m_pMidBg = BattleSubBGNode::createNearScene(&(sceneConf->midFileVec),sceneConf->fMidBgWidth);
    if (m_pMidBg)
    {
        m_pMapUnits->addChild(m_pMidBg, kMidLayerOrder);
        m_pMidBg->setPosition(ccp(0, sceneConf->fMidBgPosY));
    }
    
    // 必须有地表层
    m_pLandBG = BattleSubBGNode::createNearScene(&(sceneConf->landFileVec),sceneConf->fLandBgWidth);
    m_pMapUnits->addChild(m_pLandBG, kBackgroudOrder);
    m_pLandBG->setPosition(ccp(0, sceneConf->fLandBgPosY));
    
    m_pNearBg = BattleSubBGNode::createNearScene(&(sceneConf->nearFileVec), sceneConf->fNearBgWidth);
    if (m_pNearBg)
    {
        m_pMapUnits->addChild(m_pNearBg, kNearLayerOrder);
        m_pNearBg->setPosition(ccp(0, 0));
    }
    if (m_bMarquee)
    {
        CCAnimation *ani = CRUtils::createAni("90_effect_", 1, 8);
        CCSprite *pSprite1 = CCSprite::create();
        pSprite1->runAction(CCRepeatForever::create(CCAnimate::create(ani)));
        pSprite1->setPosition(ccp(300,300));
        m_pMapUnits->addChild(pSprite1, kFarLayerOrder);
        CCSprite *pSprite2 = CCSprite::create();
        pSprite2->runAction(CCRepeatForever::create(CCAnimate::create(ani)));
        pSprite2->setPosition(ccp(800,280));
        m_pMapUnits->addChild(pSprite2, kFarLayerOrder);
    }
    return true;
}

void BattleMap::resetBackGroud(int fMapLength)
{
    CCPoint mapPos = m_pMapUnits->getPosition();
    
    if (!m_bMarquee)//不是跑马灯
    {
        if (m_pFarBg)
        {
            CCPoint farBgPt = m_pFarBg->getPosition();
            farBgPt.x = -mapPos.x / (fMapLength - SCREEN_WIDTH) * (m_pFarBg->getNodeWidth() - SCREEN_WIDTH);
            farBgPt.x = -mapPos.x - farBgPt.x;
            m_pFarBg->setPosition(farBgPt);
        }
        
        if (m_pMidBg)
        {
            CCPoint midBgPt = m_pMidBg->getPosition();
            midBgPt.x = -mapPos.x / (fMapLength - SCREEN_WIDTH) * (m_pMidBg->getNodeWidth() - SCREEN_WIDTH);
            midBgPt.x = -mapPos.x - midBgPt.x;
            m_pMidBg->setPosition(midBgPt);
        }
        
        if (m_pNearBg)
        {
            CCPoint nearPt = m_pNearBg->getPosition();
            nearPt.x = -mapPos.x / (fMapLength - SCREEN_WIDTH) * (m_pNearBg->getNodeWidth() - SCREEN_WIDTH);
            nearPt.x = -mapPos.x - nearPt.x;
            m_pNearBg->setPosition(nearPt);
        }
    }
    else
    {
        if (m_pFarBg)
        {
            recreateBackGround(m_pFarBg,2);
        }
        if (m_pMidBg)
        {
            recreateBackGround(m_pMidBg,7);
        }
        if (m_pNearBg)
        {
            recreateBackGround(m_pNearBg,5);
        }
    }
}

void  BattleMap::recreateBackGround(BattleSubBGNode* bg,float speedX)
{
    bg->setPositionX(bg->getPositionX()-speedX);
    CCArray *arr = bg->getChildren();
    CCObject *obj = NULL;
    CCARRAY_FOREACH(arr, obj)
    {
        CCNode *node = (CCNode*)obj;
        CCPoint poi = bg->convertToWorldSpace(node->getPosition());
        if (poi.x<0 && poi.x+node->getContentSize().width<-SCREEN_WIDTH/2)//左边屏幕外移除
        {
            node->removeFromParent();
        }
    }
    CCArray *arr1 = bg->getChildren();
    if (arr1->count()<=2)
    {
        CCSprite *spr = (CCSprite*)arr->objectAtIndex(arr->count()-1);
        CCSprite *newspr = CCSprite::createWithSpriteFrame(spr->displayFrame());
        newspr->setAnchorPoint(CCPointZero);
        newspr->setPosition(ccp(spr->getPositionX()+spr->getContentSize().width-1,spr->getPositionY()));
        bg->addChild(newspr);
    }
}

float BattleMap::getBattleMapLength()
{
    return m_pLandBG->getNodeWidth();
}