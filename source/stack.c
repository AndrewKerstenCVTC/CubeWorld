#include "stack.h"

#include "utility.h"

void stack_init(struct stack* stack, size_t capacity)
{
	stack->elements = malloc(capacity * sizeof(void*));
	check_allocation(stack->elements);
	stack->capacity = capacity;
	stack->count = 0;
}

void stack_free(struct stack* stack)
{
	free(stack->elements);
	stack->capacity = 0;
	stack->count = 0;
}

bool stack_push(struct stack* stack, void* element)
{
	if (stack->count >= stack->capacity)
	{
		return false;
	}

	stack->elements[stack->count++] = element;

	return true;
}

void* stack_pop(struct stack* stack)
{
	if (stack->count == 0)
	{
		return NULL;
	}

	stack->count--;

	return stack->elements[stack->count];
}
