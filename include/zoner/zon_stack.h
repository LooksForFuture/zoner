#ifndef __ZON_STACK__
#define __ZON_STACK__

/*
 * Stack memory allocator
 * It's similar to a linear allocator, but allows LIFO deallocation.
 * Note: Each allocation stores its previous index, adding minimal overhead
 */

#include <stdlib.h>

typedef struct {
	size_t size;
	size_t index;
	void *memory;
} ZonStack;

/* creates a stack allocator */
ZonStack zon_stackCreate(void *, size_t);

/* resets the index to 0 */
void zon_stackReset(ZonStack *);

/* returns the managed ememory back */
void *zon_stackUnlock(ZonStack *);

/* allocates with manual alignment control */
void *zon_stackAlloc(ZonStack *, size_t, size_t);

/* allocates with automaic alignment based on max_align_t */
void *zon_stackMalloc(ZonStack *, size_t);

/* rewind allocation in LIFO order */
void zon_stackPop(ZonStack *);

/* returns the current stack index */
size_t zon_stackMarker(ZonStack *);

/* rewinds the stack index to the given position - unsafe */
void zon_stackRewind(ZonStack *, size_t);

#endif //__ZON_STACK__
