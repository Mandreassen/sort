assert_sort: assert_sort.c selection_sort.c quicksort.c mergesort.c heapsort.c parallel_quicksort.c Makefile
	gcc assert_sort.c selection_sort.c quicksort.c mergesort.c heapsort.c parallel_quicksort.c -O2 -g -o assert_sort -lpthread
    
clean:
	rm -f assert_sort