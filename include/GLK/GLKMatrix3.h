//
//  GLKMatrix3.h
//  GLKit
//
//  Copyright (c) 2011, Apple Inc. All rights reserved.
//

#ifndef __GLK_MATRIX_3_H
#define __GLK_MATRIX_3_H

#include <stddef.h>
#include <stdbool.h>
#include <math.h>

#if defined(__ARM_NEON__)
#include <arm_neon.h>
#endif

#include <GLKMathTypes.h>
#include <GLKVector3.h>
#include <GLKQuaternion.h>

#ifdef __cplusplus
extern "C" {
#endif

	extern const GLKMatrix3 GLKMatrix3Identity;

	static __inline GLKMatrix3 GLKMatrix3Make(float m00, float m01, float m02,
		float m10, float m11, float m12,
		float m20, float m21, float m22);

	static __inline GLKMatrix3 GLKMatrix3MakeAndTranspose(float m00, float m01, float m02,
		float m10, float m11, float m12,
		float m20, float m21, float m22);

	static __inline GLKMatrix3 GLKMatrix3MakeWithArray(float values[9]);

	static __inline GLKMatrix3 GLKMatrix3MakeWithArrayAndTranspose(float values[9]);

	static __inline GLKMatrix3 GLKMatrix3MakeWithRows(GLKVector3 row0,
		GLKVector3 row1,
		GLKVector3 row2);

	static __inline GLKMatrix3 GLKMatrix3MakeWithColumns(GLKVector3 column0,
		GLKVector3 column1,
		GLKVector3 column2);

	/*
	The quaternion will be normalized before conversion.
	*/
	static __inline GLKMatrix3 GLKMatrix3MakeWithQuaternion(GLKQuaternion quaternion);

	static __inline GLKMatrix3 GLKMatrix3MakeScale(float sx, float sy, float sz);
	static __inline GLKMatrix3 GLKMatrix3MakeRotation(float radians, float x, float y, float z);

	static __inline GLKMatrix3 GLKMatrix3MakeXRotation(float radians);
	static __inline GLKMatrix3 GLKMatrix3MakeYRotation(float radians);
	static __inline GLKMatrix3 GLKMatrix3MakeZRotation(float radians);

	/*
	Returns the upper left 2x2 portion of the 3x3 matrix.
	*/
	static __inline GLKMatrix2 GLKMatrix3GetMatrix2(GLKMatrix3 matrix);

	static __inline GLKVector3 GLKMatrix3GetRow(GLKMatrix3 matrix, int row);
	static __inline GLKVector3 GLKMatrix3GetColumn(GLKMatrix3 matrix, int column);

	static __inline GLKMatrix3 GLKMatrix3SetRow(GLKMatrix3 matrix, int row, GLKVector3 vector);
	static __inline GLKMatrix3 GLKMatrix3SetColumn(GLKMatrix3 matrix, int column, GLKVector3 vector);

	static __inline GLKMatrix3 GLKMatrix3Transpose(GLKMatrix3 matrix);

	GLKMatrix3 GLKMatrix3Invert(GLKMatrix3 matrix, bool *isInvertible);
	GLKMatrix3 GLKMatrix3InvertAndTranspose(GLKMatrix3 matrix, bool *isInvertible);

	static __inline GLKMatrix3 GLKMatrix3Multiply(GLKMatrix3 matrixLeft, GLKMatrix3 matrixRight);

	static __inline GLKMatrix3 GLKMatrix3Add(GLKMatrix3 matrixLeft, GLKMatrix3 matrixRight);
	static __inline GLKMatrix3 GLKMatrix3Subtract(GLKMatrix3 matrixLeft, GLKMatrix3 matrixRight);

	static __inline GLKMatrix3 GLKMatrix3Scale(GLKMatrix3 matrix, float sx, float sy, float sz);
	static __inline GLKMatrix3 GLKMatrix3ScaleWithVector3(GLKMatrix3 matrix, GLKVector3 scaleVector);
	/*
	The last component of the GLKVector4, scaleVector, is ignored.
	*/
	static __inline GLKMatrix3 GLKMatrix3ScaleWithVector4(GLKMatrix3 matrix, GLKVector4 scaleVector);

	static __inline GLKMatrix3 GLKMatrix3Rotate(GLKMatrix3 matrix, float radians, float x, float y, float z);
	static __inline GLKMatrix3 GLKMatrix3RotateWithVector3(GLKMatrix3 matrix, float radians, GLKVector3 axisVector);
	/*
	The last component of the GLKVector4, axisVector, is ignored.
	*/
	static __inline GLKMatrix3 GLKMatrix3RotateWithVector4(GLKMatrix3 matrix, float radians, GLKVector4 axisVector);

	static __inline GLKMatrix3 GLKMatrix3RotateX(GLKMatrix3 matrix, float radians);
	static __inline GLKMatrix3 GLKMatrix3RotateY(GLKMatrix3 matrix, float radians);
	static __inline GLKMatrix3 GLKMatrix3RotateZ(GLKMatrix3 matrix, float radians);

	static __inline GLKVector3 GLKMatrix3MultiplyVector3(GLKMatrix3 matrixLeft, GLKVector3 vectorRight);

	static __inline void GLKMatrix3MultiplyVector3Array(GLKMatrix3 matrix, GLKVector3 *vectors, size_t vectorCount);

	static __inline GLKMatrix3 GLKMatrix3Make(float m00, float m01, float m02,
		float m10, float m11, float m12,
		float m20, float m21, float m22)
	{
		GLKMatrix3 m = { m00, m01, m02,
			m10, m11, m12,
			m20, m21, m22 };
		return m;
	}

	static __inline GLKMatrix3 GLKMatrix3MakeAndTranspose(float m00, float m01, float m02,
		float m10, float m11, float m12,
		float m20, float m21, float m22)
	{
		GLKMatrix3 m = { m00, m10, m20,
			m01, m11, m21,
			m02, m12, m22 };
		return m;
	}

	static __inline GLKMatrix3 GLKMatrix3MakeWithArray(float values[9])
	{
		GLKMatrix3 m = { values[0], values[1], values[2],
			values[3], values[4], values[5],
			values[6], values[7], values[8] };
		return m;
	}

	static __inline GLKMatrix3 GLKMatrix3MakeWithArrayAndTranspose(float values[9])
	{
		GLKMatrix3 m = { values[0], values[3], values[6],
			values[1], values[4], values[7],
			values[2], values[5], values[8] };
		return m;
	}

	static __inline GLKMatrix3 GLKMatrix3MakeWithRows(GLKVector3 row0,
		GLKVector3 row1,
		GLKVector3 row2)
	{
		GLKMatrix3 m = { row0.v[0], row1.v[0], row2.v[0],
			row0.v[1], row1.v[1], row2.v[1],
			row0.v[2], row1.v[2], row2.v[2] };
		return m;
	}

	static __inline GLKMatrix3 GLKMatrix3MakeWithColumns(GLKVector3 column0,
		GLKVector3 column1,
		GLKVector3 column2)
	{
		GLKMatrix3 m = { column0.v[0], column0.v[1], column0.v[2],
			column1.v[0], column1.v[1], column1.v[2],
			column2.v[0], column2.v[1], column2.v[2] };
		return m;
	}

	static __inline GLKMatrix3 GLKMatrix3MakeWithQuaternion(GLKQuaternion quaternion)
	{
		quaternion = GLKQuaternionNormalize(quaternion);

		float x = quaternion.q[0];
		float y = quaternion.q[1];
		float z = quaternion.q[2];
		float w = quaternion.q[3];

		float _2x = x + x;
		float _2y = y + y;
		float _2z = z + z;
		float _2w = w + w;

		GLKMatrix3 m = { 1.0f - _2y * y - _2z * z,
			_2x * y + _2w * z,
			_2x * z - _2w * y,

			_2x * y - _2w * z,
			1.0f - _2x * x - _2z * z,
			_2y * z + _2w * x,

			_2x * z + _2w * y,
			_2y * z - _2w * x,
			1.0f - _2x * x - _2y * y };

		return m;
	}

	static __inline GLKMatrix3 GLKMatrix3MakeScale(float sx, float sy, float sz)
	{
		GLKMatrix3 m = GLKMatrix3Identity;
		m.m[0] = sx;
		m.m[4] = sy;
		m.m[8] = sz;
		return m;
	}

	static __inline GLKMatrix3 GLKMatrix3MakeRotation(float radians, float x, float y, float z)
	{
		GLKVector3 v = GLKVector3Normalize(GLKVector3Make(x, y, z));
		float cos = cosf(radians);
		float cosp = 1.0f - cos;
		float sin = sinf(radians);

		GLKMatrix3 m = { cos + cosp * v.v[0] * v.v[0],
			cosp * v.v[0] * v.v[1] + v.v[2] * sin,
			cosp * v.v[0] * v.v[2] - v.v[1] * sin,

			cosp * v.v[0] * v.v[1] - v.v[2] * sin,
			cos + cosp * v.v[1] * v.v[1],
			cosp * v.v[1] * v.v[2] + v.v[0] * sin,

			cosp * v.v[0] * v.v[2] + v.v[1] * sin,
			cosp * v.v[1] * v.v[2] - v.v[0] * sin,
			cos + cosp * v.v[2] * v.v[2] };

		return m;
	}

	static __inline GLKMatrix3 GLKMatrix3MakeXRotation(float radians)
	{
		float cos = cosf(radians);
		float sin = sinf(radians);

		GLKMatrix3 m = { 1.0f, 0.0f, 0.0f,
			0.0f, cos, sin,
			0.0f, -sin, cos };

		return m;
	}

	static __inline GLKMatrix3 GLKMatrix3MakeYRotation(float radians)
	{
		float cos = cosf(radians);
		float sin = sinf(radians);

		GLKMatrix3 m = { cos, 0.0f, -sin,
			0.0f, 1.0f, 0.0f,
			sin, 0.0f, cos };

		return m;
	}

	static __inline GLKMatrix3 GLKMatrix3MakeZRotation(float radians)
	{
		float cos = cosf(radians);
		float sin = sinf(radians);

		GLKMatrix3 m = { cos, sin, 0.0f,
			-sin, cos, 0.0f,
			0.0f, 0.0f, 1.0f };

		return m;
	}

	static __inline GLKMatrix2 GLKMatrix3GetMatrix2(GLKMatrix3 matrix)
	{
		GLKMatrix2 m = { matrix.m[0], matrix.m[1],
			matrix.m[3], matrix.m[4] };
		return m;
	}

	static __inline GLKVector3 GLKMatrix3GetRow(GLKMatrix3 matrix, int row)
	{
		GLKVector3 v = { matrix.m[row], matrix.m[3 + row], matrix.m[6 + row] };
		return v;
	}

	static __inline GLKVector3 GLKMatrix3GetColumn(GLKMatrix3 matrix, int column)
	{
#if defined(__ARM_NEON__)
		GLKVector3 v;
		*((float32x2_t *)&v) = vld1_f32(&(matrix.m[column * 3]));
		v.v[2] = matrix.m[column * 3 + 2];
		return v;
#else
		GLKVector3 v = { matrix.m[column * 3 + 0], matrix.m[column * 3 + 1], matrix.m[column * 3 + 2] };
		return v;
#endif
	}

	static __inline GLKMatrix3 GLKMatrix3SetRow(GLKMatrix3 matrix, int row, GLKVector3 vector)
	{
		matrix.m[row] = vector.v[0];
		matrix.m[row + 3] = vector.v[1];
		matrix.m[row + 6] = vector.v[2];

		return matrix;
	}

	static __inline GLKMatrix3 GLKMatrix3SetColumn(GLKMatrix3 matrix, int column, GLKVector3 vector)
	{
#if defined(__ARM_NEON__)
		float *dst = &(matrix.m[column * 3]);
		vst1_f32(dst, vld1_f32(vector.v));
		dst[2] = vector.v[2];
		return matrix;
#else
		matrix.m[column * 3 + 0] = vector.v[0];
		matrix.m[column * 3 + 1] = vector.v[1];
		matrix.m[column * 3 + 2] = vector.v[2];

		return matrix;
#endif
	}

	static __inline GLKMatrix3 GLKMatrix3Transpose(GLKMatrix3 matrix)
	{
		GLKMatrix3 m = { matrix.m[0], matrix.m[3], matrix.m[6],
			matrix.m[1], matrix.m[4], matrix.m[7],
			matrix.m[2], matrix.m[5], matrix.m[8] };
		return m;
	}

	static __inline GLKMatrix3 GLKMatrix3Multiply(GLKMatrix3 matrixLeft, GLKMatrix3 matrixRight)
	{
#if defined(__ARM_NEON__)
		GLKMatrix3 m;
		float32x4x3_t iMatrixLeft;
		float32x4x3_t iMatrixRight;
		float32x4x3_t mm;

		iMatrixLeft.val[0] = *(float32x4_t *)&matrixLeft.m[0]; // 0 1 2 3
		iMatrixLeft.val[1] = *(float32x4_t *)&matrixLeft.m[3]; // 3 4 5 6
		iMatrixLeft.val[2] = *(float32x4_t *)&matrixLeft.m[5]; // 5 6 7 8

		iMatrixRight.val[0] = *(float32x4_t *)&matrixRight.m[0];
		iMatrixRight.val[1] = *(float32x4_t *)&matrixRight.m[3];
		iMatrixRight.val[2] = *(float32x4_t *)&matrixRight.m[5];

		iMatrixLeft.val[2] = vextq_f32(iMatrixLeft.val[2], iMatrixLeft.val[2], 1); // 6 7 8 x

		mm.val[0] = vmulq_n_f32(iMatrixLeft.val[0], vgetq_lane_f32(iMatrixRight.val[0], 0));
		mm.val[1] = vmulq_n_f32(iMatrixLeft.val[0], vgetq_lane_f32(iMatrixRight.val[0], 3));
		mm.val[2] = vmulq_n_f32(iMatrixLeft.val[0], vgetq_lane_f32(iMatrixRight.val[1], 3));

		mm.val[0] = vmlaq_n_f32(mm.val[0], iMatrixLeft.val[1], vgetq_lane_f32(iMatrixRight.val[0], 1));
		mm.val[1] = vmlaq_n_f32(mm.val[1], iMatrixLeft.val[1], vgetq_lane_f32(iMatrixRight.val[1], 1));
		mm.val[2] = vmlaq_n_f32(mm.val[2], iMatrixLeft.val[1], vgetq_lane_f32(iMatrixRight.val[2], 2));

		mm.val[0] = vmlaq_n_f32(mm.val[0], iMatrixLeft.val[2], vgetq_lane_f32(iMatrixRight.val[0], 2));
		mm.val[1] = vmlaq_n_f32(mm.val[1], iMatrixLeft.val[2], vgetq_lane_f32(iMatrixRight.val[1], 2));
		mm.val[2] = vmlaq_n_f32(mm.val[2], iMatrixLeft.val[2], vgetq_lane_f32(iMatrixRight.val[2], 3));

		*(float32x4_t *)&m.m[0] = mm.val[0];
		*(float32x4_t *)&m.m[3] = mm.val[1];
		*(float32x2_t *)&m.m[6] = vget_low_f32(mm.val[2]);
		m.m[8] = vgetq_lane_f32(mm.val[2], 2);

		return m;
#else
		GLKMatrix3 m;

		m.m[0] = matrixLeft.m[0] * matrixRight.m[0] + matrixLeft.m[3] * matrixRight.m[1] + matrixLeft.m[6] * matrixRight.m[2];
		m.m[3] = matrixLeft.m[0] * matrixRight.m[3] + matrixLeft.m[3] * matrixRight.m[4] + matrixLeft.m[6] * matrixRight.m[5];
		m.m[6] = matrixLeft.m[0] * matrixRight.m[6] + matrixLeft.m[3] * matrixRight.m[7] + matrixLeft.m[6] * matrixRight.m[8];

		m.m[1] = matrixLeft.m[1] * matrixRight.m[0] + matrixLeft.m[4] * matrixRight.m[1] + matrixLeft.m[7] * matrixRight.m[2];
		m.m[4] = matrixLeft.m[1] * matrixRight.m[3] + matrixLeft.m[4] * matrixRight.m[4] + matrixLeft.m[7] * matrixRight.m[5];
		m.m[7] = matrixLeft.m[1] * matrixRight.m[6] + matrixLeft.m[4] * matrixRight.m[7] + matrixLeft.m[7] * matrixRight.m[8];

		m.m[2] = matrixLeft.m[2] * matrixRight.m[0] + matrixLeft.m[5] * matrixRight.m[1] + matrixLeft.m[8] * matrixRight.m[2];
		m.m[5] = matrixLeft.m[2] * matrixRight.m[3] + matrixLeft.m[5] * matrixRight.m[4] + matrixLeft.m[8] * matrixRight.m[5];
		m.m[8] = matrixLeft.m[2] * matrixRight.m[6] + matrixLeft.m[5] * matrixRight.m[7] + matrixLeft.m[8] * matrixRight.m[8];

		return m;
#endif
	}

	static __inline GLKMatrix3 GLKMatrix3Add(GLKMatrix3 matrixLeft, GLKMatrix3 matrixRight)
	{
#if defined(__ARM_NEON__)
		GLKMatrix3 m;

		*(float32x4_t *)&(m.m[0]) = vaddq_f32(*(float32x4_t *)&(matrixLeft.m[0]), *(float32x4_t *)&(matrixRight.m[0]));
		*(float32x4_t *)&(m.m[4]) = vaddq_f32(*(float32x4_t *)&(matrixLeft.m[4]), *(float32x4_t *)&(matrixRight.m[4]));
		m.m[8] = matrixLeft.m[8] + matrixRight.m[8];

		return m;
#else
		GLKMatrix3 m;

		m.m[0] = matrixLeft.m[0] + matrixRight.m[0];
		m.m[1] = matrixLeft.m[1] + matrixRight.m[1];
		m.m[2] = matrixLeft.m[2] + matrixRight.m[2];

		m.m[3] = matrixLeft.m[3] + matrixRight.m[3];
		m.m[4] = matrixLeft.m[4] + matrixRight.m[4];
		m.m[5] = matrixLeft.m[5] + matrixRight.m[5];

		m.m[6] = matrixLeft.m[6] + matrixRight.m[6];
		m.m[7] = matrixLeft.m[7] + matrixRight.m[7];
		m.m[8] = matrixLeft.m[8] + matrixRight.m[8];

		return m;
#endif
	}

	static __inline GLKMatrix3 GLKMatrix3Subtract(GLKMatrix3 matrixLeft, GLKMatrix3 matrixRight)
	{
#if defined(__ARM_NEON__)
		GLKMatrix3 m;

		*(float32x4_t *)&(m.m[0]) = vsubq_f32(*(float32x4_t *)&(matrixLeft.m[0]), *(float32x4_t *)&(matrixRight.m[0]));
		*(float32x4_t *)&(m.m[4]) = vsubq_f32(*(float32x4_t *)&(matrixLeft.m[4]), *(float32x4_t *)&(matrixRight.m[4]));
		m.m[8] = matrixLeft.m[8] - matrixRight.m[8];

		return m;
#else
		GLKMatrix3 m;

		m.m[0] = matrixLeft.m[0] - matrixRight.m[0];
		m.m[1] = matrixLeft.m[1] - matrixRight.m[1];
		m.m[2] = matrixLeft.m[2] - matrixRight.m[2];

		m.m[3] = matrixLeft.m[3] - matrixRight.m[3];
		m.m[4] = matrixLeft.m[4] - matrixRight.m[4];
		m.m[5] = matrixLeft.m[5] - matrixRight.m[5];

		m.m[6] = matrixLeft.m[6] - matrixRight.m[6];
		m.m[7] = matrixLeft.m[7] - matrixRight.m[7];
		m.m[8] = matrixLeft.m[8] - matrixRight.m[8];

		return m;
#endif
	}

	static __inline GLKMatrix3 GLKMatrix3Scale(GLKMatrix3 matrix, float sx, float sy, float sz)
	{
		GLKMatrix3 m = { matrix.m[0] * sx, matrix.m[1] * sx, matrix.m[2] * sx,
			matrix.m[3] * sy, matrix.m[4] * sy, matrix.m[5] * sy,
			matrix.m[6] * sz, matrix.m[7] * sz, matrix.m[8] * sz };
		return m;
	}

	static __inline GLKMatrix3 GLKMatrix3ScaleWithVector3(GLKMatrix3 matrix, GLKVector3 scaleVector)
	{
		GLKMatrix3 m = { matrix.m[0] * scaleVector.v[0], matrix.m[1] * scaleVector.v[0], matrix.m[2] * scaleVector.v[0],
			matrix.m[3] * scaleVector.v[1], matrix.m[4] * scaleVector.v[1], matrix.m[5] * scaleVector.v[1],
			matrix.m[6] * scaleVector.v[2], matrix.m[7] * scaleVector.v[2], matrix.m[8] * scaleVector.v[2] };
		return m;
	}

	static __inline GLKMatrix3 GLKMatrix3ScaleWithVector4(GLKMatrix3 matrix, GLKVector4 scaleVector)
	{
		GLKMatrix3 m = { matrix.m[0] * scaleVector.v[0], matrix.m[1] * scaleVector.v[0], matrix.m[2] * scaleVector.v[0],
			matrix.m[3] * scaleVector.v[1], matrix.m[4] * scaleVector.v[1], matrix.m[5] * scaleVector.v[1],
			matrix.m[6] * scaleVector.v[2], matrix.m[7] * scaleVector.v[2], matrix.m[8] * scaleVector.v[2] };
		return m;
	}

	static __inline GLKMatrix3 GLKMatrix3Rotate(GLKMatrix3 matrix, float radians, float x, float y, float z)
	{
		GLKMatrix3 rm = GLKMatrix3MakeRotation(radians, x, y, z);
		return GLKMatrix3Multiply(matrix, rm);
	}

	static __inline GLKMatrix3 GLKMatrix3RotateWithVector3(GLKMatrix3 matrix, float radians, GLKVector3 axisVector)
	{
		GLKMatrix3 rm = GLKMatrix3MakeRotation(radians, axisVector.v[0], axisVector.v[1], axisVector.v[2]);
		return GLKMatrix3Multiply(matrix, rm);
	}

	static __inline GLKMatrix3 GLKMatrix3RotateWithVector4(GLKMatrix3 matrix, float radians, GLKVector4 axisVector)
	{
		GLKMatrix3 rm = GLKMatrix3MakeRotation(radians, axisVector.v[0], axisVector.v[1], axisVector.v[2]);
		return GLKMatrix3Multiply(matrix, rm);
	}

	static __inline GLKMatrix3 GLKMatrix3RotateX(GLKMatrix3 matrix, float radians)
	{
		GLKMatrix3 rm = GLKMatrix3MakeXRotation(radians);
		return GLKMatrix3Multiply(matrix, rm);
	}

	static __inline GLKMatrix3 GLKMatrix3RotateY(GLKMatrix3 matrix, float radians)
	{
		GLKMatrix3 rm = GLKMatrix3MakeYRotation(radians);
		return GLKMatrix3Multiply(matrix, rm);
	}

	static __inline GLKMatrix3 GLKMatrix3RotateZ(GLKMatrix3 matrix, float radians)
	{
		GLKMatrix3 rm = GLKMatrix3MakeZRotation(radians);
		return GLKMatrix3Multiply(matrix, rm);
	}

	static __inline GLKVector3 GLKMatrix3MultiplyVector3(GLKMatrix3 matrixLeft, GLKVector3 vectorRight)
	{
		GLKVector3 v = { matrixLeft.m[0] * vectorRight.v[0] + matrixLeft.m[3] * vectorRight.v[1] + matrixLeft.m[6] * vectorRight.v[2],
			matrixLeft.m[1] * vectorRight.v[0] + matrixLeft.m[4] * vectorRight.v[1] + matrixLeft.m[7] * vectorRight.v[2],
			matrixLeft.m[2] * vectorRight.v[0] + matrixLeft.m[5] * vectorRight.v[1] + matrixLeft.m[8] * vectorRight.v[2] };
		return v;
	}

	static __inline void GLKMatrix3MultiplyVector3Array(GLKMatrix3 matrix, GLKVector3 *vectors, size_t vectorCount)
	{
		int i;
		for (i = 0; i < vectorCount; i++)
			vectors[i] = GLKMatrix3MultiplyVector3(matrix, vectors[i]);
	}

#ifdef __cplusplus
}
#endif

#endif /* __GLK_MATRIX_3_H */