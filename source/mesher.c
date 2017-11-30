#include "mesher.h"

#include "chunk_mesh.h"
#include "queue.h"
#include "stack.h"
#include "utility.h"
#include "window.h"
#include "world.h"

#include "tinycthread.h"

#define MESHER_CHUNK_CAPACITY (32 * 32 * 16)
#define MESHER_MESH_CAPACITY 16384
#define MESHER_VBO_LENGTH (1024 * 1024 * 1024)
#define MESHER_BUFFER_LENGTH 5000000

// TODO:  If we run into an issue where a released mesh gets overwritten with
// new data while the old data is still in use on the GPU because the GPU is a
// lagging behind one or more frames then we should keep two lists of released
// meshes.  Alternate which list gets filled every couple of seconds.
// Basically, double buffer marking meshes as released to gurantee they are old

struct mesher
{
	bool running;
	thrd_t thread;
	mtx_t mutex_chunks;
	mtx_t mutex_meshes;

	// Chunks pending meshing.
	struct queue chunks;

	// A buffer for chunk_mesh structures.
	struct chunk_mesh* mesh_buffer;

	// A stack for storing and retrieving unused ringbuffer entries.
	struct stack mesh_stack;

	// Queue for keeping record of the ringbuffer entries in use.
	struct queue mesh_queue;

	GLuint vbo;

	struct
	{
		size_t length;
		size_t capacity;
		GLubyte* front;
		GLubyte* back;
		GLubyte* head;
		GLubyte* tail;
	} ringbuffer;

	GLubyte* buffer;
};

static struct mesher mesher = { 0 };

// ---------------- START RINGBUFFER FUNCTIONS ---------------- //

static size_t ringbuffer_bytes_free()
{
	if (mesher.ringbuffer.head >= mesher.ringbuffer.tail)
	{
		return mesher.ringbuffer.capacity - (mesher.ringbuffer.head - mesher.ringbuffer.tail);
	}
	else
	{
		return mesher.ringbuffer.tail - mesher.ringbuffer.head - 1;
	}
}

static size_t ringbuffer_bytes_from_end()
{
	return mesher.ringbuffer.back - mesher.ringbuffer.head;
}

static size_t ringbuffer_bytes_used()
{
	return mesher.ringbuffer.capacity - ringbuffer_bytes_free();
}

static size_t ringbuffer_offset()
{
	return mesher.ringbuffer.head - mesher.ringbuffer.front;
}

static struct chunk_mesh* ringbuffer_copy_into(void* source, size_t length)
{
	// Before we get started, check and see if the front of the mesh queue is a
	// released mesh.  Recursively check.

	while (true)
	{
		struct chunk_mesh* mesh = queue_peek(&mesher.mesh_queue);

		if (mesh == NULL)
		{
			break;
		}

		mtx_lock(&mesher.mutex_meshes);

		bool released = mesh->private.released;

		mtx_unlock(&mesher.mutex_meshes);

		if (released == true)
		{
			mesh = queue_pop(&mesher.mesh_queue);

			mesher.ringbuffer.tail += mesh->private.length;

			stack_push(&mesher.mesh_stack, mesh);
		}
		else
		{
			break;
		}
	}

	// Get on with the actual copy logic.

	size_t bytes_from_end = ringbuffer_bytes_from_end();

	if (length > bytes_from_end)
	{
		// We don't want to wrap the data over the end of the buffer.  We
		// insert a dummy mesh entry to use up the space between the head and
		// the end of the buffer.  The dummy entry is set as released.

		struct chunk_mesh* mesh = stack_pop(&mesher.mesh_stack);

		if (mesh == NULL)
		{
			return NULL;
		}

		mesh->private.length = bytes_from_end;
		mesh->private.offset = ringbuffer_offset();
		mesh->private.released = true;

		mesh->first = mesh->private.offset / 8;
		mesh->count = mesh->private.length / 8;

		if (queue_push(&mesher.mesh_queue, mesh) == false)
		{
			// We failed to push the mesh onto the "in use" queue.  Push this
			// mesh back onto the "unused" stack.
			stack_push(&mesher.mesh_stack, mesh);

			return NULL;
		}

		mesher.ringbuffer.head = mesher.ringbuffer.front;
	}

