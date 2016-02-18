//
//  ConfigReader.h
//  dragon
//
//  Created by xingchong on 2/18/16.
//
//

#ifndef __dragon__ConfigReader__
#define __dragon__ConfigReader__

#include <stdio.h>
#include "HCSVFile.h"
#include "cocos2d.h"

USING_NS_CC;


class ConfigReader : public Ref
{
public:
    enum class FileType
    {
        CSV,
        XLS
    };
    
    static ConfigReader *getInstance();
    
public:
    ConfigReader();
    ~ConfigReader();
    
    HCSVFile *getFileByName(std::string &fileName);
    
    void setFileType(FileType fileType){_fileType = fileType;};
    
    void cleanAll();
    
private:
    FileType _fileType;
};



#endif /* defined(__dragon__ConfigReader__) */
