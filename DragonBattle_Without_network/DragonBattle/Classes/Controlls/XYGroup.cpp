//
//  XYGroup.cpp
//  XY
//
//  Created by long on 12-12-24.
//
//

#include "XYGroup.h"
XYGroup::XYGroup(void)
{
    arrGroupBtn = CCArray::create();
    CC_SAFE_RETAIN(arrGroupBtn);
    m_pTarget = NULL;
}
XYGroup::~XYGroup(void)
{
    arrGroupBtn->removeAllObjects();
    CC_SAFE_RELEASE_NULL(arrGroupBtn);
}

XYGroup* XYGroup::create()
{
    XYGroup *pGroup = new XYGroup();
    pGroup->autorelease();
    return pGroup;
}
void XYGroup::addBtn(XYSelectButton *btn)
{
    if (arrGroupBtn->containsObject(btn))
    {
        return;
    }
    arrGroupBtn->addObject(btn);
    btn->setTouchEndEvent(this, callfuncN_selector(XYGroup::btnTouch));
}

void XYGroup::removeBtn(XYSelectButton *btn)
{
    if (arrGroupBtn->containsObject(btn))
    {
        arrGroupBtn->removeObject(btn);
    }
}

XYSelectButton *XYGroup::getSelectBtn()
{
    CCObject *pObj = NULL;
    CCARRAY_FOREACH(arrGroupBtn,pObj)
    {
        XYSelectButton *pbtn = (XYSelectButton*)pObj;
        if(pbtn->getTurnOn())
            return pbtn;
    }
    return NULL;
}

void XYGroup::setSelectButton(XYSelectButton *pselebtn)
{
    CCObject *pObj = NULL;
    CCARRAY_FOREACH(arrGroupBtn,pObj)
    {
        XYSelectButton *pbtn = (XYSelectButton*)pObj;
        if (pbtn != pselebtn)
        {
            pbtn->unselet();
        }
    }
    pselebtn->selet();
}

void XYGroup::setSelectButtonByIndex(int index)
{
    if (index < 0 || index >= arrGroupBtn->count()) {
        return;
    }
    
    XYSelectButton* pselebtn = dynamic_cast<XYSelectButton*>(arrGroupBtn->objectAtIndex(index));
    if (pselebtn) {
        setSelectButton(pselebtn);
    }
}

void XYGroup::btnTouch(CCNode* node)
{
    CCObject *pObj = NULL;
    XYSelectButton *mybtn = dynamic_cast<XYSelectButton*>(node);
    CCARRAY_FOREACH(arrGroupBtn,pObj)
    {
        XYSelectButton *pbtn = (XYSelectButton*)pObj;
        if (pbtn != mybtn && pbtn->getEnable())
        {
            pbtn->unselet();
        }
    }
    if(m_pTarget)
    {
        (m_pTarget->*m_pTouchEnded)(mybtn);
    }
}
