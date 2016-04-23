// Application files
#include "sort.h"
#include <stdlib.h>
#include <stdio.h>

static void merge(int *data, int *temp, int min, int mid, int max)
{
    int pos = 0;
    int low = min;
    int high = mid;    

    // Merge lower and higher part of array
    while (low < mid && high < max) {
        if (data[low] < data[high])
            temp[pos++] = data[low++];
        else
            temp[pos++] = data[high++];        
    }
    
    // Get remaining lower elements
    while (low < mid) {
        temp[pos++] = data[low++];
    }
    
    // Get remaining higher elements
    while (high < max) {
        temp[pos++] = data[high++];
    }    
    
    // Copy back sorted data
    pos = 0;
    while (min < max) {
        data[min++] = temp[pos++];
    }
}

static void _split(int *data, int *temp, int min, int max)
{
    if (max - min <= 1)
        return; // Done splitting
    
    int mid = (max + min) / 2;

    _split(data, temp, min, mid);
    _split(data, temp, mid, max);
    
    merge(data, temp, min, mid, max);
}


void mergesort(int *data, int size)
{
    int *temp = malloc(sizeof(int) * size);
    
    _split(data, temp, 0, size);  
    
    free(temp);
}