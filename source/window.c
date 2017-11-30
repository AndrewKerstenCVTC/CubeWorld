#include "window.h"

#include <GLFW/glfw3.h>
#include <stdio.h>

struct window
{
	GLFWwindow* window;
	int width;
	int height;

	GLFWwindow* offscreen_context;
};

static struct window Window = { 0 };

bool window_initialize(void)
{
	Window.width = CGRADE_WINDOW_WIDTH;
	Window.height = CGRADE_WINDOW_HEIGHT;

	printf("Window resolution: %d x %d\n", Window.width, Window.height);

	if (glfwInit() == false)
	{
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, CGRADE_WINDOW_OPENGL_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, CGRADE_WINDOW_OPENGL_MINOR);
	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	Window.window = glfwCreateWindow(Window.width, Window.height, CGRADE_WINDOW_TITLE, NULL, NULL);

	if (Window.window == NULL)
	{
		return false;
	}

	const GLFWvidmode* monitor = glfwGetVideoMode(glfwGetPrimaryMonitor());

	glfwSetWindowPos(Window.window, monitor->width / 2 - Window.width / 2, monitor->height / 2 - Window.height / 2);

	glfwMakeContextCurrent(Window.window);


	// Create the offscreen context.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, CGRADE_WINDOW_OPENGL_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, CGRADE_WINDOW_OPENGL_MINOR);
	glfwWindowHint(GLFW_VISIBLE, 0);

	Window.offscreen_context = glfwCreateWindow(640, 480, "", NULL, Window.window);

	if (Window.offscreen_context == NULL)
	{
		return false;
	}

	return true;
}

void window_swap_buffers(void)
{
	glfwSwapBuffers(Window.window);
}

bool window_should_close(void)
{
	return glfwWindowShouldClose(Window.window);
}

void window_poll_input(void)
{
	glfwPollEvents();
}

int window_width(void)
{
	return Window.width;
}

int window_height(void)
{
	return Window.height;
}

float window_aspect(void)
{
	return (float)Window.width / (float)Window.height;
}

void window_claim_offscreen_context(void)
{
	glfwMakeContextCurrent(Window.offscreen_context);
}

void window_cursor_get(double* x, double* y)
{
	glfwGetCursorPos(Window.window, x, y);
}

void window_cursor_set(double x, double y)
{
	glfwSetCursorPos(Window.window, x, y);
}

void window_cursor_hide(void)
{
	glfwSetInputMode(Window.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void window_cursor_show(void)
{
	glfwSetInputMode(Window.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

int window_mouse_button(int button)
{
	return glfwGetMouseButton(Window.window, button);
}

int window_keyboard_key(int id)
{
	return glfwGetKey(Window.window, id);
}
