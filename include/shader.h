#pragma once

#include <GL/glew.h>

#define SHADER_ASSET_DIRECTORY "assets/shaders/"
#define SHADER_NULL 0

typedef GLuint SHADER;

SHADER shader_create(const char* vert_path, const char* frag_path);

void shader_use(SHADER shader);
