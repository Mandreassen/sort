/* author: Marius Andreassen (marius.andreassen@uit.no) */

// Standard libs
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

// Application files
#include "sort.h"

typedef enum {
    false,
    true
} bool;

/* Get time */
unsigned long long now(void)
{
    struct timeval time;
    gettimeofday(&time, NULL);
    return time.tv_sec * 1000000 + time.tv_usec;
}

/* Validate sorted data-set */
bool validate_sort(int *data, int size)
{
    while (--size) {
        if (data[size - 1] > data[size])
            return false; // Invalid
    }

    return true; // Valid
}

/* Print array of ints */
void print_data(int *data, int size)
{
    int i;
    for (i = 0; i < size; i++) {
        printf("[%d] -> %d\n", i, data[i]);
    }
}

/* Generate new random data-set */
int *new_data(int size)
{
    int i;
    int *data = malloc(sizeof(int) * size);    
    for (i = 0; i < size; i++) {
        data[i] = rand() % size;
    }
    
    return data; // New random data    
}


/* Test given sorting algorithm */
void test_algorithm(void (*sort_func)(int*, int), int size, char *name)
{
    int *data;
    char *result;
    unsigned long long time;
    
    data = new_data(size); 
    
    printf("Running %s...", name);
    fflush(stdout);    
    
    time = now(); // Start timer
    
    sort_func(data, size); // Run sort
    
    time = now() - time; // Stop timer    
    
    result = (validate_sort(data, size)) ? "Success" : "Failure";
    
    printf("\r%s completed in %.2f sec. Result: %s\n", name, (float) time/1000000, result);
    
    free(data);
}


/* Program entry point */
int main(int argc, char **argv)
{    
    int size = 1000; // Default    
    if (argc > 1) 
        size = atoi(argv[1]); // Custom
        
    printf("Running assert with %d elemets:\n", size);
      
    test_algorithm(selection_sort, size, "Sel. sort");
    test_algorithm(quicksort, size, "Quicksort");
    test_algorithm(mergesort, size, "Mergesort");

    return 0;
}