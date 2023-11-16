#ifndef GLOW_MATH_H
#define GLOW_MATH_H

#include <math.h>

#define toRadian(x) (float)(((x) * 3.14 / 180.0f))
#define toDegree(x) (float)(((x) * 180.0f / 3.14))

typedef struct {
	float x, y, z;
} Vector3f;

typedef struct {
	float m[4][4];
} Matrix4f;

float dot(const Vector3f vec1, const Vector3f vec2);

Vector3f vec3f(float x, float y, float z);
Vector3f vec3f_sum(const Vector3f vec1, const Vector3f vec2);
Vector3f vec3f_sub(const Vector3f vec1, const Vector3f vec2);
Vector3f vec3f_cross(const Vector3f vec1, const Vector3f vec2);
Vector3f vec3f_multiply(const Vector3f vec1, const Vector3f vec2);
Vector3f vec3f_normalize(const Vector3f vec);

Matrix4f mat4f(float value);
Matrix4f mat4f_translate(const Matrix4f mat, const Vector3f position);
Matrix4f mat4f_rotate(const Matrix4f mat, float angle, const Vector3f axis);
Matrix4f mat4f_scale(const Matrix4f mat, const Vector3f scale);
Matrix4f mat4f_camera(const Vector3f pos, const Vector3f target, const Vector3f up);

Matrix4f mat4f_projection(float FoVY, float aspectRatio, float nearZ, float farZ);


#endif
