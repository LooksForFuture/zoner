#include <zoner/zon_stack.h>

#include <assert.h>
#include <stdint.h>
#include <stddef.h>

#define MAX_ALIGN 256

int
main()
{
	//align malloc
	void *raw = malloc(65536 + MAX_ALIGN);
	uintptr_t aligned_addr =
		((uintptr_t)raw + MAX_ALIGN-1) & ~((uintptr_t)MAX_ALIGN-1);
	size_t totalSize = 65536 - ((uintptr_t)aligned_addr - (uintptr_t)raw);
	void *mem = (void *)aligned_addr;

	ZonStack stack = zon_stackCreate(mem, totalSize);
	assert(stack.size == totalSize);
	assert(stack.index == 0);
	assert(stack.memory == mem);

	for (size_t align = 1; align <= MAX_ALIGN; align *= 2) {
		void* p = zon_stackAlloc(&stack, 512, align);
		assert(p);
		assert((uintptr_t)p % align == 0);
		assert(((uintptr_t)p & (align - 1)) == 0);

		p = zon_stackMalloc(&stack, 512);
		assert(((uintptr_t)p & alignof(max_align_t) - 1) == 0);
	}

	size_t mark = zon_stackMarker(&stack);
	assert(mark == stack.index);

	for (size_t align = 1; align <= MAX_ALIGN; align *= 2) {
		void* p = zon_stackAlloc(&stack, 512, align);
		assert(p);
		assert((uintptr_t)p % align == 0);
		assert(((uintptr_t)p & (align - 1)) == 0);

		zon_stackRewind(&stack, mark);
		assert(stack.index == mark);
	}

	zon_stackReset(&stack);
	assert(stack.index == 0);
	assert(zon_stackMalloc(&stack, totalSize) == NULL);

	zon_stackPop(&stack); //should not crash

	void* p1 = zon_stackMalloc(&stack, 64);
	zon_stackReset(&stack);
	void* p2 = zon_stackMalloc(&stack, 64);
	assert(p2 == p1);

	void* returned = zon_stackUnlock(&stack);
	assert(returned == mem);
	assert(stack.memory == NULL);

	free(raw);
}
