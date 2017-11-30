#include "bitset.h"

#include "utility.h"

#include <string.h>

void bitset_init(struct bitset* bitset, size_t capacity)
{
	bitset->capacity = capacity;

	size_t num_blocks = (capacity + BITSET_BLOCK_SIZE) / BITSET_BLOCK_SIZE;

	bitset->buffer = calloc(num_blocks, sizeof(BITSET_BLOCK));

	if (bitset->buffer == NULL)
	{
		allocation_failure();
	}
}

void bitset_free(struct bitset* bitset)
{
	free(bitset->buffer);
}

void bitset_zero(struct bitset* bitset)
{
	size_t num_blocks = (bitset->capacity + BITSET_BLOCK_SIZE) / BITSET_BLOCK_SIZE;
	memset(bitset->buffer, 0, num_blocks);
}

void bitset_bit_set(struct bitset* bitset, int index)
{
	int element = index / BITSET_BLOCK_SIZE;
	int offset = index % BITSET_BLOCK_SIZE;

	bitset->buffer[element] |= (1 << offset);
}

void bitset_bit_clear(struct bitset* bitset, int index)
{
	int element = index / BITSET_BLOCK_SIZE;
	int offset = index % BITSET_BLOCK_SIZE;

	bitset->buffer[element] &= ~(1 << offset);
}

bool bitset_bit_get(struct bitset* bitset, int index)
{
	int element = index / BITSET_BLOCK_SIZE;
	int offset = index % BITSET_BLOCK_SIZE;

	return bitset->buffer[element] & (1 << offset);
}
