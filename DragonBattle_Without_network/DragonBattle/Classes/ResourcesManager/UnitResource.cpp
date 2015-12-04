//
//  AnimateResouce.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-17.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "UnitResource.h"
#include "xml_decode.h"
#include "CommonUtility.h"

using namespace std;
USING_NS_CC;

UnitResource* UnitResource::pSharedInstance = NULL;

UnitResource::UnitResource()
{
    m_unitResourceMap.clear();
}

UnitResource::~UnitResource()
{
    if (!m_unitResourceMap.empty())
    {
        UnitResourceMap::iterator it;
        for (it = m_unitResourceMap.begin(); it != m_unitResourceMap.end(); ++it)
        {
            UnitResource_t* pAnimMap = it->second;
            if (pAnimMap && pAnimMap->animMap.empty())
            {
                AnimationInfoMap::iterator animIt;
                for (animIt = pAnimMap->animMap.begin(); animIt != pAnimMap->animMap.end(); ++it)
                {
                    if (animIt->second)
                    {
                        delete animIt->second;
                        animIt->second = NULL;
                    }
                }
                pAnimMap->animMap.clear();
                
                delete pAnimMap;
                pAnimMap = NULL;
            }
        }
        m_unitResourceMap.clear();
    }
}

UnitResource* UnitResource::sharedInstance()
{
    if (!pSharedInstance)
    {
        pSharedInstance = new UnitResource();
    }
    return pSharedInstance;
}

void UnitResource::purgeInstance()
{
    if (pSharedInstance)
    {
        delete pSharedInstance;
        pSharedInstance = NULL;
    }
}

