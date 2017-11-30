#include "transform.h"

void transform_init(struct transform* transform)
{
	transform->translation = GLKVector3Make(0.0, 0.0, 0.0);
	transform->scale = GLKVector3Make(1.0, 1.0, 1.0);

	transform->matrix = GLKMatrix4Identity;

	transform_calc(transform);
}

void transform_calc(struct transform* transform)
{
	// 1. Translate
	// 2. Rotate
	// 3. Scale

	transform->matrix = GLKMatrix4TranslateWithVector3(GLKMatrix4Identity, transform->translation);
	transform->matrix = GLKMatrix4ScaleWithVector3(transform->matrix, transform->scale);
}
