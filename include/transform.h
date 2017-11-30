#pragma once

#include "GLKMath.h"

struct transform
{
	GLKVector3 translation;
	GLKVector3 scale;

	GLKMatrix4 matrix;
};

void transform_init(struct transform* transform);

void transform_calc(struct transform* transform);
