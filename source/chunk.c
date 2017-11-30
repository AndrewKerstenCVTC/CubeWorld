#include "chunk.h"

#include "utility.h"

#include <string.h>

int chunk_calculate_key(int x, int y, int z)
{
	int result = 0;

	result |= (x & 0x3FFF) << 0;
	result |= (y & 0x000F) << 28;
	result |= (z & 0x3FFF) << 14;

	return result;
}

void chunk_init(struct chunk* chunk, int x, int y, int z)
{
	chunk->key = chunk_calculate_key(x, y, z);

	chunk->x = x;
	chunk->y = y;
	chunk->z = z;

	//chunk->aabb.min = GLKVector3Make(x * CHUNK_LENGTH, y * CHUNK_LENGTH, z * CHUNK_LENGTH);
	//chunk->aabb.max = GLKVector3AddScalar(chunk->aabb.min, CHUNK_LENGTH);

	int xx = x * CHUNK_LENGTH - 1;
	int zz = z * CHUNK_LENGTH - 1;
	int d = CHUNK_LENGTH + 1;

	int miny = y * CHUNK_LENGTH - 1;
	int maxy = miny + d;

	chunk->corners[0] = GLKVector3Make(x + 0, miny, z + 0);
	chunk->corners[1] = GLKVector3Make(x + d, miny, z + 0);
	chunk->corners[2] = GLKVector3Make(x + 0, miny, z + d);
	chunk->corners[3] = GLKVector3Make(x + d, miny, z + d);
	chunk->corners[4] = GLKVector3Make(x + 0, maxy, z + 0);
	chunk->corners[5] = GLKVector3Make(x + d, maxy, z + 0);
	chunk->corners[6] = GLKVector3Make(x + 0, maxy, z + d);
	chunk->corners[7] = GLKVector3Make(x + d, maxy, z + d);

	transform_init(&chunk->transform);
	chunk->transform.translation = GLKVector3Make(x * CHUNK_LENGTH, y * CHUNK_LENGTH, z * CHUNK_LENGTH);
	transform_calc(&chunk->transform);
}

void chunk_clear(struct chunk* chunk)
{
	memset(chunk->blocks, 0, CHUNK_VOLUME_EX * sizeof(char));
}

INLINE int chunk_index_get(int x, int y, int z)
{
	return y * CHUNK_SLICE + z * CHUNK_LENGTH + x;
}

INLINE int chunk_index_ex_get(int x, int y, int z)
{
	return (y + 1) * CHUNK_SLICE_EX + (z + 1) * CHUNK_LENGTH_EX + (x + 1);
}
