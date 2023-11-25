#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "mymath.h"

typedef struct {
	Matrix4f position;
	Matrix4f rotation;
	Matrix4f scaling;
} Transform;

Matrix4f translate(Matrix4f mat, Vector3f position);
Matrix4f rotate(Matrix4f mat, float angle, Vector3f axis);
Matrix4f scale(Matrix4f mat, Vector3f scale);
Matrix4f view(Vector3f pos, Vector3f center, Vector3f up);
Matrix4f perspective(float fovy, float aspect, float zNear, float zFar);
Matrix4f ortho(float left, float right, float bottom, float top, float zNear, float zFar);

#endif
