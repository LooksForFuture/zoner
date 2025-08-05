#include <zoner/zon_stack.h>

#ifndef NDEBUG
#include <assert.h>
#endif
#include <stddef.h>
#include <stdint.h>
#include <stdalign.h>

#define ZON_ALIGN_PTR(p, align) \
	(uint8_t*)(((uintptr_t)p + align - 1) & ~((uintptr_t)align - 1))

ZonStack
zon_stackCreate(void *base, size_t size)
{
	return (ZonStack){
		.base = base,
		.current = base,
		.end = (uint8_t*)base + size
	};
}

void
zon_stackReset(ZonStack *allocator)
{
#ifndef NDEBUG
	assert(allocator != NULL);
#endif
	allocator->current = allocator->base;
}

void *
zon_stackUnlock(ZonStack *allocator)
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
zon_stackAlloc(ZonStack *allocator, size_t size, size_t alignment)
{
#ifndef NDEBUG
	assert(allocator);
	assert(allocator->current);
	//alignment should be power of 2
	assert(alignment != 0 && (alignment & (alignment - 1)) == 0);
#endif
	uint8_t *aligned = ZON_ALIGN_PTR(allocator->current, alignment);
	uint8_t *headerPos = ZON_ALIGN_PTR(aligned+size, alignof(size_t));
	uint8_t *next = headerPos + sizeof(size_t);

	if (next > allocator->end) return NULL;

	*(size_t *)headerPos =
		(size_t)(allocator->current - (uint8_t *)allocator->base);
	allocator->current = next;

	return aligned;
}

void *
zon_stackMalloc(ZonStack *allocator, size_t size)
{
#ifndef NDEBUG
	assert(allocator && allocator->base);
#endif
	return zon_stackAlloc(allocator, size, alignof(max_align_t));
}

void
zon_stackPop(ZonStack *allocator)
{
#ifndef NDEBUG
	assert(allocator && allocator->base);
#endif
	if (allocator->current == allocator->base) return;
	size_t prevOffset = *(size_t *)(allocator->current - sizeof(size_t));
	allocator->current = (uint8_t *)allocator->base + prevOffset;
}

size_t
zon_stackMarker(ZonStack *allocator)
{
	return (size_t)(allocator->current - (uint8_t *)allocator->base);
}

void
zon_stackRewind(ZonStack *allocator, size_t marker)
{
#ifndef NDEBUG
	assert(marker <=
	       (size_t)(allocator->current - (uint8_t *)allocator->base));
#endif
        allocator->current = (uint8_t *)allocator->base + marker;
}

#undef ZON_ALIGN_UP
