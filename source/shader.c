#include "shader.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

SHADER shader_create(const char* vert_path, const char* frag_path)
{
	// 'shader' is the return value of this function.
	SHADER shader = 0;

	// The following buffers and files get released in the clean up portion of
	// this function.
	FILE* vert_file = NULL;
	FILE* frag_file = NULL;

	char* vert_source = NULL;
	char* frag_source = NULL;

	// The following variables are shared by the vertex and fragment shader
	// initialization code.
	size_t elements_read = 0;
	GLint compile_status = 0;

	// Load the vertex shader source.  If any part fails jump to cleanup.
	vert_file = fopen(vert_path, "rb");

	if (vert_file == NULL)
	{
		goto cleanup;
	}

	fseek(vert_file, 0, SEEK_END);
	long vert_length = ftell(vert_file);
	rewind(vert_file);

	vert_source = (char*)malloc(vert_length + 1);

	if (vert_source == NULL)
	{
		goto cleanup;
	}

	elements_read = fread(vert_source, 1, vert_length, vert_file);

	if (elements_read != vert_length)
	{
		goto cleanup;
	}

	vert_source[vert_length] = 0;

	// Load the fragment shader source.  If any part fails jump to cleanup.
	frag_file = fopen(frag_path, "rb");

	if (frag_file == NULL)
	{
		goto cleanup;
	}

	fseek(frag_file, 0, SEEK_END);
	long frag_length = ftell(frag_file);
	rewind(frag_file);

	frag_source = (char*)malloc(frag_length + 1);

	if (frag_source == NULL)
	{
		goto cleanup;
	}

	elements_read = fread(frag_source, 1, frag_length, frag_file);

	if (elements_read != frag_length)
	{
		goto cleanup;
	}

	frag_source[frag_length] = 0;

	// Compile the vertex and fragment shaders

	GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
	GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vert_shader, 1, (const GLchar**)&vert_source, NULL);
	glShaderSource(frag_shader, 1, (const GLchar**)&frag_source, NULL);

	glCompileShader(vert_shader);
	glCompileShader(frag_shader);

	glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &compile_status);

	if (compile_status == GL_FALSE)
	{
		char error[4096];
		GLsizei error_length;

		glGetShaderInfoLog(vert_shader, sizeof(error), &error_length, error);

		printf("%.*s\n", (int)error_length, error);

		//assert(false);
	}

	glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &compile_status);

	if (compile_status == GL_FALSE)
	{
		char error[4096];
		GLsizei error_length;

		glGetShaderInfoLog(frag_shader, sizeof(error), &error_length, error);

		printf("%.*s\n", (int)error_length, error);

		//assert(false);
	}

	shader = glCreateProgram();

	glAttachShader(shader, vert_shader);
	glAttachShader(shader, frag_shader);

	glLinkProgram(shader);

	glDeleteShader(vert_shader);
	glDeleteShader(frag_shader);

cleanup:
	free(vert_source);
	free(frag_source);

	if (vert_file != NULL)
	{
		fclose(vert_file);
	}

	if (frag_file != NULL)
	{
		fclose(frag_file);
	}

	return shader;
}

void shader_use(SHADER shader)
{
	glUseProgram(shader);
}
