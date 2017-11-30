#pragma once

#include "chunk.h"

#include <stdbool.h>

bool generator_start_thread(void);

void generator_stop_thread(void);

void generator_queue_work(struct chunk* chunk);
