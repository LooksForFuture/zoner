#include <zoner/zon_flpool.h>

#ifndef NDEBUG
#include <assert.h>
#endif

ZonFLPool
zon_flpoolCreate(void *memory, size_t size, size_t count)
{
#ifndef NDEBUG
	assert(memory != NULL);
	assert(size != 0);
	assert(count != 0);
#endif
	ZonFLPool pool;
	pool.memory = memory;
	pool.size = size;
	pool.count = count;

	char *current = (char *)memory;
	for (size_t i = 0; i < count - 1; ++i) {
		void *next = current + size;
		*(void **)current = next;
		current += size;
	}
	*(void **)current = NULL;

	pool.head = memory;
	return pool;
}

void
zon_flpoolReset(ZonFLPool *pool)
{
#ifndef NDEBUG
	assert(pool != NULL);
	assert(pool->memory != NULL);
#endif
	char *current = (char *)pool->memory;
	for (size_t i = 0; i < pool->count - 1; ++i) {
		void *next = current + pool->size;
		*(void **)current = next;
		current += pool->size;
	}
	*(void **)current = NULL;

	pool->head = pool->memory;
}

void *
zon_flpoolUnlock(ZonFLPool *pool)
{
#ifndef NDEBUG
	assert(pool != NULL);
#endif
	void *ptr = pool->memory;
	pool->memory = NULL;
	pool->head = NULL;
	return ptr;
}

void *
zon_flpoolPop(ZonFLPool *pool)
{
#ifndef NDEBUG
	assert(pool != NULL);
	assert(pool->memory != NULL);
#endif
	if (!pool->head) return NULL;

	void *object = pool->head;
	pool->head = *(void **)object;
	return object;
}

void
zon_flpoolPush(ZonFLPool *pool, void *item)
{
#ifndef NDEBUG
	assert(pool != NULL);
	assert(item != NULL);
	assert(pool->memory != NULL);
	assert((char *)pool->memory + pool->count *
			pool->size > (char *)item);
#endif
	*(void **)item = pool->head;
	pool->head = item;
}