bool UnitResource::loadUnitResouceFromFile()
{
    bool ret = true;
    const char* xmlFile = CCFileUtils::sharedFileUtils()->fullPathForFilename("config/sprite_resource.xml").c_str();
    string pach = CCFileUtils::sharedFileUtils()->getWritablePath() + "sprite_resource.xml";
    xmlDocPtr doc = XmlEncrypt::getXmlDocPtr(xmlFile,false);
    if (!doc)
    {
        ERROR_RETURN(("UnitResource::loadUnitResouceFromFile xmlParseDoc sprite_resource.xml failded"), false);
    }
    
    xmlNodePtr cur = xmlDocGetRootElement(doc);
    if (!cur)
    {
        ERROR_LOG("UnitResource::loadUnitResouceFromFile xmlDocGetRootElement sprite_resource.xml failded");
        ret = false;
        goto exit;
    }
    
    cur = cur->xmlChildrenNode;
    while (cur)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar*)"SPRITE")))
        {
            UnitResource_t* pUnitRes = new UnitResource_t();
            DECODE_XML_PROP_INT(pUnitRes->ID, cur, "ID");
            DECODE_XML_PROP_FLOAT_DEFAULT(pUnitRes->fScale, cur, "scale", 1.0f);
            char buff[32] = {0};
            DECODE_XML_PROP_STR(buff, cur, "resFile");
            pUnitRes->strFile = buff;
            DECODE_XML_PROP_STR(buff, cur, "roleFile");
            pUnitRes->strRoleFile = buff;
            DECODE_XML_PROP_STR(buff, cur, "headImage");
            pUnitRes->headImage = buff;
            
            DECODE_XML_PROP_STR_DEFAULT(buff, cur, "music","");
            pUnitRes->strmusic = buff;
            
            DECODE_XML_PROP_STR_DEFAULT(buff, cur, "diemusic","");
            pUnitRes->strDiemusic = buff;
            
            float contentSize[2] = {0};
            float offPos[2] = {0};
            decode_xml_prop_arr_float_default(contentSize, size_t(2), cur, (void*)"contentSize", float(0));
            decode_xml_prop_arr_float_default(offPos, 2, cur, (void*)"offPos", float(0));
            
            pUnitRes->contentSize = CCSize(contentSize[0], contentSize[1]);
            pUnitRes->offPos = CCPoint(offPos[0], offPos[1]);
            
            decode_xml_prop_float_default(&(pUnitRes->fMoveSpeed), cur, (void*)"moveSpeed", 4);
            decode_xml_prop_float_default(&(pUnitRes->fJumpSpeed), cur, (void*)"jumpSpeed", 10.5);
            
            xmlNodePtr chl = cur->xmlChildrenNode;
            while (chl)
            {
                if (!xmlStrcmp(chl->name, reinterpret_cast<const xmlChar*>("ANIMAT")))
                {
                    AnimationInfo_t* anim = new AnimationInfo_t();
                    int kType;
                    DECODE_XML_PROP_INT(kType, chl, "ActionType");
                    DECODE_XML_PROP_INT(anim->nStartFrame, chl, "frameStart");
                    DECODE_XML_PROP_INT(anim->nFrameCount, chl, "frameCount");
                    pUnitRes->animMap.insert(std::pair<kAnimationType, AnimationInfo_t*>((kAnimationType)kType, anim));
                }
                else if (!xmlStrcmp(chl->name, reinterpret_cast<const xmlChar*>("SOURCE_LIST")))
                {
                    this->loadUnitResource(chl, &(pUnitRes->fileResourceVec));
                }
                else if (!xmlStrcmp(chl->name, reinterpret_cast<const xmlChar*>("AIR_ATTACK")))
                {
                    this->loadAttackAction(chl, &(pUnitRes->airAttack));
                }
                else if (!xmlStrcmp(chl->name, reinterpret_cast<const xmlChar*>("ATTACK")))
                {
                    this->loadAttackAction(chl, &(pUnitRes->attackVec));
                }
                else if (!xmlStrcmp(chl->name, reinterpret_cast<const xmlChar*>("SKILL")))
                {
                    xmlNodePtr skillChl = chl->xmlChildrenNode;
                    while (skillChl)
                    {
                        if (!xmlStrcmp(skillChl->name, reinterpret_cast<const xmlChar*>("SKILL_ACTION")))
                        {
                            SkillInfo_t* skill = new SkillInfo_t();
                            DECODE_XML_PROP_INT(skill->nID, skillChl, "ID");
                            char iconBuff[64] = {0};
                            DECODE_XML_PROP_STR(iconBuff, skillChl, "iconEmpty");
                            skill->strSkillIconEmpty = iconBuff;
                            DECODE_XML_PROP_STR(iconBuff, skillChl, "iconFull");
                            skill->strSkillIconFull = iconBuff;
                            skill->attackVec = AttackInfoVec();
                            this->loadAttackAction(skillChl, &(skill->attackVec));
                            loadUnitSkillResource(skillChl,&(skill->fileResourceVec));
                            pUnitRes->skillMap.insert(std::pair<int,SkillInfo_t*>(skill->nID, skill));
                        }
                        skillChl = skillChl->next;
                    }
                }
                else if (!xmlStrcmp(chl->name, reinterpret_cast<const xmlChar*>("BE_HIT")))
                {
                    pUnitRes->underAttacks.clear();
                    xmlNodePtr beHitChl = chl->xmlChildrenNode;
                    while (beHitChl)
                    {
                        if (!xmlStrcmp(beHitChl->name, reinterpret_cast<const xmlChar*>("ANIMAT")))
                        {
                            AnimationInfo_t* anim = new AnimationInfo_t();
                            DECODE_XML_PROP_INT(anim->nStartFrame, beHitChl, "frameStart");
                            DECODE_XML_PROP_INT(anim->nFrameCount, beHitChl, "frameCount");
                            decode_xml_prop_float_default(&(anim->fSelfSpeedX), beHitChl, (void*)"selfSpeedX", 0);
                            decode_xml_prop_float_default(&(anim->fSelfSpeedAddX), beHitChl, (void*)"selfSpeedAddX", 0);
                            decode_xml_prop_float_default(&(anim->fSelfSpeedY), beHitChl, (void*)"selfSpeedY", 0);
                            decode_xml_prop_float_default(&(anim->fSelfSpeedAddY), beHitChl, (void*)"selfSpeedAddY", JUMP_ACCELERATED_SPEED_Y);
                            pUnitRes->underAttacks.push_back(anim);
                        }
                        beHitChl = beHitChl->next;
                    }
                }
                
                chl = chl->next;
            }
            m_unitResourceMap.insert(std::pair<uint32_t, UnitResource_t*>(pUnitRes->ID, pUnitRes));
        }
        cur = cur->next;
    }
    
