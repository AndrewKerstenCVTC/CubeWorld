#include "generator.h"

#include "mesher.h"
#include "queue.h"

#include "simplex.h"
#include "tinycthread.h"

#define GENERATOR_SEED 19940126
#define GENERATOR_CHUNK_CAPACITY (32 * 32 * 16)

struct generator
{
	bool running;
	thrd_t thread;
	mtx_t mutex;

	struct osn_context* noise;
	struct queue chunks;
};

static struct generator generator = { 0 };

static void thread_sleep(int nano_seconds)
{
	struct _ttherad_timespec time_sleep = { 0 };
	timespec_get(&time_sleep, TIME_UTC);

	time_sleep.tv_nsec += nano_seconds;
	time_sleep.tv_sec += time_sleep.tv_nsec / 1000000000;
	time_sleep.tv_nsec %= 1000000000;

	thrd_sleep(&time_sleep, NULL);
}

static int generator_loop(void* arg)
{
	while (true)
	{
		mtx_lock(&generator.mutex);

		if (generator.running == false)
		{
			mtx_unlock(&generator.mutex);

			break;
		}

		struct chunk* chunk = (struct chunk*) queue_pop(&generator.chunks);

		mtx_unlock(&generator.mutex);

		if (chunk == NULL)
		{
			//thread_sleep(100000000);
			continue;
		}

		// TODO: Generate chunk.
		chunk_clear(chunk);

		double chunk_x_offset = chunk->x * CHUNK_LENGTH - 1;
		double chunk_y_offset = chunk->y * CHUNK_LENGTH - 1;
		double chunk_z_offset = chunk->z * CHUNK_LENGTH - 1;

		double feature_size = 24.0;
		double max_y = CHUNK_LENGTH * 2;
		
		for (int z = 0; z < CHUNK_LENGTH_EX; z++)
		{
			for (int x = 0; x < CHUNK_LENGTH_EX; x++)
			{
				double sample_x = (chunk_x_offset + x) / feature_size;
				double sample_z = (chunk_z_offset + z) / feature_size;

				double v0 = simplex2(generator.noise, sample_x / 4.0, sample_z / 4.0);
				double v1 = simplex2(generator.noise, sample_x / 2.0, sample_z / 2.0);
				double v2 = simplex2(generator.noise, sample_x, sample_z);

				double value = ((v0 * 4 / 7.0 + v1 * 2 / 7.0 + v2 * 1 / 7.0) + 1.0) / 2.0;

				int cutoff = (int)(value * max_y);

				for (int y = 0; y < CHUNK_LENGTH_EX; y++)
				{
					if (chunk_y_offset + y <= cutoff)
					{
						//chunk_set(chunk, x, y, z, true);
						chunk->blocks[y * CHUNK_SLICE_EX + z * CHUNK_LENGTH_EX + x] = 1;
					}
				}
			}
		}

		// Pass the chunk on to the mesher.
		mesher_queue_work(chunk);
	}

	return 0;
}

bool generator_start_thread(void)
{
	generator.running = true;
	simplex(GENERATOR_SEED, &generator.noise);
	queue_init(&generator.chunks, GENERATOR_CHUNK_CAPACITY);

	if (mtx_init(&generator.mutex, mtx_plain) == thrd_error)
	{
		return false;
	}

	if (thrd_create(&generator.thread, generator_loop, NULL) == thrd_error)
	{
		return false;
	}

	return true;
}

void generator_stop_thread(void)
{
	mtx_lock(&generator.mutex);
	generator.running = false;
	mtx_unlock(&generator.mutex);
}

void generator_queue_work(struct chunk* chunk)
{
	mtx_lock(&generator.mutex);

	while (queue_push(&generator.chunks, chunk) == false)
	{
		// Work queue is full, sleep for 0.1ms.
		mtx_unlock(&generator.mutex);

		thread_sleep(100000);

		mtx_lock(&generator.mutex);
	}

	mtx_unlock(&generator.mutex);
}
