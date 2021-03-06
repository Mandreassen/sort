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

// Global constants
bool print = false;


/* Get time NOW! */
unsigned long long now(void)
{
    struct timeval time;
    gettimeofday(&time, NULL);
    return time.tv_sec * 1000000 + time.tv_usec;
}

/* Sort int array.
 * Data elements must be within range (0, range).
 * WARNING: Memory at size of range will
 * be allocated during sorting. */
bool range_sort(int *data, int size, int range)
{
    int i, pos;
    int *buff = calloc(sizeof(int), range);
    if (buff == NULL)
        return false;
    
    for (i = 0; i < size; i++) {
        if (data[i] > range) {
            free(buff);
            return false; // Error data not within range
        }
        buff[data[i]]++;
    }
    
    pos = 0;
    for (i = 0; i < range; i++) {
        while (buff[i] > 0) {
            data[pos++] = i;
            buff[i]--;
        }
    }
    
    free(buff);
    
    return true; // Success
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
    printf("\nDATA:\n");
    for (i = 0; i < size; i++) {
        printf("[%d] -> %d\n", i, data[i]);
    }
}

/* Generate new random data set */
int *new_data(int size)
{
    int i;
    int *data = malloc(sizeof(int) * size);
    if (data == NULL)
        return NULL; // No memory
     
    for (i = 0; i < size; i++) {
        data[i] = rand() % size;
    }
    
    return data; // New random data    
}

/* Destroy test data */
void teardown(test_data_t *test_data)
{
    free(test_data->unsorted);
    free(test_data->sorted);
    free(test_data);
}

/* Initialize test data */
test_data_t *init(int size)
{
    bool success;
    unsigned long long time;
    test_data_t *new = malloc(sizeof(test_data_t));
    if (new == NULL)
        return NULL; // No memory
    
    new->size = size;
    
    // Generate test data
    new->unsorted = new_data(size);
    if (new->unsorted == NULL) {
        free(new);
        return NULL; // No memory
    }
    
    // Setup correctness test
    new->sorted = malloc(sizeof(int) * size);
    if (new->sorted == NULL) {
        free(new->unsorted);
        free(new);
        return NULL; // No memory
    }
    
    memcpy(new->sorted, new->unsorted, sizeof(int) * size);
    
    time = now();
    success = range_sort(new->sorted, size, size);
    time = now() - time;
    
    if (success) {
        printf("Correctness data sorted in %.2f sec.\n", (float) time/1000000);
    } else {
        teardown(new);
        return NULL; // Error
    }
    
    return new; // Success
}



/* Test given sorting algorithm with given test data */
void test_algorithm(void (*sort_func)(int*, int), test_data_t *test_data, char *name)
{
    int *data;
    unsigned long long time;    
   
    // Allocate space for this test run
    data = malloc(sizeof(int) * test_data->size);
    if (data == NULL) {
        printf("Memory allocation error.\n");
        return;
    }
    
    // Copy test data
    memcpy(data, test_data->unsorted, sizeof(int) * test_data->size); 
    
    printf("Running %s...", name);
    fflush(stdout);    
    
    // Run test
    time = now(); // Start timer    
    sort_func(data, test_data->size); // Run sort    
    time = now() - time; // Stop timer   
    
    if (print)
        print_data(data, test_data->size); 
   
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
    int size = 1000; // Default size
    
    while (--argc > 0) {
        if (strcmp(argv[argc], "-p") == 0) 
            print = true; // Set print
        else
            size = (atoi(argv[argc]) > 0) ? atoi(argv[argc]) : 1000; // Custom size
    }
        
    // INIT test data
    printf("Initializing test data...\n");    
    test_data_t *test_data = init(size);
    if (test_data == NULL) {
        printf("Could not allocate memory for test data.\n");
        return 1;
    }
        
    // Run tests
    printf("Running tests with %d elemets:\n", size);     
    test_algorithm(mergesort, test_data, "Mergesort");
    test_algorithm(heapsort, test_data, "Heapsort ");
    test_algorithm(quicksort, test_data, "Quicksort");
    test_algorithm(parallel_quicksort, test_data, "ParallelQ");
    if (size < 100000)
        test_algorithm(selection_sort, test_data, "Sel. sort");   
    
    // Cleanup
    teardown(test_data);

    return 0;
}
