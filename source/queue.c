#include "queue.h"

#include "utility.h"

void queue_init(struct queue* queue, size_t capacity)
{
	queue->capacity = capacity;
	queue->count = 0;
	queue->front = 0;
	queue->back = 0;

	queue->elements = malloc(sizeof(void*) * capacity);

	if (queue->elements == NULL)
	{
		allocation_failure();
	}
}

void queue_free(struct queue* queue)
{
	queue->capacity = 0;
	queue->count = 0;
	queue->front = 0;
	queue->back = 0;

	free(queue->elements);
}

bool queue_push(struct queue* queue, void* element)
{
	if (queue->count >= queue->capacity)
	{
		return false;
	}

	queue->elements[queue->back] = element;

	queue->count++;
	queue->back++;

	if (queue->back >= queue->capacity)
	{
		queue->back = 0;
	}

	return true;
}

void* queue_pop(struct queue* queue)
{
	if (queue->count == 0)
	{
		return NULL;
	}

	void* result = queue->elements[queue->front];

	queue->count--;
	queue->front++;

	if (queue->front >= queue->capacity)
	{
		queue->front = 0;
	}

	return result;
}

void* queue_peek(struct queue* queue)
{
	if (queue->count == 0)
	{
		return NULL;
	}

	return queue->elements[queue->front];
}
