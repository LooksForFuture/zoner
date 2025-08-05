#include <zoner/zon_stack.h>

#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#define MEM_SIZE 65536
#define MAX_ALIGN 256

int
main()
{
	//align malloc
	void *mem = malloc(MEM_SIZE);

	ZonStack stack = zon_stackCreate(mem, MEM_SIZE);
	assert(stack.base == mem);
	assert(stack.current == mem);
	assert(stack.end == (uint8_t *)mem + MEM_SIZE);

	for (size_t align = 1; align <= MAX_ALIGN; align *= 2) {
		void* p = zon_stackAlloc(&stack, 512, align);
		assert(p);
		assert((uintptr_t)p % align == 0);
		assert(((uintptr_t)p & align - 1) == 0);

		p = zon_stackMalloc(&stack, 512);
		assert(((uintptr_t)p & alignof(max_align_t) - 1) == 0);
	}

	size_t mark = zon_stackMarker(&stack);
	assert((uint8_t *)mem + mark == stack.current);
	for (size_t align = 1; align <= MAX_ALIGN; align *= 2) {
		void* p = zon_stackAlloc(&stack, 512, align);
		assert(p);
		assert((uintptr_t)p % align == 0);
		assert(((uintptr_t)p & (align - 1)) == 0);

		zon_stackRewind(&stack, mark);
		assert(stack.current == (uint8_t *)mem + mark);
	}

	zon_stackReset(&stack);
	assert(stack.current == mem);
	assert(zon_stackMalloc(&stack, MEM_SIZE) == NULL);

	zon_stackPop(&stack); //should not crash

	void *p1 = zon_stackMalloc(&stack, 64);
	zon_stackReset(&stack);
	void *p2 = zon_stackMalloc(&stack, 64);
	assert(p2 == p1);

	for (int i = 0; i < 1000; ++i) {
		size_t s = (rand() % 64) + 8;
		s = (s + 7) & ~7;
		void* p = zon_stackAlloc(&stack, s, 8);
		assert(((uintptr_t)p & 7) == 0);
		zon_stackPop(&stack);
	}

	void *returned = zon_stackUnlock(&stack);
	assert(returned == mem);
	assert(stack.base == NULL);

	free(mem);
}
