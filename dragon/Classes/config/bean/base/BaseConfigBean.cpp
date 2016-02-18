//
//  BaseConfigBean.cpp
//  dragon
//
//  Created by xingchong on 2/18/16.
//
//

#include "BaseConfigBean.h"

BaseConfigBean::BaseConfigBean()
:_separate(";")
,_separateSub("|")
,_fileName("")
{
    
}

BaseConfigBean::~BaseConfigBean()
{
    
}

std::vector<std::string> BaseConfigBean::stringToVector(std::string &str)
{
    std::vector<std::string> ret;
    this->split(str, _separateSub, ret);
    return ret;
}

std::vector<std::vector<std::string>> BaseConfigBean::stringToMutiVector(std::string &str)
{
    std::vector<std::vector<std::string>> ret;
    
    std::vector<std::string> tempRet;
    this->split(str, _separate, tempRet);
    
    for (auto tempStr : tempRet) {
        std::vector<std::string> subRet;
        this->split(tempStr, _separateSub, subRet);
        ret.push_back(subRet);
    }
    return ret;
}

void  BaseConfigBean::split(std::string &s, std::string &separate, std::vector<std::string> &ret)
{
    size_t last = 0;
    size_t index=s.find_first_of(separate,last);
    while (index!=std::string::npos)
    {
        ret.push_back(s.substr(last,index-last));
        last=index+1;
        index=s.find_first_of(separate,last);
    }
    if (index-last>0)
    {
        ret.push_back(s.substr(last,index-last));
    }
}