	if (length > ringbuffer_bytes_free())
	{
		return NULL;
	}

	struct chunk_mesh* mesh = stack_pop(&mesher.mesh_stack);

	if (mesh == NULL)
	{
		return NULL;
	}

	mesh->private.length = length;
	mesh->private.offset = ringbuffer_offset();
	mesh->private.released = false;

	mesh->first = mesh->private.offset / 8;
	mesh->count = mesh->private.length / 8;

	if (queue_push(&mesher.mesh_queue, mesh) == false)
	{
		// We failed to push the mesh onto the "in use" queue.  Push this
		// mesh back onto the "unused" stack.
		stack_push(&mesher.mesh_stack, mesh);

		return NULL;
	}

	memcpy(mesher.ringbuffer.head, source, length);
	
	glFlushMappedNamedBufferRange(mesher.vbo, mesher.ringbuffer.head - mesher.ringbuffer.front, length);

	mesher.ringbuffer.head += length;

	return mesh;
}

// ---------------- END RINGBUFFER FUNCTIONS ---------------- //

static void thread_sleep(int nano_seconds)
{
	struct _ttherad_timespec time_sleep = { 0 };
	timespec_get(&time_sleep, TIME_UTC);

	time_sleep.tv_nsec += nano_seconds;
	time_sleep.tv_sec += time_sleep.tv_nsec / 1000000000;
	time_sleep.tv_nsec %= 1000000000;
	
	thrd_sleep(&time_sleep, NULL);
}

void mesher_release_mesh(struct chunk* chunk)
{
	mtx_lock(&mesher.mutex_meshes);

	chunk->mesh->private.released = true;

	mtx_unlock(&mesher.mutex_meshes);

	chunk->mesh = NULL;
}

