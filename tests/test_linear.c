#include <zoner/zon_linear.h>

#include <assert.h>

int
main()
{
	void *mem = malloc(4096);
	assert(mem);

	ZonLinear l1 = zon_linearCreate(mem, 4096);
	assert(l1.memory == mem);
	assert(l1.size == 4096);
	assert(l1.index == 0);

	void *buf = zon_linearMalloc(&l1, 1024);
	assert(buf == mem);
	assert(l1.index == 1024);

	void *buf2 = zon_linearMalloc(&l1, 512);
	assert((char *)buf2 == (char *)mem + 1024);
	assert(l1.index == 1024 + 512);

	ZonLinear l2 = zon_linearCreate(buf2, 512);
	assert(l2.memory == buf2);

	ZonAllocator interface = zon_linearInterface(&l2);
	assert(interface.ctx == &l2);

	void *buf3 = interface.malloc(&interface, 64);
	assert(buf3 == buf2);
	assert(l2.index == 64);

	void *buf4 = interface.malloc(&interface, 128);
	assert((char *)buf4 == (char *)buf3 + 64);
	assert(l2.index == 64 + 128);

	assert(interface.malloc(&interface, 512) == NULL);
	assert(l2.index == 64 + 128);

	zon_linearReset(&l2);
	assert(l2.index == 0);
	assert(zon_linearMalloc(&l2, 512) != NULL);
	assert(l2.index == 512);

	zon_linearReset(&l1);
	assert(l1.index == 0);

	assert(zon_linearUnlock(&l2) == buf2);
	assert(l2.memory == NULL);
	assert(zon_linearUnlock(&l1) == mem);
	assert(l1.memory == NULL);
	free(mem);
	return 0;
}
