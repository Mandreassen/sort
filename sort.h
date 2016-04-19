#ifndef SORT_H
#define SORT_H

// Macros
#define SWAP(x,y) int t;t=x;x=y;y=t;

void selection_sort(int *data, int size);

void mergesort(int *data, int size);

void quicksort(int *data, int size);

#endif
