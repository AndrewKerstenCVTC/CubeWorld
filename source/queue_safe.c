#include "queue_safe.h"

#include "utility.h"

void queue_safe_init(struct queue_safe* queue, size_t capacity)
{
	queue->capacity = capacity;
	queue->count = 0;
	queue->front = 0;
	queue->back = 0;

	queue->elements = malloc(sizeof(void*) * capacity);
	check_allocation(queue->elements);

	if (mtx_init(&queue->mutex, mtx_plain) == thrd_error)
	{
		log_error_exit("Mutex creation failed.");
	}
}

void queue_safe_free(struct queue_safe* queue)
{
	queue->capacity = 0;
	queue->count = 0;
	queue->front = 0;
	queue->back = 0;

	free(queue->elements);

	mtx_destroy(&queue->mutex);
}

bool queue_safe_push(struct queue_safe* queue, void* element)
{
	mtx_lock(&queue->mutex);

	if (queue->count >= queue->capacity)
	{
		mtx_unlock(&queue->mutex);

		return false;
	}

	queue->elements[queue->back] = element;

	queue->count++;
	queue->back++;

	if (queue->back >= queue->capacity)
	{
		queue->back = 0;
	}

	mtx_unlock(&queue->mutex);

	return true;
}

void* queue_safe_pop(struct queue_safe* queue)
{
	mtx_lock(&queue->mutex);

	if (queue->count == 0)
	{
		mtx_unlock(&queue->mutex);

		return NULL;
	}

	void* result = queue->elements[queue->front];

	queue->count--;
	queue->front++;

	if (queue->front >= queue->capacity)
	{
		queue->front = 0;
	}

	mtx_unlock(&queue->mutex);

	return result;
}
