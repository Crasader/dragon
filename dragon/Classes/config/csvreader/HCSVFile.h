//
//  HCSVFile.h
//  xygame
//
//  Created by xingchong on 11/19/14.
//
//

#ifndef __xygame__HCSVFile__
#define __xygame__HCSVFile__

#include <map>
#include <string>
#include <vector>
#include "DataSingleRecord.h"
#include "cocos2d.h"

#ifdef WIN32
#include "libxl.h"
using namespace libxl;
#endif

class HCSVFile 
{    
    
public:
    bool openFile(const char* fileName);
	bool openXlsFile(const char* fileName);
	const char* getDataType(const char* fieldName);

    Value	    getData(int id,const char* fieldName);
	ValueMap    getRowData(int id);
	ValueVector getAllId();
    
public:
    int strToVector(const char* strSource, std::vector<std::string>& vRet, const char* szKey);
    int strToValueVector(const char* strSource, ValueVector& vRet, const char* szKey);
    
private:
	bool createDBFromMemery(const char* pMemStart, const unsigned long dwSize);
	const char* GetLineFromMemory(char* pStringBuf, int nBufSize, const char* pMemStart, const char* pMemEnd);
	
#ifdef WIN32
	wchar_t* ctow(const char *str);
	char *w2c(char *pcstr,const wchar_t *pwstr, size_t len);

	std::string getXlsCellData(Sheet *pSheet, unsigned int row, unsigned int col);
#endif

private:
	std::map<int,DataSingleRecord> m_mapFileData;
	std::vector<std::string>       m_vecFieldType;
	std::vector<std::string>       m_vecFieldName;
	std::map<std::string,int>      m_mapColomIndex;
	std::string                    m_FileName;
};

#endif /* defined(__xygame__HCSVFile__) */
