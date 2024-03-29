//
//  GLKVector3.h
//  GLKit
//
//  Copyright (c) 2011, Apple Inc. All rights reserved.
//

#ifndef __GLK_VECTOR_3_H
#define __GLK_VECTOR_3_H

#include <stdbool.h>
#include <math.h>

#include <GLKMathTypes.h>

#ifdef __cplusplus
extern "C" {
#endif

	static __inline GLKVector3 GLKVector3Make(float x, float y, float z);
	static __inline GLKVector3 GLKVector3MakeWithArray(float values[3]);

	static __inline GLKVector3 GLKVector3Negate(GLKVector3 vector);

	static __inline GLKVector3 GLKVector3Add(GLKVector3 vectorLeft, GLKVector3 vectorRight);
	static __inline GLKVector3 GLKVector3Subtract(GLKVector3 vectorLeft, GLKVector3 vectorRight);
	static __inline GLKVector3 GLKVector3Multiply(GLKVector3 vectorLeft, GLKVector3 vectorRight);
	static __inline GLKVector3 GLKVector3Divide(GLKVector3 vectorLeft, GLKVector3 vectorRight);

	static __inline GLKVector3 GLKVector3AddScalar(GLKVector3 vector, float value);
	static __inline GLKVector3 GLKVector3SubtractScalar(GLKVector3 vector, float value);
	static __inline GLKVector3 GLKVector3MultiplyScalar(GLKVector3 vector, float value);
	static __inline GLKVector3 GLKVector3DivideScalar(GLKVector3 vector, float value);

	/*
	Returns a vector whose elements are the larger of the corresponding elements of the vector arguments.
	*/
	static __inline GLKVector3 GLKVector3Maximum(GLKVector3 vectorLeft, GLKVector3 vectorRight);
	/*
	Returns a vector whose elements are the smaller of the corresponding elements of the vector arguments.
	*/
	static __inline GLKVector3 GLKVector3Minimum(GLKVector3 vectorLeft, GLKVector3 vectorRight);

	/*
	Returns true if all of the first vector's elements are equal to all of the second vector's arguments.
	*/
	static __inline bool GLKVector3AllEqualToVector3(GLKVector3 vectorLeft, GLKVector3 vectorRight);
	/*
	Returns true if all of the vector's elements are equal to the provided value.
	*/
	static __inline bool GLKVector3AllEqualToScalar(GLKVector3 vector, float value);
	/*
	Returns true if all of the first vector's elements are greater than all of the second vector's arguments.
	*/
	static __inline bool GLKVector3AllGreaterThanVector3(GLKVector3 vectorLeft, GLKVector3 vectorRight);
	/*
	Returns true if all of the vector's elements are greater than the provided value.
	*/
	static __inline bool GLKVector3AllGreaterThanScalar(GLKVector3 vector, float value);
	/*
	Returns true if all of the first vector's elements are greater than or equal to all of the second vector's arguments.
	*/
	static __inline bool GLKVector3AllGreaterThanOrEqualToVector3(GLKVector3 vectorLeft, GLKVector3 vectorRight);
	/*
	Returns true if all of the vector's elements are greater than or equal to the provided value.
	*/
	static __inline bool GLKVector3AllGreaterThanOrEqualToScalar(GLKVector3 vector, float value);

	static __inline GLKVector3 GLKVector3Normalize(GLKVector3 vector);

	static __inline float GLKVector3DotProduct(GLKVector3 vectorLeft, GLKVector3 vectorRight);
	static __inline float GLKVector3Length(GLKVector3 vector);
	static __inline float GLKVector3Distance(GLKVector3 vectorStart, GLKVector3 vectorEnd);

	static __inline GLKVector3 GLKVector3Lerp(GLKVector3 vectorStart, GLKVector3 vectorEnd, float t);

	static __inline GLKVector3 GLKVector3CrossProduct(GLKVector3 vectorLeft, GLKVector3 vectorRight);

	/*
	Project the vector, vectorToProject, onto the vector, projectionVector.
	*/
	static __inline GLKVector3 GLKVector3Project(GLKVector3 vectorToProject, GLKVector3 projectionVector);

	static __inline GLKVector3 GLKVector3Make(float x, float y, float z)
	{
		GLKVector3 v = { x, y, z };
		return v;
	}

	static __inline GLKVector3 GLKVector3MakeWithArray(float values[3])
	{
		GLKVector3 v = { values[0], values[1], values[2] };
		return v;
	}

	static __inline GLKVector3 GLKVector3Negate(GLKVector3 vector)
	{
		GLKVector3 v = { -vector.v[0], -vector.v[1], -vector.v[2] };
		return v;
	}

	static __inline GLKVector3 GLKVector3Add(GLKVector3 vectorLeft, GLKVector3 vectorRight)
	{
		GLKVector3 v = { vectorLeft.v[0] + vectorRight.v[0],
			vectorLeft.v[1] + vectorRight.v[1],
			vectorLeft.v[2] + vectorRight.v[2] };
		return v;
	}

	static __inline GLKVector3 GLKVector3Subtract(GLKVector3 vectorLeft, GLKVector3 vectorRight)
	{
		GLKVector3 v = { vectorLeft.v[0] - vectorRight.v[0],
			vectorLeft.v[1] - vectorRight.v[1],
			vectorLeft.v[2] - vectorRight.v[2] };
		return v;
	}

	static __inline GLKVector3 GLKVector3Multiply(GLKVector3 vectorLeft, GLKVector3 vectorRight)
	{
		GLKVector3 v = { vectorLeft.v[0] * vectorRight.v[0],
			vectorLeft.v[1] * vectorRight.v[1],
			vectorLeft.v[2] * vectorRight.v[2] };
		return v;
	}

	static __inline GLKVector3 GLKVector3Divide(GLKVector3 vectorLeft, GLKVector3 vectorRight)
	{
		GLKVector3 v = { vectorLeft.v[0] / vectorRight.v[0],
			vectorLeft.v[1] / vectorRight.v[1],
			vectorLeft.v[2] / vectorRight.v[2] };
		return v;
	}

	static __inline GLKVector3 GLKVector3AddScalar(GLKVector3 vector, float value)
	{
		GLKVector3 v = { vector.v[0] + value,
			vector.v[1] + value,
			vector.v[2] + value };
		return v;
	}

	static __inline GLKVector3 GLKVector3SubtractScalar(GLKVector3 vector, float value)
	{
		GLKVector3 v = { vector.v[0] - value,
			vector.v[1] - value,
			vector.v[2] - value };
		return v;
	}

	static __inline GLKVector3 GLKVector3MultiplyScalar(GLKVector3 vector, float value)
	{
		GLKVector3 v = { vector.v[0] * value,
			vector.v[1] * value,
			vector.v[2] * value };
		return v;
	}

	static __inline GLKVector3 GLKVector3DivideScalar(GLKVector3 vector, float value)
	{
		GLKVector3 v = { vector.v[0] / value,
			vector.v[1] / value,
			vector.v[2] / value };
		return v;
	}

	static __inline GLKVector3 GLKVector3Maximum(GLKVector3 vectorLeft, GLKVector3 vectorRight)
	{
		GLKVector3 max = vectorLeft;
		if (vectorRight.v[0] > vectorLeft.v[0])
			max.v[0] = vectorRight.v[0];
		if (vectorRight.v[1] > vectorLeft.v[1])
			max.v[1] = vectorRight.v[1];
		if (vectorRight.v[2] > vectorLeft.v[2])
			max.v[2] = vectorRight.v[2];
		return max;
	}

	static __inline GLKVector3 GLKVector3Minimum(GLKVector3 vectorLeft, GLKVector3 vectorRight)
	{
		GLKVector3 min = vectorLeft;
		if (vectorRight.v[0] < vectorLeft.v[0])
			min.v[0] = vectorRight.v[0];
		if (vectorRight.v[1] < vectorLeft.v[1])
			min.v[1] = vectorRight.v[1];
		if (vectorRight.v[2] < vectorLeft.v[2])
			min.v[2] = vectorRight.v[2];
		return min;
	}

	static __inline bool GLKVector3AllEqualToVector3(GLKVector3 vectorLeft, GLKVector3 vectorRight)
	{
		bool compare = false;
		if (vectorLeft.v[0] == vectorRight.v[0] &&
			vectorLeft.v[1] == vectorRight.v[1] &&
			vectorLeft.v[2] == vectorRight.v[2])
			compare = true;
		return compare;
	}

	static __inline bool GLKVector3AllEqualToScalar(GLKVector3 vector, float value)
	{
		bool compare = false;
		if (vector.v[0] == value &&
			vector.v[1] == value &&
			vector.v[2] == value)
			compare = true;
		return compare;
	}

	static __inline bool GLKVector3AllGreaterThanVector3(GLKVector3 vectorLeft, GLKVector3 vectorRight)
	{
		bool compare = false;
		if (vectorLeft.v[0] > vectorRight.v[0] &&
			vectorLeft.v[1] > vectorRight.v[1] &&
			vectorLeft.v[2] > vectorRight.v[2])
			compare = true;
		return compare;
	}

	static __inline bool GLKVector3AllGreaterThanScalar(GLKVector3 vector, float value)
	{
		bool compare = false;
		if (vector.v[0] > value &&
			vector.v[1] > value &&
			vector.v[2] > value)
			compare = true;
		return compare;
	}

	static __inline bool GLKVector3AllGreaterThanOrEqualToVector3(GLKVector3 vectorLeft, GLKVector3 vectorRight)
	{
		bool compare = false;
		if (vectorLeft.v[0] >= vectorRight.v[0] &&
			vectorLeft.v[1] >= vectorRight.v[1] &&
			vectorLeft.v[2] >= vectorRight.v[2])
			compare = true;
		return compare;
	}

	static __inline bool GLKVector3AllGreaterThanOrEqualToScalar(GLKVector3 vector, float value)
	{
		bool compare = false;
		if (vector.v[0] >= value &&
			vector.v[1] >= value &&
			vector.v[2] >= value)
			compare = true;
		return compare;
	}

	static __inline GLKVector3 GLKVector3Normalize(GLKVector3 vector)
	{
		float scale = 1.0f / GLKVector3Length(vector);
		GLKVector3 v = { vector.v[0] * scale, vector.v[1] * scale, vector.v[2] * scale };
		return v;
	}

	static __inline float GLKVector3DotProduct(GLKVector3 vectorLeft, GLKVector3 vectorRight)
	{
		return vectorLeft.v[0] * vectorRight.v[0] + vectorLeft.v[1] * vectorRight.v[1] + vectorLeft.v[2] * vectorRight.v[2];
	}

	static __inline float GLKVector3Length(GLKVector3 vector)
	{
		return sqrtf(vector.v[0] * vector.v[0] + vector.v[1] * vector.v[1] + vector.v[2] * vector.v[2]);
	}

	static __inline float GLKVector3Distance(GLKVector3 vectorStart, GLKVector3 vectorEnd)
	{
		return GLKVector3Length(GLKVector3Subtract(vectorEnd, vectorStart));
	}

	static __inline GLKVector3 GLKVector3Lerp(GLKVector3 vectorStart, GLKVector3 vectorEnd, float t)
	{
		GLKVector3 v = { vectorStart.v[0] + ((vectorEnd.v[0] - vectorStart.v[0]) * t),
			vectorStart.v[1] + ((vectorEnd.v[1] - vectorStart.v[1]) * t),
			vectorStart.v[2] + ((vectorEnd.v[2] - vectorStart.v[2]) * t) };
		return v;
	}

	static __inline GLKVector3 GLKVector3CrossProduct(GLKVector3 vectorLeft, GLKVector3 vectorRight)
	{
		GLKVector3 v = { vectorLeft.v[1] * vectorRight.v[2] - vectorLeft.v[2] * vectorRight.v[1],
			vectorLeft.v[2] * vectorRight.v[0] - vectorLeft.v[0] * vectorRight.v[2],
			vectorLeft.v[0] * vectorRight.v[1] - vectorLeft.v[1] * vectorRight.v[0] };
		return v;
	}

	static __inline GLKVector3 GLKVector3Project(GLKVector3 vectorToProject, GLKVector3 projectionVector)
	{
		float scale = GLKVector3DotProduct(projectionVector, vectorToProject) / GLKVector3DotProduct(projectionVector, projectionVector);
		GLKVector3 v = GLKVector3MultiplyScalar(projectionVector, scale);
		return v;
	}

#ifdef __cplusplus
}
#endif

#endif /* __GLK_VECTOR_3_H */