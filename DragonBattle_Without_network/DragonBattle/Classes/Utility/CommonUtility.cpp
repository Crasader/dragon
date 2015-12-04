//
//  CommonUtility.cpp
//  DragonBattle
//
//  Created by 永翔 何 on 13-5-23.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "CommonUtility.h"
#include <sys/time.h>
#include <string>
#include "EffectResource.h"
#include "CRUtils.h"
#include "XYButton.h"
#include "ParticleResource.h"
#include "ItemSprite.h"
#include "EffectResource.h"

using namespace cocos2d::extension;

const long CommonUtility::getCurrentMillisecond()
{    
    struct timeval tv;    
    gettimeofday(&tv, NULL);    
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;    
}

void CommonUtility::saveMessage(const char* pPrefix, int nServerId, int nUserId, int nIndex, messageBody_t& body)
{
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
    char str[128] = {0};
    sprintf(str, "%s%dMessage%dUserID%d", pPrefix, nServerId, nUserId, nIndex);
    userDefault->setIntegerForKey(str, body.nUserID);
    sprintf(str, "%s%dMessage%dTime%d", pPrefix, nServerId, nUserId, nIndex);
    userDefault->setIntegerForKey(str, body.nTime);
    sprintf(str, "%s%dMessage%dNick%d", pPrefix, nServerId, nUserId, nIndex);
    userDefault->setStringForKey(str, body.nick);
    sprintf(str, "%s%dMessage%dLevel%d", pPrefix, nServerId, nUserId, nIndex);
    userDefault->setIntegerForKey(str, body.nLevel);
    sprintf(str, "%s%dMessage%dUnitID%d", pPrefix, nServerId, nUserId, nIndex);
    userDefault->setIntegerForKey(str, body.nUnitID);
    sprintf(str, "%s%dMessage%dVipLevel%d", pPrefix, nServerId, nUserId, nIndex);
    userDefault->setIntegerForKey(str, body.nVipLevel);
    sprintf(str, "%s%dMessage%dChannelID%d", pPrefix, nServerId, nUserId, nIndex);
    userDefault->setIntegerForKey(str, body.nChannelID);
    sprintf(str, "%s%dMessage%dTargetID%d", pPrefix, nServerId, nUserId, nIndex);
    userDefault->setIntegerForKey(str, body.nTargetID);
    sprintf(str, "%s%dMessage%dMsg%d", pPrefix, nServerId, nUserId, nIndex);
    userDefault->setStringForKey(str, body.msg);
}

void CommonUtility::insertMessage(std::map<int, messageBody>& messageMap, const char* pPrefix, int nServerId, int nUserId, messageBody& t, int nLimit)
{
    if (messageMap.size() < nLimit)
    {
        for (int i = 0; i < nLimit; i++)
        {
            if (messageMap.find(i) == messageMap.end())
            {
                messageMap.insert(std::pair<int, messageBody_t>(i, t));
                CommonUtility::saveMessage(pPrefix, nServerId, nUserId, i, t);
                return;
            }
        }
    }
    else
    {
        int index = 0;
        int min = 0;
        for (int i = 0; i < nLimit; i++)
        {
            std::map<int, messageBody>::iterator it = messageMap.find(i);
            if (min == 0 || it->second.nTime < min)
            {
                index = i;
                min = it->second.nTime;
            }
        }
        messageMap[index] = t;
        CommonUtility::saveMessage(pPrefix, nServerId, nUserId, index, t);
    }
}

char* CommonUtility::getTalentString(char* pStr, int nTalent, float fTalentValue)
{
    switch (nTalent)
    {
        case 1:
            sprintf(pStr, "天赋:使敌人内攻减少%0.1f%%", fTalentValue * 100);
            break;
        case 2:
            sprintf(pStr, "天赋:使敌人外攻减少%0.1f%%", fTalentValue * 100);
            break;
        case 3:
            sprintf(pStr, "天赋:使敌人内防减少%0.1f%%", fTalentValue * 100);
            break;
        case 4:
            sprintf(pStr, "天赋:使敌人外防减少%0.1f%%", fTalentValue * 100);
            break;
        case 5:
            sprintf(pStr, "天赋:使敌人生命上限减少%0.1f%%", fTalentValue * 100);
            break;
        case 6:
            sprintf(pStr, "天赋:使掌门经验加成%0.1f%%", fTalentValue * 100);
            break;
        case 7:
            sprintf(pStr, "天赋:使掌门银两加成%0.1f%%", fTalentValue * 100);
            break;
        case 8:
            sprintf(pStr, "天赋:增加掌门体力上限%0.1f%%", fTalentValue * 100);
            break;
        case 9:
            sprintf(pStr, "天赋:增加掌门怒气上限%0.1f%%", fTalentValue * 100);
            break;
        case 10:
            sprintf(pStr, "天赋:掌门有%0.1f%%概率秒杀敌人", fTalentValue * 100);
            break;
        default:
            break;
    }
    return pStr;
}

