#include "world.h"

#include "camera.h"
#include "generator.h"
#include "renderer.h"
#include "utility.h"

#define WORLD_CHUNK_RADIUS_DEFAULT 16
#define WORLD_CHUNK_BUFFER_CAPACITY (32 * 32 * 8)
#define WORLD_CHUNK_AVAILABLE_CAPACITY (32 * 32 * 8)
#define WORLD_CHUNK_READY_CAPACITY 1024

static struct world world = { 0 };

static void load_chunk(int x, int y, int z)
{
	int key = chunk_calculate_key(x, y, z);

	if (kh_get(pending, world.chunks_pending, key) != kh_end(world.chunks_pending))
	{
		return;
	}

	if (kh_get(32, world.chunks_active, key) != kh_end(world.chunks_active))
	{
		return;
	}

	int result = 0;
	khint_t index = kh_put(pending, world.chunks_pending, key, &result);

	struct chunk* chunk = queue_pop(&world.chunks_available);

	if (chunk == NULL)
	{
		return;
	}

	chunk_init(chunk, x, y, z);

	generator_queue_work(chunk);
}

void world_init(void)
{
	world.player_chunk_x = 0;
	world.player_chunk_z = 0;

	world.chunk_radius = 20;
	world.chunk_radius_unload = world.chunk_radius + 8;

	queue_init(&world.chunks_available, WORLD_CHUNK_AVAILABLE_CAPACITY);

	world.chunk_buffer = malloc(WORLD_CHUNK_BUFFER_CAPACITY * sizeof(struct chunk));
	check_allocation(world.chunk_buffer);

	for (int i = 0; i < WORLD_CHUNK_BUFFER_CAPACITY; i++)
	{
		queue_push(&world.chunks_available, &world.chunk_buffer[i]);
	}

	queue_safe_init(&world.chunks_ready, WORLD_CHUNK_READY_CAPACITY);

	world.chunks_active = kh_init(32);

	world.chunks_pending = kh_init(pending);

	// Generate the chunks around the origin.

	for (int z = -world.chunk_radius; z < world.chunk_radius; z++)
	{
		for (int x = -world.chunk_radius; x < world.chunk_radius; x++)
		{
			load_chunk(x, 0, z);
			load_chunk(x, 1, z);
		}
	}
}

void world_free(void)
{
	free(world.chunk_buffer);
	queue_free(&world.chunks_available);
	queue_safe_free(&world.chunks_ready);
}

void world_tick(void)
{
	// Insert processed chunks into the world.
	struct chunk* chunk = NULL;

	while (chunk = queue_safe_pop(&world.chunks_ready), chunk)
	{
		int hashmap_result = 0;
		khint_t k = kh_put(32, world.chunks_active, chunk->key, &hashmap_result);
		kh_value(world.chunks_active, k) = chunk;

		khint_t iter = kh_get(pending, world.chunks_pending, chunk->key);
		kh_del(pending, world.chunks_pending, iter);
	}

	// Check if we need to generate some chunks.
	int player_chunk_x_new = Camera.position.x / CHUNK_LENGTH;
	int player_chunk_z_new = Camera.position.z / CHUNK_LENGTH;

	if (player_chunk_x_new != world.player_chunk_x)
	{
		int radius = world.chunk_radius * (player_chunk_x_new - world.player_chunk_x);

		int small_x = min(world.player_chunk_x + radius, player_chunk_x_new + radius);
		int large_x = max(world.player_chunk_x + radius, player_chunk_x_new + radius);

		for (int x = small_x; x < large_x; x++)
		{
			for (int z = player_chunk_z_new - world.chunk_radius; z < player_chunk_z_new + world.chunk_radius; z++)
			{
				load_chunk(x, 0, z);
				load_chunk(x, 1, z);
			}
		}
	}

	if (player_chunk_z_new != world.player_chunk_z)
	{
		int radius = world.chunk_radius * (player_chunk_z_new - world.player_chunk_z);

		int small_z = min(world.player_chunk_z + radius, player_chunk_z_new + radius);
		int large_z = max(world.player_chunk_z + radius, player_chunk_z_new + radius);

		for (int z = small_z; z < large_z; z++)
		{
			for (int x = player_chunk_x_new - world.chunk_radius; x < player_chunk_x_new + world.chunk_radius; x++)
			{
				load_chunk(x, 0, z);
				load_chunk(x, 1, z);
			}
		}
	}

	world.player_chunk_x = player_chunk_x_new;
	world.player_chunk_z = player_chunk_z_new;

	// Submit chunks around the camera for rendering.
	for (khint_t iter = kh_begin(world.chunks_active); iter != kh_end(world.chunks_active); ++iter)
	{
		if (kh_exist(world.chunks_active, iter))
		{
			struct chunk* chunk = kh_value(world.chunks_active, iter);

			int dist_x = abs(world.player_chunk_x - chunk->x);
			int dist_z = abs(world.player_chunk_z - chunk->z);

			if (dist_x > world.chunk_radius || dist_z > world.chunk_radius)
			{
				if (dist_x > world.chunk_radius_unload || dist_z > world.chunk_radius_unload)
				{
					kh_del(32, world.chunks_active, iter);

					if (chunk->mesh != NULL)
					{
						chunk->mesh->release(chunk);
					}
					
					queue_push(&world.chunks_available, chunk);
				}

				continue;
			}

			render_chunk(chunk);
		}
	}
}

void world_add_chunk(struct chunk* chunk)
{
	queue_safe_push(&world.chunks_ready, chunk);
}
