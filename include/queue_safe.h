#pragma once

#include "tinycthread.h"

#include <stdbool.h>
#include <stdlib.h>

struct queue_safe
{
	mtx_t mutex;

	size_t capacity;
	size_t count;
	size_t front;
	size_t back;
	void** elements;
};

void queue_safe_init(struct queue_safe* queue, size_t capacity);

void queue_safe_free(struct queue_safe* queue);

bool queue_safe_push(struct queue_safe* queue, void* element);

void* queue_safe_pop(struct queue_safe* queue);
