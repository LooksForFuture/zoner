#include <zoner/zon_heap.h>

#include <assert.h>
#include <stdint.h>
#include <stdalign.h>

#define MAX_ALIGN 256

int
main()
{
	for (size_t align = 1; align <= MAX_ALIGN; align *= 2) {
		void *p1 = zon_alloc(512, align);
		assert(p1);
		void *p2 = zon_heap.alloc(&zon_heap, 512, align);
		assert(p2);

		assert((uintptr_t)p1 % align == 0);
		assert(((uintptr_t)p1 & align - 1) == 0);
		assert((uintptr_t)p2 % align == 0);
		assert(((uintptr_t)p2 & align - 1) == 0);

		p1 = zon_reallocAligned(p1, 1024, align);
		assert(p1);
		p2 = zon_heap.reallocAligned(&zon_heap, p2, 1024, align);
		assert(p2);

		assert((uintptr_t)p1 % align == 0);
		assert(((uintptr_t)p1 & align - 1) == 0);
		assert((uintptr_t)p2 % align == 0);
		assert(((uintptr_t)p2 & align - 1) == 0);

		zon_free(p1);
		zon_heap.free(&zon_heap, p2);
	}

	int *p = zon_malloc(10 * sizeof(int));
	assert(p);
	assert(((uintptr_t)p & alignof(max_align_t) - 1) == 0);
	for (int i = 0; i < 10; i++) {
		p[i] = i;
	}

	p = zon_realloc(p, 12 * sizeof(int));
	assert(((uintptr_t)p & alignof(max_align_t) - 1) == 0);
	for (int i = 0; i < 10; i++) {
		assert(p[i] == i);
	}

	zon_free(p);

	p = zon_malloc(12 * sizeof(int));
	assert(p);
	assert(((uintptr_t)p & alignof(max_align_t) - 1) == 0);
	for (int i = 0; i < 12; i++) {
		p[i] = i;
	}

	p = zon_realloc(p, 8 * sizeof(int));
	assert(((uintptr_t)p & alignof(max_align_t) - 1) == 0);
	for (int i = 0; i < 8; i++) {
		assert(p[i] == i);
	}

	zon_free(p);

	p = NULL;
	p = zon_heap.realloc(&zon_heap, p, 10240);
	assert(p);
	assert(((uintptr_t)p & alignof(max_align_t) - 1) == 0);
	zon_heap.free(&zon_heap, p);

	assert(zon_malloc(100000000000000) == NULL);

	return 0;
}
