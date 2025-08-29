#include <zoner/zon_heap.h>

#ifndef NDEBUG
#include <assert.h>
#endif
#include <stddef.h>
#include <stdint.h>
#include <stdalign.h>

#define ZON_ALIGN_PTR(p, align) \
	(uint8_t*)(((uintptr_t)p + align - 1) & ~((uintptr_t)align - 1))

void *
zon_alloc(size_t size, size_t alignment)
{
	uint8_t *ptr = malloc(size + alignment + sizeof(void *));
	if (ptr == NULL) return NULL;
	void *aligned = ZON_ALIGN_PTR(ptr + sizeof(void *), alignment);
	*((void **)aligned - 1) = ptr;
	return aligned;
}

static void *
zon_heapAlloc(const ZonAllocator *allocator, size_t size, size_t alignment)
{
	return zon_alloc(size, alignment);
}

void *
zon_malloc(size_t size)
{
	return zon_alloc(size, alignof(max_align_t));
}

static void *
zon_heapMalloc(const ZonAllocator *allocator, size_t size)
{
	return zon_alloc(size, alignof(max_align_t));
}

void *
zon_reallocAligned(void *raw, size_t size, size_t alignment)
{
#ifndef NDEBUG
	//alignment should be power of 2
	assert(alignment != 0 && (alignment & (alignment - 1)) == 0);
#endif
	if (raw) {
		void *p = *((void **)raw - 1);
		uint8_t *ptr = realloc(p, size + alignment + sizeof(void *));
		if (ptr == NULL) return NULL;
		void *aligned = ZON_ALIGN_PTR(ptr + sizeof(void *), alignment);
		*((void **)aligned - 1) = ptr;
		return aligned;
	}

	return zon_alloc(size, alignment);
}

void *
zon_realloc(void *raw, size_t size)
{
	if (raw) {
		void *p = *((void **)raw - 1);
		uint8_t *ptr = realloc(p, size + alignof(max_align_t) + sizeof(void *));
		if (ptr == NULL) return NULL;
		void *aligned = ZON_ALIGN_PTR(ptr + sizeof(void *), alignof(max_align_t));
		*((void **)aligned - 1) = ptr;
		return aligned;
	}

	return zon_alloc(size, alignof(max_align_t));
}

static void *
zon_heapReallocAligned(const ZonAllocator *allocator,
		       void *ptr, size_t size, size_t alignment)
{
	return zon_reallocAligned(ptr, size, alignment);
}

static void *
zon_heapRealloc(const ZonAllocator *allocator, void *ptr, size_t size)
{
	return zon_realloc(ptr, size);
}

void
zon_free(void *ptr)
{
	if (ptr) free(*((void **)ptr - 1));
}

static void
zon_heapFree(const ZonAllocator *allocator, void *ptr)
{
	zon_free(ptr);
}

const ZonAllocator zon_heap = {
	.ctx=NULL,
	.alloc=zon_heapAlloc,
	.malloc=zon_heapMalloc,
	.reallocAligned=zon_heapReallocAligned,
	.realloc=zon_heapRealloc,
	.free=zon_heapFree
};
