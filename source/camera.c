#include "camera.h"

#include "keyboard.h"
#include "mouse.h"
#include "window.h"

void camera_init()
{
	Camera.up = GLKVector3Make(0.0f, 1.0f, 0.0f);
	Camera.position = GLKVector3Make(0.0f, 0.0f, 0.0f);
	Camera.direction = GLKVector3Make(0.0f, 0.0f, -1.0f);
	Camera.target = GLKVector3Add(Camera.position, Camera.direction);

	Camera.rotation_y = 0.0f;
	Camera.rotation_x = 0.0f;

	window_cursor_set(window_width() / 2, window_height() / 2);
	window_cursor_hide();
}

void camera_update()
{
	float delta_x = mouse_x() - (window_width() / 2);
	float delta_y = mouse_y() - (window_height() / 2);

	Camera.rotation_y -= (delta_x / CAMERA_HORIZONTAL_SENSITIVITY);
	Camera.rotation_x -= (delta_y / CAMERA_HORIZONTAL_SENSITIVITY);

	if (Camera.rotation_x > (M_PI / 2))
	{
		Camera.rotation_x = (M_PI / 2);
	}
	else if (Camera.rotation_x < -(M_PI / 2))
	{
		Camera.rotation_x = -(M_PI / 2);
	}

	Camera.direction.x = sinf(Camera.rotation_y);
	Camera.direction.z = cosf(Camera.rotation_y);

	Camera.direction.y = sinf(Camera.rotation_x);

	window_cursor_set(window_width() / 2, window_height() / 2);

	Camera.target = GLKVector3Add(Camera.position, Camera.direction);

	if (keyboard_key(GLFW_KEY_W).down)
	{
		GLKVector3 distance = GLKVector3MultiplyScalar(Camera.direction, CAMERA_MOVEMENT_SPEED);
		Camera.position = GLKVector3Add(Camera.position, distance);
		Camera.target = GLKVector3Add(Camera.position, Camera.direction);
	}

	if (keyboard_key(GLFW_KEY_S).down)
	{
		GLKVector3 distance = GLKVector3MultiplyScalar(Camera.direction, -CAMERA_MOVEMENT_SPEED);
		Camera.position = GLKVector3Add(Camera.position, distance);
		Camera.target = GLKVector3Add(Camera.position, Camera.direction);
	}

	if (keyboard_key(GLFW_KEY_A).down)
	{
		GLKVector3 delta = GLKVector3CrossProduct(Camera.up, Camera.direction);
		delta = GLKVector3Normalize(delta);
		delta = GLKVector3MultiplyScalar(delta, CAMERA_STRAFE_SPEED);

		Camera.position = GLKVector3Add(Camera.position, delta);
		Camera.target = GLKVector3Add(Camera.position, Camera.direction);
	}

	if (keyboard_key(GLFW_KEY_D).down)
	{
		GLKVector3 delta = GLKVector3CrossProduct(Camera.direction, Camera.up);
		delta = GLKVector3Normalize(delta);
		delta = GLKVector3MultiplyScalar(delta, CAMERA_STRAFE_SPEED);

		Camera.position = GLKVector3Add(Camera.position, delta);
		Camera.target = GLKVector3Add(Camera.position, Camera.direction);
	}

	if (keyboard_key(GLFW_KEY_SPACE).down)
	{
		GLKVector3 distance = GLKVector3MultiplyScalar(GLKVector3Make(0.0f, 1.0f, 0.0f), CAMERA_RISE_SPEED);
		Camera.position = GLKVector3Add(Camera.position, distance);
		Camera.target = GLKVector3Add(Camera.position, Camera.direction);
	}

	if (keyboard_key(GLFW_KEY_LEFT_SHIFT).down)
	{
		GLKVector3 distance = GLKVector3MultiplyScalar(GLKVector3Make(0.0f, 1.0f, 0.0f), -CAMERA_SINK_SPEED);
		Camera.position = GLKVector3Add(Camera.position, distance);
		Camera.target = GLKVector3Add(Camera.position, Camera.direction);
	}

	Camera.direction = GLKVector3Normalize(Camera.direction);
	GLKVector3 right = GLKVector3CrossProduct(Camera.direction, GLKVector3Make(0.0f, 1.0f, 0.0f));
	Camera.up = GLKVector3CrossProduct(right, Camera.direction);
}

void camera_look_through(GLKMatrix4* view)
{
	*view = GLKMatrix4MakeLookAt(
		Camera.position.x,
		Camera.position.y,
		Camera.position.z,

		Camera.target.x,
		Camera.target.y,
		Camera.target.z,

		Camera.up.x,
		Camera.up.y,
		Camera.up.z
	);
}
