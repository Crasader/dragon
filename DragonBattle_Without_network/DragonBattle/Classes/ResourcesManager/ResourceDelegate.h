//
//  ResourceDelegate.h
//  AssetsManagerTest
//
//  Created by 白明江 on 2/17/14.
//
//

#ifndef __AssetsManagerTest__ResourceDelegate__
#define __AssetsManagerTest__ResourceDelegate__

#include <iostream>


enum ErrorCode
{
    // Error caused by creating a file to store downloaded data
    kCreateFile,
    /** Error caused by network
     -- network unavaivable
     -- timeout
     -- ...
     */
    kNetwork,
    /** There is not a new version
     */
    kNoNewVersion,
    /** Error caused in uncompressing stage
     -- can not open zip file
     -- can not read file global information
     -- can not read file information
     -- can not create a directory
     -- ...
     */
    kUncompress,
};

class ResourceDelegate
{
public:
    /* @brief Call back function for error
     @param errorCode Type of error
     */
    virtual void onError(ErrorCode errorCode) = 0;
    /** @brief Call back function for recording downloading percent
     @param percent How much percent downloaded
     @warn This call back function just for recording downloading percent.
     AssetsManager will do some other thing after downloading, you should
     write code in onSuccess() after downloading.
     */
    virtual void onProgress(int percent) = 0;
    /** @brief Call back function for success
     */
    virtual void onSuccess() = 0;
};

#endif /* defined(__AssetsManagerTest__ResourceDelegate__) */
