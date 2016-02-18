//
//  BaseConfigBean.h
//  dragon
//
//  Created by xingchong on 2/18/16.
//
//

#ifndef __dragon__BaseConfigBean__
#define __dragon__BaseConfigBean__

#include <map>
#include <string>
#include <vector>
#include <stdio.h>
#include "cocos2d.h"
#include "ConfigReader.h"
#include "HCSVFile.h"

USING_NS_CC;

class BaseConfigBean : public Ref
{
public:
    BaseConfigBean();
    ~BaseConfigBean();
    
    std::vector<std::string> stringToVector(std::string &str);
    std::vector<std::vector<std::string>> stringToMutiVector(std::string &str);
    
private:
    void split(std::string& s, std::string& delim,std::vector< std::string > &ret);
    
protected:
    std::string _separate;
    std::string _separateSub;
    std::string _fileName;
    std::string _configId;
};

#endif /* defined(__dragon__BaseConfigBean__) */
