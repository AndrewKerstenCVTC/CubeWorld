#include "keystate.h"

#include <GLFW/glfw3.h>

#define KEYBOARD_KEY_COUNT 350

struct keyboard
{
	struct keystate keys[KEYBOARD_KEY_COUNT];
};

void keyboard_init(void);

void keyboard_update(void);

struct keystate keyboard_key(int id);
