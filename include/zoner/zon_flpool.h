#ifndef __ZON_FLPOOL__
#define __ZON_FLPOOL__

/*
 * Free List Pool.
 * Manages a pool of items of the same type (same size).
 * Records free entries using an approach similar to linked list.
 * Attention: Use only if the size of your object type is >= sizeof(void *).
 */

#include <stdlib.h>

typedef struct {
	void *memory;
	void *head;
	size_t size; //size of the type of items in pool
	size_t count; //count of objects in pool
} ZonFLPool;

/* returns an flpool managing the given memory */
ZonFLPool zon_flpoolCreate(void *, size_t, size_t);

/* resets the pool */
void zon_flpoolReset(ZonFLPool *);

/* returns the managed memory back */
void *zon_flpoolUnlock(ZonFLPool *);

/* returns a free item from the pool */
void *zon_flpoolPop(ZonFLPool *);

/*
 * sends the given item back to the pool
 * attention: writes data the item
 */
void zon_flpoolPush(ZonFLPool *, void *);

#endif //__ZON_FLPOOL__
