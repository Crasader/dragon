//
//  CsvConfig.h
//  dragon
//
//  Created by xingchong on 2/24/16.
//
//

#ifndef __dragon__CsvConfig__
#define __dragon__CsvConfig__

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;

class CsvConfig : public Ref
{
public:
    static CsvConfig* create();
    
public:
    CsvConfig();
    ~CsvConfig();
    
    bool init();
    
public:
    std::string _playerInit;
    std::string _monsterInit;
};

#endif /* defined(__dragon__CsvConfig__) */
