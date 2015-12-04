//
//  ThrowResource.cpp
//  DragonBattle
//
//  Created by 永翔 何 on 13-5-7.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "ThrowResource.h"

using namespace std;
USING_NS_CC;

ThrowResource* ThrowResource::pSharedInstance = NULL;

ThrowResource::ThrowResource()
{
    m_throwResourceMap.clear();
}

ThrowResource::~ThrowResource()
{
    ThrowResourceMap::iterator it = m_throwResourceMap.begin();
    while (it != m_throwResourceMap.end())
    {
        if (it->second)
        {
            delete it->second;
            it->second = NULL;
        }
        it ++;
    }
    m_throwResourceMap.clear();
}

ThrowResource* ThrowResource::sharedInstance()
{
    if (!pSharedInstance)
    {
        pSharedInstance = new ThrowResource();
    }
    return pSharedInstance;
}

void ThrowResource::purgeInstance()
{
    if (pSharedInstance)
    {
        delete pSharedInstance;
        pSharedInstance = NULL;
    }
}

bool ThrowResource::initAllThrowResource()
{
    bool ret = true;
    
    const char* xmlFile = CCFileUtils::sharedFileUtils()->fullPathForFilename("config/throw_resource_config.xml").c_str();
    //xmlDocPtr doc = xmlParseFile(xmlFile);
    xmlDocPtr doc = XmlEncrypt::getXmlDocPtr(xmlFile, false);
    if (!doc)
    {
        ERROR_RETURN(("ThrowResource::initAllThrowResource xmlParseDoc throw_resource_config.xml failded"), false);
    }
    
    xmlNodePtr cur = xmlDocGetRootElement(doc);
    if (!cur)
    {
        ERROR_LOG("ThrowResource::initAllThrowResource xmlDocGetRootElement throw_resource_config.xml failded");
        ret = false;
        goto exit;
    }
    
    {
        xmlNodePtr chl = cur->xmlChildrenNode;
        while (chl)
        {
            if (!xmlStrcmp(chl->name, reinterpret_cast<const xmlChar*>("THROW_RESOURCE")))
            {
                ThrowResource_t* pConfig = new ThrowResource_t;
                DECODE_XML_PROP_INT(pConfig->nID, chl, "ID");
                
                float point[2] = {0};
                decode_xml_prop_arr_float_default(point, 2, chl, (void*)"collideStart", 0);
                pConfig->ptCollideStart = CCPoint(point[0], point[1]);
                float explodeStart[2] = {0};
                decode_xml_prop_arr_float_default(explodeStart, 2, chl, (void*)"explodeStart", 0);
                pConfig->ptExplodeStart = CCPoint(explodeStart[0], explodeStart[1]);
                float size[2] = {0};
                decode_xml_prop_arr_float_default(size, 2, chl, (void*)"contentSize", 0);
                pConfig->contentSize = CCSize(size[0], size[1]);
                decode_xml_prop_arr_float_default(size, 2, chl, (void*)"explodeSize", 0);
                pConfig->explodeSize = CCSize(size[0], size[1]);
                
                DECODE_XML_PROP_INT(pConfig->nAttackCount, chl, "attackCount");
                DECODE_XML_PROP_FLOAT(pConfig->fHitRecover, chl, "hitRecover");
                DECODE_XML_PROP_INT(pConfig->nUnderAttackIndex, chl, "underAttackIndex");
                DECODE_XML_PROP_FLOAT_DEFAULT(pConfig->fScale, chl, "fScale",1.0f);
                xmlNodePtr nodeChl = chl->xmlChildrenNode;
                while (nodeChl)
                {
                    if (!xmlStrcmp(nodeChl->name, reinterpret_cast<const xmlChar*>("CREATE_ACTION")))
                    {
                        pConfig->pCreateAction = parseAction(nodeChl);
                    }
                    else if (!xmlStrcmp(nodeChl->name, reinterpret_cast<const xmlChar*>("MOVE_ACTION")))
                    {
                        pConfig->pMoveAction = parseAction(nodeChl);
                    }
                    else if (!xmlStrcmp(nodeChl->name, reinterpret_cast<const xmlChar*>("ATTACK_ACTION")))
                    {
                        pConfig->pAttackAction = parseAction(nodeChl);
                    }
                    nodeChl = nodeChl->next;
                }
                m_throwResourceMap.insert(std::pair<uint32_t, ThrowResource_t*>(pConfig->nID, pConfig));
            }
            chl = chl->next;
        }
    }
exit:
    xmlFreeDoc(doc);
    return ret;
}

AttackEffectInfo_t* ThrowResource::parseAction(xmlNodePtr nodeChl)
{
    AttackEffectInfo_t* t = new AttackEffectInfo_t;
    char fileHead[64] = {0};
    DECODE_XML_PROP_STR(fileHead, nodeChl, "fileHead");
    t->strFileHead = fileHead;
    DECODE_XML_PROP_INT(t->nStartFrame, nodeChl, "startFrame");
    DECODE_XML_PROP_INT(t->nFrameCount, nodeChl, "frameCount");
    return t;
}

CCAnimation* ThrowResource::createAnimation(AttackEffectInfo_t* pInfo)
{
    CCArray* pFrameArray = CCArray::create();
    char frameName[32] = {0};
    
    for (int i = 0; i < pInfo->nFrameCount; i++)
    {
        sprintf(frameName, "%s%04d.png", pInfo->strFileHead.data(), pInfo->nStartFrame + i);
        
        CCSpriteFrame* pSpriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName);
        if (!pSpriteFrame)
        {
            CCLog("ThrowResource 图片未加载成功，请检查:%s", frameName);
            CCMessageBox("ThrowResource 图片未加载成功，请检查:%s", frameName);
            return NULL;
        }
        
        CCAnimationFrame* pAnimFrame = new CCAnimationFrame();
        pAnimFrame->autorelease();
        pAnimFrame->initWithSpriteFrame(pSpriteFrame, 1.0f, NULL);
        pFrameArray->addObject(pAnimFrame);
    }
    
    CCAnimation* pAnimation = CCAnimation::create(pFrameArray, 1.0 / 40, 1);
    //pAnimation->retain();
    return pAnimation;
}

CCAnimate* ThrowResource::createAnimate(AttackEffectInfo_t* pInfo)
{
    CCAnimate* pAnimate = CCAnimate::create(createAnimation(pInfo));
    return pAnimate;
}

ThrowResource_t* ThrowResource::getConf(uint32_t unitID)
{
    ThrowResourceMap::const_iterator it = m_throwResourceMap.find(unitID);
    if (it != m_throwResourceMap.end())
    {
        return it->second;
    }
    else
    {
        return NULL;
    }
}
