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
} ZonFLPool;

/* returns an flpool managing the given memory */
ZonFLPool zon_flpoolCreate(void *, size_t, size_t);

/* returns the managed memory back */
void *zon_flpoolUnlock(ZonFLPool *);

/* returns a free item from the pool */
void *zon_flpoolPop(ZonFLPool *);

/*
 * sends the given item back to the pool
 * attention: writes data the item
 */
void zon_flpoolPush(ZonFLPool *, void *);

/* resets the pool */
void zon_flpoolReset(ZonFLPool *);

#endif //__ZON_FLPOOL__
