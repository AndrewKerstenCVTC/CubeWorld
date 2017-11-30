#pragma once

#include "aabb.h"
#include "chunk_mesh.h"
#include "color.h"
#include "inline.h"
#include "transform.h"

#include <GL/glew.h>

#include <stdbool.h>

#define CHUNK_LENGTH 32
#define CHUNK_SLICE (CHUNK_LENGTH * CHUNK_LENGTH)
#define CHUNK_VOLUME (CHUNK_LENGTH * CHUNK_LENGTH * CHUNK_LENGTH)

#define CHUNK_LENGTH_EX 34
#define CHUNK_SLICE_EX (CHUNK_LENGTH_EX * CHUNK_LENGTH_EX)
#define CHUNK_VOLUME_EX (CHUNK_LENGTH_EX * CHUNK_LENGTH_EX * CHUNK_LENGTH_EX)

struct chunk
{
	int key;

	int x;
	int y;
	int z;

	//struct aabb aabb;
	GLKVector3 corners[8];

	struct transform transform;
	struct chunk_mesh* mesh;

	char blocks[CHUNK_VOLUME_EX];
	struct color colors[CHUNK_VOLUME];
};

void chunk_init(struct chunk* chunk, int x, int y, int z);

void chunk_clear(struct chunk* chunk);

int chunk_calculate_key(int x, int y, int z);

extern INLINE int chunk_index_get(int x, int y, int z);

extern INLINE int chunk_index_ex_get(int x, int y, int z);
