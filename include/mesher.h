#pragma once

#include "chunk.h"

#include <GL/glew.h>

#include <stdbool.h>

bool mesher_start_thread(void);

void mesher_stop_thread(void);

void mesher_queue_work(struct chunk* chunk);

// TODO:  Now called using function ptr.  Remove me.
void mesher_release_mesh(struct chunk* chunk);

//GLuint mesher_vao_get(void);

void mesher_setup_opengl_buffer(void);
