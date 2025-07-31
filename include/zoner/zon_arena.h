#ifndef __ZON_ARENA__
#define __ZON_ARENA__

/*
 * Arena memory allocator
 * Similar to ZonLinear, but can rewind allocation index to a given checkpoint
 */

#include <stdlib.h>

#include <zoner/zon_allocator.h>

typedef struct {
	size_t size;
	size_t index;
	size_t mark;
	void* memory;
} ZonArena;

/* creates an arena allocator */
ZonArena zon_arenaCreate(void *, size_t);

/* resets the index to 0 */
void zon_arenaReset(ZonArena *);

/* returns the managed ememory back */
void *zon_arenaUnlock(ZonArena *);

/* wraps the allocator in the generic interface */
ZonAllocator zon_arenaInterface(ZonArena *);

/* marks the given index for later rewind */
void zon_arenaMark(ZonArena *, size_t);

/* rewinds the allocation index to the mark */
void zon_arenaRewind(ZonArena *);

/* allocates */
void *zon_arenaMalloc(ZonArena *, size_t);

#endif //__ZON_ARENA__
