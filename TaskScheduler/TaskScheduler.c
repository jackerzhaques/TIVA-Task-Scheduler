//Project includes
#include "TaskScheduler.h"
#include "PriorityQueue.h"

//Tivaware includes
#include <driverlib/sysctl.h>
#include <driverlib/timer.h>
#include <inc/hw_ints.h>
#include <driverlib/interrupt.h>
#include <inc/hw_memmap.h>
#include <driverlib/rom.h>

#ifndef NULL
    #define NULL 0
#endif

static volatile TaskScheduler   scheduler;

//Does not need to be volatile because it is only used in the ISR
static PriorityQueue   queue;
static PriorityQueue*           pqueue;

/*
 * Timer ISR for the task scheduler
 *
 * Updates each timer's task
 * If the task is ready to be fired, then it is added to a queue and processed
 *
 * This timer must be given a low priority ISR because it is possible to have
 * a heavy load to process
 */
void TaskSchedulerTimer_ISR(void){
    //Loop through every task
    Task *pTask = scheduler.pTaskListRoot;
    while(pTask){
        if(pTask->enabled){
            pTask->ticks++;

            if(pTask->ticks >= pTask->maxTicks){
                pTask->ticks = 0;

                AddTaskToQueue(pqueue, pTask);
            }
        }
        else{
            //Do nothing, but reset the ticks
            pTask->ticks = 0;
        }

        //Move to next task
        pTask = pTask->pNextTask;
    }

    //Run all pending tasks
    RunAllTasks(pqueue);

    TimerIntClear(scheduler.timerBase, TIMER_TIMA_TIMEOUT);
}

void InitializeTaskScheduler(uint32_t timerBase, uint32_t sysCtlTimerPeriph, uint32_t sysClkFreq, uint32_t timerIntBase){
    //Initialize the task scheduler
    scheduler.timerBase = timerBase;
    scheduler.pTaskListRoot = NULL;

    //Initialize the priority queue
    pqueue = &queue;
    InitializeQueue(pqueue);

    //Initialize the timer
    SysCtlPeripheralEnable(sysCtlTimerPeriph);

    //Wait for the clock to stabilize
    SysCtlDelay(100);
    IntMasterEnable();

    //Configure the timer to be a periodic 100us timer
    TimerConfigure(timerBase, TIMER_CFG_PERIODIC);

    TimerLoadSet(timerBase, TIMER_A, sysClkFreq * (TASK_SCHEDULER_TIMER_PERIOD / 1000000.0f));

    //Configure the ISR
    TimerIntRegister(timerBase, TIMER_A, TaskSchedulerTimer_ISR);
    IntEnable(timerIntBase);
    TimerIntEnable(timerBase, TIMER_TIMA_TIMEOUT);

    //Enable the timer and interrupts
    TimerEnable(timerBase, TIMER_A);
}

void AddTask(Task *pTask){
    //Reset the task and calculate the max ticks
    pTask->ticks = 0;
    pTask->maxTicks = pTask->period * TASK_SCHEDULER_TICKS_IN_ONE_SECOND;

    //Add the task to the list
    pTask->pNextTask = scheduler.pTaskListRoot;
    scheduler.pTaskListRoot = pTask;
}

//Only remove the task if you must
//It is recommended to just disable the task
void RemoveTask(Task *pTask){
    Task *pTaskRoot = scheduler.pTaskListRoot;

    //Loop until we are pointing to the task before the one to be removed
    //Or no task was found
    bool taskFound = false;
    while(pTaskRoot->pNextTask){
        if(pTaskRoot->pNextTask == pTask){
            taskFound = true;
            break;
        }
        else{
            //Keep searching
            pTaskRoot = pTaskRoot->pNextTask;
        }
    }

    if(taskFound){
        //Remove the task from the list
        pTaskRoot->pNextTask = pTask->pNextTask;
    }
}

void DisableTask(Task *pTask){
    pTask->enabled = false;
}
