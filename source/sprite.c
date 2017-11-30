#include "sprite.h"

struct sprite sprite_create(float x, float y, float width, float height, TEXTURE texture)
{
	struct sprite sprite = { 0 };

	sprite.position = GLKVector3Make(x, y, 0);
	sprite.size = GLKVector3Make(width, height, 0);
	sprite.texture = texture;

	return sprite;
}
