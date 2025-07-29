#ifndef __ZON_LINEAR__
#define __ZON_LINEAR__

/*
 * Linear memory allocator
 * cannot deallocate memory
 */

#include <stdlib.h>

typedef struct {
	size_t size;
	size_t index;
	void* memory;
} ZonLinear;

/* creates a linear allocator */
ZonLinear zon_linearCreate(void *, size_t);

/* returns the managed ememory back and sets allocator size to 0 */
void *zon_linearUnlock(ZonLinear *);

/* allocates */
void *zon_linearMalloc(ZonLinear *, size_t);

#endif //__Z_LINEAR__
