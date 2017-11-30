#pragma once

#include <stdbool.h>
#include <stdlib.h>

typedef int BITSET_BLOCK;

#define BITSET_BLOCK_SIZE (sizeof(BITSET_BLOCK) * 8)

struct bitset
{
	BITSET_BLOCK* buffer;
	size_t capacity;
};

void bitset_init(struct bitset* bitset, size_t capacity);

void bitset_free(struct bitset* bitset);

void bitset_zero(struct bitset* bitset);

void bitset_bit_set(struct bitset* bitset, int index);

void bitset_bit_clear(struct bitset* bitset, int index);

bool bitset_bit_get(struct bitset* bitset, int index);
