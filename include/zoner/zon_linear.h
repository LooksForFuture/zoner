#ifndef __ZON_LINEAR__
#define __ZON_LINEAR__

/*
 * Linear memory allocator
 * cannot deallocate memory
 */

#include <stdlib.h>

#include <zoner/zon_allocator.h>

typedef struct {
	size_t size;
	size_t index;
	void* memory;
} ZonLinear;

/* creates a linear allocator */
ZonLinear zon_linearCreate(void *, size_t);

/* resets the index to 0 */
void zon_linearReset(ZonLinear *);

/* returns the managed ememory back */
void *zon_linearUnlock(ZonLinear *);

/* wraps the allocator in the generic interface */
ZonAllocator zon_linearInterface(ZonLinear *);

/* allocates */
void *zon_linearMalloc(ZonLinear *, size_t);

#endif //__ZON_LINEAR__