//返回所需长度的字符串
std::string CommonUtility::getStringByLength(std::string& str, int length)
{
    int contentLeght = str.length();
    if (length >= contentLeght)
    {
        return str;
    }
    int i = 0;
    int index = 0;
    const char* pStr = str.c_str();
    while(index < length)
    {
        char ch = pStr[i];
        //这里上面说过了
        if (ch > -127 && ch < 0)
        {
            i += 3;
        }
        else
        {
            i++;
        }
        index++;
    }
    
    return str.substr(0, i);
}

const int CommonUtility::getUTF8Length(const char* string)
{
    int i = 0, j = 0;
    while (string[i]) {
        if ((string[i] & 0xc0) != 0x80) j++;
        i++;
    }
    return j;
}

//该方法是临时做的，没考虑到UTF8字符串的所有情况，对UTF8的边际判断也不准确
const char* CommonUtility::getUTF8Substring(const char* string, int start, int length)
{
    int s = 0;
    int sIndex = 0;
    int eIndex = 0;
    int i = 0;
    for (i = 0; string[i] != '\0'; i++, s++)
    {
        if (start == s)
        {
            sIndex = i;
        }
        if (start + length == s)
        {
            eIndex = i;
            break;
        }
        if (string[i] < 0)
        {
            i += 2;
        }
    }
    if (eIndex == 0)
        eIndex = i;
    char* val = new char(eIndex - sIndex + 1);
    for (i = 0; i < eIndex - sIndex; i++)
    {
        val[i] = string[sIndex + i];
    }
    val[eIndex - sIndex] = '\0';
    return val;
}

std::string& CommonUtility::trim(std::string& str)
{
    if (str.empty())
    {
        return str;
    }
    
    str.erase(0, str.find_first_not_of(" "));
    str.erase(str.find_last_not_of(" ") + 1);
    return str;
}

int get_hex(char ch)
{
    if (ch >= '0' && ch <= '9') return ch - '0';
    else if (ch >= 'A' && ch <= 'F') return  10 + ch - 'A';
    else if (ch >= 'a' && ch <= 'f') return  10 + ch - 'a';
    else return 0;
}

char* hex2str(const char* hex_str, char* str)
{
    for (int i = 0; i < strlen(hex_str) / 2; i++)
    {
        unsigned char c = (get_hex(hex_str[i*2]) << 4) + get_hex(hex_str[i * 2 + 1]);
        str[i] = c;
    }
    return str;
}

void split(std::string src, const char* token, strArray& vect){
    int nend = 0;
    int nbegin = 0;
    
    while (nend != -1)
    {
        nend = src.find(token, nbegin);
        if (nend == -1)
        {
            vect.push_back(src.substr(nbegin, src.length() - nbegin));
        } else {
            vect.push_back(src.substr(nbegin, nend - nbegin));
        }
        nbegin = nend + strlen(token);
    }
}

CCControlButton* CommonUtility::setDisabledButton(CCControlButton* pOriginBtn, const char* pBgFileName, int nFontSize, CCObject* target, SEL_CCControlHandler action)
{
    CCControlButton* pBtn = NULL;
    CCNode* pObj = dynamic_cast<CCNode*>(pOriginBtn->getUserObject());
    if (pObj != NULL)
    {
        pObj->removeFromParent();
    }
    
    CCSprite* pTmp = CCSprite::create(pBgFileName);
    CCScale9Sprite* pBg = CCScale9Sprite::create(pBgFileName);
    CCLabelTTF* pTitle = CCLabelTTF::create(pOriginBtn->getTitleForState(CCControlStateDisabled)->getCString(), "Arial-BoldMT", nFontSize);
    
    pBtn = CCControlButton::create(pTitle, pBg);
    pOriginBtn->getParent()->addChild(pBtn);
    pBtn->setPreferredSize(pTmp->getContentSize());
    pBtn->setPosition(pOriginBtn->getPosition());
    pBtn->setTouchPriority(pOriginBtn->getTouchPriority());
    pBtn->addTargetWithActionForControlEvents(target, action, CCControlEventTouchUpInside);
    pOriginBtn->setUserObject(pBtn);
    
    pBtn->setVisible(true);
    pOriginBtn->setVisible(false);
    return pBtn;
}

void CommonUtility::setButtonVisible(cocos2d::extension::CCControlButton* pOriginBtn, bool bIsVisible)
{
    CCControlButton* pBtn = dynamic_cast<CCControlButton*>(pOriginBtn->getUserObject());
    if (pBtn != NULL)
    {
        if (bIsVisible)
        {
            pBtn->setVisible(true);
            pOriginBtn->setVisible(false);
        }
        else
        {
            pBtn->removeFromParent();
            pOriginBtn->setVisible(false);
            pOriginBtn->setUserObject(NULL);
        }
        
    }
    else
    {
        pOriginBtn->setVisible(bIsVisible);
    }
}

