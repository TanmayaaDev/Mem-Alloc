# MemAlloc-C

A custom, zero-dependency C99 heap memory allocator built using POSIX `sbrk()` system calls. Designed to manage raw virtual memory pages, implement First-Fit allocation strategies, support block splitting, and handle adjacent chunk coalescing during deallocation to eliminate heap fragmentation.

---

## Features

- **Virtual Memory Management:** Interfaces directly with the OS kernel via `sbrk()` to request and expand the heap layout.
- **First-Fit Search & Split:** Scans bidirectional block headers to locate suitable free regions and optimizes allocation sizes.
- **Bidirectional Coalescing:** Automatically merges adjacent free memory blocks during `my_free()` to prevent memory fragmentation.
- **Full Standard API:** Implements custom equivalents for `malloc`, `free`, `calloc`, and `realloc`.
- **Diagnostic Tooling:** Includes a `my_heap_dump()` utility to inspect block metadata, size metrics, and allocation states in real time.

---

## Directory Structure

```text
MemAlloc-C/
├── include/
│   └── my_alloc.h
├── src/
│   └── my_alloc.c
├── tests/
│   └── test_allocator.c
├── Makefile
└── README.md