static void mesher_mesh(struct chunk* chunk)
{
	static int blah = 0;
	blah++;

	const unsigned char NORMAL_Y_NEG = 0;
	const unsigned char NORMAL_Y_POS = 1;
	const unsigned char NORMAL_Z_NEG = 2;
	const unsigned char NORMAL_Z_POS = 3;
	const unsigned char NORMAL_X_NEG = 4;
	const unsigned char NORMAL_X_POS = 5;

	size_t buffer_index = 0;

	for (int y = 0; y < CHUNK_LENGTH; y++)
	{
		for (int z = 0; z < CHUNK_LENGTH; z++)
		{
			for (int x = 0; x < CHUNK_LENGTH; x++)
			{
				if (chunk->blocks[chunk_index_ex_get(x, y, z)] == 0)
				{
					continue;
				}

				GLubyte vx = (GLubyte)x;
				GLubyte vy = (GLubyte)y;
				GLubyte vz = (GLubyte)z;

				// TODO:  Replace with chunk->colors values.
				GLubyte vr = 255;
				GLubyte vg = 255;
				GLubyte vb = 255;
				GLubyte va = 255;

				// FACE: -Y

				if (chunk->blocks[chunk_index_ex_get(vx, vy - 1, vz)] <= 0)
				{
					mesher.buffer[buffer_index++] = vx;
					mesher.buffer[buffer_index++] = vy;
					mesher.buffer[buffer_index++] = vz + 1;
					mesher.buffer[buffer_index++] = NORMAL_Y_NEG;
					mesher.buffer[buffer_index++] = vr;
					mesher.buffer[buffer_index++] = vg;
					mesher.buffer[buffer_index++] = vb;
					mesher.buffer[buffer_index++] = va;

					mesher.buffer[buffer_index++] = vx;
					mesher.buffer[buffer_index++] = vy;
					mesher.buffer[buffer_index++] = vz;
					mesher.buffer[buffer_index++] = NORMAL_Y_NEG;
					mesher.buffer[buffer_index++] = vr;
					mesher.buffer[buffer_index++] = vg;
					mesher.buffer[buffer_index++] = vb;
					mesher.buffer[buffer_index++] = va;

					mesher.buffer[buffer_index++] = vx + 1;
					mesher.buffer[buffer_index++] = vy;
					mesher.buffer[buffer_index++] = vz;
					mesher.buffer[buffer_index++] = NORMAL_Y_NEG;
					mesher.buffer[buffer_index++] = vr;
					mesher.buffer[buffer_index++] = vg;
					mesher.buffer[buffer_index++] = vb;
					mesher.buffer[buffer_index++] = va;

					mesher.buffer[buffer_index++] = vx + 1;
					mesher.buffer[buffer_index++] = vy;
					mesher.buffer[buffer_index++] = vz + 1;
					mesher.buffer[buffer_index++] = NORMAL_Y_NEG;
					mesher.buffer[buffer_index++] = vr;
					mesher.buffer[buffer_index++] = vg;
					mesher.buffer[buffer_index++] = vb;
					mesher.buffer[buffer_index++] = va;

					mesher.buffer[buffer_index++] = vx;
					mesher.buffer[buffer_index++] = vy;
					mesher.buffer[buffer_index++] = vz + 1;
					mesher.buffer[buffer_index++] = NORMAL_Y_NEG;
					mesher.buffer[buffer_index++] = vr;
					mesher.buffer[buffer_index++] = vg;
					mesher.buffer[buffer_index++] = vb;
					mesher.buffer[buffer_index++] = va;

					mesher.buffer[buffer_index++] = vx + 1;
					mesher.buffer[buffer_index++] = vy;
					mesher.buffer[buffer_index++] = vz;
					mesher.buffer[buffer_index++] = NORMAL_Y_NEG;
					mesher.buffer[buffer_index++] = vr;
					mesher.buffer[buffer_index++] = vg;
					mesher.buffer[buffer_index++] = vb;
					mesher.buffer[buffer_index++] = va;
				}

				// FACE: +Y

				if (chunk->blocks[chunk_index_ex_get(vx, vy + 1, vz)] <= 0)
				{
					mesher.buffer[buffer_index++] = vx;
					mesher.buffer[buffer_index++] = vy + 1;
					mesher.buffer[buffer_index++] = vz;
					mesher.buffer[buffer_index++] = NORMAL_Y_POS;
					mesher.buffer[buffer_index++] = vr;
					mesher.buffer[buffer_index++] = vg;
					mesher.buffer[buffer_index++] = vb;
					mesher.buffer[buffer_index++] = va;

					mesher.buffer[buffer_index++] = vx;
					mesher.buffer[buffer_index++] = vy + 1;
					mesher.buffer[buffer_index++] = vz + 1;
					mesher.buffer[buffer_index++] = NORMAL_Y_POS;
					mesher.buffer[buffer_index++] = vr;
					mesher.buffer[buffer_index++] = vg;
					mesher.buffer[buffer_index++] = vb;
					mesher.buffer[buffer_index++] = va;

					mesher.buffer[buffer_index++] = vx + 1;
					mesher.buffer[buffer_index++] = vy + 1;
					mesher.buffer[buffer_index++] = vz + 1;
					mesher.buffer[buffer_index++] = NORMAL_Y_POS;
					mesher.buffer[buffer_index++] = vr;
					mesher.buffer[buffer_index++] = vg;
					mesher.buffer[buffer_index++] = vb;
					mesher.buffer[buffer_index++] = va;

					mesher.buffer[buffer_index++] = vx + 1;
					mesher.buffer[buffer_index++] = vy + 1;
					mesher.buffer[buffer_index++] = vz;
					mesher.buffer[buffer_index++] = NORMAL_Y_POS;
					mesher.buffer[buffer_index++] = vr;
					mesher.buffer[buffer_index++] = vg;
					mesher.buffer[buffer_index++] = vb;
					mesher.buffer[buffer_index++] = va;

					mesher.buffer[buffer_index++] = vx;
					mesher.buffer[buffer_index++] = vy + 1;
					mesher.buffer[buffer_index++] = vz;
					mesher.buffer[buffer_index++] = NORMAL_Y_POS;
					mesher.buffer[buffer_index++] = vr;
					mesher.buffer[buffer_index++] = vg;
					mesher.buffer[buffer_index++] = vb;
					mesher.buffer[buffer_index++] = va;

					mesher.buffer[buffer_index++] = vx + 1;
					mesher.buffer[buffer_index++] = vy + 1;
					mesher.buffer[buffer_index++] = vz + 1;
					mesher.buffer[buffer_index++] = NORMAL_Y_POS;
					mesher.buffer[buffer_index++] = vr;
					mesher.buffer[buffer_index++] = vg;
					mesher.buffer[buffer_index++] = vb;
					mesher.buffer[buffer_index++] = va;
				}

				// FACE: -Z

				if (chunk->blocks[chunk_index_ex_get(vx, vy, vz - 1)] <= 0)
				{
					mesher.buffer[buffer_index++] = vx;
					mesher.buffer[buffer_index++] = vy;
					mesher.buffer[buffer_index++] = vz;
					mesher.buffer[buffer_index++] = NORMAL_Z_NEG;
					mesher.buffer[buffer_index++] = vr;
					mesher.buffer[buffer_index++] = vg;
					mesher.buffer[buffer_index++] = vb;
					mesher.buffer[buffer_index++] = va;

					mesher.buffer[buffer_index++] = vx;
					mesher.buffer[buffer_index++] = vy + 1;
					mesher.buffer[buffer_index++] = vz;
					mesher.buffer[buffer_index++] = NORMAL_Z_NEG;
					mesher.buffer[buffer_index++] = vr;
					mesher.buffer[buffer_index++] = vg;
					mesher.buffer[buffer_index++] = vb;
					mesher.buffer[buffer_index++] = va;

					mesher.buffer[buffer_index++] = vx + 1;
					mesher.buffer[buffer_index++] = vy + 1;
					mesher.buffer[buffer_index++] = vz;
					mesher.buffer[buffer_index++] = NORMAL_Z_NEG;
					mesher.buffer[buffer_index++] = vr;
					mesher.buffer[buffer_index++] = vg;
					mesher.buffer[buffer_index++] = vb;
					mesher.buffer[buffer_index++] = va;

					mesher.buffer[buffer_index++] = vx + 1;
					mesher.buffer[buffer_index++] = vy;
					mesher.buffer[buffer_index++] = vz;
					mesher.buffer[buffer_index++] = NORMAL_Z_NEG;
					mesher.buffer[buffer_index++] = vr;
					mesher.buffer[buffer_index++] = vg;
					mesher.buffer[buffer_index++] = vb;
					mesher.buffer[buffer_index++] = va;

					mesher.buffer[buffer_index++] = vx;
					mesher.buffer[buffer_index++] = vy;
					mesher.buffer[buffer_index++] = vz;
					mesher.buffer[buffer_index++] = NORMAL_Z_NEG;
					mesher.buffer[buffer_index++] = vr;
					mesher.buffer[buffer_index++] = vg;
					mesher.buffer[buffer_index++] = vb;
					mesher.buffer[buffer_index++] = va;

					mesher.buffer[buffer_index++] = vx + 1;
					mesher.buffer[buffer_index++] = vy + 1;
					mesher.buffer[buffer_index++] = vz;
					mesher.buffer[buffer_index++] = NORMAL_Z_NEG;
					mesher.buffer[buffer_index++] = vr;
					mesher.buffer[buffer_index++] = vg;
					mesher.buffer[buffer_index++] = vb;
					mesher.buffer[buffer_index++] = va;
				}

				// FACE: +Z

				if (chunk->blocks[chunk_index_ex_get(vx, vy, vz + 1)] <= 0)
				{
					mesher.buffer[buffer_index++] = vx + 1;
					mesher.buffer[buffer_index++] = vy;
					mesher.buffer[buffer_index++] = vz + 1;
					mesher.buffer[buffer_index++] = NORMAL_Z_POS;
					mesher.buffer[buffer_index++] = vr;
					mesher.buffer[buffer_index++] = vg;
					mesher.buffer[buffer_index++] = vb;
					mesher.buffer[buffer_index++] = va;

					mesher.buffer[buffer_index++] = vx + 1;
					mesher.buffer[buffer_index++] = vy + 1;
					mesher.buffer[buffer_index++] = vz + 1;
					mesher.buffer[buffer_index++] = NORMAL_Z_POS;
					mesher.buffer[buffer_index++] = vr;
					mesher.buffer[buffer_index++] = vg;
					mesher.buffer[buffer_index++] = vb;
					mesher.buffer[buffer_index++] = va;

					mesher.buffer[buffer_index++] = vx;
					mesher.buffer[buffer_index++] = vy + 1;
					mesher.buffer[buffer_index++] = vz + 1;
					mesher.buffer[buffer_index++] = NORMAL_Z_POS;
					mesher.buffer[buffer_index++] = vr;
					mesher.buffer[buffer_index++] = vg;
					mesher.buffer[buffer_index++] = vb;
					mesher.buffer[buffer_index++] = va;

					mesher.buffer[buffer_index++] = vx;
					mesher.buffer[buffer_index++] = vy;
					mesher.buffer[buffer_index++] = vz + 1;
					mesher.buffer[buffer_index++] = NORMAL_Z_POS;
					mesher.buffer[buffer_index++] = vr;
					mesher.buffer[buffer_index++] = vg;
					mesher.buffer[buffer_index++] = vb;
					mesher.buffer[buffer_index++] = va;

					mesher.buffer[buffer_index++] = vx + 1;
					mesher.buffer[buffer_index++] = vy;
					mesher.buffer[buffer_index++] = vz + 1;
					mesher.buffer[buffer_index++] = NORMAL_Z_POS;
					mesher.buffer[buffer_index++] = vr;
					mesher.buffer[buffer_index++] = vg;
					mesher.buffer[buffer_index++] = vb;
					mesher.buffer[buffer_index++] = va;

					mesher.buffer[buffer_index++] = vx;
					mesher.buffer[buffer_index++] = vy + 1;
					mesher.buffer[buffer_index++] = vz + 1;
					mesher.buffer[buffer_index++] = NORMAL_Z_POS;
					mesher.buffer[buffer_index++] = vr;
					mesher.buffer[buffer_index++] = vg;
					mesher.buffer[buffer_index++] = vb;
					mesher.buffer[buffer_index++] = va;
				}

				// FACE: -X

				if (chunk->blocks[chunk_index_ex_get(vx - 1, vy, vz)] <= 0)
				{
					mesher.buffer[buffer_index++] = vx;
					mesher.buffer[buffer_index++] = vy;
					mesher.buffer[buffer_index++] = vz + 1;
					mesher.buffer[buffer_index++] = NORMAL_X_NEG;
					mesher.buffer[buffer_index++] = vr;
					mesher.buffer[buffer_index++] = vg;
					mesher.buffer[buffer_index++] = vb;
					mesher.buffer[buffer_index++] = va;

					mesher.buffer[buffer_index++] = vx;
					mesher.buffer[buffer_index++] = vy + 1;
					mesher.buffer[buffer_index++] = vz + 1;
					mesher.buffer[buffer_index++] = NORMAL_X_NEG;
					mesher.buffer[buffer_index++] = vr;
					mesher.buffer[buffer_index++] = vg;
					mesher.buffer[buffer_index++] = vb;
					mesher.buffer[buffer_index++] = va;

					mesher.buffer[buffer_index++] = vx;
					mesher.buffer[buffer_index++] = vy + 1;
					mesher.buffer[buffer_index++] = vz;
					mesher.buffer[buffer_index++] = NORMAL_X_NEG;
					mesher.buffer[buffer_index++] = vr;
					mesher.buffer[buffer_index++] = vg;
					mesher.buffer[buffer_index++] = vb;
					mesher.buffer[buffer_index++] = va;

					mesher.buffer[buffer_index++] = vx;
					mesher.buffer[buffer_index++] = vy;
					mesher.buffer[buffer_index++] = vz;
					mesher.buffer[buffer_index++] = NORMAL_X_NEG;
					mesher.buffer[buffer_index++] = vr;
					mesher.buffer[buffer_index++] = vg;
					mesher.buffer[buffer_index++] = vb;
					mesher.buffer[buffer_index++] = va;

					mesher.buffer[buffer_index++] = vx;
					mesher.buffer[buffer_index++] = vy;
					mesher.buffer[buffer_index++] = vz + 1;
					mesher.buffer[buffer_index++] = NORMAL_X_NEG;
					mesher.buffer[buffer_index++] = vr;
					mesher.buffer[buffer_index++] = vg;
					mesher.buffer[buffer_index++] = vb;
					mesher.buffer[buffer_index++] = va;

					mesher.buffer[buffer_index++] = vx;
					mesher.buffer[buffer_index++] = vy + 1;
					mesher.buffer[buffer_index++] = vz;
					mesher.buffer[buffer_index++] = NORMAL_X_NEG;
					mesher.buffer[buffer_index++] = vr;
					mesher.buffer[buffer_index++] = vg;
					mesher.buffer[buffer_index++] = vb;
					mesher.buffer[buffer_index++] = va;
				}

				// FACE: +X

				if (chunk->blocks[chunk_index_ex_get(vx + 1, vy, vz)] <= 0)
				{
					mesher.buffer[buffer_index++] = vx + 1;
					mesher.buffer[buffer_index++] = vy;
					mesher.buffer[buffer_index++] = vz;
					mesher.buffer[buffer_index++] = NORMAL_X_POS;
					mesher.buffer[buffer_index++] = vr;
					mesher.buffer[buffer_index++] = vg;
					mesher.buffer[buffer_index++] = vb;
					mesher.buffer[buffer_index++] = va;

					mesher.buffer[buffer_index++] = vx + 1;
					mesher.buffer[buffer_index++] = vy + 1;
					mesher.buffer[buffer_index++] = vz;
					mesher.buffer[buffer_index++] = NORMAL_X_POS;
					mesher.buffer[buffer_index++] = vr;
					mesher.buffer[buffer_index++] = vg;
					mesher.buffer[buffer_index++] = vb;
					mesher.buffer[buffer_index++] = va;

					mesher.buffer[buffer_index++] = vx + 1;
					mesher.buffer[buffer_index++] = vy + 1;
					mesher.buffer[buffer_index++] = vz + 1;
					mesher.buffer[buffer_index++] = NORMAL_X_POS;
					mesher.buffer[buffer_index++] = vr;
					mesher.buffer[buffer_index++] = vg;
					mesher.buffer[buffer_index++] = vb;
					mesher.buffer[buffer_index++] = va;

					mesher.buffer[buffer_index++] = vx + 1;
					mesher.buffer[buffer_index++] = vy;
					mesher.buffer[buffer_index++] = vz + 1;
					mesher.buffer[buffer_index++] = NORMAL_X_POS;
					mesher.buffer[buffer_index++] = vr;
					mesher.buffer[buffer_index++] = vg;
					mesher.buffer[buffer_index++] = vb;
					mesher.buffer[buffer_index++] = va;

					mesher.buffer[buffer_index++] = vx + 1;
					mesher.buffer[buffer_index++] = vy;
					mesher.buffer[buffer_index++] = vz;
					mesher.buffer[buffer_index++] = NORMAL_X_POS;
					mesher.buffer[buffer_index++] = vr;
					mesher.buffer[buffer_index++] = vg;
					mesher.buffer[buffer_index++] = vb;
					mesher.buffer[buffer_index++] = va;

					mesher.buffer[buffer_index++] = vx + 1;
					mesher.buffer[buffer_index++] = vy + 1;
					mesher.buffer[buffer_index++] = vz + 1;
					mesher.buffer[buffer_index++] = NORMAL_X_POS;
					mesher.buffer[buffer_index++] = vr;
					mesher.buffer[buffer_index++] = vg;
					mesher.buffer[buffer_index++] = vb;
					mesher.buffer[buffer_index++] = va;
				}

			} // x
		} // z
	} // y

	if (buffer_index == 0)
	{
		chunk->mesh = NULL;
	}
	else
	{
		struct chunk_mesh* mesh = ringbuffer_copy_into(mesher.buffer, buffer_index);

		if (mesh == NULL)
		{
			log_warning("Meshing generated a NULL mesh.");
		}
		else
		{
			mesh->release = mesher_release_mesh;
		}

		chunk->mesh = mesh;
	}

	world_add_chunk(chunk);
}

