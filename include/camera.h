#pragma once

#include "GLKMath.h"

#define CAMERA_MOVEMENT_SPEED 2.0f
#define CAMERA_STRAFE_SPEED CAMERA_MOVEMENT_SPEED
#define CAMERA_RISE_SPEED CAMERA_MOVEMENT_SPEED
#define CAMERA_SINK_SPEED CAMERA_RISE_SPEED
#define CAMERA_HORIZONTAL_SENSITIVITY 800.0f

struct
{
	GLKVector3 position;
	GLKVector3 target;
	GLKVector3 up;

	GLKVector3 direction;

	float rotation_y;
	float rotation_x;
} Camera;

void camera_init();

void camera_update();

void camera_look_through(GLKMatrix4* view);
