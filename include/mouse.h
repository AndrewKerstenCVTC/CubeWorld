#pragma once

#include "keystate.h"

#define MOUSE_BUTTON_COUNT 3

struct mouse
{
	struct
	{
		float x;
		float y;
	} position;

	struct
	{
		float x;
		float y;
	} position_old;

	struct keystate buttons[MOUSE_BUTTON_COUNT];
};

void mouse_init(void);

void mouse_update(void);

float mouse_x(void);

float mouse_y(void);