static int mesher_loop(void* arg)
{
	window_claim_offscreen_context();

	while (true)
	{
		mtx_lock(&mesher.mutex_chunks);

		if (mesher.running == false)
		{
			// Stop the thread.
			mtx_unlock(&mesher.mutex_chunks);

			break;
		}

		struct chunk* chunk = (struct chunk*) queue_pop(&mesher.chunks);

		mtx_unlock(&mesher.mutex_chunks);

		if (chunk == NULL)
		{
			//thread_sleep(100000000);
			continue;
		}
		
		// Mesh the chunk.
		mesher_mesh(chunk);
	}

	return 0;
}

bool mesher_start_thread(void)
{
	// ---------------- Mesher Data Initialization ---------------- //
	queue_init(&mesher.chunks, MESHER_CHUNK_CAPACITY);

	queue_init(&mesher.mesh_queue, MESHER_MESH_CAPACITY);

	stack_init(&mesher.mesh_stack, MESHER_MESH_CAPACITY);

	mesher.mesh_buffer = malloc(MESHER_MESH_CAPACITY * sizeof(struct chunk_mesh));
	check_allocation(mesher.mesh_buffer);

	mesher.buffer = malloc(MESHER_BUFFER_LENGTH * sizeof(GLubyte));
	check_allocation(mesher.buffer);

	for (int i = 0; i < MESHER_MESH_CAPACITY; i++)
	{
		if (stack_push(&mesher.mesh_stack, &mesher.mesh_buffer[i]) == false)
		{
			log_error_exit("Mesher mesh stack overflow.");
		}
	}

	// ---------------- Threading ---------------- //
	mesher.running = true;

	if (mtx_init(&mesher.mutex_meshes, mtx_plain) == thrd_error)
	{
		return false;
	}

	if (mtx_init(&mesher.mutex_chunks, mtx_plain) == thrd_error)
	{
		return false;
	}
	
	if (thrd_create(&mesher.thread, mesher_loop, NULL) == thrd_error)
	{
		return false;
	}

	return true;
}

