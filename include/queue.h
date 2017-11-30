#pragma once

#include <stdbool.h>
#include <stdlib.h>

struct queue
{
	size_t capacity;
	size_t count;
	size_t front;
	size_t back;
	void** elements;
};

void queue_init(struct queue* queue, size_t capacity);

void queue_free(struct queue* queue);

bool queue_push(struct queue* queue, void* element);

void* queue_pop(struct queue* queue);

void* queue_peek(struct queue* queue);
