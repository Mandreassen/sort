// Application files
#include "sort.h"
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct task {
    int *data;
    int min;
    int max;
    int level;
} task_t;



int select_pivot(int *data, int min, int max)
{
    int h, l, m;
    
    m = (min + max) / 2;
    
    if (data[min] < data[max]) {
        l = min;
        h = max;
    } else {
        l = max;
        h = min;
    }

    if (data[m] < data[l]) {
        SWAP(data[l], data[max]);
        return max;
    } else if (data[m] > data[h]) {
        SWAP(data[h], data[max]);
        return max;
    } else {
        SWAP(data[m], data[max]);
        return max;
    }        
}


static int partition(int *data, int min, int max)
{    
    int pivot = select_pivot(data, min, max);
    int wall  = min;
    int curr  = min;
    
    while (curr < pivot) {
        
        if (data[curr] < data[pivot]) {
            SWAP(data[curr], data[wall]);
            curr++;
            wall++;                      
        } else {
            curr++; 
        }
    }
    
    SWAP(data[pivot], data[wall]);
    
    return wall;
}

task_t *create_task(int *data, int min, int max, int level)
{
    task_t *new = malloc(sizeof(task_t));
    new->data = data;
    new->min  = min;
    new->max  = max;
    new->level = level;
    
    return new;
}


static void _quicksort(int *data, int min, int max)
{
    if (max - min < 1)
        return; // Done
        
    int split = partition(data, min, max);
    
    _quicksort(data, min, split - 1);
    _quicksort(data, split + 1, max);
}


static void *_thread(void *task)
{
    int *data = ((task_t*) task)->data;
    int min   = ((task_t*) task)->min;
    int max   = ((task_t*) task)->max;
    int level = ((task_t*) task)->level;
    
    if (max - min < 1)
        return NULL; // Done     
        
    int split = partition(data, min, max);
    
    if (level > sysconf(_SC_NPROCESSORS_ONLN)) {
        
        _quicksort(data, min, split - 1);
        _quicksort(data, split + 1, max);  
              
    } else {
    
        task_t *taskA = create_task(data, min, split - 1, level + 1);
        task_t *taskB = create_task(data, split + 1, max, level + 1);    

        pthread_t a, b; 
        pthread_create(&a, NULL, _thread, (void*) taskA);
        pthread_create(&b, NULL, _thread, (void*) taskB);    
        pthread_join(a, NULL);
        pthread_join(b, NULL);
        
        free(taskA);
        free(taskB);
    }
    
    return NULL; // Done
}


void sort_ints(int *data, int size)
{
    if (size < 1000000)
        return _quicksort(data, 0, size - 1);    
    
    task_t *task = create_task(data, 0, size-1, 1);    
    _thread((void*) task);
    free(task);
}
