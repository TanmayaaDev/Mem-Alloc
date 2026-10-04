#define _DEFAULT_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "my_alloc.h"

// Head of the memory heap linked list
static BlockHeader_t *head = NULL;

// Locate header metadata from a user data pointer
static BlockHeader_t *get_block_ptr(void *ptr) {
    return (BlockHeader_t *)ptr - 1;
}

// Find the first free block that fits the requested size (First-Fit Strategy)
static BlockHeader_t *find_free_block(BlockHeader_t **last, size_t size) {
    BlockHeader_t *current = head;
    while (current) {
        *last = current;
        if (current->is_free && current->size >= size) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Request new memory space from the OS kernel using sbrk()
static BlockHeader_t *request_space(BlockHeader_t *last, size_t size) {
    BlockHeader_t *block = sbrk(0);
    void *request = sbrk(size + HEADER_SIZE);
    
    if (request == (void *)-1) {
        return NULL; // sbrk failed (out of memory)
    }

    if (last) {
        last->next = block;
    }
    
    block->size = size;
    block->is_free = false;
    block->next = NULL;
    block->prev = last;
    
    return block;
}

void *my_malloc(size_t size) {
    if (size <= 0) {
        return NULL;
    }

    size_t aligned_size = (size + 7) & ~7;
    BlockHeader_t *block = NULL;

    if (!head) {
        block = request_space(NULL, aligned_size);
        if (!block) return NULL;
        head = block;
    } else {
        BlockHeader_t *last = head;
        block = find_free_block(&last, aligned_size);
        
        if (!block) {
            block = request_space(last, aligned_size);
            if (!block) return NULL;
        } else {
            block->is_free = false;
        }
    }

    return (void *)(block + 1);
}

void my_free(void *ptr) {
    if (!ptr) return;

    BlockHeader_t *block = get_block_ptr(ptr);
    block->is_free = true;

    // Coalesce with next block if it is also free
    if (block->next && block->next->is_free) {
        block->size += HEADER_SIZE + block->next->size;
        block->next = block->next->next;
        if (block->next) {
            block->next->prev = block;
        }
    }

    // Coalesce with previous block if it is also free
    if (block->prev && block->prev->is_free) {
        block->prev->size += HEADER_SIZE + block->size;
        block->prev->next = block->next;
        if (block->next) {
            block->next->prev = block->prev;
        }
    }
}

void *my_calloc(size_t num, size_t size) {
    size_t total_size = num * size;
    void *ptr = my_malloc(total_size);
    if (!ptr) {
        return NULL;
    }
    memset(ptr, 0, total_size);
    return ptr;
}

void *my_realloc(void *ptr, size_t size) {
    if (!ptr) {
        return my_malloc(size);
    }
    if (size == 0) {
        my_free(ptr);
        return NULL;
    }

    BlockHeader_t *block = get_block_ptr(ptr);
    if (block->size >= size) {
        return ptr;
    }

    void *new_ptr = my_malloc(size);
    if (!new_ptr) {
        return NULL;
    }

    memcpy(new_ptr, ptr, block->size);
    my_free(ptr);
    return new_ptr;
}

void my_heap_dump(void) {
    BlockHeader_t *current = head;
    int index = 0;
    printf("\n--- HEAP MEMORY DUMP ---\n");
    while (current) {
        printf("Block %d: Addr=%p | Size=%zu bytes | Status=%s\n",
               index++, (void *)current, current->size,
               current->is_free ? "FREE" : "ALLOCATED");
        current = current->next;
    }
    printf("------------------------\n\n");
}