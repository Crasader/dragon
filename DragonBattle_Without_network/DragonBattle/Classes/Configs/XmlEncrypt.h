#ifndef XML_MANAGER_H__
#define XML_MANAGER_H__

#ifdef WONPEE_SERVER
#include "../AES.h"
#include "../../share/xmldecode.h"
#else
#include "xml_decode.h"
#include "AES.h"
#endif

#define XML_ENCRYPT_KEY (unsigned char*)"@oXpwMeLen"

class XmlEncrypt
{
public:
    //
    static void listDir(char* path);
    
	static xmlDocPtr getXmlDocPtr(const char* xmlFile,bool bEncrypt,const char* writeFile = NULL);

	// 对xml文件加密
	static int encryptXmlFile(const char* xml_file, const char* cipher_file);
	static int encryptXmlFile1(const char* xmlFile,const char* writeFile);
	// 对加密的二进制数据解密
	static int decryptXmlFile(const char* cipher_file, const char* xml_file);
	
	// 从文件中解析
	static char* readEncryptXmlFile(const char* cipher_file);
    static char* readEncryptXmlFile1(const char* encryptFile);
    static char MakecodeChar(char c,int key);
    static char CutcodeChar(char c,int key);
    static void Makecode(char *pstr,int *pkey);
    static void Cutecode(char *pstr,int *pkey);
};

#endif
