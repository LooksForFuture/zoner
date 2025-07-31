#include <zoner/zon_arena.h>

#ifndef NDEBUG
#include <assert.h>
#endif

ZonArena
zon_arenaCreate(void *memory, size_t size)
{
	ZonArena arena;
	arena.size = size;
	arena.index = 0;
	arena.mark = 0;
	arena.memory = memory;
	return arena;
}

void
zon_arenaReset(ZonArena *allocator)
{
	allocator->mark = 0;
	allocator->index = 0;
}


void *
zon_arenaUnlock(ZonArena *allocator)
{
	void *ptr = allocator->memory;
	allocator->memory = NULL;
	return ptr;
}

void
zon_arenaMark(ZonArena *allocator, size_t index)
{
	allocator->mark = index;
}

void
zon_arenaRewind(ZonArena *allocator)
{
	allocator->index = allocator->mark;
}

void *
zon_arenaMalloc(ZonArena *allocator, size_t size)
{
#ifndef NDEBUG
	assert(allocator->memory);
#endif
	if (allocator->index + size > allocator->size) return NULL;
	void *ptr = (char *)allocator->memory + allocator->index;
	allocator->index += size;
	return ptr;
}

static void *
mallocInterface(ZonAllocator *interface, size_t size)
{
	ZonArena *allocator = interface->ctx;
#ifndef NDEBUG
	assert(allocator->memory);
#endif
	if (allocator->index + size > allocator->size) return NULL;
	void *ptr = (char *)allocator->memory + allocator->index;
	allocator->index += size;
	return ptr;
}

ZonAllocator
zon_arenaInterface(ZonArena *allocator)
{
	ZonAllocator interface;
	interface.ctx = allocator;
	interface.malloc = &mallocInterface;
	return interface;
}
