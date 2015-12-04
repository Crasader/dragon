//
//  NetResourceManager.cpp
//  NetResourceManagerTest
//
//  Created by 白明江 on 2/17/14.
//
//

#include "NetResourceManager.h"

#include "cocos2d.h"

#include <curl/curl.h>
#include <curl/easy.h>

#include <stdio.h>
#include <vector>

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>


#include "support/zip_support/unzip.h"

using namespace cocos2d;
using namespace std;

#define KEY_OF_VERSION   "current-version-code"
#define KEY_OF_DOWNLOADED_VERSION    "downloaded-version-code"
#define TEMP_PACKAGE_FILE_NAME    "cocos2dx-update-temp-package.zip"
#define BUFFER_SIZE    8192
#define MAX_FILENAME   512

// Message type
#define NetResourceManager_MESSAGE_UPDATE_SUCCEED                0
#define NetResourceManager_MESSAGE_RECORD_DOWNLOADED_VERSION     1
#define NetResourceManager_MESSAGE_PROGRESS                      2
#define NetResourceManager_MESSAGE_ERROR                         3

// Some data struct for sending messages

struct ErrorMessage
{
    ErrorCode code;
    NetResourceManager* manager;
};

struct ProgressMessage
{
    int percent;
    NetResourceManager* manager;
};

// Implementation of NetResourceManager

NetResourceManager::NetResourceManager(const char* packageUrl/* =NULL */, const char* versionFileUrl/* =NULL */, const char* storagePath/* =NULL */)
:  _storagePath(storagePath)
, _version("")
, _packageUrl(packageUrl)
, _versionFileUrl(versionFileUrl)
, _downloadedVersion("")
, _curl(NULL)
, _tid(NULL)
, _connectionTimeout(0)
, _delegate(NULL)
{
    checkStoragePath();
    _schedule = new Helper();
}

NetResourceManager::~NetResourceManager()
{
    if (_schedule)
    {
        _schedule->release();
    }
}

void NetResourceManager::checkStoragePath()
{
    if (_storagePath.size() > 0 && _storagePath[_storagePath.size() - 1] != '/')
    {
        _storagePath.append("/");
    }
}

static size_t getVersionCode(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    string *version = (string*)userdata;
    version->append((char*)ptr, size * nmemb);
    
    return (size * nmemb);
}

bool NetResourceManager::checkUpdate()
{
    if (_versionFileUrl.size() == 0) return false;
    
    _curl = curl_easy_init();
    if (! _curl)
    {
        CCLOG("can not init curl");
        return false;
    }
    
    // Clear _version before assign new value.
    _version.clear();
    
    CURLcode res;
    curl_easy_setopt(_curl, CURLOPT_URL, _versionFileUrl.c_str());
    curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, getVersionCode);
    curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &_version);
    if (_connectionTimeout) curl_easy_setopt(_curl, CURLOPT_CONNECTTIMEOUT, _connectionTimeout);
    res = curl_easy_perform(_curl);
    
    if (res != 0)
    {
        sendErrorMessage(kNetwork);
        CCLOG("can not get version file content, error code is %d", res);
        curl_easy_cleanup(_curl);
        return false;
    }
    
    string recordedVersion = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_OF_VERSION);
    if (recordedVersion == _version)
    {
        sendErrorMessage(kNoNewVersion);
        CCLOG("there is not new version");
        // Set resource search path.
        setSearchPath();
        return false;
    }
    
    CCLOG("there is a new version: %s", _version.c_str());
    
    return true;
}

void* DownloadAndUncompress(void *data)
{
    NetResourceManager* self = (NetResourceManager*)data;
    
    do
    {
        if (self->_downloadedVersion != self->_version)
        {
            if (! self->downLoad()) break;
            
            // Record downloaded version.
            NetResourceManager::Message *msg1 = new NetResourceManager::Message();
            msg1->what = NetResourceManager_MESSAGE_RECORD_DOWNLOADED_VERSION;
            msg1->obj = self;
            self->_schedule->sendMessage(msg1);
        }
        
        // Uncompress zip file.
        if (! self->uncompress())
        {
            self->sendErrorMessage(kUncompress);
            break;
        }
        
        // Record updated version and remove downloaded zip file
        NetResourceManager::Message *msg2 = new NetResourceManager::Message();
        msg2->what = NetResourceManager_MESSAGE_UPDATE_SUCCEED;
        msg2->obj = self;
        self->_schedule->sendMessage(msg2);
    } while (0);
    
    if (self->_tid)
    {
        delete self->_tid;
        self->_tid = NULL;
    }
    
    return NULL;
}

