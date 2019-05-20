#ifndef TIVA_TASK_SCHEDULER_PRIORITY_QUEUE_H
#define TIVA_TASK_SCHEDULER_PRIORITY_QUEUE_H

#include <stdint.h>
#include <stdbool.h>

#include "TaskScheduler.h"

#define MAX_TASKS   15   //Must be an order of 2 - 1 (i.e 1, 3, 7, 15, 31)


struct PriorityQueue_tag;
typedef struct PriorityQueue_tag PriorityQueue;

struct PriorityQueue_tag{
    int nTasks;
    Task* pendingTasks[MAX_TASKS];
};

void InitializeQueue(PriorityQueue *queue);
void AddTaskToQueue(PriorityQueue *queue, Task* task);
void RunNextTask(PriorityQueue *queue);
void RunAllTasks(PriorityQueue *queue);
bool IsQueueFull(PriorityQueue *queue);

#endif