exit:
    xmlFreeDoc(doc);
    return ret;
}

bool UnitResource::loadAttackAction(xmlNodePtr chl, AttackInfoVec* pAttackVec)
{
    pAttackVec->clear();
    xmlNodePtr cur = chl->xmlChildrenNode;
    while (cur)
    {
        if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar*>("ATTACK_ACTION")))
        {
            AttackInfo_t* attack = new AttackInfo_t();
            decode_xml_prop_float_default(&(attack->fOutAttackCorrect), cur, (void*)"outAttackCorrect", 0);
            decode_xml_prop_float_default(&(attack->fInAttackCorrect), cur, (void*)"inAttackCorrect", 0);
            float attackRect[4];
            decode_xml_prop_arr_float_default(attackRect, size_t(4), cur, (void*)"attackRect", float(0));
            attack->rcAttackRect = CCRect(attackRect[0], attackRect[1], attackRect[2], attackRect[3]);
            
            decode_xml_prop_float_default(&(attack->hitRecover), cur, (void*)"hitRecover", 0);
            DECODE_XML_PROP_INT(attack->isFallDown, cur, "fallDown");
            decode_xml_prop_float_default(&(attack->fSpeedX), cur, (void*)"speedX", 0);
            decode_xml_prop_float_default(&(attack->fSpeedY), cur, (void*)"speedY", 0);
            decode_xml_prop_float_default(&attack->fSpeedAddY, cur, (void*)"speedAddY", -0.5);
            //decode_xml_prop_float_default(&(attack->fMoveX), cur, (void*)"moveX", 0);
            DECODE_XML_PROP_INT(attack->nUnderAttackIndex, cur, "underAttackIdx");
            DECODE_XML_PROP_INT_DEFAULT(attack->nSharkType, cur, "sharkType", -1);
            DECODE_XML_PROP_INT_DEFAULT(attack->isAngry, cur, "isAngry", 1);
            char music[64] = {0};
            DECODE_XML_PROP_STR_DEFAULT(music, cur, "music","");
            attack->music = music;
            int tmp = 0;
            DECODE_XML_PROP_INT_DEFAULT(tmp, cur, "isBlinkScreen", 0);
            attack->isBlinkScreen = (bool)tmp;
            
            xmlNodePtr animatChl = cur->xmlChildrenNode;
            while (animatChl)
            {
                if (!xmlStrcmp(animatChl->name, reinterpret_cast<const xmlChar*>("ANIMAT")))
                {
                    char buffAction[64] = {0};
                    DECODE_XML_PROP_STR(buffAction, animatChl, "Action");
                    
                    if (!strcmp(buffAction, "EffectOnAttack") || !strcmp(buffAction, "EffectOnEnemy"))
                    {//攻击特效
                        AttackEffectInfo_t* attackEffect = new AttackEffectInfo_t();
                        char fileHead[64] = {0};
                        DECODE_XML_PROP_STR(fileHead, animatChl, "fileHead");
                        attackEffect->strFileHead = fileHead;
                        float ptOffset[2] = {0};
                        decode_xml_prop_arr_float_default(ptOffset, 2, animatChl, (void*)"ptOffset", 0);
                        attackEffect->ptOffect = CCPoint(ptOffset[0], ptOffset[1]);
                        DECODE_XML_PROP_INT(attackEffect->nStartFrame, animatChl, "frameStart");
                        DECODE_XML_PROP_INT(attackEffect->nFrameCount, animatChl, "frameCount");
                        DECODE_XML_PROP_INT_DEFAULT(attackEffect->isScale, animatChl, "isScale",0);
                        
                        if (!strcmp(buffAction, "EffectOnAttack"))
                        {
                            attack->attactEffectVec.push_back(attackEffect);
                        }
                        else
                        {
                            attack->attackHitVec.push_back(attackEffect);
                        }
                    }
                    else
                    {
                        AnimationInfo_t* pAnimation = new AnimationInfo_t();
                        DECODE_XML_PROP_INT(pAnimation->nStartFrame, animatChl, "frameStart");
                        DECODE_XML_PROP_INT(pAnimation->nFrameCount, animatChl, "frameCount");
                        decode_xml_prop_float_default(&(pAnimation->fFrameTime), animatChl, (void*)"frameTime", 0);
                        decode_xml_prop_float_default(&(pAnimation->fSelfSpeedX), animatChl, (void*)"selfSpeedX", 0);
                        decode_xml_prop_float_default(&(pAnimation->fSelfSpeedAddX), animatChl, (void*)"selfSpeedAddX", 0);
                        decode_xml_prop_float_default(&(pAnimation->fSelfSpeedY), animatChl, (void*)"selfSpeedY", 0);
                        decode_xml_prop_float_default(&(pAnimation->fSelfSpeedAddY), animatChl, (void*)"selfSpeedAddY", JUMP_ACCELERATED_SPEED_Y);
                        attack->attackAnimVec.push_back(pAnimation);
                    }
                }
                else if (!xmlStrcmp(animatChl->name, reinterpret_cast<const xmlChar*>("THROW")))
                {
                    char buffAction[64] = {0};
                    DECODE_XML_PROP_STR(buffAction, animatChl, "throwMode");
                    attack->nThrowMode = (strcmp(buffAction, "kThrowSequence") == 0) ? kThrowSequence : kThrowMultiOnceTime;
                    DECODE_XML_PROP_INT_DEFAULT(attack->nThrowintervalTime, animatChl,"intervaltime", 3);
                    int throwId[50] = {0};
                    int throwCnt = decode_xml_prop_arr_int_default(throwId, 50, animatChl, (void*)"throwIdVec", 0);
                    for (int i = 0; i < throwCnt; i++)
                    {
                        attack->throwVec.push_back(throwId[i]);
                    }
                }
                animatChl = animatChl->next;
            }
            pAttackVec->push_back(attack);
        }
        cur = cur->next;
    }
    return true;
}

