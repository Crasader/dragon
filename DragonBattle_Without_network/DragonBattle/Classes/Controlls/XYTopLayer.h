//
//  XYTopLayer.h
//  XY
//
//  Created by long on 12-12-25.
//
//

#ifndef __XY__XYTopLayer__
#define __XY__XYTopLayer__

#include "online_cli_proto_cmd.h"
#include "ItemConf.h"
#include "GameData.h"

class XYTopLayer:public CCLayer
{
public:
    XYTopLayer(void);
    ~XYTopLayer(void);
    static XYTopLayer *getInstance();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void registerWithTouchDispatcher(void);
    void ShowWaiting();
    void RemoveWaiting();
    
    void ShowTip(const char *str);
    void ShowTip(int swapID);
    void ShowSingleTip(int nItemId, int nItemCount);
    void ShowSimpleTip(CCArray* content, ccColor3B color = ccGREEN);
    void ShowBoxTip(const std::vector<package_item_t>* pItemVec, CCPoint startPos);
    void ShowTip(const std::vector<ItemUnit_t>* pItemVec);
    void ShowTip(const std::vector<package_item_t>* pItemVec);
    void ShowTip(const std::vector<item_unit_t>* pItemVec);
    
    virtual bool init();
    void fadeLbl(CCNode *p);
    CREATE_FUNC(XYTopLayer);

    void ShowAwardLayer( std::vector<ItemUnit_t>itemVec);
    void ShowAwardLayer(std::vector<item_unit_t> itemVec);

private:
    void showTipEffect(CCPoint startPos);
    void flyItemToPortrait(int nItemId, int nItemCount, CCPoint worldPosition, int nIndex, bool bUpAndDownFirst, bool bIsOriginSize);
    CCPoint getTipPosition();
    
};
#endif
