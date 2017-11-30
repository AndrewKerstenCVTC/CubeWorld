#pragma once

#include <stdbool.h>
#include <stdlib.h>

struct stack
{
	void** elements;
	size_t capacity;
	size_t count;
};

void stack_init(struct stack* stack, size_t capacity);

void stack_free(struct stack* stack);

bool stack_push(struct stack* stack, void* element);

void* stack_pop(struct stack* stack);
