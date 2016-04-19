// Application files
#include "sort.h"
#include <stdlib.h>

static void merge(int *data, int min, int mid, int max)
{
    int buff[max - min + 1];
    int pos = 0;
    int low = min;
    int high = mid + 1;
    
    // Sort data
    while (low <= mid && high <= max) {
        if (data[low] <= data[high])
            buff[pos++] = data[low++];
        else 
            buff[pos++] = data[high++];
    }
    
    // Get remaining lower elements
    while (low <= mid) {
        buff[pos++] = data[low++];
    }
    
    // Get remaining higher elements
    while (high <= max) {
        buff[pos++] = data[high++];
    }
    
    // Copy back data
    while (--pos >= 0) {
        data[min + pos] = buff[pos];
    }
}

static void _mergesort(int *data, int min, int max)
{
    if (min >= max)
        return;
        
    int mid = (min + max) / 2;
    
    _mergesort(data, min, mid);
    _mergesort(data, mid + 1, max);
    
    merge(data, min, mid, max);
}


void mergesort(int *data, int size)
{
    _mergesort(data, 0, size - 1);    
}