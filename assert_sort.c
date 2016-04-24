/* author: Marius Andreassen (marius.andreassen@uit.no) */

// Standard libs
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

// Application files
#include "sort.h"

// Text colors
#define RED  "\x1B[31m"
#define GREEN  "\x1B[32m"
#define NORM  "\x1B[0m"

typedef enum {
    false,
    true
} bool;

typedef struct test_data {
    int *unsorted; // Data for testing
    int *sorted;   // Data for correctness test
    int size;      // Data size
} test_data_t;


/* Get time NOW! */
unsigned long long now(void)
{
    struct timeval time;
    gettimeofday(&time, NULL);
    return time.tv_sec * 1000000 + time.tv_usec;
}

/* Validate sorted data set */
bool validate_sort(int *sorted, test_data_t *test_data)
{
    int i;
    for (i = 0; i < test_data->size; i++) {
        if (sorted[i] != test_data->sorted[i])
            return false; // invalid
    }
    return true; // valid
}

/* Print array of ints */
void print_data(int *data, int size)
{
    int i;
    for (i = 0; i < size; i++) {
        printf("[%d] -> %d\n", i, data[i]);
    }
}

/* Generate new random data set */
int *new_data(int size)
{
    int i;
    int *data = malloc(sizeof(int) * size);    
    for (i = 0; i < size; i++) {
        data[i] = rand() % size;
    }
    
    return data; // New random data    
}

/* Initialize test data */
test_data_t *init(int size)
{
    unsigned long long time;
    test_data_t *new = malloc(sizeof(test_data_t));
    if (new == NULL)
        return NULL; // error
    
    new->size = size;
    
    // Generate test data
    new->unsorted = new_data(size); 
    
    // Setup correctness test
    new->sorted = malloc(sizeof(int) * size);
    memcpy(new->sorted, new->unsorted, sizeof(int) * size);
    
    time = now();
    sort_ints(new->sorted, size);
    time = now() - time;
    
    printf("Correctness data sorted in %.2f sec.\n", (float) time/1000000);
    
    return new;
}

/* Destroy test data */
void teardown(test_data_t *test_data)
{
    free(test_data->unsorted);
    free(test_data->sorted);
    free(test_data);
}

/* Test given sorting algorithm with given test data */
void test_algorithm(void (*sort_func)(int*, int), test_data_t *test_data, char *name)
{
    int *data;
    unsigned long long time;    
   
    // Copy test data
    data = malloc(sizeof(int) * test_data->size);
    memcpy(data, test_data->unsorted, sizeof(int) * test_data->size); 
    
    printf("Running %s...", name);
    fflush(stdout);    
    
    // Run test
    time = now(); // Start timer    
    sort_func(data, test_data->size); // Run sort    
    time = now() - time; // Stop timer    
   
    // Print result 
    printf("\r%s completed in %.2f sec. Result: ", name, (float) time/1000000);
    if (validate_sort(data, test_data))
        printf(GREEN "Success\n" NORM);
    else
        printf(RED "Failure\n" NORM);        
        
    free(data);
}


/* ASSERT SORT:
 * usage: ./assert_sort <number of test elems>
 * WARNING: Large data sets require a big 
 * memory pool. */
int main(int argc, char **argv)
{    
    int size = 1000; // Default    
    if (argc > 1) 
        size = atoi(argv[1]); // Custom
        
    printf("Initializing test data...\n");
    
    test_data_t *test_data = init(size);
        
    printf("Running tests with %d elemets:\n", size);
      
    test_algorithm(selection_sort, test_data, "Sel. sort");
    test_algorithm(quicksort, test_data, "Quicksort");
    test_algorithm(mergesort, test_data, "Mergesort");
    
    teardown(test_data);

    return 0;
}