void mesher_setup_opengl_buffer(void)
{
	glGenBuffers(1, &mesher.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesher.vbo);

	GLbitfield flags = 0;
	flags |= GL_MAP_WRITE_BIT;
	flags |= GL_MAP_PERSISTENT_BIT;

	// The following flags cause a GL_INVALID_VALUE error when passed to glBufferStorage()
	// They must be set AFTER the glBufferStorage() call, NOT before.
	//flags |= GL_MAP_FLUSH_EXPLICIT_BIT;
	//flags |= GL_MAP_UNSYNCHRONIZED_BIT;

	glBufferStorage(GL_ARRAY_BUFFER, MESHER_VBO_LENGTH, NULL, flags);

	flags |= GL_MAP_FLUSH_EXPLICIT_BIT;
	flags |= GL_MAP_UNSYNCHRONIZED_BIT;

	mesher.ringbuffer.front = glMapBufferRange(GL_ARRAY_BUFFER, 0, MESHER_VBO_LENGTH, flags);

	glVertexAttribIPointer(0, 4, GL_UNSIGNED_BYTE, 8, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 8, (GLvoid*)4);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	mesher.ringbuffer.back = mesher.ringbuffer.front + MESHER_VBO_LENGTH;
	mesher.ringbuffer.head = mesher.ringbuffer.front;
	mesher.ringbuffer.tail = mesher.ringbuffer.front;
	mesher.ringbuffer.length = MESHER_VBO_LENGTH;
	mesher.ringbuffer.capacity = MESHER_VBO_LENGTH - 1;
}

void mesher_stop_thread(void)
{
	mtx_lock(&mesher.mutex_chunks);

	mesher.running = false;

	mtx_unlock(&mesher.mutex_chunks);
}

void mesher_queue_work(struct chunk* chunk)
{
	mtx_lock(&mesher.mutex_chunks);

	while (queue_push(&mesher.chunks, chunk) == false)
	{
		// Work queue is full, sleep for 0.1ms.
		mtx_unlock(&mesher.mutex_chunks);

		thread_sleep(100000);

		mtx_lock(&mesher.mutex_chunks);
	}

	mtx_unlock(&mesher.mutex_chunks);
}
