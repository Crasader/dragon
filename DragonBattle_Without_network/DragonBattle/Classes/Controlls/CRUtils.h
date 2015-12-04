

#ifndef lmsg_CRUtils_h
#define lmsg_CRUtils_h
#include <time.h>
#include "cocos2d.h"
USING_NS_CC;
using namespace std;
    class CRUtils
    {
    public:
        static bool SpriteIntersects(CCSprite *pSpr,CCSprite * pSpr2);
        static bool UnitIntersects(CCSprite *unit,CCSprite *unit2);
        static void getSpriteRect(CCSprite *pSpr, CCRect &rc);
        static int getRandomNum(int nMinNum, int nMaxNum);
        
        static clock_t getNowTime();
        
        static double diffBySec(clock_t start);
        
        static bool isInScreen(CCNode * node,CCNode * parent);
        
        static void addClassCount(string strclassname);
        
        static void removeClassCount(string strclassname);
        
        static void prinfgClassleak();
        
        static CCAnimation *createAni(const char *fileName,int nstart,int num);
        static CCSpriteFrame *getlightSpriteFrame(const char *fileName);
        static CCSpriteFrame *getgaySpriteFrame(const char *fileName);
        static void writeToReadAbleFile(string fileName,string context);
        static CCSprite *createStar(int stars,float between,const char* redstar,const char* yellowstar);
        static CCSpriteFrame *initSpriteFrameWithName(const char*pszFilename);
        static void setVec(std::string* pStr, std::vector<string>* pVec);
        static std::string randomName();
    };

#endif
