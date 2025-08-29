#ifndef __ZON_HEAP__
#define __ZON_HEAP__

/*
 * Heap memory allocation
 * Contains functions for heap memory allocation which take care of alignment.
 * Also has a global constant heap allocator.
 * Attention: This is totally a WIP. It's not going to stay like this for
 * a long time. It will be replaced by a custom heap that is fine tunable
 * for different needs.
 */

#include <zoner/zon_allocator.h>

#include <stddef.h>

/* constant global allocator to be used by other structures*/
extern const ZonAllocator zon_heap;

/* allocate memory directly on heap with custom alignment */
void *zon_alloc(size_t, size_t);

/* allocate memory directly on heap with alignof(max_align_t) */
void *zon_malloc(size_t);

/*
 * reallocate the memory with custom alignment
 * Attention: Please don't chnage the alignment. It may cuase bugs.
 */
void *zon_reallocAligned(void *, size_t, size_t);

/*
 * reallocate memory with alignof(max_align_t)
 * Attention: Please don't chnage the alignment. It may cause bugs.
 */
void *zon_realloc(void *, size_t);

/* free the allocated memory */
void zon_free(void *);

#endif //__ZON_HEAP__
