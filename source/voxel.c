#include <stdio.h>

#include "window.h"
#include "renderer.h"
#include "mouse.h"
#include "keyboard.h"
#include "generator.h"
#include "mesher.h"
#include "world.h"
#include "utility.h"

#include <stdlib.h>

void voxel_main_loop(void)
{
	while (window_should_close() == false)
	{
		window_poll_input();

		mouse_update();
		keyboard_update();

		if (keyboard_key(GLFW_KEY_ESCAPE).down == GLFW_PRESS)
		{
			break;
		}

		renderer_update();

		world_tick();

		renderer_render();
		window_swap_buffers();

		log_opengl_errors();
	}
}

int main(int argc, char** argv)
{
	if (window_initialize() == false)
	{
		return -1;
	}

	mouse_init();
	keyboard_init();

	if (renderer_initialize() == false)
	{
		return -1;
	}

	log_opengl_errors();

	if (mesher_start_thread() == false)
	{
		return -1;
	}

	if (generator_start_thread() == false)
	{
		return -1;
	}

	world_init();

	voxel_main_loop();

	generator_stop_thread();
	mesher_stop_thread();

	return 0;
}
