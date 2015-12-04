//
//  tools.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-19.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "tools.h"
#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include <sys/sysctl.h>
#include <mach/mach.h>
#endif

// 当前设备可用内存
double availableMemory()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    vm_statistics_data_t vmStats;
    mach_msg_type_number_t infoCount = HOST_VM_INFO_COUNT;
    kern_return_t kernReturn
    = host_statistics(mach_host_self(),
                      HOST_VM_INFO,
                      (host_info_t)&vmStats,
                      &infoCount);
    if(kernReturn != KERN_SUCCESS) {
        return 0;
    }
    return ((vm_page_size * vmStats.free_count) / 1024.0) / 1024.0;
#else
    return 0;
#endif
}

// 任务占用内存
double usedMemory()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    task_basic_info_data_t taskInfo;
    mach_msg_type_number_t infoCount = TASK_BASIC_INFO_COUNT;
    kern_return_t kernReturn = task_info(mach_task_self(),
                                         TASK_BASIC_INFO, (task_info_t)&taskInfo, &infoCount);
    if(kernReturn != KERN_SUCCESS) {
        return 0;
    }
    return taskInfo.resident_size / 1024.0 / 1024.0;
#else
    return 0;
    #endif
}