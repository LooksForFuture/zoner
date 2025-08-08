#ifndef __ZON_ARENA__
#define __ZON_ARENA__

/*
 * Arena memory allocator
 * Allocates memory linearly
 */

#include <stddef.h>
#include <stdint.h>

typedef struct {
	void *base;
	uint8_t *current;
	uint8_t *end;
} ZonArena;

/* creates an arena allocator */
ZonArena zon_arenaCreate(void *, size_t);

/* resets the index to 0 */
void zon_arenaReset(ZonArena *);

/* returns the managed ememory back */
void *zon_arenaUnlock(ZonArena *);

/* allocates with manual alignment control */
void *zon_arenaAlloc(ZonArena *, size_t, size_t);

/* allocates with automaic alignment based on max_align_t */
void *zon_arenaMalloc(ZonArena *, size_t);

/* returns the current pointer */
size_t zon_arenaMarker(ZonArena *);

/* rewinds the current pointer to the given position */
void zon_arenaRewind(ZonArena *, size_t);

#endif //__ZON_ARENA__
