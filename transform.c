#include "transform.h"

#include <stdio.h>
#include <math.h>

Matrix4f
translate(Matrix4f mat, Vector3f position)
{
	// https://wikimedia.org/api/rest_v1/media/math/render/svg/a7bb4edd686c84393b22e27b4507f2e1ebc03aac
	
	mat.m[3][0] += position.x;
	mat.m[3][1] += position.y;
	mat.m[3][2] += position.z;

	return mat;
}

Matrix4f
rotate(Matrix4f mat, float angle, Vector3f axis)
{
	// https://wikimedia.org/api/rest_v1/media/math/render/svg/467b8626a8361b118452bfed7dd84a6b47855268

	float const a = angle;
	float const sinA = __builtin_sinf(a);
	float const cosA = __builtin_cosf(a);

	Vector3f normalizedAxis = vec3f_normalize(axis);

	Matrix4f Rotate = mat4f(1.0f);
	Rotate.m[0][0] = normalizedAxis.x * ((1.0f - cosA) * normalizedAxis.x) + cosA;
	Rotate.m[0][1] = normalizedAxis.y * (normalizedAxis.x * (1.0f - cosA)) + normalizedAxis.z * sinA;
	Rotate.m[0][2] = normalizedAxis.z * (normalizedAxis.x * (1.0f - cosA)) - normalizedAxis.y * sinA;

	Rotate.m[1][0] = normalizedAxis.x * (normalizedAxis.y * (1.0f - cosA)) - normalizedAxis.z * sinA;
	Rotate.m[1][1] = normalizedAxis.y * (normalizedAxis.y * (1.0f - cosA)) + cosA;
	Rotate.m[1][2] = normalizedAxis.z * (normalizedAxis.y * (1.0f - cosA)) + normalizedAxis.x * sinA;

	Rotate.m[2][0] = normalizedAxis.x * (normalizedAxis.z * (1.0f - cosA)) + normalizedAxis.y * sinA;
	Rotate.m[2][1] = normalizedAxis.y * (normalizedAxis.z * (1.0f - cosA)) - normalizedAxis.x * sinA;
	Rotate.m[2][2] = normalizedAxis.z * (normalizedAxis.z * (1.0f - cosA)) + cosA;

	Matrix4f Result = mat4f(1.0f);
	Result.m[0][0] = mat.m[0][0] * Rotate.m[0][0] + mat.m[1][0] * Rotate.m[0][1] + mat.m[2][0] * Rotate.m[0][2];
	Result.m[0][1] = mat.m[0][1] * Rotate.m[0][0] + mat.m[1][1] * Rotate.m[0][1] + mat.m[2][1] * Rotate.m[0][2];
	Result.m[0][2] = mat.m[0][2] * Rotate.m[0][0] + mat.m[1][2] * Rotate.m[0][1] + mat.m[2][2] * Rotate.m[0][2];

	Result.m[1][0] = mat.m[0][0] * Rotate.m[1][0] + mat.m[1][0] * Rotate.m[1][1] + mat.m[2][0] * Rotate.m[1][2];
	Result.m[1][1] = mat.m[0][1] * Rotate.m[1][0] + mat.m[1][1] * Rotate.m[1][1] + mat.m[2][1] * Rotate.m[1][2];
	Result.m[1][2] = mat.m[0][2] * Rotate.m[1][0] + mat.m[1][2] * Rotate.m[1][1] + mat.m[2][2] * Rotate.m[1][2];

	Result.m[2][0] = mat.m[0][0] * Rotate.m[2][0] + mat.m[1][0] * Rotate.m[2][1] + mat.m[2][0] * Rotate.m[2][2];
	Result.m[2][1] = mat.m[0][1] * Rotate.m[2][0] + mat.m[1][1] * Rotate.m[2][1] + mat.m[2][1] * Rotate.m[2][2];
	Result.m[2][2] = mat.m[0][2] * Rotate.m[2][0] + mat.m[1][2] * Rotate.m[2][1] + mat.m[2][2] * Rotate.m[2][2];

	Result.m[3][0] = mat.m[3][0];
	Result.m[3][1] = mat.m[3][1];
	Result.m[3][2] = mat.m[3][2];

	return Result;
}

Matrix4f
scale(Matrix4f mat, Vector3f scale)
{
	// https://wikimedia.org/api/rest_v1/media/math/render/svg/f8b981072c2d00c1ce373cd483c00fd6d927f668
	
	mat.m[0][0] *= scale.x;
	mat.m[1][1] *= scale.y;
	mat.m[2][2] *= scale.z;

	return mat;
}

Matrix4f
view(Vector3f pos, Vector3f center, Vector3f up)
{
	// https://4.bp.blogspot.com/-9usAHn8R0rY/T69efkqZ5nI/AAAAAAAAAU8/WDxyd_kP_ek/s1600/view_matrix.png
	
	Vector3f f = vec3f_normalize(vec3f_subtract(center, pos));
	Vector3f s = vec3f_normalize(vec3f_cross(f, up));
	Vector3f u = vec3f_cross(s, f);

	Matrix4f Result = mat4f(1.0f);
	Result.m[0][0] =  s.x;
	Result.m[1][0] =  s.y;
	Result.m[2][0] =  s.z;

	Result.m[0][1] =  u.x;
	Result.m[1][1] =  u.y;
	Result.m[2][1] =  u.z;

	Result.m[0][2] =  -f.x;
	Result.m[1][2] =  -f.y;
	Result.m[2][2] =  -f.z;

	Result.m[3][0] = -dot(s, pos);
	Result.m[3][1] = -dot(u, pos);
	Result.m[3][2] =  dot(f, pos);

	return Result;
}

Matrix4f
perspective(float fovy, float aspect, float zNear, float zFar)
{
	// https://i.stack.imgur.com/1qkwc.png
	
	float tanHalfFovy = tan(toRadian(fovy / 2.0f));

	Matrix4f Result = mat4f(1.0f);
	Result.m[0][0] = 1.0f / (aspect * tanHalfFovy);
	Result.m[1][1] = 1.0f / (tanHalfFovy);
	Result.m[2][2] = -(zFar + zNear) / (zFar - zNear);
	Result.m[2][3] = -1.0f;
	Result.m[3][2] = -(2.0f * zFar * zNear) / (zFar - zNear);

	return Result;
}

Matrix4f
ortho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	Matrix4f Result = mat4f(1.0f);
	Result.m[0][0] = 2.0f / (right - left);
	Result.m[1][1] = 2.0f / (top - bottom);
	Result.m[2][2] = -2.0f / (zFar - zNear);

	Result.m[3][0] = -(right + left) / (right - left);
	Result.m[3][1] = -(top + bottom) / (top - bottom);
	Result.m[3][2] = -(zFar + zNear) / (zFar - zNear);

	return Result;
}

