/* author: Marius Andreassen (marius.andreassen@uit.no) */

// Application files
#include "sort.h"

/* Select pivot and partition data */
static int partition(int *data, int min, int max)
{
    int pivot = max;
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
    
    return wall; // pivot position
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


void quicksort(int *data, int size)
{
    _quicksort(data, 0, size - 1);    
}
