#include <zoner/zon_stack.h>

#ifndef NDEBUG
#include <assert.h>
#endif
#include <stddef.h>
#include <stdalign.h>

#define ZON_ALIGN_UP(p, align) (((p) + ((align)-1)) & ~((align)-1))

ZonStack
zon_stackCreate(void *memory, size_t size)
{
	return (ZonStack){ size, 0, memory };
}

void
zon_stackReset(ZonStack *allocator)
{
#ifndef NDEBUG
	assert(allocator != NULL);
#endif
	allocator->index = 0;
}

void *
zon_stackUnlock(ZonStack *allocator)
{
#ifndef NDEBUG
	assert(allocator != NULL);
#endif
	void *ptr = allocator->memory;
	allocator->memory = NULL;
	return ptr;
}

void *
zon_stackAlloc(ZonStack *allocator, size_t size, size_t alignment)
{
#ifndef NDEBUG
	assert(allocator && allocator->memory);
	//alignment should be power of 2
	assert(alignment != 0 && (alignment & (alignment - 1)) == 0);
#endif
	size_t aligned = ZON_ALIGN_UP(allocator->index, alignment);
	size_t headerOffset = ZON_ALIGN_UP(aligned + size, alignof(size_t));
	size_t total = headerOffset + sizeof(size_t);
	if (total > allocator->size) return NULL;

	*(size_t*)((char*)allocator->memory + headerOffset) = allocator->index;
	allocator->index = total;
	return (char*)allocator->memory + aligned;
}

void *
zon_stackMalloc(ZonStack *allocator, size_t size)
{
#ifndef NDEBUG
	assert(allocator && allocator->memory);
#endif
	return zon_stackAlloc(allocator, size, alignof(max_align_t));
}

void
zon_stackPop(ZonStack *allocator)
{
#ifndef NDEBUG
	assert(allocator && allocator->memory);
	assert(allocator->index > 0 ? allocator->index > sizeof(size_t) : 1);
#endif
	if (allocator->index == 0) return;
	allocator->index = *(size_t *)((char *)allocator->memory +
		allocator->index - sizeof(size_t));
}

size_t
zon_stackMarker(ZonStack *allocator)
{
	return allocator->index;
}

void
zon_stackRewind(ZonStack *allocator, size_t marker)
{
	if (marker <= allocator->size) allocator->index = marker;
}

#undef ZON_ALIGN_UP
