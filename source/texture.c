#include "texture.h"

#include <stdlib.h>
#include <stdio.h>

#include "lodepng.h"

TEXTURE texture_load(const char* filepath)
{
	unsigned error;
	unsigned char* image;
	unsigned width;
	unsigned height;

	error = lodepng_decode32_file(&image, &width, &height, filepath);

	if (error)
	{
		printf("Texture Error: %u - %s\n", error, lodepng_error_text(error));
		return 0;
	}

	GLuint result = 0;

	glGenTextures(1, &result);
	glBindTexture(GL_TEXTURE_2D, result);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glBindTexture(GL_TEXTURE_2D, 0);

	free(image);

	printf("Texture Loaded: %s\n", filepath);

	return result;
}

TEXTURE texture_load_spine(const char* path, int* out_width, int* out_height)
{
	unsigned error;
	unsigned char* image;
	unsigned width;
	unsigned height;

	error = lodepng_decode32_file(&image, &width, &height, path);

	if (error)
	{
		printf("Texture Error: %u - %s\n", error, lodepng_error_text(error));
		return 0;
	}

	GLuint result = 0;

	glGenTextures(1, &result);
	glBindTexture(GL_TEXTURE_2D, result);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glBindTexture(GL_TEXTURE_2D, 0);

	free(image);

	printf("SPINE Texture Loaded: %s\n", path);

	*out_width = (int)width;
	*out_height = (int)height;

	return result;
}
