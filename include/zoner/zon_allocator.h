#ifndef __ZON_ALLOCATOR__
#define __ZON_ALLOCATOR__

/*
 * Generic interface for allocators.
 * This interface is only for allocators which have an malloc function.
 */

#include <stdlib.h>

typedef struct ZonAllocator_s {
	void *ctx; //pointing to the actual allocator

	//allocation with custom alignment
	void *(*alloc)(const struct ZonAllocator_s *, size_t, size_t);

	//allocation with alignof(max_align_t)
	void *(*malloc)(const struct ZonAllocator_s *, size_t);

	//reallocate a piece of memory with custom alignment
	void *(*reallocAligned)(const struct ZonAllocator_s *, void *,
				size_t, size_t);

	//reallocate a piece of memory with alignof(max_align_t)
	void *(*realloc)(const struct ZonAllocator_s *, void *, size_t);

	//free as in freedom
	void (*free)(const struct ZonAllocator_s *, void *);
} ZonAllocator;

#endif //__ZON_ALLOCATOR__
