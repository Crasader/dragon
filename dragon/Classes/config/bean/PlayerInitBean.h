//
//  PlayerInitBean.h
//  dragon
//
//  Created by xingchong on 2/18/16.
//
//

#ifndef __dragon__PlayerInitBean__
#define __dragon__PlayerInitBean__

#include <stdio.h>


USING_NS_CC;
using namespace std;


class PlayerInitBean : public Ref
{
public:
    static PlayerInitBean* create(int configId);
    
public:
    PlayerInitBean();
    ~PlayerInitBean();
    
    bool init(int configId);
    
    int getId(){return _id;};
    
    std::string getName(){return _name;};
    
    int getResTemplateId(){return _resTemplateId;};
    
    int getResEffectId(){return _resEffectId;};
    
    ValueVector getSkillInitIds(){return _skillInitIds;};
    
    int getAiInitId(){return _aiInitId;};
    
private:
    int _configId;
    std::string _fileName;
    int _id;
    std::string _name;
    int _resTemplateId;
    int _resEffectId;
    ValueVector _skillInitIds;
    int _aiInitId;
};

#endif /* defined(__dragon__PlayerInitBean__) */
