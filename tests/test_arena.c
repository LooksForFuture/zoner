#include <zoner/zon_arena.h>

#include <assert.h>

int
main()
{
	void *mem = malloc(4096);
	assert(mem);

	ZonArena l1 = zon_arenaCreate(mem, 4096);
	assert(l1.memory == mem);
	assert(l1.size == 4096);
	assert(l1.index == 0);

	void *buf = zon_arenaMalloc(&l1, 1024);
	assert(buf == mem);
	assert(l1.index == 1024);

	void *buf2 = zon_arenaMalloc(&l1, 512);
	assert((char *)buf2 == (char *)mem + 1024);
	assert(l1.index == 1024 + 512);

	ZonArena l2 = zon_arenaCreate(buf2, 512);
	assert(l2.memory == buf2);

	ZonAllocator interface = zon_arenaInterface(&l2);
	assert(interface.ctx == &l2);

	void *buf3 = interface.malloc(&interface, 64);
	assert(buf3 == buf2);
	assert(l2.index == 64);

	void *buf4 = interface.malloc(&interface, 128);
	assert((char *)buf4 == (char *)buf3 + 64);
	assert(l2.index == 64 + 128);

	assert(interface.malloc(&interface, 512) == NULL);
	assert(l2.index == 64 + 128);

	zon_arenaReset(&l2);
	assert(l2.index == 0);
	assert(zon_arenaMalloc(&l2, 512) != NULL);
	assert(l2.index == 512);

	assert(zon_arenaUnlock(&l2) == buf2);
	assert(l2.memory == NULL);

	assert(l1.mark == 0);
	zon_arenaMark(&l1, l1.index);
	assert(l1.mark == 1024 + 512);
	zon_arenaMalloc(&l1, 512);
	assert(l1.index == 2048);
	zon_arenaRewind(&l1);
	assert(l1.index == 1024 + 512);
	assert(l1.mark == 1024 + 512);

	zon_arenaReset(&l1);
	assert(l1.index == 0);

	assert(zon_arenaUnlock(&l1) == mem);
	assert(l1.memory == NULL);
	free(mem);
	return 0;
}