void NetResourceManager::update()
{
    if (_tid) return;
    
    // 1. Urls of package and version should be valid;
    // 2. Package should be a zip file.
    if (_versionFileUrl.size() == 0 ||
        _packageUrl.size() == 0 ||
        (std::string::npos == _packageUrl.find(".dat") && std::string::npos == _packageUrl.find(".zip")))
    {
        CCLOG("no version file url, or no package url, or the package is not a zip file");
        return;
    }
    
    // Check if there is a new version.
    //if (! checkUpdate()) return;
    
    // Is package already downloaded?
    _downloadedVersion = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_OF_DOWNLOADED_VERSION);
    
    _tid = new pthread_t();
    pthread_create(&(*_tid), NULL, DownloadAndUncompress, this);
}

bool NetResourceManager::uncompress()
{
    // Open the zip file
    string outFileName = _storagePath + TEMP_PACKAGE_FILE_NAME;
    return uncompressZip(outFileName.c_str());
}

bool NetResourceManager::uncompressZip(const char* zip_file_path)
{
    // Open the zip file
    unzFile zipfile = unzOpen(zip_file_path);
    if (! zipfile)
    {
        CCLOG("can not open downloaded zip file %s", zip_file_path);
        return false;
    }
    
    // Get info about the zip file
    unz_global_info global_info;
    if (unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK)
    {
        CCLOG("can not read file global info of %s", zip_file_path);
        unzClose(zipfile);
        return false;
    }
    
    // Buffer to hold data read from the zip file
    char readBuffer[BUFFER_SIZE];
    
    CCLOG("start uncompressing");
    
    // Loop to extract all files.
    uLong i;
    for (i = 0; i < global_info.number_entry; ++i)
    {
        // Get info about current file.
        unz_file_info fileInfo;
        char fileName[MAX_FILENAME];
        if (unzGetCurrentFileInfo(zipfile,
                                  &fileInfo,
                                  fileName,
                                  MAX_FILENAME,
                                  NULL,
                                  0,
                                  NULL,
                                  0) != UNZ_OK)
        {
            CCLOG("can not read file info");
            unzClose(zipfile);
            return false;
        }
        
        string fullPath = _storagePath + fileName;
        
        // Check if this entry is a directory or a file.
        const size_t filenameLength = strlen(fileName);
        if (fileName[filenameLength-1] == '/')
        {
            // Entry is a direcotry, so create it.
            // If the directory exists, it will failed scilently.
            if (!createDirectory(fullPath.c_str()))
            {
                CCLOG("can not create directory %s", fullPath.c_str());
                unzClose(zipfile);
                return false;
            }
        }
        else
        {
            // Entry is a file, so extract it.
            
            // Open current file.
            if (unzOpenCurrentFile(zipfile) != UNZ_OK)
            {
                CCLOG("can not open file %s", fileName);
                unzClose(zipfile);
                return false;
            }
            
            // Create a file to store current file.
            FILE *out = fopen(fullPath.c_str(), "wb");
            if (! out)
            {
                CCLOG("can not open destination file %s", fullPath.c_str());
                unzCloseCurrentFile(zipfile);
                unzClose(zipfile);
                return false;
            }
            
            // Write current file content to destinate file.
            int error = UNZ_OK;
            do
            {
                error = unzReadCurrentFile(zipfile, readBuffer, BUFFER_SIZE);
                if (error < 0)
                {
                    CCLOG("can not read zip file %s, error code is %d", fileName, error);
                    unzCloseCurrentFile(zipfile);
                    unzClose(zipfile);
                    return false;
                }
                
                if (error > 0)
                {
                    fwrite(readBuffer, error, 1, out);
                }
            } while(error > 0);
            
            fclose(out);
        }
        
        unzCloseCurrentFile(zipfile);
        
        // Goto next entry listed in the zip file.
        if ((i+1) < global_info.number_entry)
        {
            if (unzGoToNextFile(zipfile) != UNZ_OK)
            {
                CCLOG("can not read next file");
                unzClose(zipfile);
                return false;
            }
        }
    }
    
    CCLOG("end uncompressing");
    
    return true;
}

