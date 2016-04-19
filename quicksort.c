// Application files
#include "sort.h"

static int partition(int *data, int min, int max)
{
	int pivot 	= max;
	int wall 	= min;
	int current = min;

	while (current < pivot) {
		
        if (data[current] > data[pivot]) {
			current++;
            
        } else {
			SWAP(data[current], data[wall]);
			wall++;
			current++;
		}	
	}	

	SWAP(data[pivot], data[wall]);

	return wall;
}

static void _quicksort(int *data, int min, int max)
{
    if (min >= max)
        return;
        
    int mid = partition(data, min, max);

    _quicksort(data, min, mid - 1);
    _quicksort(data, mid + 1, max);
}


void quicksort(int *data, int size)
{
    _quicksort(data, 0, size - 1);    
}
