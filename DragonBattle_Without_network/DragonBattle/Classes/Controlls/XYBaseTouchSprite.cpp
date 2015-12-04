//
//  XYBaseTouchSprite.cpp
//  DragonBattle
//
//  Created by zeng xiaolong on 13-6-3.
//
//

#include "XYBaseTouchSprite.h"
#include <math.h>
USING_NS_CC;
BYGraySprite::BYGraySprite(){
    
}

BYGraySprite::~BYGraySprite(){
    
}

BYGraySprite* BYGraySprite::create( const char* pszFileName ){
    BYGraySprite* graySprite = new BYGraySprite;
    if (graySprite && graySprite->initWithGrayFile(pszFileName)){
        graySprite->autorelease();
        return graySprite;
    }else{
        CC_SAFE_RELEASE(graySprite);
        return NULL;
    }
}

bool BYGraySprite::initWithGrayFile( const char* pszFilename)
{
    do{
        CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage(pszFilename);
        CCRect rect = CCRectZero;
        rect.size = pTexture->getContentSize();
        CCSprite::initWithTexture(pTexture, rect);
        GLchar* pszFragSource =
        (GLchar*)"#ifdef GL_ES \n \
        precision mediump float; \n \
        #endif \n \
        uniform float u_time;\n \
        uniform sampler2D u_texture; \n \
        varying vec2 v_texCoord; \n \
        varying vec4 v_fragmentColor; \n \
        void main(void) \n \
        { \n \
        // Convert to greyscale using NTSC weightings \n \
        float grey = dot(texture2D(u_texture, v_texCoord).rgb, vec3(0.299, 0.587, 0.114)); \n \
        gl_FragColor = vec4(grey*u_time, grey*u_time, grey*u_time, texture2D(u_texture, v_texCoord).a); \n \
        }";
        
        CCGLProgram* pProgram = new CCGLProgram();
        pProgram->initWithVertexShaderByteArray(ccPositionTextureColor_vert, pszFragSource);
        this->setShaderProgram(pProgram);
        pProgram->release();
        CHECK_GL_ERROR_DEBUG();
        
        this->getShaderProgram()->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
        this->getShaderProgram()->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
        this->getShaderProgram()->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
        CHECK_GL_ERROR_DEBUG();
        
        this->getShaderProgram()->link();
        CHECK_GL_ERROR_DEBUG();
        
        this->getShaderProgram()->updateUniforms();
        CHECK_GL_ERROR_DEBUG();
        this->scheduleUpdate();
        this->getShaderProgram()->use();
        timeUniformLocation = glGetUniformLocation(getShaderProgram()->getProgram(), "u_time");
        return true;
    } while (0);
    return false;
}

void BYGraySprite::update(float dt)
{
    totalTime += dt;
    getShaderProgram()->updateUniforms();
    this->getShaderProgram()->use();
     glUniform1f(timeUniformLocation, fabsf(sin(totalTime)));
}

void BYGraySprite::draw(){
    CCSprite::draw();
    return;
    ccGLEnableVertexAttribs(kCCVertexAttribFlag_PosColorTex );
    ccGLBlendFunc( m_sBlendFunc.src, m_sBlendFunc.dst );
    
    this->getShaderProgram()->use();
    this->getShaderProgram()->setUniformsForBuiltins();

    ccGLBindTexture2D( this->getTexture()->getName() );
    
#define kQuadSize sizeof(m_sQuad.bl)
    long offset = (long)&m_sQuad;
    
    // vertex
    int diff = offsetof( ccV3F_C4B_T2F, vertices);
    glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*) (offset + diff));
    
    // texCoods
    diff = offsetof( ccV3F_C4B_T2F, texCoords);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));
    
    // color
    diff = offsetof( ccV3F_C4B_T2F, colors);
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    CC_INCREMENT_GL_DRAWS(1);
}

XYBaseTouchSprite::XYBaseTouchSprite(void):CCSprite()
{
    m_isEnabled = true;
    
    m_iPriority = 0;

    m_pTarget = NULL;
    m_pTouchBegan = NULL;
    m_pTouchEnded = NULL;
    m_pTouchRectLayer = NULL;
}

XYBaseTouchSprite::~XYBaseTouchSprite(void)
{

}

bool XYBaseTouchSprite::initWithFile(const char * pFile, int nPriority)
{
    m_strPic = pFile;
    m_iPriority = nPriority;
    if (pFile == NULL)
    {
        if(!CCSprite::init())
        {
            return false;
        }
    }
    CCSpriteFrame* FileFrame = CRUtils::initSpriteFrameWithName(pFile);
    if(!CCSprite::initWithSpriteFrame(FileFrame))
    {
        return false;
    }
    m_TouchRect = ((CCMenuItem*)this)->rect();
    m_TouchRect.origin = CCPointZero;
    return true;
}

void XYBaseTouchSprite::setProirity(int nPriority)
{
    m_iPriority = nPriority;
    if (getParent())
    {
        CCDirector::sharedDirector()->getTouchDispatcher()->setPriority(m_iPriority,this);
    }
}

int XYBaseTouchSprite::getProirity()
{
    return m_iPriority;
}

void XYBaseTouchSprite::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, m_iPriority, true);
    CCSprite::onEnter();
}

void XYBaseTouchSprite::onExit()
{
    //移除监听
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCSprite::onExit();
}

XYBaseTouchSprite *  XYBaseTouchSprite::create(const char * pNormalFile,int nPriority)
{
    XYBaseTouchSprite *pBtn = new XYBaseTouchSprite();
	if (pBtn && pBtn->initWithFile(pNormalFile,nPriority))
    {
	    pBtn->autorelease();
        return pBtn;
    }
    CC_SAFE_DELETE(pBtn);
	return NULL;
}


bool XYBaseTouchSprite::ccTouchBegan(CCTouch* pTouch, CCEvent* event)
{
    for (CCNode *c = this; c != NULL; c = c->getParent())
    {
        if (c->isVisible() == false)
        {
            return false;
        }
    }
    m_isMove =false;
    if (m_isEnabled)
    {
        CCPoint touchLocation = pTouch->getLocation();
        m_startPoi = touchLocation;
        touchLocation = this->convertToNodeSpace(touchLocation);
        CCRect r = m_TouchRect;

        if (r.containsPoint(touchLocation))
        {
//            CCLayerColor *layer = CCLayerColor::create(ccc4(0, 0, 0, 128), m_TouchRect.size.width, m_TouchRect.size.height);
//            layer->setPosition(ccp(m_TouchRect.origin.x, m_TouchRect.origin.y));
//            addChild(layer);
            touchBegin();
            return true;
        }
    }
    return false;
}

void XYBaseTouchSprite::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchLocation = pTouch->getLocation();
    if (ccpDistance(m_startPoi,touchLocation)>35)
    {
        m_isMove = true;
    }
}

void XYBaseTouchSprite::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_isEnabled)
    {
        CCPoint touchLocation = pTouch->getLocationInView();
        touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
        
        CCPoint local = this->convertToNodeSpace(touchLocation);
        CCRect r = m_TouchRect;
        if (r.containsPoint(local)&&!m_isMove)
        {
            touchEnd();
        }
    }
    m_isMove = false;
}

void XYBaseTouchSprite::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{

}

void XYBaseTouchSprite::touchBegin()
{
    if(m_pTarget && m_pTouchBegan)
    {
        (m_pTarget->*m_pTouchBegan)(this);
    }
}

void XYBaseTouchSprite::touchEnd()
{
    if(m_pTarget && m_pTouchEnded)
    {
        (m_pTarget->*m_pTouchEnded)(this);
    }
}
