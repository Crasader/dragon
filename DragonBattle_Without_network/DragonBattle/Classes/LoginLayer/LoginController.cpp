//
//  LoginManager.cpp
//  DragonBattle
//
//  Created by zeng xiaolong on 13-6-22.
//
//

#include "LoginController.h"
#include "CCBLoginLayer.h"
#include "CCBSelectServerLayer.h"
#include "CCBCreateRoleLayer.h"
#include "CCBSelectRoleLayer.h"
#include "CCBStartLayer.h"
#include "CheckResLayer.h"


LoginController* LoginController::pSharedInstance = NULL;
LoginController::LoginController()
{
    m_arrLayers = CCArray::create();
    CC_SAFE_RETAIN(m_arrLayers);
    m_loginLayer = NULL;
    m_pSverList = NULL;
    m_iLastedSver = -1;
}

LoginController::~LoginController()
{
    CC_SAFE_RELEASE_NULL(m_arrLayers);
    CC_SAFE_RELEASE_NULL(m_pSverList);
}

LoginController* LoginController::sharedInstance()
{
    if (!pSharedInstance)
    {
        pSharedInstance = new LoginController();
    }
    return pSharedInstance;
}

LoginLayer* LoginController::creatLayer()
{
    m_arrLayers->removeAllObjects();
    m_loginLayer = LoginLayer::create();
    
#if 1
    pushLayer(LOGIN);
#else
    CheckResLayer* pLayer = CheckResLayer::create();
    m_loginLayer->addChild(pLayer);
#endif
    
    CCLog("%f, %f", m_loginLayer->getContentSize().width, m_loginLayer->getContentSize().height);
    
    return m_loginLayer;
}

void LoginController::removeLoginLayer()
{
    m_arrLayers->removeAllObjects();
    m_loginLayer = NULL;
    CC_SAFE_RELEASE(pSharedInstance);
    pSharedInstance = NULL;
}

CCLayer* LoginController::pushLayer(LOGINLAYERTYPE type)
{
    if (m_loginLayer == NULL)
    {
        return NULL;
    }
    m_curLayer = getLayerByType(type);
    //m_curLayer->setPosition(ccp((SCREEN_WIDTH - m_curLayer->getContentSize().width) / 2, (SCREEN_HEIGTH - m_curLayer->getContentSize().height) / 2));
    m_loginLayer->addChild(m_curLayer);

    m_arrLayers->addObject(m_curLayer);
    return m_curLayer;
}

CCLayer *LoginController::getLayerByType(LOGINLAYERTYPE type)
{
    CCParticleSystemQuad* particle = CCParticleSystemQuad::create("effect/flower_fly_effect.plist");

    CCLayer *layer = NULL;
    switch (type)
    {
        case REGIST:
        {
            
        }
            break;
        case LOGIN:
        {
            layer = (CCLayer*)CREATE_CCB_NODE("CCBLoginLayer", CCBLoginLayerLoader::loader(), "CCBLoginResource/CCBLoginLayer.ccbi", m_loginLayer);
        }
            break;
        case SELECTSERVER:
        {
            layer = (CCLayer*)CREATE_CCB_NODE("CCBSelectServerLayer", CCBSelectServerLayerLoader::loader(), "CCBLoginResource/CCBSelectServerLayer.ccbi", m_loginLayer);
        }
            break;
        case CREATEROLE:
        {
            layer = (CCLayer*)CREATE_CCB_NODE("CCBCreateRoleLayer", CCBCreateRoleLayerLoader::loader(), "CCBLoginResource/CCBCreateRoleLayer.ccbi", m_loginLayer);
        }
            break;
        case GAME_START:
        {
            layer = (CCLayer*)CREATE_CCB_NODE("CCBStartLayer", CCBStartLayerLoader::loader(), "CCBLoginResource/CCBStartLayer.ccbi", m_loginLayer);
            break;
        }
        case SELECTROLE:
        {
            layer = (CCLayer*)CREATE_CCB_NODE("CCBSelectRoleLayer", CCBSelectRoleLayerLoader::loader(), "CCBLoginResource/CCBSelectRoleLayer.ccbi", m_loginLayer);
            break;
        }
            
        case UPDATE_RESC:
        {
            layer = CheckResLayer::create();
            break;
        }
        default:
            break;
    }
    
    if (layer)
    {
        if (layer->getChildByTag(FLOWER_TAG))
        {
            layer->removeChildByTag(FLOWER_TAG);
        }
        
        if (type == LOGIN || type == START || type == REGIST)
        {
            layer->addChild(particle);
            particle->setTag(FLOWER_TAG);
        }
        
    }
    
    return layer;
}

void LoginController::popLayer()
{
    if (m_arrLayers->count()>0)
    {
        CCLayer*layer = (CCLayer*)(m_arrLayers->lastObject());
        layer->removeFromParent();
        m_arrLayers->removeLastObject();
    }
}

CCDictionary* LoginController::getLastSvrInfo()
{
    CCDictionary* pDict = NULL;
    for (int i = 0; m_pSverList && i < m_pSverList->count(); i ++)
    {
        CCDictionary *serverinfo = (CCDictionary*)m_pSverList->objectAtIndex(i);
        CCString *strid = (CCString*)serverinfo->objectForKey("zone_id");
        
        if (m_iLastedSver == strid->intValue())
        {
            pDict = serverinfo;
            break;
        }
    }
    return pDict;
}

CCLayer* LoginController::getCurrentLayer()
{
    return m_curLayer;
}
