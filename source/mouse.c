#include "mouse.h"

#include <string.h>

#include "window.h"

static struct mouse mouse = { 0 };

void mouse_init(void)
{
	mouse.position.x = 0;
	mouse.position.y = 0;

	mouse.position_old.x = 0;
	mouse.position_old.y = 0;

	memset(mouse.buttons, 0, sizeof(struct keystate) * MOUSE_BUTTON_COUNT);
}

void mouse_update(void)
{
	mouse.position_old.x = mouse.position.x;
	mouse.position_old.y = mouse.position.y;

	double current_x = 0;
	double current_y = 0;

	window_cursor_get(&current_x, &current_y);

	mouse.position.x = (float)current_x;
	mouse.position.y = (float)current_y;

	for (int i = 0; i < MOUSE_BUTTON_COUNT; i++)
	{
		if (window_mouse_button(i) == WINDOW_PRESS)
		{
			mouse.buttons[i].down = true;
			mouse.buttons[i].ticks_held++;
		}
		else
		{
			mouse.buttons[i].down = false;
			mouse.buttons[i].ticks_held = 0;
		}
	}
}

float mouse_x(void)
{
	return mouse.position.x;
}

float mouse_y(void)
{
	return mouse.position.y;
}
