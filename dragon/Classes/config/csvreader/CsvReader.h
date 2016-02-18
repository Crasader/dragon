//
//  CsvReader.h
//  xygame
//
//  Created by xingchong on 11/18/14.
//
//

#ifndef __xygame__CsvReader__
#define __xygame__CsvReader__


#include <string>
#include <map>
#include "cocos2d.h"
class HCSVFile;

USING_NS_CC;

class CsvReader :public Ref
{
public:
    CsvReader();
    ~CsvReader();
    
    static CsvReader* getInstance();
    HCSVFile* getCSVFileByName(const char* filename);
	static void clear();
    
private:
    static CsvReader *s_instance;
    std::map<std::string,HCSVFile*> m_mapFiles;
};

#endif /* defined(__xygame__CsvReader__) */