/*
 * Create a direcotry is platform depended.
 */
bool NetResourceManager::createDirectory(const char *path)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    mode_t processMask = umask(0);
    int ret = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
    umask(processMask);
    if (ret != 0 && (errno != EEXIST))
    {
        return false;
    }
    
    return true;
#else
    BOOL ret = CreateDirectoryA(path, NULL);
	if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
	{
		return false;
	}
    return true;
#endif
}

void NetResourceManager::setSearchPath()
{
    vector<string> searchPaths = CCFileUtils::sharedFileUtils()->getSearchPaths();
    vector<string>::iterator iter = searchPaths.begin();
    searchPaths.insert(iter, _storagePath);
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);
}

static size_t downLoadPackage(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    FILE *fp = (FILE*)userdata;
    size_t written = fwrite(ptr, size, nmemb, fp);
    return written;
}

int ProgressFunc(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded)
{
    NetResourceManager* manager = (NetResourceManager*)ptr;
    NetResourceManager::Message *msg = new NetResourceManager::Message();
    msg->what = NetResourceManager_MESSAGE_PROGRESS;
    
    int percent = (int)(nowDownloaded/totalToDownload*100);
    
    ProgressMessage *progressData = new ProgressMessage();
    progressData->percent = percent;
    progressData->manager = manager;
    msg->obj = progressData;
    
    manager->_schedule->sendMessage(msg);
    
    CCLOG("downloading11... %d%%", percent);
    
    return 0;
}

bool NetResourceManager::downLoad()
{
    // Create a file to save package.
    string outFileName = _storagePath + TEMP_PACKAGE_FILE_NAME;
    FILE *fp = fopen(outFileName.c_str(), "wb");
    if (! fp)
    {
        sendErrorMessage(kCreateFile);
        CCLOG("can not create file %s", outFileName.c_str());
        return false;
    }
    
    // Download pacakge
    CURLcode res;
    curl_easy_setopt(_curl, CURLOPT_URL, _packageUrl.c_str());
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, downLoadPackage);
    curl_easy_setopt(_curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(_curl, CURLOPT_NOPROGRESS, false);
    curl_easy_setopt(_curl, CURLOPT_PROGRESSFUNCTION, ProgressFunc);
    curl_easy_setopt(_curl, CURLOPT_PROGRESSDATA, this);
    res = curl_easy_perform(_curl);
    curl_easy_cleanup(_curl);
    if (res != 0)
    {
        sendErrorMessage(kNetwork);
        CCLOG("error when download package");
        fclose(fp);
        return false;
    }
    
    CCLOG("succeed downloading package %s", _packageUrl.c_str());
    
    fclose(fp);
    return true;
}

const char* NetResourceManager::getPackageUrl() const
{
    return _packageUrl.c_str();
}

void NetResourceManager::setPackageUrl(const char *packageUrl)
{
    _packageUrl = packageUrl;
}

const char* NetResourceManager::getStoragePath() const
{
    return _storagePath.c_str();
}

void NetResourceManager::setStoragePath(const char *storagePath)
{
    _storagePath = storagePath;
    checkStoragePath();
}

const char* NetResourceManager::getVersionFileUrl() const
{
    return _versionFileUrl.c_str();
}

void NetResourceManager::setVersionFileUrl(const char *versionFileUrl)
{
    _versionFileUrl = versionFileUrl;
}

string NetResourceManager::getVersion()
{
    return CCUserDefault::sharedUserDefault()->getStringForKey(KEY_OF_VERSION);
}

void NetResourceManager::deleteVersion()
{
    CCUserDefault::sharedUserDefault()->setStringForKey(KEY_OF_VERSION, "");
}

void NetResourceManager::setDelegate(ResourceDelegate *delegate)
{
    _delegate = delegate;
}

void NetResourceManager::setConnectionTimeout(unsigned int timeout)
{
    _connectionTimeout = timeout;
}

