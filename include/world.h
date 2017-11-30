#pragma once

#include "chunk.h"
#include "khash.h"
#include "queue.h"
#include "queue_safe.h"

#define WORLD_HEIGHT_CHUNKS 4

KHASH_MAP_INIT_INT(32, void*)

KHASH_MAP_INIT_INT(pending, int)

struct world
{
	// TODO:  chunk_buffer and chunks_available can be combined into a pool.
	// (NO freelist.  We don't want to accidentally overwrite data in the chunk
	// that is preserved between uses.  For example the VBO.  Freelist may be an
	// option if per chunk VBOs get switched to a single shared VBO)

	// Chunk buffer.
	struct chunk* chunk_buffer;

	// Chunks that have been initialized and are available for use.
	struct queue chunks_available;

	// Chunks that have been processed and are ready to be made active.
	// This is a thread safe place for processed chunks to be dropped off until
	// the main thread can merge them into the active chunk 3D array.
	struct queue_safe chunks_ready;

	khash_t(32)* chunks_active;

	khash_t(pending)* chunks_pending;

	int chunk_radius;
	int chunk_radius_unload;

	int player_chunk_x;
	int player_chunk_z;
};

void world_init(void);

void world_free(void);

void world_tick(void);

// Thead safe.
void world_add_chunk(struct chunk* chunk);
