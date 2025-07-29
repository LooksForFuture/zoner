#include <zoner/zon_flpool.h>

#include <assert.h>

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
	Avatar avatars[200];
	ZonFLPool pool = zon_flpoolCreate(avatars, sizeof(Avatar), 200);
	assert(pool.memory == avatars);
	assert(pool.head == avatars);

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

	assert(zon_flpoolUnlock(&pool) == avatars);
	assert(pool.memory == NULL);
	return 0;
}
