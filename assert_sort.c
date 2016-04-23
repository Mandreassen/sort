/* author: Marius Andreassen (marius.andreassen@uit.no) */

// Standard libs
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

// Application files
#include "sort.h"

typedef enum {
    false,
    true
} bool;


typedef struct test_data {
    int *unsorted; // Data for testing
    int *sorted;   // Data for correctness test
    int size;      // Data size
} test_data_t;


/* Get time NOW!*/
unsigned long long now(void)
{
    struct timeval time;
    gettimeofday(&time, NULL);
    return time.tv_sec * 1000000 + time.tv_usec;
}

/* Validate sorted data-set */
bool validate_sort(int *sorted, test_data_t *test_data)
{
    int i;
    for (i = 0; i < test_data->size; i++) {
        if (sorted[i] != test_data->sorted[i])
            return false;
    }
    return true;
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

/* Initialize test data */
test_data_t *init(int size)
{
    test_data_t *new = malloc(sizeof(test_data_t));
    if (new == NULL)
        return NULL; // error
    
    new->size = size;
    
    // Generate test data
    new->unsorted = new_data(size); 
    
    // Setup correctness test
    new->sorted = malloc(sizeof(int) * size);
    new->sorted = memcpy(new->sorted, new->unsorted, sizeof(int) * size);
    sort_ints(new->sorted, size);
    
    return new;
}

/* Destroy test data */
void teardown(test_data_t *test_data)
{
    free(test_data->unsorted);
    free(test_data->sorted);
    free(test_data);
}

/* Test given sorting algorithm */
void test_algorithm(void (*sort_func)(int*, int), test_data_t *test_data, char *name)
{
    int *data;
    char *result;
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
    
    // Validate result
    result = (validate_sort(data, test_data)) ? "Success" : "Failure";
    
    printf("\r%s completed in %.2f sec. Result: %s\n", name, (float) time/1000000, result);    
    free(data);
}


/* Program entry point */
int main(int argc, char **argv)
{    
    int size = 1000; // Default    
    if (argc > 1) 
        size = atoi(argv[1]); // Custom
        
    printf("Initializing test data...\n");
    
    test_data_t *test_data = init(size);
        
    printf("Running tests with %d elemets:\n", size);
      
    //test_algorithm(selection_sort, size, "Sel. sort");
    test_algorithm(quicksort, test_data, "Quicksort");
    test_algorithm(mergesort, test_data, "Mergesort");
    test_algorithm(sort_ints, test_data, "Threadsort");
    
    teardown(test_data);

    return 0;
}