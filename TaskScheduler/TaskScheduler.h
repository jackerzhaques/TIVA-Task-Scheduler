#ifndef TIVA_TASK_SCHEDULER_H
#define TIVA_TASK_SCHEDULER_H

#include <stdint.h>
#include <stdbool.h>

struct Task_tag;
typedef struct Task_tag Task;

struct Task_tag {
    uint32_t taskTimerTicks;
    float taskPeriod;
    void (*pCallback)(void);
    bool taskEnabled;
    Task *pNextTask;
};

typedef struct TaskScheduler_tag{
    uint32_t timerBase;
    Task *pTaskListRoot;
} TaskScheduler;

void InitializeTaskScheduler(uint32_t timerBase);

void AddTask(Task *pTask);
void RemoveTask(Task *pTask);
void DisableTask(Task *pTask);

#endif
