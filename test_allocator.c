#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "my_alloc.h"

int main(void) {
    printf("Starting custom allocator test suite...\n");

    // Test 1: Basic Malloc and Write
    char *str1 = (char *)my_malloc(64);
    assert(str1 != NULL);
    strcpy(str1, "Hello from custom memory allocator!");
    printf("Test 1 Passed: %s\n", str1);

    // Test 2: Calloc Zero Initialization
    int *arr = (int *)my_calloc(10, sizeof(int));
    assert(arr != NULL);
    for (int i = 0; i < 10; i++) {
        assert(arr[i] == 0);
    }
    printf("Test 2 Passed: calloc zero-initialization verified.\n");

    // Test 3: Free and Heap Dump
    my_free(str1);
    my_free(arr);
    printf("Test 3 Passed: Memory blocks freed successfully.\n");

    my_heap_dump();

    printf("All allocator tests completed successfully!\n");
    return 0;
}