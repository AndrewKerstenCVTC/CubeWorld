//
//  GLKVector4.h
//  GLKit
//
//  Copyright (c) 2011, Apple Inc. All rights reserved.
//

#ifndef __GLK_VECTOR_4_H
#define __GLK_VECTOR_4_H

#include <stdbool.h>
#include <math.h>

#if defined(__ARM_NEON__)
#include <arm_neon.h>
#endif

#include <GLKMathTypes.h>

#ifdef __cplusplus
extern "C" {
#endif

	static __inline GLKVector4 GLKVector4Make(float x, float y, float z, float w);
	static __inline GLKVector4 GLKVector4MakeWithArray(float values[4]);
	static __inline GLKVector4 GLKVector4MakeWithVector3(GLKVector3 vector, float w);

	static __inline GLKVector4 GLKVector4Negate(GLKVector4 vector);

	static __inline GLKVector4 GLKVector4Add(GLKVector4 vectorLeft, GLKVector4 vectorRight);
	static __inline GLKVector4 GLKVector4Subtract(GLKVector4 vectorLeft, GLKVector4 vectorRight);
	static __inline GLKVector4 GLKVector4Multiply(GLKVector4 vectorLeft, GLKVector4 vectorRight);
	static __inline GLKVector4 GLKVector4Divide(GLKVector4 vectorLeft, GLKVector4 vectorRight);

	static __inline GLKVector4 GLKVector4AddScalar(GLKVector4 vector, float value);
	static __inline GLKVector4 GLKVector4SubtractScalar(GLKVector4 vector, float value);
	static __inline GLKVector4 GLKVector4MultiplyScalar(GLKVector4 vector, float value);
	static __inline GLKVector4 GLKVector4DivideScalar(GLKVector4 vector, float value);

	/*
	Returns a vector whose elements are the larger of the corresponding elements of the vector arguments.
	*/
	static __inline GLKVector4 GLKVector4Maximum(GLKVector4 vectorLeft, GLKVector4 vectorRight);
	/*
	Returns a vector whose elements are the smaller of the corresponding elements of the vector arguments.
	*/
	static __inline GLKVector4 GLKVector4Minimum(GLKVector4 vectorLeft, GLKVector4 vectorRight);

	/*
	Returns true if all of the first vector's elements are equal to all of the second vector's arguments.
	*/
	static __inline bool GLKVector4AllEqualToVector4(GLKVector4 vectorLeft, GLKVector4 vectorRight);
	/*
	Returns true if all of the vector's elements are equal to the provided value.
	*/
	static __inline bool GLKVector4AllEqualToScalar(GLKVector4 vector, float value);
	/*
	Returns true if all of the first vector's elements are greater than all of the second vector's arguments.
	*/
	static __inline bool GLKVector4AllGreaterThanVector4(GLKVector4 vectorLeft, GLKVector4 vectorRight);
	/*
	Returns true if all of the vector's elements are greater than the provided value.
	*/
	static __inline bool GLKVector4AllGreaterThanScalar(GLKVector4 vector, float value);
	/*
	Returns true if all of the first vector's elements are greater than or equal to all of the second vector's arguments.
	*/
	static __inline bool GLKVector4AllGreaterThanOrEqualToVector4(GLKVector4 vectorLeft, GLKVector4 vectorRight);
	/*
	Returns true if all of the vector's elements are greater than or equal to the provided value.
	*/
	static __inline bool GLKVector4AllGreaterThanOrEqualToScalar(GLKVector4 vector, float value);

	static __inline GLKVector4 GLKVector4Normalize(GLKVector4 vector);

	static __inline float GLKVector4DotProduct(GLKVector4 vectorLeft, GLKVector4 vectorRight);
	static __inline float GLKVector4Length(GLKVector4 vector);
	static __inline float GLKVector4Distance(GLKVector4 vectorStart, GLKVector4 vectorEnd);

	static __inline GLKVector4 GLKVector4Lerp(GLKVector4 vectorStart, GLKVector4 vectorEnd, float t);

	/*
	Performs a 3D cross product. The last component of the resulting cross product will be zeroed out.
	*/
	static __inline GLKVector4 GLKVector4CrossProduct(GLKVector4 vectorLeft, GLKVector4 vectorRight);

	/*
	Project the vector, vectorToProject, onto the vector, projectionVector.
	*/
	static __inline GLKVector4 GLKVector4Project(GLKVector4 vectorToProject, GLKVector4 projectionVector);

	static __inline GLKVector4 GLKVector4Make(float x, float y, float z, float w)
	{
		GLKVector4 v = { x, y, z, w };
		return v;
	}

	static __inline GLKVector4 GLKVector4MakeWithArray(float values[4])
	{
#if defined(__ARM_NEON__)
		float32x4_t v = vld1q_f32(values);
		return *(GLKVector4 *)&v;
#else
		GLKVector4 v = { values[0], values[1], values[2], values[3] };
		return v;
#endif
	}

	static __inline GLKVector4 GLKVector4MakeWithVector3(GLKVector3 vector, float w)
	{
		GLKVector4 v = { vector.v[0], vector.v[1], vector.v[2], w };
		return v;
	}

	static __inline GLKVector4 GLKVector4Negate(GLKVector4 vector)
	{
#if defined(__ARM_NEON__)
		float32x4_t v = vnegq_f32(*(float32x4_t *)&vector);
		return *(GLKVector4 *)&v;
#else
		GLKVector4 v = { -vector.v[0], -vector.v[1], -vector.v[2], -vector.v[3] };
		return v;
#endif
	}

	static __inline GLKVector4 GLKVector4Add(GLKVector4 vectorLeft, GLKVector4 vectorRight)
	{
#if defined(__ARM_NEON__)
		float32x4_t v = vaddq_f32(*(float32x4_t *)&vectorLeft,
			*(float32x4_t *)&vectorRight);
		return *(GLKVector4 *)&v;
#else
		GLKVector4 v = { vectorLeft.v[0] + vectorRight.v[0],
			vectorLeft.v[1] + vectorRight.v[1],
			vectorLeft.v[2] + vectorRight.v[2],
			vectorLeft.v[3] + vectorRight.v[3] };
		return v;
#endif
	}

	static __inline GLKVector4 GLKVector4Subtract(GLKVector4 vectorLeft, GLKVector4 vectorRight)
	{
#if defined(__ARM_NEON__)
		float32x4_t v = vsubq_f32(*(float32x4_t *)&vectorLeft,
			*(float32x4_t *)&vectorRight);
		return *(GLKVector4 *)&v;
#else
		GLKVector4 v = { vectorLeft.v[0] - vectorRight.v[0],
			vectorLeft.v[1] - vectorRight.v[1],
			vectorLeft.v[2] - vectorRight.v[2],
			vectorLeft.v[3] - vectorRight.v[3] };
		return v;
#endif
	}

	static __inline GLKVector4 GLKVector4Multiply(GLKVector4 vectorLeft, GLKVector4 vectorRight)
	{
#if defined(__ARM_NEON__)
		float32x4_t v = vmulq_f32(*(float32x4_t *)&vectorLeft,
			*(float32x4_t *)&vectorRight);
		return *(GLKVector4 *)&v;
#else
		GLKVector4 v = { vectorLeft.v[0] * vectorRight.v[0],
			vectorLeft.v[1] * vectorRight.v[1],
			vectorLeft.v[2] * vectorRight.v[2],
			vectorLeft.v[3] * vectorRight.v[3] };
		return v;
#endif
	}

	static __inline GLKVector4 GLKVector4Divide(GLKVector4 vectorLeft, GLKVector4 vectorRight)
	{
#if defined(__ARM_NEON__)
		float32x4_t *vLeft = (float32x4_t *)&vectorLeft;
		float32x4_t *vRight = (float32x4_t *)&vectorRight;
		float32x4_t estimate = vrecpeq_f32(*vRight);
		estimate = vmulq_f32(vrecpsq_f32(*vRight, estimate), estimate);
		estimate = vmulq_f32(vrecpsq_f32(*vRight, estimate), estimate);
		float32x4_t v = vmulq_f32(*vLeft, estimate);
		return *(GLKVector4 *)&v;
#else
		GLKVector4 v = { vectorLeft.v[0] / vectorRight.v[0],
			vectorLeft.v[1] / vectorRight.v[1],
			vectorLeft.v[2] / vectorRight.v[2],
			vectorLeft.v[3] / vectorRight.v[3] };
		return v;
#endif
	}

	static __inline GLKVector4 GLKVector4AddScalar(GLKVector4 vector, float value)
	{
#if defined(__ARM_NEON__)
		float32x4_t v = vaddq_f32(*(float32x4_t *)&vector,
			vdupq_n_f32((float32_t)value));
		return *(GLKVector4 *)&v;
#else
		GLKVector4 v = { vector.v[0] + value,
			vector.v[1] + value,
			vector.v[2] + value,
			vector.v[3] + value };
		return v;
#endif
	}

	static __inline GLKVector4 GLKVector4SubtractScalar(GLKVector4 vector, float value)
	{
#if defined(__ARM_NEON__)
		float32x4_t v = vsubq_f32(*(float32x4_t *)&vector,
			vdupq_n_f32((float32_t)value));
		return *(GLKVector4 *)&v;
#else
		GLKVector4 v = { vector.v[0] - value,
			vector.v[1] - value,
			vector.v[2] - value,
			vector.v[3] - value };
		return v;
#endif
	}

	static __inline GLKVector4 GLKVector4MultiplyScalar(GLKVector4 vector, float value)
	{
#if defined(__ARM_NEON__)
		float32x4_t v = vmulq_f32(*(float32x4_t *)&vector,
			vdupq_n_f32((float32_t)value));
		return *(GLKVector4 *)&v;
#else
		GLKVector4 v = { vector.v[0] * value,
			vector.v[1] * value,
			vector.v[2] * value,
			vector.v[3] * value };
		return v;
#endif
	}

	static __inline GLKVector4 GLKVector4DivideScalar(GLKVector4 vector, float value)
	{
#if defined(__ARM_NEON__)
		float32x4_t values = vdupq_n_f32((float32_t)value);
		float32x4_t estimate = vrecpeq_f32(values);
		estimate = vmulq_f32(vrecpsq_f32(values, estimate), estimate);
		estimate = vmulq_f32(vrecpsq_f32(values, estimate), estimate);
		float32x4_t v = vmulq_f32(*(float32x4_t *)&vector, estimate);
		return *(GLKVector4 *)&v;
#else
		GLKVector4 v = { vector.v[0] / value,
			vector.v[1] / value,
			vector.v[2] / value,
			vector.v[3] / value };
		return v;
#endif
	}

	static __inline GLKVector4 GLKVector4Maximum(GLKVector4 vectorLeft, GLKVector4 vectorRight)
	{
#if defined(__ARM_NEON__)
		float32x4_t v = vmaxq_f32(*(float32x4_t *)&vectorLeft,
			*(float32x4_t *)&vectorRight);
		return *(GLKVector4 *)&v;
#else
		GLKVector4 max = vectorLeft;
		if (vectorRight.v[0] > vectorLeft.v[0])
			max.v[0] = vectorRight.v[0];
		if (vectorRight.v[1] > vectorLeft.v[1])
			max.v[1] = vectorRight.v[1];
		if (vectorRight.v[2] > vectorLeft.v[2])
			max.v[2] = vectorRight.v[2];
		if (vectorRight.v[3] > vectorLeft.v[3])
			max.v[3] = vectorRight.v[3];
		return max;
#endif
	}

	static __inline GLKVector4 GLKVector4Minimum(GLKVector4 vectorLeft, GLKVector4 vectorRight)
	{
#if defined(__ARM_NEON__)
		float32x4_t v = vminq_f32(*(float32x4_t *)&vectorLeft,
			*(float32x4_t *)&vectorRight);
		return *(GLKVector4 *)&v;
#else
		GLKVector4 min = vectorLeft;
		if (vectorRight.v[0] < vectorLeft.v[0])
			min.v[0] = vectorRight.v[0];
		if (vectorRight.v[1] < vectorLeft.v[1])
			min.v[1] = vectorRight.v[1];
		if (vectorRight.v[2] < vectorLeft.v[2])
			min.v[2] = vectorRight.v[2];
		if (vectorRight.v[3] < vectorLeft.v[3])
			min.v[3] = vectorRight.v[3];
		return min;
#endif
	}

	static __inline bool GLKVector4AllEqualToVector4(GLKVector4 vectorLeft, GLKVector4 vectorRight)
	{
#if defined(__ARM_NEON_)
		float32x4_t v1 = *(float32x4_t *)&vectorLeft;
		float32x4_t v2 = *(float32x4_t *)&vectorRight;
		uint32x4_t vCmp = vceqq_f32(v1, v2);
		uint32x2_t vAnd = vand_u32(vget_low_u32(vCmp), vget_high_u32(vCmp));
		vAnd = vand_u32(vAnd, vext_u32(vAnd, vAnd, 1));
		vAnd = vand_u32(vAnd, vdup_n_u32(1));
		return (bool)vget_lane_u32(vAnd, 0);
#else
		bool compare = false;
		if (vectorLeft.v[0] == vectorRight.v[0] &&
			vectorLeft.v[1] == vectorRight.v[1] &&
			vectorLeft.v[2] == vectorRight.v[2] &&
			vectorLeft.v[3] == vectorRight.v[3])
			compare = true;
		return compare;
#endif
	}

	static __inline bool GLKVector4AllEqualToScalar(GLKVector4 vector, float value)
	{
#if defined(__ARM_NEON_)
		float32x4_t v1 = *(float32x4_t *)&vector;
		float32x4_t v2 = vdupq_n_f32(value);
		uint32x4_t vCmp = vceqq_f32(v1, v2);
		uint32x2_t vAnd = vand_u32(vget_low_u32(vCmp), vget_high_u32(vCmp));
		vAnd = vand_u32(vAnd, vext_u32(vAnd, vAnd, 1));
		vAnd = vand_u32(vAnd, vdup_n_u32(1));
		return (bool)vget_lane_u32(vAnd, 0);
#else
		bool compare = false;
		if (vector.v[0] == value &&
			vector.v[1] == value &&
			vector.v[2] == value &&
			vector.v[3] == value)
			compare = true;
		return compare;
#endif
	}

	static __inline bool GLKVector4AllGreaterThanVector4(GLKVector4 vectorLeft, GLKVector4 vectorRight)
	{
#if defined(__ARM_NEON_)
		float32x4_t v1 = *(float32x4_t *)&vectorLeft;
		float32x4_t v2 = *(float32x4_t *)&vectorRight;
		uint32x4_t vCmp = vcgtq_f32(v1, v2);
		uint32x2_t vAnd = vand_u32(vget_low_u32(vCmp), vget_high_u32(vCmp));
		vAnd = vand_u32(vAnd, vext_u32(vAnd, vAnd, 1));
		vAnd = vand_u32(vAnd, vdup_n_u32(1));
		return (bool)vget_lane_u32(vAnd, 0);
#else
		bool compare = false;
		if (vectorLeft.v[0] > vectorRight.v[0] &&
			vectorLeft.v[1] > vectorRight.v[1] &&
			vectorLeft.v[2] > vectorRight.v[2] &&
			vectorLeft.v[3] > vectorRight.v[3])
			compare = true;
		return compare;
#endif
	}

	static __inline bool GLKVector4AllGreaterThanScalar(GLKVector4 vector, float value)
	{
#if defined(__ARM_NEON_)
		float32x4_t v1 = *(float32x4_t *)&vector;
		float32x4_t v2 = vdupq_n_f32(value);
		uint32x4_t vCmp = vcgtq_f32(v1, v2);
		uint32x2_t vAnd = vand_u32(vget_low_u32(vCmp), vget_high_u32(vCmp));
		vAnd = vand_u32(vAnd, vext_u32(vAnd, vAnd, 1));
		vAnd = vand_u32(vAnd, vdup_n_u32(1));
		return (bool)vget_lane_u32(vAnd, 0);
#else
		bool compare = false;
		if (vector.v[0] > value &&
			vector.v[1] > value &&
			vector.v[2] > value &&
			vector.v[3] > value)
			compare = true;
		return compare;
#endif
	}

	static __inline bool GLKVector4AllGreaterThanOrEqualToVector4(GLKVector4 vectorLeft, GLKVector4 vectorRight)
	{
#if defined(__ARM_NEON_)
		float32x4_t v1 = *(float32x4_t *)&vectorLeft;
		float32x4_t v2 = *(float32x4_t *)&vectorRight;
		uint32x4_t vCmp = vcgeq_f32(v1, v2);
		uint32x2_t vAnd = vand_u32(vget_low_u32(vCmp), vget_high_u32(vCmp));
		vAnd = vand_u32(vAnd, vext_u32(vAnd, vAnd, 1));
		vAnd = vand_u32(vAnd, vdup_n_u32(1));
		return (bool)vget_lane_u32(vAnd, 0);
#else
		bool compare = false;
		if (vectorLeft.v[0] >= vectorRight.v[0] &&
			vectorLeft.v[1] >= vectorRight.v[1] &&
			vectorLeft.v[2] >= vectorRight.v[2] &&
			vectorLeft.v[3] >= vectorRight.v[3])
			compare = true;
		return compare;
#endif
	}

	static __inline bool GLKVector4AllGreaterThanOrEqualToScalar(GLKVector4 vector, float value)
	{
#if defined(__ARM_NEON_)
		float32x4_t v1 = *(float32x4_t *)&vector;
		float32x4_t v2 = vdupq_n_f32(value);
		uint32x4_t vCmp = vcgeq_f32(v1, v2);
		uint32x2_t vAnd = vand_u32(vget_low_u32(vCmp), vget_high_u32(vCmp));
		vAnd = vand_u32(vAnd, vext_u32(vAnd, vAnd, 1));
		vAnd = vand_u32(vAnd, vdup_n_u32(1));
		return (bool)vget_lane_u32(vAnd, 0);
#else
		bool compare = false;
		if (vector.v[0] >= value &&
			vector.v[1] >= value &&
			vector.v[2] >= value &&
			vector.v[3] >= value)
			compare = true;
		return compare;
#endif
	}

	static __inline GLKVector4 GLKVector4Normalize(GLKVector4 vector)
	{
		float scale = 1.0f / GLKVector4Length(vector);
		GLKVector4 v = GLKVector4MultiplyScalar(vector, scale);
		return v;
	}

	static __inline float GLKVector4DotProduct(GLKVector4 vectorLeft, GLKVector4 vectorRight)
	{
#if defined(__ARM_NEON__)
		float32x4_t v = vmulq_f32(*(float32x4_t *)&vectorLeft,
			*(float32x4_t *)&vectorRight);
		float32x2_t v2 = vpadd_f32(vget_low_f32(v), vget_high_f32(v));
		v2 = vpadd_f32(v2, v2);
		return vget_lane_f32(v2, 0);
#else
		return vectorLeft.v[0] * vectorRight.v[0] +
			vectorLeft.v[1] * vectorRight.v[1] +
			vectorLeft.v[2] * vectorRight.v[2] +
			vectorLeft.v[3] * vectorRight.v[3];
#endif
	}

	static __inline float GLKVector4Length(GLKVector4 vector)
	{
#if defined(__ARM_NEON__)
		float32x4_t v = vmulq_f32(*(float32x4_t *)&vector,
			*(float32x4_t *)&vector);
		float32x2_t v2 = vpadd_f32(vget_low_f32(v), vget_high_f32(v));
		v2 = vpadd_f32(v2, v2);
		return sqrt(vget_lane_f32(v2, 0));
#else
		return sqrtf(vector.v[0] * vector.v[0] +
			vector.v[1] * vector.v[1] +
			vector.v[2] * vector.v[2] +
			vector.v[3] * vector.v[3]);
#endif
	}

	static __inline float GLKVector4Distance(GLKVector4 vectorStart, GLKVector4 vectorEnd)
	{
		return GLKVector4Length(GLKVector4Subtract(vectorEnd, vectorStart));
	}

	static __inline GLKVector4 GLKVector4Lerp(GLKVector4 vectorStart, GLKVector4 vectorEnd, float t)
	{
#if defined(__ARM_NEON__)
		float32x4_t vDiff = vsubq_f32(*(float32x4_t *)&vectorEnd,
			*(float32x4_t *)&vectorStart);
		vDiff = vmulq_f32(vDiff, vdupq_n_f32((float32_t)t));
		float32x4_t v = vaddq_f32(*(float32x4_t *)&vectorStart, vDiff);
		return *(GLKVector4 *)&v;
#else
		GLKVector4 v = { vectorStart.v[0] + ((vectorEnd.v[0] - vectorStart.v[0]) * t),
			vectorStart.v[1] + ((vectorEnd.v[1] - vectorStart.v[1]) * t),
			vectorStart.v[2] + ((vectorEnd.v[2] - vectorStart.v[2]) * t),
			vectorStart.v[3] + ((vectorEnd.v[3] - vectorStart.v[3]) * t) };
		return v;
#endif
	}

	static __inline GLKVector4 GLKVector4CrossProduct(GLKVector4 vectorLeft, GLKVector4 vectorRight)
	{
		GLKVector4 v = { vectorLeft.v[1] * vectorRight.v[2] - vectorLeft.v[2] * vectorRight.v[1],
			vectorLeft.v[2] * vectorRight.v[0] - vectorLeft.v[0] * vectorRight.v[2],
			vectorLeft.v[0] * vectorRight.v[1] - vectorLeft.v[1] * vectorRight.v[0],
			0.0f };
		return v;
	}

	static __inline GLKVector4 GLKVector4Project(GLKVector4 vectorToProject, GLKVector4 projectionVector)
	{
		float scale = GLKVector4DotProduct(projectionVector, vectorToProject) / GLKVector4DotProduct(projectionVector, projectionVector);
		GLKVector4 v = GLKVector4MultiplyScalar(projectionVector, scale);
		return v;
	}

#ifdef __cplusplus
}
#endif

#endif /* __GLK_VECTOR_4_H */