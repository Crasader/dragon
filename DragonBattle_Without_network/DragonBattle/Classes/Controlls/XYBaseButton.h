//
//  XYBaseButton.h
//  DragonBattle
//
//  Created by zeng xiaolong on 13-7-17.
//
//

#ifndef __DragonBattle__XYBaseButton__
#define __DragonBattle__XYBaseButton__

#include "cocos2d.h"
#include "XYBaseTouchSprite.h"
USING_NS_CC;
class XYBaseButton : public XYBaseTouchSprite
{
    
public:
    XYBaseButton(void);
    ~XYBaseButton(void);
    
    virtual bool initWithFile(const char * pFile,int nPriority = 0);
    virtual bool initWithFile(const char * pNormalFile, const char *pSelectFile,const char *pDisableFile,int nPriority);
    
    static XYBaseButton * create(const char * pFile,int nPriority = 0);
    static XYBaseButton * createWithFileAndPriority(const char * pNormalFile, const char *pSelectFile,const char *pDisableFile,int nPriority);
    
    CCLabelTTF * setLbl(const char *pFile,const char *fontName,float fontsize);
    void setLblPosition(float widthPercent,float heigthPercent);
    void setLblColor(const ccColor3B& color3);
    void setGayImage(const char *pFile, int orderZ);
    CCSprite* setImage(const char *pFile,int orderZ);
    void setImage(const char *pFile,int orderZ,float scale,float widthPercent,float heigthPercent);
    CCSprite* addImage(const char *pFile,int orderZ,int tag);
    void addImage(const char *pFile, int orderZ, float scale, float widthPercent, float heigthPercent);
    void removeImage();
    virtual void setEnable(bool enable);
    virtual void touchBegin();
    virtual void touchEnd();
    CC_SYNTHESIZE(bool,m_sound, Sound);
    
    void setButtonBgImage(CCSpriteFrame* normalImg, CCSpriteFrame* seleImg, CCSpriteFrame* disableImg);
    void setButtonBgImage(const char * pNormalFile, const char *pSelectFile,const char *pDisableFile);
protected:
    CCSpriteFrame *m_pSelectFileFrame;
    CCSpriteFrame *m_pDisableFileFrame;
    CCSpriteFrame *m_pNormalFileFrame;
};
#endif /* defined(__DragonBattle__XYBaseButton__) */
