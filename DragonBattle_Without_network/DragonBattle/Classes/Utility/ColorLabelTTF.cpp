//
//  ColorLabelTTF.cpp
//  DragonBattle
//
//  Created by 白明江 on 10/24/13.
//
//

#include "ColorLabelTTF.h"
#include "CommonUtility.h"


ColorLabelTTF* ColorLabelTTF::create(const char* str, const char* split, ccColor3B defaultColor, std::vector<ccColor3B> colorVec, const char* font, int fontSize)
{
    ColorLabelTTF* pRet = new ColorLabelTTF;
    if (pRet->init(str, split, defaultColor, colorVec, font, fontSize)) {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_RELEASE_NULL(pRet);
    }
    
    return pRet;
}


/**
 *
 * 处理包含GOLD_ICON或YXB_ICON节点，分别代表黄金和银两的icon
 *
 **/

bool ColorLabelTTF::init(const char* str, const char* token, ccColor3B defaultColor, std::vector<ccColor3B> colorVec, const char* font, int fontSize)
{
    bool bRet = CCNode::init();
    do
    {
        strArray strArr;
        split(str, token, strArr);
        int tag = 0;
        for (int i = 0; i < strArr.size(); i ++)
        {
            const char* txt = strArr.at(i).c_str();
            if (strlen(txt) == 0 || strcmp(txt , "") == 0) continue;
            
            char str[64] = {0};
            if (strcmp(txt , "GOLD_ICON") == 0)
            {
                sprintf(str, "item_%d.png", ITEM_GOLD);
                CCSprite* pGoldSpr = CCSprite::createWithSpriteFrameName(str);
                
                this->addChild(pGoldSpr);
                pGoldSpr->setAnchorPoint(ccp(0.f, 0.5f));
                pGoldSpr->setTag(tag);
                pGoldSpr->setScale(0.5f);

                tag ++;
                continue;
            }
            
            if (strcmp(txt , "YXB_ICON") == 0)
            {
                sprintf(str, "item_%d.png", ITEM_YXB);
                CCSprite* pYxbSpr = CCSprite::createWithSpriteFrameName(str);
                
                this->addChild(pYxbSpr);
                pYxbSpr->setAnchorPoint(ccp(0.f, 0.5f));
                pYxbSpr->setTag(tag);
                pYxbSpr->setScale(0.5f);
                
                tag ++;
                continue;
            }
            
            CCLabelTTF* txtLabel = CCLabelTTF::create(txt, FONTNAME, fontSize);
            
            if (i % 2 == 1)
            {
                int index = (i + 1) / 2 - 1;
                if (index >=colorVec.size())
                {
                    txtLabel->setColor(defaultColor);
                }
                else
                {
                    ccColor3B color = colorVec.at(index);
                    txtLabel->setColor(color);
                }
                
            }
            else
            {
                txtLabel->setColor(defaultColor);
            }
            
            txtLabel->setAnchorPoint(ccp(0.f, 0.5f));
            txtLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
            txtLabel->setTag(tag);
            this->addChild(txtLabel);
            tag ++;
        }
        
        this->refresh();
        
    } while (false);
    
    return bRet;
}

void ColorLabelTTF::setString(const char* str)
{
    this->removeAllChildren();
    do
    {
        strArray strArr;
        split(str, "$", strArr);
        int tag = 0;
        for (int i = 0; i < strArr.size(); i ++)
        {
            const char* txt = strArr.at(i).c_str();
            if (strlen(txt) == 0 || strcmp(txt , "") == 0) continue;
            
            char str[64] = {0};
            if (strcmp(txt , "GOLD_ICON") == 0)
            {
                sprintf(str, "item_%d.png", ITEM_GOLD);
                CCSprite* pGoldSpr = CCSprite::createWithSpriteFrameName(str);
                
                this->addChild(pGoldSpr);
                pGoldSpr->setAnchorPoint(ccp(0.f, 0.5f));
                pGoldSpr->setTag(tag);
                pGoldSpr->setScale(0.5f);
                
                tag ++;
                continue;
            }
            
            if (strcmp(txt , "YXB_ICON") == 0)
            {
                sprintf(str, "item_%d.png", ITEM_YXB);
                CCSprite* pYxbSpr = CCSprite::createWithSpriteFrameName(str);
                
                this->addChild(pYxbSpr);
                pYxbSpr->setAnchorPoint(ccp(0.f, 0.5f));
                pYxbSpr->setTag(tag);
                pYxbSpr->setScale(0.5f);
                
                tag ++;
                continue;
            }
            
            CCLabelTTF* txtLabel = CCLabelTTF::create(txt, FONTNAME, 24);
            
            txtLabel->setColor(ccWHITE);
            txtLabel->setAnchorPoint(ccp(0.f, 0.5f));
            txtLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
            txtLabel->setTag(tag);
            this->addChild(txtLabel);
            
            tag ++;
        }
        
        this->refresh();
        
    } while (false);
}

