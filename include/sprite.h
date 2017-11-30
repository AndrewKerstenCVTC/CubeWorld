#pragma once

#include "GLKMath.h"

#include "texture.h"

struct sprite
{
	GLKVector3 size;
	GLKVector3 position;
	TEXTURE texture;
};

struct sprite sprite_create(float x, float y, float width, float height, TEXTURE texture);
