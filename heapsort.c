/* author: Marius Andreassen (marius.andreassen@uit.no) */

// Application files
#include "sort.h"

// Macros
#define LEFT ((root > 0) ? root * 2 : 1)
#define RIGHT (LEFT + 1)

static void shift_down(int *heap, int root, int size)
{
    int swapChild;
    
    while (LEFT < size) {  
       
        // Find correct swapChild
        if (RIGHT >= size)    
            swapChild = LEFT; // Root has no right child       
        else         
            swapChild = (heap[LEFT] > heap[RIGHT]) ? LEFT : RIGHT; // Root has both childs, largest is set to swap
        
        // Check if root must be swaped  
        if (heap[root] > heap[swapChild])
            return; // Done
        
        // Swap and continue   
        SWAP(heap[root], heap[swapChild]);

        root = swapChild;
    }
}


void heapsort(int *data, int size)
{
    int i;
    
    // Build heap
    for (i = size / 2; i >= 0; i--) {
        shift_down(data, i, size);
    }
      
    // Sort
    while (--size > 0) {
        SWAP(data[0], data[size]);
        shift_down(data, 0, size);
    }
}
