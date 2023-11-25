#include "mymath.h"

#include <math.h>

float
dot(Vector3f a, Vector3f b)
{
	Vector3f Result = vec3f_multiply_vec(a, b);

	return (Result.x + Result.y + Result.z);
}

Vector2i
vec2i(int x, int y)
{
	return (Vector2i) { .x = x, .y = y };
}

Vector3f
vec3f(float x, float y, float z)
{
	return (Vector3f) { .x = x, .y = y, .z = z };
}

Vector3f
vec3f_subtract(Vector3f a, Vector3f b)
{
	return (Vector3f) {
		.x = a.x - b.x,
		.y = a.y - b.y,
		.z = a.z - b.z
	};
}

Vector3f
vec3f_normalize(Vector3f vec)
{
	float length = sqrt(vec.x * vec.x +
						vec.y * vec.y +
						vec.z * vec.z);

	if (length != 0.0f) {
		float inverse_length = 1.0f / length;
		return vec3f(vec.x * inverse_length,
					 vec.y * inverse_length,
					 vec.z * inverse_length);
	} else {
		return vec3f(0.0f, 0.0f, 0.0f);
	}
}

Vector3f
vec3f_multiply_var(Vector3f vec, float value)
{
	Vector3f Result;

	Result.x = vec.x * value;
	Result.y = vec.y * value;
	Result.z = vec.z * value;

	return Result;
}

Vector3f
vec3f_multiply_vec(Vector3f a, Vector3f b)
{
	Vector3f Result;

	Result.x = a.x * b.x;
	Result.y = a.y * b.y;
	Result.z = a.z * b.z;

	return Result;
}

Vector3f
vec3f_cross(Vector3f x, Vector3f y)
{
	return (Vector3f) {
		.x = x.y * y.z - y.y * x.z,
		.y = x.z * y.x - y.z * x.x,
		.z = x.x * y.y - y.x * x.y
	};
}

Matrix4f
mat4f(float value)
{
	return (Matrix4f) {
		.m[0][0] = value, .m[0][1] = 0.0f,  .m[0][2] = 0.0f,  .m[0][3] = 0.0f,
		.m[1][0] = 0.0f,  .m[1][1] = value, .m[1][2] = 0.0f,  .m[1][3] = 0.0f,
		.m[2][0] = 0.0f,  .m[2][1] = 0.0f,  .m[2][2] = value, .m[2][3] = 0.0f,
		.m[3][0] = 0.0f,  .m[3][1] = 0.0f,  .m[3][2] = 0.0f,  .m[3][3] = value
	};
}

Matrix4f
mat4f_multiply_mat(Matrix4f matA, Matrix4f matB)
{
    Matrix4f Result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				Result.m[i][j] += matA.m[i][k] * matB.m[k][j];
			}
		}
	}

    return Result;
}

