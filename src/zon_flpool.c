#include <zoner/zon_flpool.h>

ZonFLPool
zon_flpoolCreate(void *memory, size_t size, size_t count)
{
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
	void *ptr = pool->memory;
	pool->memory = NULL;
	return ptr;
}

void *
zon_flpoolPop(ZonFLPool *pool)
{
	if (!pool->head) return NULL;

	void *object = pool->head;
	pool->head = *(void **)object;
	return object;
}

void
zon_flpoolPush(ZonFLPool *pool, void *item)
{
	*(void **)item = pool->head;
	pool->head = item;
}
