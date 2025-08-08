#include <zoner/zon_arena.h>

#ifndef NDEBUG
#include <assert.h>
#endif
#include <stddef.h>
#include <stdint.h>
#include <stdalign.h>

#define ZON_ALIGN_PTR(p, align) \
	(uint8_t*)(((uintptr_t)p + align - 1) & ~((uintptr_t)align - 1))

ZonArena
zon_arenaCreate(void *base, size_t size)
{
	return (ZonArena){
		.base=base,
		.current=base,
		.end= (uint8_t*)base + size
	};
}

void
zon_arenaReset(ZonArena *allocator)
{
#ifndef NDEBUG
	assert(allocator != NULL);
#endif
	allocator->current = allocator->base;
}

void *
zon_arenaUnlock(ZonArena *allocator)
{
#ifndef NDEBUG
	assert(allocator != NULL);
#endif
	void *ptr = allocator->base;
	allocator->base = NULL;
	allocator->current = NULL;
	return ptr;
}

void *
zon_arenaAlloc(ZonArena *allocator, size_t size, size_t alignment)
{
#ifndef NDEBUG
	assert(allocator);
	assert(allocator->current);
	//alignment should be power of 2
	assert(alignment != 0 && (alignment & (alignment - 1)) == 0);
#endif
	uint8_t *aligned = ZON_ALIGN_PTR(allocator->current, alignment);
	uint8_t *next = aligned + size;
	if (next > allocator->end) return NULL;

	allocator->current = next;
	return aligned;
}

void *
zon_arenaMalloc(ZonArena *allocator, size_t size)
{
#ifndef NDEBUG
	assert(allocator && allocator->base);
#endif
	return zon_arenaAlloc(allocator, size, alignof(max_align_t));
}

size_t
zon_arenaMarker(ZonArena *allocator)
{
	return (size_t)(allocator->current - (uint8_t *)allocator->base);
}

void
zon_arenaRewind(ZonArena *allocator, size_t marker)
{
#ifndef NDEBUG
	assert(marker <=
	       (size_t)(allocator->current - (uint8_t *)allocator->base));
#endif
	allocator->current = (uint8_t *)allocator->base + marker;
}

#undef ZON_ALIGN_PTR
