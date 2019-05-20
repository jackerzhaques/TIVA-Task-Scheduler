#include "PriorityQueue.h"

#ifndef NULL
    #define NULL 0
#endif

//forward declarations
void Heapify(PriorityQueue *queue, int index);
void SwapItems(PriorityQueue *queue, int indexA, int indexB);
int ParentIndex(int index);

void InitializeQueue(PriorityQueue *queue){
    uint8_t i = 0;
    queue->nTasks = 0;
    for(i = 0; i < MAX_TASKS; i++){
        queue->pendingTasks[i] = NULL;
    }
}

void AddTaskToQueue(PriorityQueue *queue, Task* task){
    if(!IsQueueFull(queue)){
        uint8_t i, parent;

        //Add task to queue
        queue->pendingTasks[queue->nTasks] = task;
        queue->nTasks++;

        //Fix min heap property of queue
        i = queue->nTasks - 1;
        parent = ParentIndex(i);
        while(i != 0 && queue->pendingTasks[parent]->priority > queue->pendingTasks[i]->priority){
            SwapItems(queue, i, parent);
            i = parent;
            parent = ParentIndex(i);
        }
    }
    else{
        //Do nothing, queue is full
    }
}

void RunNextTask(PriorityQueue *queue){
    if(queue->nTasks > 0){
        queue->pendingTasks[0]->pCallback();

        //Remove the task
        SwapItems(queue, 0, queue->nTasks - 1);
        queue->pendingTasks[queue->nTasks - 1] = NULL;
        queue->nTasks--;

        //Reorder the queue
        Heapify(queue, 0);
    }
    else{
        //No tasks to run
    }
}

void RunAllTasks(PriorityQueue *queue){
    while(queue->nTasks > 0){
        RunNextTask(queue);
    }
}

bool IsQueueFull(PriorityQueue *queue){
    if(queue->nTasks >= MAX_TASKS){
        return true;
    }
    else{
        return false;
    }
}

//Fixes the min heap when extracting a node
void Heapify(PriorityQueue *queue, int index){
    uint8_t left = 2*index + 1;
    uint8_t right = 2*index + 2;
    uint8_t smallest = index;

    //If the left task has lower priority, set it as the lowest
    if(left < queue->nTasks && queue->pendingTasks[left]->priority < queue->pendingTasks[smallest]->priority){
        smallest = left;
    }

    //If the right task has lower priority, set it as the lowest
    if(right < queue->nTasks && queue->pendingTasks[right]->priority < queue->pendingTasks[smallest]->priority){
        smallest = right;
    }

    //If the passed index was not the lowest, swap the two and repeat
    if(smallest != index){
        SwapItems(queue, smallest, index);
        Heapify(queue, smallest);
    }
}

void SwapItems(PriorityQueue *queue, int indexA, int indexB){
    Task* tempTaskPointer = queue->pendingTasks[indexB];
    queue->pendingTasks[indexB] = queue->pendingTasks[indexA];
    queue->pendingTasks[indexA] = tempTaskPointer;
}

int ParentIndex(int index){
    return (index - 1) / 2;
}
