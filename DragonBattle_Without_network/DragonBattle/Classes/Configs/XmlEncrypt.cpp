#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string>
#include "XmlEncrypt.h"

#ifdef WONPEE_SERVER
#else
#include "cocos2d.h"
USING_NS_CC;
#endif


static int s_key[]={1,2,3,4,5};

void XmlEncrypt::listDir(char* path)
{
	DIR* pDir = opendir(path);
	char childPath[512];
	memset(childPath, 0, sizeof(childPath));
	struct dirent *ent;

	while ((ent = readdir(pDir)) != NULL)
	{
		if (ent->d_type & DT_DIR)
		{
			if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
			{
				continue;
			}
			sprintf(childPath, "%s/%s", path, ent->d_name);
			printf("path:%s/n", childPath);
			listDir(childPath);
		}
		else
		{
			printf("%s ", ent->d_name);
			if (ent->d_name[0] == '.')
			{
				continue;
			}
			char xml_file[256] = {0};
			sprintf(xml_file, "%s/%s", path, ent->d_name);

			char encrypt_file[256] = {0};
			sprintf(encrypt_file, "%s/../conf_out/%s", path, ent->d_name);
			XmlEncrypt::encryptXmlFile(xml_file, encrypt_file);

			sleep(1);
			sprintf(xml_file, "%s/../conf_in/%s", path, ent->d_name);
			char* buf = XmlEncrypt::readEncryptXmlFile(encrypt_file);
			FILE* fd = fopen(xml_file, "w");
			if (fd != NULL)
			{
				fwrite(buf, strlen(buf), 1, fd);
				fclose(fd);
			}
            free(buf);
            buf = NULL;
        }
    }
}

xmlDocPtr XmlEncrypt::getXmlDocPtr(const char* xmlFile, bool bEncrypt,const char* writeFile)
{
	if (writeFile)
	{
		encryptXmlFile1(xmlFile,writeFile);
	}
	if (bEncrypt)
	{
		//char* xmlString = readEncryptXmlFile(xmlFile);
		char* xmlString = readEncryptXmlFile1(xmlFile);
		xmlDocPtr pDoc = xmlReadMemory(xmlString, strlen(xmlString), NULL, "UTF-8", XML_PARSE_RECOVER);
		free(xmlString);
		return pDoc;
	}
#ifdef WONPEE_SERVER
    return xmlParseFile(xmlFile);
#else
    unsigned long size = 0;
    char*  pBuffer = (char*)CCFileUtils::sharedFileUtils()->getFileData(xmlFile, "rt", &size);
    xmlDocPtr pDoc = xmlReadMemory(pBuffer, size, NULL, "UTF-8", XML_PARSE_RECOVER);
    free(pBuffer);
    
    if (pDoc) {
        CCLOG("apple.....Success parse %s", xmlFile);
    } else {
        CCLOG("apple.....Failed parse %s", xmlFile);
    }
    return pDoc;
#endif
}

// 对xml文件加密
int XmlEncrypt::encryptXmlFile(const char* xmlFile, const char* encryptFile)
{
	// 读xml文件
	FILE* fd;
	if ((fd = fopen(xmlFile, "r")) == NULL)
	{
		printf("can not open xml file:[%s]\n", encryptFile);
		return -1;
	}
	
	fseek(fd, 0, SEEK_END);
	size_t size = ftell(fd);
	fseek(fd, 0, SEEK_SET);
	
	int lenght = (size + 15) / 16 * 16;
	char *input = (char*)malloc(lenght);
	fread(input, 1, size, fd);
	fclose(fd);
	
	AES aes(XML_ENCRYPT_KEY);
	// 加密数据写到encryptFile中
	input = (char*)aes.Cipher(input, lenght);
	fd = fopen(encryptFile, "wb");
	fwrite(&size, 4, 1, fd);
	fwrite(input, lenght, 1, fd);
	fclose(fd);
	
	free(input);
	return 0;
}
	
