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
	assert((char *)buf == (char *)mem);
	assert(l1.index == 1024);

	void *buf2 = zon_linearMalloc(&l1, 512);
	assert((char *)buf2 == (char *)mem + 1024);
	assert(l1.index == 1024 + 512);

	assert(zon_linearUnlock(&l1) == mem);
	assert(l1.memory == NULL);
	free(mem);
	return 0;
}
