run: assert_sort.c selection_sort.c Makefile
	gcc assert_sort.c selection_sort.c quicksort.c mergesort.c -O2 -g -o assert_sort
    
clean:
	rm -f assert_sort