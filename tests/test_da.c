#include <zoner/zon_heap.h>
#include <zoner/zon_da.h>

#include <assert.h>
#include <stdio.h>

#define TEST_ARRAY_INIT(arr) do { \
	assert((arr).count == 0 && "empty array element count is not zero"); \
	assert((arr).capacity == 0 && \
	       "capacity of non initialized array is not zero"); \
	assert((arr).items == NULL && \
	       "non initialized array items pointer is not NULL"); \
} while (0)

typedef struct {
	ZonAllocator allocator;
	int *items;
	size_t capacity;
	size_t count;
} IntArray;

static void
test_init()
{
	IntArray arr = {0};
	TEST_ARRAY_INIT(arr);
	assert(arr.allocator.ctx == 0 &&
	       "context of default array allocator must be NULL");
}

static void
test_appendSingle()
{
	IntArray arr = {0};
	TEST_ARRAY_INIT(arr);

	int value = 42;
	ZON_DA_APPEND(&arr, value);

	assert(arr.count == 1 && "array element count not incremented");
	assert(arr.items[0] == value && "value not appended correctly");
	assert(arr.capacity == ZON_DA_INIT_CAP &&
	       "capacity not equal to init cap after first append");

	zon_free(arr.items);
}

static void
test_appendMany()
{
	IntArray arr = {0};
	TEST_ARRAY_INIT(arr);

	for (int i = 0; i < 100; i++) {
		ZON_DA_APPEND(&arr, i);
	}

	assert(arr.count == 100);
	assert(arr.capacity >= 100);

	zon_free(arr.items);
}

static void
test_insert()
{
	IntArray arr = {0};
	TEST_ARRAY_INIT(arr);

	for (int i = 0; i < 5; i++) {
		ZON_DA_APPEND(&arr, i);
	}

	ZON_DA_INSERT(&arr, 99, 2);

	assert(arr.count == 6);
	assert(arr.items[2] == 99);
	assert(arr.items[3] == 2);

	ZON_DA_INSERT(&arr, 99, 0);

	assert(arr.count == 7);
	assert(arr.items[0] == 99);
	assert(arr.items[1] == 0);
	assert(arr.items[2] == 1);
	assert(arr.items[3] == 99);

	ZON_DA_INSERT(&arr, 100, arr.count);

	assert(arr.count == 8);
	assert(arr.items[5] == 3);
	assert(arr.items[6] == 4);
	assert(arr.items[7] == 100);

	zon_free(arr.items);
}

static void
test_insertEmpty()
{
	IntArray arr = {0};
	TEST_ARRAY_INIT(arr);

	ZON_DA_INSERT(&arr, 42, 0);

	assert(arr.count == 1);
	assert(arr.items[0] == 42);

	zon_free(arr.items);
}

int
main()
{
	IntArray arr = {0};
	zon_free(arr.items);

	test_init();
	test_appendSingle();
	test_appendMany();
	test_insert();
	test_insertEmpty();

	return 0;
}