void ColorLabelTTF::setStringWithNodeTag(const char* str, int tag)
{
    CCLabelTTF* findLbl = dynamic_cast<CCLabelTTF*>(this->getChildByTag(tag));
    if (findLbl)
    {
        findLbl->setString(str);
        this->refresh();
    }
    else
    {
        CCLOGWARN("Not find a CCLabelTTF Node with the tag %d!", tag);
    }
    
}

void ColorLabelTTF::setColor(ccColor3B defaultColor)
{
    for (int i = 0; i < this->getChildrenCount(); i ++)
    {
        CCLabelTTF* findLbl = dynamic_cast<CCLabelTTF*>(this->getChildByTag(i));
        if (findLbl)
        {
            findLbl->setColor(defaultColor);
        }
    }
}

void ColorLabelTTF::setColorWithNodeTag(ccColor3B color, int tag)
{
    CCLabelTTF* findLbl = dynamic_cast<CCLabelTTF*>(this->getChildByTag(tag));
    if (findLbl)
    {
        findLbl->setColor(color);
    }
    else
    {
        CCLOGWARN("Not find a CCLabelTTF Node with the tag %d!", tag);
    }
}

void ColorLabelTTF::setColor(std::vector<ccColor3B> colorVec)
{
    for (int i = 0; i < this->getChildrenCount(); i ++)
    {
        CCLabelTTF* findLbl = dynamic_cast<CCLabelTTF*>(this->getChildByTag(i));
        if (findLbl)
        {
            if (i < colorVec.size() - 1)
            {
                findLbl->setColor(colorVec.at(i));
            }
        }
    }
}

void ColorLabelTTF::setColor(ccColor3B defaultColor, std::vector<ccColor3B> colorVec)
{
    
}

void ColorLabelTTF::setFontSize(float size)
{
    for (int i = 0; i < this->getChildrenCount(); i ++)
    {
        CCLabelTTF* findLbl = dynamic_cast<CCLabelTTF*>(this->getChildByTag(i));
        if (findLbl)
        {
            findLbl->setFontSize(size);
            
        }
    }
    
    refresh();
}
void ColorLabelTTF::setFontSizeWithNodeTag(float size, int tag)
{
    CCLabelTTF* findLbl = dynamic_cast<CCLabelTTF*>(this->getChildByTag(tag));
    if (findLbl)
    {
        findLbl->setFontSize(size);
        refresh();
    }
    else
    {
        CCLOGWARN("Not find a CCLabelTTF Node with the tag %d!", tag);
    }
}

void ColorLabelTTF::refresh()
{
    float width = 0.f;
    float height = 0.f;
    
    for (int i = 0; i < this->getChildrenCount(); i ++)
    {
        CCNode* pNode = this->getChildByTag(i);
        pNode->setPositionY(0);
        pNode->setPositionX(width);
        
        width += pNode->getContentSize().width * pNode->getScale();
        if (height < pNode->getContentSize().height * pNode->getScale())
        {
            height = pNode->getContentSize().height * pNode->getScale();
        }
    }
    
    for (int i = 0; i < this->getChildrenCount(); i ++)
    {
        CCNode* pNode = this->getChildByTag(i);
        pNode->setPositionY(height / 2);
    }
    
    this->setContentSize(CCSizeMake(width, height));
}