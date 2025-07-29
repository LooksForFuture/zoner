#include <zoner/zon_flpool.h>

#include <assert.h>

#define OBJECT_COUNT 200

typedef struct {
	float x;
	float y;
	float width;
	float height;
	float rotation;
} Transform;

typedef struct {
	Transform transform;
	int textureId;
	int health;
} Avatar;

int
main()
{
	assert(sizeof(Avatar) >= sizeof(void *));
	Avatar avatars[OBJECT_COUNT];
	ZonFLPool pool = zon_flpoolCreate(avatars,
			sizeof(Avatar), OBJECT_COUNT);
	assert(pool.memory == avatars);
	assert(pool.head == avatars);

	for (int i = 0; i < OBJECT_COUNT - 1; i++) {
		assert(*(void **)&avatars[i] == &avatars[i+1]);
	}
	assert(*(void **)&avatars[OBJECT_COUNT-1] == NULL);

	Avatar *av1 = (Avatar *)zon_flpoolPop(&pool);
	assert(av1 == avatars);
	assert(pool.head == &avatars[1]);

	Avatar *av2 = (Avatar *)zon_flpoolPop(&pool);
	assert(av2 == &avatars[1]);
	assert(pool.head == &avatars[2]);

	Avatar *av3 = (Avatar *)zon_flpoolPop(&pool);
	assert(av3 == &avatars[2]);
	assert(pool.head == &avatars[3]);

	zon_flpoolPush(&pool, av1);
	assert(pool.head == av1);
	assert(*(void **)av1 == &avatars[3]);

	zon_flpoolPush(&pool, av3);
	assert(pool.head == av3);
	assert(*(void **)av3 == av1);

	zon_flpoolPush(&pool, av2);
	assert(pool.head == av2);
	assert(*(void **)av2 == av3);

	zon_flpoolReset(&pool);
	assert(pool.head == pool.memory && pool.head == avatars);
	for (int i = 0; i < OBJECT_COUNT - 1; i++) {
		assert(*(void **)&avatars[i] == &avatars[i+1]);
	}
	assert(*(void **)&avatars[OBJECT_COUNT-1] == NULL);

	assert(zon_flpoolUnlock(&pool) == avatars);
	assert(pool.memory == NULL);
	return 0;
}
