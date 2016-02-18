#pragma once

#include <string>
#include <map>
#include "cocos2d.h"

class HCSVFile;

USING_NS_CC;
using namespace std;

class XlsReader : public Ref
{
private:
	 static XlsReader *s_instance;
	 map<string,HCSVFile*> m_mapFiles;


public:
	XlsReader(void);
	~XlsReader(void);

	static XlsReader * getInstance();

	HCSVFile* getCSVFileByName(const char* filename);

	static void cleanup();

};