// 对加密的二进制数据解密
int XmlEncrypt::decryptXmlFile(const char* encryptFile, const char* xmlFile)
{
	// 读xml加密文件
	FILE* fd;
	if ((fd = fopen(encryptFile, "rb")) == NULL)
	{
		printf("can not open cipher file:[%s]\n", encryptFile);
		return -1;
	}
	
	fseek(fd, 0, SEEK_END);
	size_t size = ftell(fd);
	fseek(fd, 0, SEEK_SET);
	char input[2000000];
	memset(input, 0, sizeof(input));
	fread(input, size, 1, fd);
	fclose(fd);
	
	int fileLen = *(int*)(input);
	//printf("invCipherXmlFile:[%s][%s][%d][%d] \n", cipher_file, xml_file, (int)size, file_len);
	
	//写回
	AES aes(XML_ENCRYPT_KEY);
	char* buf = (char*)aes.InvCipher(input + 4, size - 4);
	fd = fopen(xmlFile, "w");
	if (fd != NULL)
	{
		fwrite(buf, fileLen, 1, fd);
		fclose(fd);	
	}
	
	return 0;
}
	
// 从文件中解析
char* XmlEncrypt::readEncryptXmlFile(const char* encryptFile)
{
	// 读xml加密文件
	FILE* fd;
	if ((fd = fopen(encryptFile, "rb")) == NULL)
	{
		printf("can not open cipher file:[%s]\n", encryptFile);
		return NULL;
	}
	
	fseek(fd, 0, SEEK_END);
	size_t size = ftell(fd);
	fseek(fd, 0, SEEK_SET);
	
	int fileLen = 0;
	fread(&fileLen, 4, 1, fd);
	
	char *input = (char*)malloc(size - 4 + 1);
	memset(input, 0, (size - 4 + 1));
	fread(input, size - 4, 1, fd);
	fclose(fd);
	
	AES aes(XML_ENCRYPT_KEY);
	input = (char*)aes.InvCipher(input, size - 4);
	input[fileLen] = '\0';
	return input;
}

int XmlEncrypt::encryptXmlFile1(const char* xmlFile,const char* writeFile)
{
	// 读xml文件
	FILE* fd;
	if ((fd = fopen(xmlFile, "r")) == NULL || writeFile == NULL)
	{
		printf("can not open xml file:[%s]\n",xmlFile);
		return -1;
	}
    
	fseek(fd, 0, SEEK_END);
	size_t size = ftell(fd);
	fseek(fd, 0, SEEK_SET);
	
	int lenght = size+1;
	char *input = (char*)malloc(lenght);
	fread(input, 1, size, fd);
	fclose(fd);
    
	Makecode(input,s_key);
	input[lenght -1] = '\0';
	FILE *fw;
	std::string file = writeFile;
	fw = fopen(writeFile, "w");
	if (fw != NULL)
	{
		fwrite(input, lenght, 1, fw);
	}
    fclose(fw);
    free(input);
    return 1;
    /*
    FILE *fc,*ff;
    fc=fopen(file.c_str(),"r");
    fseek(fc, 0, SEEK_END);
    size = ftell(fc);
	fseek(fc, 0, SEEK_SET);
    int lenght1 = size;
	char *input1 = (char*)malloc(lenght1);
    fread(input1, 1, size, fc);
    Cutecode(input1,key);
    fclose(fc);
    
    ff=fopen(file.c_str(),"w");
    fseek(ff, 0, SEEK_SET);
	if (ff != NULL)
	{
		fwrite(input1,lenght1, 1, ff);
	}
    fclose(ff);
     */
}

char* XmlEncrypt::readEncryptXmlFile1(const char* encryptFile)
{
	// 读xml加密文件
	FILE* fd;
	if ((fd = fopen(encryptFile, "r")) == NULL)
	{
		printf("can not open cipher file:[%s]\n", encryptFile);
		return NULL;
	}
	fseek(fd, 0, SEEK_END);
	size_t size = ftell(fd);
	fseek(fd, 0, SEEK_SET);
	int lenght1 = size;
	char *input1 = (char*)malloc(lenght1);
	fread(input1, 1, size, fd);
	Cutecode(input1,s_key);
    fclose(fd);
	return input1;
}

//单个字符异或运算
char XmlEncrypt::MakecodeChar(char c,int key)
{
	return c = c ^ key;
}

//单个字符解密
char XmlEncrypt::CutcodeChar(char c,int key)
{
	return c ^ key;
}

//加密
void XmlEncrypt::Makecode(char *pstr,int *pkey)
{
	int len = strlen(pstr);//获取长度
    for (int i = 0; i < len; i++)
	{
        *(pstr + i) = MakecodeChar(*(pstr + i), pkey[i % 5]);
	}
}

//解密
void XmlEncrypt::Cutecode(char *pstr,int *pkey)
{
	int len = strlen(pstr);
	for (int i = 0; i < len; i++)
	{
		*(pstr + i) = CutcodeChar(*(pstr + i), pkey[i % 5]);
	}
}

