// Application files
#include "sort.h"

static void heapify_down(int *heap, int index, int max)
{
	int min_elem;
	
	while (index * 2 <= max) {
        	
		min_elem = index * 2;
		if ((index * 2) + 1 <= max && heap[index*2] < heap[(index*2)+1]) {	
			min_elem = (index * 2) + 1;
		}

		if(heap[index] < heap[min_elem]) {	
			SWAP(heap[index], heap[min_elem]);
			index = min_elem;
		} else {
			break;
        }			
	}	
}

void heapsort(int *data, int size)
{
    int i;
    data -= 1;    
    
    //Build heap
    for (i = size / 2; i >= 1; i--) {
        heapify_down(data, i, size);
    }
    
    while (size > 1) {
        SWAP(data[1], data[size]);
        heapify_down(data, 1, --size);
    }
}
