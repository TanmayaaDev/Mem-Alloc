#ifndef MY_ALLOC_H
#define MY_ALLOC_H

#include <stddef.h>
#include <stdbool.h>

// Header block metadata prepended to every allocated chunk
typedef struct BlockHeader {
    size_t size;               // Size of usable data region
    bool is_free;              // Allocation flag
    struct BlockHeader *next;  // Next block in doubly-linked heap
    struct BlockHeader *prev;  // Previous block for coalescing
} BlockHeader_t;

#define HEADER_SIZE sizeof(BlockHeader_t)

// Core Allocator API
void *my_malloc(size_t size);
void my_free(void *ptr);
void *my_calloc(size_t num, size_t size);
void *my_realloc(void *ptr, size_t size);

// Debugging Utilities
void my_heap_dump(void);

#endif