bool UnitResource::loadUnitSkillResource(xmlNodePtr chl,std::vector<std::string>* pFileVec)
{
    xmlNodePtr cur = chl->xmlChildrenNode;
    while (cur)
    {
        if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar*>("SOURCE_LIST")))
        {
            xmlNodePtr curl = cur->xmlChildrenNode;
            char strFile[64] = {0};
            while (curl)
            {
                if ((!xmlStrcmp(curl->name, (const xmlChar*)"SOURCE")))
                {
                    DECODE_XML_PROP_STR(strFile, curl, "file");
                    pFileVec->push_back(strFile);
                }
                curl = curl->next;
            }
        }
        cur = cur->next;
    }
    return true;
}

bool UnitResource::loadUnitResource(xmlNodePtr chl,std::vector<std::string>* pFileVec)
{
    xmlNodePtr cur = chl->xmlChildrenNode;
    char strFile[64] = {0};
    while (cur)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar*)"SOURCE")))
        {
            DECODE_XML_PROP_STR(strFile, cur, "file");
            pFileVec->push_back(strFile);
        }
        cur = cur->next;
    }
    return true;
}

CCAnimate* UnitResource::createAnimate(uint32_t unitID, kAnimationType animType)
{
    CCAnimate * pAnimate =  CCAnimate::create(createAnimation(unitID, animType));
    
    return pAnimate;
}

