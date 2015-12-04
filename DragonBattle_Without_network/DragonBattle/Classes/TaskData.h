//
//  TaskData.h
//  DragonBattle
//
//  Created by zeng xiaolong on 13-9-13.
//
//

#ifndef DragonBattle_TaskData_h
#define DragonBattle_TaskData_h
#include "cocos2d.h"
USING_NS_CC;
enum kNpcTaskState
{
    kTaskNull = 0,
    kTaskGot,   //已接
    kTaskCanGet,//可接
    kTaskCanGive,//可交
    kTaskMovie//剧情
};

class TaskState:public CCObject
{
public:
    int taskID;
    kNpcTaskState taskState;//1已接 2可接  3可交 4剧情  如果已接并且有剧情 状态为剧情
    static TaskState *create(int taskID,kNpcTaskState taskState)
    {
        TaskState *task = new TaskState();
        task->taskID = taskID;
        task->taskState = taskState;
        task->autorelease();
        return task;
    }
};
#endif
