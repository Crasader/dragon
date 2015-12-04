//
//  ZQTextLink.cpp
//  TianJiang
//
//  Created by bless on 13-5-8.
//
//

#include "ZQTextLink.h"
ZQTextLink::ZQTextLink()
{
    
}
ZQTextLink::~ZQTextLink()
{
    
}
ZQTextLink *ZQTextLink::create(CCArray *pArray, int nMaxWidth, int nDistanceHeight)//TextLinkData数组
{
    ZQTextLink *pLink = new ZQTextLink();
    if (pLink && pLink->init(pArray,nMaxWidth,nDistanceHeight))
    {
        pLink->autorelease();
        return pLink;
    }
    CC_SAFE_DELETE(pLink);
    return NULL;
}
bool ZQTextLink::init(CCArray *pArray, int nMaxWidth, int nDistanceHeight)
{
    if (!CCLayer::init()) {
        return false;
    }
    
    int nWidth = 0;
    int nHeight = 0;
    int nOnelineHeight = 0;
    for (int i=0; i<pArray->count(); i++)
    {
        TextLinkData *pData = dynamic_cast<TextLinkData *>(pArray->objectAtIndex(i));
        if (pData)
        {
            switch (pData->nType)
            {
                case ENUM_TEXTLINK_TYPE_LAB:
                {
                    CCLabelTTF *pLab = CCLabelTTF::create(pData->strName.c_str(),FONTNAME, pData->nFontSize);
                    nOnelineHeight = pLab->getContentSize().height;
                    //如果长度不大于剩余长度则直接添加
                    if (pLab->getContentSize().width < nMaxWidth - nWidth) {
                        addChild(pLab);
                        pLab->setAnchorPoint(ccp(0,0.5));
                        pLab->setPosition(ccp(nWidth, nHeight));
                        pLab->setColor(pData->color);
                        nWidth += pLab->getContentSize().width;
                        
                    }
                    else
                    {
                        //如果长度大于剩余长度则填满，然后剩余的换行
                      
                        int nPosition = 0;
//                        int nLength = 1;
                        int nMaxLength = pData->strName.size();
                        while (1) {
                            int nLength = 0;
                            int nLabWidth = nMaxWidth - nWidth;
                            while (1 && nPosition + nLength + 1 < nMaxLength) {
                                if (!isascii(pData->strName.at(nPosition))) {
                                    CCLabelTTF *pLab = CCLabelTTF::create(pData->strName.substr(nPosition, nLength + 3).c_str(), FONTNAME, pData->nFontSize);
                                    if (pLab->getContentSize().width > nLabWidth) {
                                        
                                        break;
                                    }
                                    nLength+=3;
                                }
                                else
                                {
                                    CCLabelTTF *pLab = CCLabelTTF::create(pData->strName.substr(nPosition, nLength + 1).c_str(), FONTNAME, pData->nFontSize);
                                    if (pLab->getContentSize().width > nLabWidth) {
                                        
                                        break;
                                    }
                                    nLength+=1;
                                }
                                
                    
                            }
                            if (nLength != 0) {
                                CCLabelTTF *pLab = CCLabelTTF::create(pData->strName.substr(nPosition, nLength).c_str(), FONTNAME, pData->nFontSize);
                                addChild(pLab);
                                pLab->setAnchorPoint(ccp(0,0.5));
                                pLab->setPosition(ccp(nWidth, nHeight));
                                pLab->setColor(pData->color);
                                nWidth += pLab->getContentSize().width;
                                nPosition += nLength;
                                
                                if (nPosition < nMaxLength - 1) {
                                    nWidth = 0;
                                    nHeight -= pLab->getContentSize().height + nDistanceHeight;
                                }
                            }
                            else
                            {
                                if (nPosition < nMaxLength - 1) {
                                    nWidth = 0;
                                    nHeight -= pLab->getContentSize().height + nDistanceHeight;
                                }
                                
                            }
          
                            if (nPosition >= nMaxLength - 1) {
                                break;
                            }
                            
                        }
                          
                    }

//                    setContentSize(CCSizeMake(nMaxWidth, abs(nHeight)));
                    break;
                }
                case ENUM_TEXTLINK_TYPE_BTN:
                {
                    
                    CCLabelTTF *pLab = CCLabelTTF::create(pData->strName.c_str(), FONTNAME, pData->nFontSize);
                    nOnelineHeight = pLab->getContentSize().height;
                    //如果长度不大于剩余长度则直接添加
                    if (pLab->getContentSize().width < nMaxWidth - nWidth) {
                        
                        //CCNode *pHLinkNode = Utils::createHLinkText(pData->strName.c_str(), FONTNAME, pData->nFontSize, pData->color, pData->invocation);
                        
                        CCControlButton *pHLinkNode = CCControlButton::create(pData->strName.c_str(), FONTNAME, pData->nFontSize);
                        CCLabelTTF *lbl = (CCLabelTTF*)pHLinkNode->getTitleLabel();
                        lbl->setColor(pData->color);
                        pHLinkNode->addTargetWithActionForControlEvents(pData->invocation->getTarget(), pData->invocation->getAction(), pData->invocation->getControlEvent());
                        addChild(pHLinkNode);
                        pHLinkNode->setPosition(ccp(nWidth, nHeight-pHLinkNode->getContentSize().height/2));
                        pHLinkNode->setPositionY(-pHLinkNode->getContentSize().height/2);
                        nWidth += pHLinkNode->getContentSize().width;
                        
                    
                        CCControlButton *pBtn =  dynamic_cast<CCControlButton *>(pHLinkNode->getChildByTag(0));
                        if (pBtn) {
                            pBtn->setUserObject((CCObject *)pData->pData);
                        }
 
                        nWidth += pLab->getContentSize().width;
                    }
                    else
                    {

                        //如果长度大于剩余长度则填满，然后剩余的换行
                        
                        int nPosition = 0;
                        //                        int nLength = 1;
                        int nMaxLength = pData->strName.size();
                        while (1) {
                            int nLength = 0;
                            int nLabWidth = nMaxWidth - nWidth;
                            while (1 && nPosition + nLength + 1 < nMaxLength) {
                                if (!isascii(pData->strName.at(nPosition))) {
                                    CCLabelTTF *pLab = CCLabelTTF::create(pData->strName.substr(nPosition, nLength + 3).c_str(), FONTNAME, pData->nFontSize);
                                    if (pLab->getContentSize().width > nLabWidth) {
                                        
                                        break;
                                    }
                                    nLength+=3;
                                }
                                else
                                {
                                    CCLabelTTF *pLab = CCLabelTTF::create(pData->strName.substr(nPosition, nLength + 1).c_str(), FONTNAME, pData->nFontSize);
                                    if (pLab->getContentSize().width > nLabWidth) {
                                        
                                        break;
                                    }
                                    nLength+=1;
                                }
                                
                                
                            }
                            if (nLength != 0) {
                                
                                CCControlButton *pHLinkNode = CCControlButton::create(pData->strName.c_str(), FONTNAME, pData->nFontSize);
                                CCLabelTTF *lbl = (CCLabelTTF*)pHLinkNode->getTitleLabel();
                                lbl->setColor(pData->color);
                                pHLinkNode->addTargetWithActionForControlEvents(pData->invocation->getTarget(), pData->invocation->getAction(), pData->invocation->getControlEvent());
                                addChild(pHLinkNode);
                                pHLinkNode->setPosition(ccp(nWidth, nHeight-pHLinkNode->getContentSize().height/2));
                                
                                
                                CCControlButton *pBtn =  dynamic_cast<CCControlButton *>(pHLinkNode->getChildByTag(0));
                                if (pBtn) {
                                    pBtn->setUserObject((CCObject *)pData->pData);
                                }
                                
                                nWidth += pHLinkNode->getContentSize().width;
                                nPosition += nLength;
                                
                                if (nPosition < nMaxLength - 1) {
                                    nWidth = 0;
                                    nHeight -= pLab->getContentSize().height + nDistanceHeight;
                                }
                            }
                            else
                            {
                                if (nPosition < nMaxLength - 1) {
                                    nWidth = 0;
                                    nHeight -= pLab->getContentSize().height + nDistanceHeight;
                                }
                                
                            }
                            
                            if (nPosition >= nMaxLength - 1) {
                                break;
                            }
   
                        }
                        
                    }
                    
//                    setContentSize(CCSizeMake(nMaxWidth, abs(nHeight)));
                    break;
                }
                case ENUM_RICHTEXT_TYPE_SPR:
                {
                    CCSprite *pSpr = CCSprite::create(pData->strName.c_str());
                    nOnelineHeight = pSpr->getContentSize().height;
                    if (pSpr->getContentSize().width >= nMaxWidth - nWidth)
                    {
                        nWidth = 0;
                        nHeight -= nDistanceHeight;
                        
                    }
                    pSpr->setPosition(ccp(nWidth, nHeight));
                    pSpr->setAnchorPoint(ccp(0,0.5));
                    addChild(pSpr);
                    nWidth += pSpr->getContentSize().width;
                    break;
                }
                default:
                    break;
            }
        }
    }
    setContentSize(CCSizeMake(nMaxWidth, abs(nHeight) + nOnelineHeight));
    return true;
}