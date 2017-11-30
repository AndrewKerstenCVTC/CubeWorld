#pragma once

#include "GL/glew.h"

struct color
{
	union
	{
		struct
		{
			GLubyte r;
			GLubyte g;
			GLubyte b;
			GLubyte a;
		};

		GLubyte components[4];
	};
};
