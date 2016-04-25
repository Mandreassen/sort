/* author: Marius Andreassen (marius.andreassen@uit.no) */

// Standard libs
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

// Application files
#include "sort.h"


typedef struct task {
    int *data;
    int min;
    int max;
    int level;
} task_t;

/* Find pivot an place it at the end the of sub-array */
static int select_pivot(int *data, int min, int max)
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

/* Select pivot and partition data */
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

/* Recursive quicksort algorithm */
static void _quicksort(int *data, int min, int max)
{
    if (max - min < 1)
        return; // Done
        
    int split = partition(data, min, max);
    
    _quicksort(data, min, split - 1);
    _quicksort(data, split + 1, max);
}

/* Threaded quicksort algorithm */
static void *_thread(void *task)
{
    task_t t = *((task_t*) task);
    
    if (t.max - t.min < 1)
        return NULL; // Done     
        
    int split = partition(t.data, t.min, t.max);
    
    if (t.level > sysconf(_SC_NPROCESSORS_ONLN)) {
        
        _quicksort(t.data, t.min, split - 1);
        _quicksort(t.data, split + 1, t.max);  
              
    } else {
    
        pthread_t a, b; 
        task_t taskA = {.data = t.data, .min = t.min, .max = split - 1, .level = t.level + 1};
        task_t taskB = {.data = t.data, .min = split + 1, .max = t.max, .level = t.level + 1};
        
        pthread_create(&a, NULL, _thread, (void*) &taskA);
        pthread_create(&b, NULL, _thread, (void*) &taskB);
        
        pthread_join(a, NULL);
        pthread_join(b, NULL);
    }
    
    return NULL; // Done
}


/* SORT INTS:
 * Data sets smaller than 1000000 will
 * be sorted using a normal quicksort 
 * implementation.
 * Larger data sets will be sorted using
 * a threaded quicksort implementation. */
void sort_ints(int *data, int size)
{
    if (size < 1000000)
        return _quicksort(data, 0, size - 1);    
    
    task_t task = {.data = data, .min = 0, .max = size - 1, .level = 1};
    
    _thread((void*) &task);
}
