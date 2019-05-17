#ifndef TIVA_TASK_SCHEDULER_H
#define TIVA_TASK_SCHEDULER_H

#include <stdint.h>
#include <stdbool.h>

//For the most accurate timer, choose a frequency that is an integer
#define TASK_SCHEDULER_TIMER_PERIOD         100     //us
#define TASK_SCHEDULER_TICKS_IN_ONE_SECOND  1/TASK_SCHEDULER_TIMER_PERIOD

struct Task_tag;
typedef struct Task_tag Task;

struct Task_tag {
    uint32_t ticks;
    uint32_t maxTicks;
    float period;

    uint8_t priority;
    bool enabled;

    void (*pCallback)(void);
    Task *pNextTask;
};

typedef struct TaskScheduler_tag{
    uint32_t timerBase;
    Task *pTaskListRoot;
} TaskScheduler;

void InitializeTaskScheduler(uint32_t timerBase, uint32_t sysClkFreq, uint32_t timerIntBase);

void AddTask(Task *pTask);
void RemoveTask(Task *pTask);
void DisableTask(Task *pTask);

#endif
