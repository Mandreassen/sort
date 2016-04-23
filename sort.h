#ifndef SORT_H
#define SORT_H

// Macros:
#define SWAP(x,y) int t;t=x;x=y;y=t;

// Embeded
void sort_ints(int *data, int size);


// To implement:
void selection_sort(int *data, int size);

void mergesort(int *data, int size);

void quicksort(int *data, int size);


#endif
