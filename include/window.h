#pragma once

#include <stdbool.h>

#define CGRADE_WINDOW_WIDTH 1280
#define CGRADE_WINDOW_HEIGHT 720
#define CGRADE_WINDOW_TITLE "Voxel"

#define CGRADE_WINDOW_OPENGL_MAJOR 4
#define CGRADE_WINDOW_OPENGL_MINOR 3

// Copy of GLFW_RELEASE and GLFW_PRESS
#define WINDOW_RELEASE 0
#define WINDOW_PRESS 1

bool window_initialize(void);

void window_swap_buffers(void);

bool window_should_close(void);

void window_poll_input(void);

int window_width(void);

int window_height(void);

float window_aspect(void);

void window_claim_offscreen_context(void);

void window_cursor_get(double* x, double* y);

void window_cursor_set(double x, double y);

void window_cursor_hide(void);

void window_cursor_show(void);

int window_mouse_button(int button);

int window_keyboard_key(int id);