unsigned int NetResourceManager::getConnectionTimeout()
{
    return _connectionTimeout;
}


// protected 
void NetResourceManager::sendErrorMessage(ErrorCode code)
{
    Message *msg = new Message();
    msg->what = NetResourceManager_MESSAGE_ERROR;
    
    ErrorMessage *errorMessage = new ErrorMessage();
    errorMessage->code = code;
    errorMessage->manager = this;
    msg->obj = errorMessage;
    
    _schedule->sendMessage(msg);
}

// Implementation of NetResourceManagerHelper

NetResourceManager::Helper::Helper()
{
    _messageQueue = new list<Message*>();
    pthread_mutex_init(&_messageQueueMutex, NULL);
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

NetResourceManager::Helper::~Helper()
{
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    delete _messageQueue;
}

void NetResourceManager::Helper::sendMessage(Message *msg)
{
    pthread_mutex_lock(&_messageQueueMutex);
    ProgressMessage* cur_msg = (ProgressMessage*)(msg->obj);
    bool hasMsg = false;
    
    for (std::list<Message*>::iterator it = _messageQueue->begin(); it != _messageQueue->end(); it ++)
    {
        ProgressMessage* item_msg = (ProgressMessage*)((*it)->obj);
        if (item_msg->percent == cur_msg->percent)
        {
            hasMsg = true;
            break;
        }
    }
    if (!hasMsg)
    {
        _messageQueue->push_back(msg);
    }
    
    pthread_mutex_unlock(&_messageQueueMutex);
}

void NetResourceManager::Helper::update(float dt)
{
    Message *msg = NULL;
    
    // Returns quickly if no message
    pthread_mutex_lock(&_messageQueueMutex);
    if (0 == _messageQueue->size())
    {
        pthread_mutex_unlock(&_messageQueueMutex);
        return;
    }
    
    // Gets message
    msg = *(_messageQueue->begin());
    _messageQueue->pop_front();
    pthread_mutex_unlock(&_messageQueueMutex);
    
    switch (msg->what) {
        case NetResourceManager_MESSAGE_UPDATE_SUCCEED:
            handleUpdateSucceed(msg);
            
            break;
        case NetResourceManager_MESSAGE_RECORD_DOWNLOADED_VERSION:
            CCUserDefault::sharedUserDefault()->setStringForKey(KEY_OF_DOWNLOADED_VERSION,
                                                                ((NetResourceManager*)msg->obj)->_version.c_str());
            CCUserDefault::sharedUserDefault()->flush();
            
            break;
        case NetResourceManager_MESSAGE_PROGRESS:
            if (((ProgressMessage*)msg->obj)->manager->_delegate)
            {
                ((ProgressMessage*)msg->obj)->manager->_delegate->onProgress(((ProgressMessage*)msg->obj)->percent);
            }
            
            delete (ProgressMessage*)msg->obj;
            
            break;
        case NetResourceManager_MESSAGE_ERROR:
            // error call back
            if (((ErrorMessage*)msg->obj)->manager->_delegate)
            {
                ((ErrorMessage*)msg->obj)->manager->_delegate->onError(((ErrorMessage*)msg->obj)->code);
            }
            
            delete ((ErrorMessage*)msg->obj);
            
            break;
        default:
            break;
    }
    
    delete msg;
}

void NetResourceManager::Helper::handleUpdateSucceed(Message *msg)
{
    NetResourceManager* manager = (NetResourceManager*)msg->obj;
    
    // Record new version code.
    CCUserDefault::sharedUserDefault()->setStringForKey(KEY_OF_VERSION, manager->_version.c_str());
    
    // Unrecord downloaded version code.
    CCUserDefault::sharedUserDefault()->setStringForKey(KEY_OF_DOWNLOADED_VERSION, "");
    CCUserDefault::sharedUserDefault()->flush();
    
    // Set resource search path.
    manager->setSearchPath();
    
    // Delete unloaded zip file.
    string zipfileName = manager->_storagePath + TEMP_PACKAGE_FILE_NAME;
    if (remove(zipfileName.c_str()) != 0)
    {
        CCLOG("can not remove downloaded zip file %s", zipfileName.c_str());
    }
    
    if (manager) manager->_delegate->onSuccess();
}