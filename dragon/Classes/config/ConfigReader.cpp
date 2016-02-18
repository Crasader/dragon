//
//  ConfigReader.cpp
//  dragon
//
//  Created by xingchong on 2/18/16.
//
//

#include "ConfigReader.h"
#include "CsvReader.h"
#include "XlsReader.h"

ConfigReader *_instance = nullptr;

ConfigReader::ConfigReader()
:_fileType(FileType::CSV)
{
    
}

ConfigReader::~ConfigReader()
{
    
}

ConfigReader* ConfigReader::getInstance()
{
    if (!_instance) {
        _instance = new ConfigReader();
    }
    return _instance;
}

HCSVFile* ConfigReader::getFileByName(std::string &fileName)
{
    HCSVFile *file = nullptr;
    if (_fileType == FileType::CSV)
    {
        file = CsvReader::getInstance()->getCSVFileByName(fileName.c_str());
    }
    else if(_fileType == FileType::XLS)
    {
        file = XlsReader::getInstance()->getCSVFileByName(fileName.c_str());
    }
    return file;
}

void ConfigReader::cleanAll()
{
    if (FileType::CSV == _fileType)
    {
        CsvReader::getInstance()->clear();
    }
    else if (FileType::XLS == _fileType)
    {
        XlsReader::getInstance()->cleanup();
    }
}