CCAnimation* UnitResource::createAnimation(uint32_t unitID, kAnimationType animType)
{
    if (m_unitResourceMap.find(unitID) == m_unitResourceMap.end())
    {
        CCLOG("should right player type!");
        return NULL;
    }
    
    UnitResource_t* pUnitAnim = m_unitResourceMap[unitID];
    if (!pUnitAnim || pUnitAnim->animMap.find(animType) == pUnitAnim->animMap.end() || !pUnitAnim->animMap[animType])
    {
        CCLOG("should right animation type!");
        return NULL;
    }
    
    CCArray* pFrameArray = CCArray::create();
    char frameName[32] = {0};
    
    for (int i = 0; i < pUnitAnim->animMap[animType]->nFrameCount; i++)
    {
        sprintf(frameName, "%s%04d.png", pUnitAnim->strFile.data(), pUnitAnim->animMap[animType]->nStartFrame + i);

        CCSpriteFrame* pSpriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName);
        if (!pSpriteFrame)
        {
            CCLog("UnitResource1 图片未加载成功，请检查:%s", frameName);
            CCMessageBox("UnitResource1 图片未加载成功，请检查:%s", frameName);
            return NULL;
        }
        pFrameArray->addObject(pSpriteFrame);
    }
    
    CCAnimation* pAnimation = CCAnimation::createWithSpriteFrames(pFrameArray, ANIMAT_DELAY_PER_UNIT);
    return pAnimation;
}


CCAnimation* UnitResource::createAnimation(uint32_t unitID, AnimationInfo_t* pInfo)
{
    if (m_unitResourceMap.find(unitID) == m_unitResourceMap.end())
    {
        CCLOG("should right player type!");
        return NULL;
    }
    
    UnitResource_t* pUnitAnim = m_unitResourceMap[unitID];
    CCArray* pFrameArray = CCArray::create();
    char frameName[32] = {0};
    
    for (int i = 0; i < pInfo->nFrameCount; i++)
    {
        sprintf(frameName, "%s%04d.png", pUnitAnim->strFile.data(), pInfo->nStartFrame + i);
        
        CCSpriteFrame* pSpriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName);
        if (!pSpriteFrame)
        {
            CCLog("UnitResource2 图片未加载成功，请检查:%s", frameName);
            CCMessageBox("UnitResource2 图片未加载成功，请检查:%s", frameName);
            return NULL;
        }
        pFrameArray->addObject(pSpriteFrame);
    }
    
    CCAnimation* pAnimation = CCAnimation::createWithSpriteFrames(pFrameArray, ANIMAT_DELAY_PER_UNIT);
    return pAnimation;
}

CCAnimate* UnitResource::createAnimate(uint32_t unitID, AnimationInfo_t* pInfo)
{
    CCAnimation* pAnimation = createAnimation(unitID, pInfo);
    CCAnimate * pAnimate =  CCAnimate::create(pAnimation);
    return pAnimate;
}

CCAnimation* UnitResource::createAnimation(AttackEffectInfo_t* pInfo)
{
    CCArray* pFrameArray = CCArray::create();
    char frameName[32] = {0};
    
    for (int i = 0; i < pInfo->nFrameCount; i++)
    {
        sprintf(frameName, "%s%04d.png", pInfo->strFileHead.data(), pInfo->nStartFrame + i);
        
        CCSpriteFrame* pSpriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName);
        pFrameArray->addObject(pSpriteFrame);
    }
    
    CCAnimation* pAnimation = CCAnimation::createWithSpriteFrames(pFrameArray, ANIMAT_DELAY_PER_UNIT);
    return pAnimation;
}

CCAnimate*   UnitResource::createAnimate(AttackEffectInfo_t* pInfo)
{
    CCAnimate * pAnimate =  CCAnimate::create(createAnimation(pInfo));
    return pAnimate;
}

UnitResource_t* UnitResource::getUnitResConf(uint32_t unitID)
{
    if (m_unitResourceMap.find(unitID) == m_unitResourceMap.end())
    {
        CCLOG("should right player type!");
        return NULL;
    }
    return m_unitResourceMap[unitID];
}

CCSprite* UnitResource::getUnitSprite(uint32_t unitID)
{
    if (m_unitResourceMap.find(unitID) == m_unitResourceMap.end())
    {
        CCLOG("should right player type!");
        return NULL;
    }
    
    return CCSprite::createWithSpriteFrameName(m_unitResourceMap[unitID]->strRoleFile.data());
}