void CommonUtility::setButtonEnableState(cocos2d::extension::CCControlButton* pOriginBtn, bool bIsEnabled, const char* pBgFileName, int nFontSize, CCObject* target, cocos2d::extension::SEL_CCControlHandler action)
{
    if (bIsEnabled)
    {
        pOriginBtn->setVisible(true);
        pOriginBtn->setEnabled(true);
        CCControlButton* pBtn = dynamic_cast<CCControlButton*>(pOriginBtn->getUserObject());
        if (pBtn != NULL)
        {
            pBtn->removeFromParent();
            pOriginBtn->setUserObject(NULL);
        }
    }
    else
    {
        CommonUtility::setDisabledButton(pOriginBtn, pBgFileName, nFontSize, target, action);
    }
}

void CommonUtility::setBtnState(CCControlButton* pBtn, CCRect rect, bool bEnable)
{
    const int effectTag = 2001;
    CCNode* effect = pBtn->getChildByTag(effectTag);
    if (effect == NULL)
    {
        effect = ParticleResource::getInstance()->createFireEffect(pBtn, CCRectMake(10, 22, 140, 50));
        effect->setTag(effectTag);
    }
    pBtn->setEnabled(bEnable);
    effect->setVisible(bEnable);
}

CCArray* CommonUtility::getTipArray(std::vector<item_unit_t>& vec)
{
    CCArray* tipsContent = CCArray::create();
    char str[128] = {0};

    for (std::vector<item_unit_t>::iterator it = vec.begin(); it != vec.end(); it++)
    {
        item_unit_t& item = *it;
        CCString* bonusStr = CCString::createWithFormat("%sX%s  ", GameData::getItem(item.item_id)->name, UserData::getShortNum(item.item_cnt, str));
        
        tipsContent->addObject(bonusStr);
    }
    
    return tipsContent;
}

void CommonUtility::setButtonGrayBg(XYButton* btn, bool enable, const char* normalfile, const char* grayFile)
{
    if (enable)
    {
        CCSpriteFrame* nomal = CCSprite::create(normalfile)->displayFrame();
        CCSpriteFrame* sele = CRUtils::getlightSpriteFrame(normalfile);
        CCSpriteFrame* disable = NULL;
        if (grayFile)
        {
            disable = CCSprite::create(grayFile)->displayFrame();
        }
        else
        {
            disable = CRUtils::getgaySpriteFrame(normalfile);
        }
        
        btn->setButtonBgImage(nomal, sele, disable);
    }
    else
    {
        CCSpriteFrame* nomal = NULL;
        if (grayFile)
        {
            nomal = CCSprite::create(grayFile)->displayFrame();
        }
        else
        {
            nomal = CRUtils::getgaySpriteFrame(normalfile);
        }
        btn->setButtonBgImage(nomal, nomal, nomal);
    }
}

void CommonUtility::createButtonEffect(CCNode* pNode, CCRect rect)
{
    const int effectTag = 1982;
    CCNode* effect = pNode->getChildByTag(effectTag);
    if (effect == NULL)
    {
        effect = ParticleResource::getInstance()->createFireEffect(pNode, rect);
        effect->setTag(effectTag);
    }
}

ItemSprite* CommonUtility::createButton(int nItemId, int nItemCount, CCObject *obj, SEL_CallFuncN callfun, bool bIsFlash, bool bShowName, int nPriority)
{
    char str[128] = {0};
    sprintf(str, "item_%d.png", nItemId);
    ItemSprite* pImg = ItemSprite::create(str, nPriority);
    pImg->setTouchEndEvent(obj, callfun);
    pImg->itemID = nItemId;
    pImg->itemNum = nItemCount;
    
    if (bIsFlash)
    {
        CCSprite* pFlash = EffectResource::sharedInstance()->spriteWithEffect(37, true, true, 3);
        pImg->addChild(pFlash);
        pFlash->setPosition(ccp(pImg->getContentSize().width / 2.0f, pImg->getContentSize().height / 2.0f));
    }
    
    if (nItemCount > 1)
    {
        char str1[128] = {0};
        sprintf(str1, "X%s", UserData::getShortNum(nItemCount, str));
        CCLabelTTF* pLabel = CCLabelTTF::create(str1, "Arial-BoldMT", 18);
        pImg->addChild(pLabel, 10);
        pLabel->setAnchorPoint(ccp(1, 0));
        pLabel->setPosition(ccp(pImg->getContentSize().width - 5, 5));
    }
    
    if (bShowName)
    {
        CCLabelTTF* pName = CCLabelTTF::create(GameData::getItem(nItemId)->name, "Arial-BoldMT", 14);
        pImg->addChild(pName);
        pName->setAnchorPoint(ccp(0.5f, 0.5f));
        pName->setPosition(ccp(pImg->getContentSize().width / 2.0f, -15));
    }
    
    return pImg;
}
