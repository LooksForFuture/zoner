#ifndef __ZON_ALLOCATOR__
#define __ZON_ALLOCATOR__

/*
 * Generic interface for allocators.
 * This interface is only for allocators which have an malloc function.
 * */

#include <stdlib.h>

typedef struct ZonAllocator_s {
	void *ctx; //pointing to the actual allocator
	void *(*malloc)(struct ZonAllocator_s *, size_t);
} ZonAllocator;

#endif //__ZON_ALLOCATOR__
