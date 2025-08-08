#include <zoner/zon_arena.h>

#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#define MEM_SIZE 65536
#define MAX_ALIGN 256

int
main()
{
	void *mem = malloc(MEM_SIZE);
	assert(mem);

	ZonArena arena = zon_arenaCreate(mem, MEM_SIZE);
	assert(arena.base == mem);
	assert(arena.current == mem);
	assert(arena.end == arena.current + MEM_SIZE);

	for (size_t align = 1; align <= MAX_ALIGN; align *= 2) {
		void* p = zon_arenaAlloc(&arena, 512, align);
		assert(p);
		assert((uintptr_t)p % align == 0);
		assert(((uintptr_t)p & align - 1) == 0);

		p = zon_arenaMalloc(&arena, 512);
		assert(((uintptr_t)p & alignof(max_align_t) - 1) == 0);
	}

	size_t mark = zon_arenaMarker(&arena);
	assert((uint8_t *)mem + mark == arena.current);
	for (size_t align = 1; align <= MAX_ALIGN; align *= 2) {
		void* p = zon_arenaAlloc(&arena, 512, align);
		assert(p);
		assert((uintptr_t)p % align == 0);
		assert(((uintptr_t)p & (align - 1)) == 0);

		zon_arenaRewind(&arena, mark);
		assert(arena.current == (uint8_t *)mem + mark);
	}

	zon_arenaReset(&arena);
	assert(arena.current == mem);
	assert(zon_arenaMalloc(&arena, MEM_SIZE+10) == NULL);

	void *p1 = zon_arenaMalloc(&arena, 64);
	zon_arenaReset(&arena);
	void *p2 = zon_arenaMalloc(&arena, 64);
	assert(p2 == p1);

	for (int i = 0; i < 1000; ++i) {
		size_t s = (rand() % 64) + 8;
		s = (s + 7) & ~7;
		void* p = zon_arenaAlloc(&arena, s, 8);
		assert(((uintptr_t)p & 7) == 0);
	}

	void *returned = zon_arenaUnlock(&arena);
	assert(returned == mem);
	assert(arena.base == NULL);

	free(mem);
	return 0;
}
