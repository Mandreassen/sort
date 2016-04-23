run: assert_sort.c selection_sort.c quicksort.c quicksort_pthread.c Makefile
	gcc assert_sort.c selection_sort.c quicksort.c mergesort.c quicksort_pthread.c -O2 -g -o assert_sort -lpthread
    
clean:
	rm -f assert_sort