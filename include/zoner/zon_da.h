#ifndef __ZON_DA__
#define __ZON_DA__

#include <zoner/zon_allocator.h>
#include <zoner/zon_heap.h>

#include <string.h>

/*
 * Dynamic arrays with macro based functions utilizing zoner based allocators.
 * Each type should have a struct for its dynamic array type.
 * A sample struct for dynamic int array is as follows:
 * struct IntArray {
 *	ZonAllocator allocator;
 * 	int *items;
 *	size_t capacity;
 *	size_t count;
 * };
 *
 * While it is possible to change type of struct members, their names
 * should not be changed.
 *
 * PS: Ofcourse. It has been inspired by dynamic arrays of tsoding's nob.h
 */

#include <stdlib.h>

#ifndef ZON_DA_INIT_CAP
#define ZON_DA_INIT_CAP 32
#endif

// Make sure the array is big enough to store the expected count of items
#define ZON_DA_RESERVE(arr, expectedCapacity) do { \
if ((expectedCapacity) > (arr)->capacity) { \
	if ((arr)->capacity == 0) (arr)->capacity = ZON_DA_INIT_CAP; \
	while ((arr)->capacity < (expectedCapacity)) { \
		(arr)->capacity *= 2; \
	} \
	if ((arr)->allocator.ctx != 0) { \
		(arr)->items = (arr)->allocator.realloc( \
			&(arr)->allocator, \
			(arr)->items, \
			(arr)->capacity * sizeof(*(arr)->items)); \
	} else { \
		(arr)->items = zon_realloc((arr)->items, \
			(arr)->capacity * sizeof(*(arr)->items)); \
	} \
} \
} while (0)

// Append the item to the end of array
#define ZON_DA_APPEND(arr, item) do { \
	if ((arr)->count + 1 > (arr)->capacity) { \
		(arr)->capacity = (arr)->capacity == 0 ? ZON_DA_INIT_CAP : \
			(arr)->capacity * 2; \
		if ((arr)->allocator.ctx != 0) { \
			(arr)->items = (arr)->allocator.realloc( \
				&(arr)->allocator, \
				(arr)->items, \
				(arr)->capacity * sizeof(*(arr)->items)); \
		} else { \
			(arr)->items = zon_realloc((arr)->items, \
				(arr)->capacity * sizeof(*(arr)->items)); \
		} \
	} \
	(arr)->items[(arr)->count++] = (item); \
} while(0)

// Insert the them into the specified index
#define ZON_DA_INSERT(arr, item, i) do { \
	ZON_DA_RESERVE((arr), (i) + 1); \
	memcpy((arr)->items + (i) + 1, \
		(arr)->items + (i), \
		sizeof(*(arr)->items) * ((arr)->count - (i))); \
	(arr)->items[(i)] = (item); \
	(arr)->count++; \
} while(0)

#endif //__ZON_DA__
