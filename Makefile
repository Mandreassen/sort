assert_sort: assert_sort.c selection_sort.c quicksort.c mergesort.c heapsort.c resources.o Makefile
	gcc assert_sort.c selection_sort.c quicksort.c mergesort.c heapsort.c resources.o -O0 -g -o assert_sort -lpthread
    
clean:
	rm -f assert_sort