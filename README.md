# Zoner
## Introduction
Zoner is a memory management library for C with focus on speed and modularity.
 It's stil a WIP project. The goal is to implement different allocators, for 
different needs such as linear allocator, pool allocator, etc.<br />

## Coding Convention
Please see [CODING_CONVENTION.md](docs/CODING_CONVENTIONS.md).

## Usage
### Attention
Please don't use the main branch. The main branch is for development 
purposes. Please use releases or other branches which will be added later.

###Basics
A documentation for usage will be added later. Here are some examples 
for now.
```c
#include <zoner/zon_linear.h>

int
main()
{
    // Create a linear (also known as bump) allocator
    ZonLinear l1 = zon_linearCreate(malloc(4096), 4096);
    char *str = zon_linearMalloc(&l1, 64);

    // You can allocate memory for another allocator too
    ZonLinear l2 = zon_linearCreate(zon_linearMalloc(1024), 1024);

    /* do stuff */

    /* Unlock memory managed by allocators to prevent further usage */
    zon_linearUnlock(&l1);
    free(zon_linearUnlock(&l2)); //unlock returns the pointer given at creation
    return 0;
}
```
