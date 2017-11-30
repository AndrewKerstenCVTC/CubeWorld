#pragma once

#include <stdbool.h>

#include "sprite.h"

bool renderer_initialize(void);

void renderer_render(void);

void renderer_update(void);

void render_chunk(struct chunk* chunk);
