#pragma once

#include "GLKMath.h"

struct aabb
{
	union
	{
		struct
		{
			GLKVector3 min;
			GLKVector3 max;
		};

		GLKVector3 minmax[2];
	};
};
