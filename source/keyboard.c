#include "keyboard.h"

#include <string.h>

#include "window.h"

static struct keyboard keyboard = { 0 };

void keyboard_init(void)
{
	memset(keyboard.keys, 0, sizeof(struct keystate) * KEYBOARD_KEY_COUNT);
}

void keyboard_update(void)
{
	for (int i = 0; i < KEYBOARD_KEY_COUNT; i++)
	{
		if (keyboard.keys[i].released == true)
		{
			keyboard.keys[i].released = false;
		}

		if (window_keyboard_key(i) == WINDOW_PRESS)
		{
			keyboard.keys[i].down = true;
			keyboard.keys[i].ticks_held++;
		}
		else
		{
			if (keyboard.keys[i].down == true)
			{
				keyboard.keys[i].released = true;
			}

			keyboard.keys[i].down = false;
			keyboard.keys[i].ticks_held = 0;
		}
	}
}

struct keystate keyboard_key(int id)
{
	return keyboard.keys[id];
}
