#ifndef MYMATH_H
#define MYMATH_H

#define toRadian(x) (float)(((x) * 0.01745329251994329576923690768489))
#define toDegree(x) (float)(((x) * 57.295779513082320876798154814105))

typedef struct {
	int x, y;
} Vector2i;

typedef struct {
	float x, y, z;
} Vector3f;

typedef struct {
	float m[4][4];
} Matrix4f;

float dot(Vector3f a, Vector3f b);

Vector2i vec2i(int x, int y);

Vector3f vec3f(float x, float y, float z);
Vector3f vec3f_subtract(Vector3f a, Vector3f b);
Vector3f vec3f_normalize(Vector3f vec);
Vector3f vec3f_multiply_var(Vector3f vec, float value);
Vector3f vec3f_multiply_vec(Vector3f a, Vector3f b);
Vector3f vec3f_cross(Vector3f x, Vector3f y);

Matrix4f mat4f(float value);
Matrix4f mat4f_multiply_mat(Matrix4f matA, Matrix4f matB);

#endif
