#include <zoner/zon_linear.h>

ZonLinear
zon_linearCreate(void *memory, size_t size)
{
	return (ZonLinear){ size, 0, memory };
}

void *
zon_linearUnlock(ZonLinear *allocator)
{
	void *ptr = allocator->memory;
	allocator->memory = NULL;
	return ptr;
}


void *
zon_linearMalloc(ZonLinear *allocator, size_t size)
{
	if (allocator->index + size > allocator->size) return NULL;
	void *ptr = (char *)allocator->memory + allocator->index;
	allocator->index += size;
	return ptr;
}

static void *
mallocInterface(ZonAllocator *interface, size_t size)
{
	ZonLinear *allocator = interface->ctx;
	if (allocator->index + size > allocator->size) return NULL;
	void *ptr = (char *)allocator->memory + allocator->index;
	allocator->index += size;
	return ptr;
}

ZonAllocator
zon_linearInterface(ZonLinear *allocator)
{
	ZonAllocator interface;
	interface.ctx = allocator;
	interface.malloc = &mallocInterface;
	return interface;
}
