#pragma once

#include <GL/glew.h>

#define TEXTURE_ASSET_DIRECTORY "assets/textures/"
#define TEXTURE_NULL 0

typedef GLuint TEXTURE;

TEXTURE texture_load(const char* path);

TEXTURE texture_load_spine(const char* path, int* width, int* height);

void texture_use(TEXTURE texture);
