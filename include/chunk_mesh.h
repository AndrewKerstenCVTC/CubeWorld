#pragma once

#include <GL/glew.h>

struct chunk_mesh;

typedef void(*chunk_mesh_release_func)(void*);

struct chunk_mesh
{
	GLint first;
	GLsizei count;

	chunk_mesh_release_func release;

	// DO NOT access private data members from outside the mesher.
	struct
	{
		size_t offset;
		size_t length;
		int released;
	} private;
};
