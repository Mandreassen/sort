/* author: Marius Andreassen (marius.andreassen@uit.no) */

// Application files
#include "sort.h"

void selection_sort(int *data, int size)
{
    int i, j, tmp;
    for (i = 0; i < size; i++) {
        tmp = i;
        for (j = i + 1; j < size; j++) {
            if (data[j] < data [tmp])
                tmp = j;
        }
        
        SWAP(data[i], data[tmp]);
    }    
}
