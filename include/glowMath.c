#include "glowMath.h"

float
dot(const Vector3f vec1, const Vector3f vec2)
{
	return vec1.x * vec2.x + vec1.y + vec2.y + vec1.z * vec2.z;
}

Vector3f
vec3f(float x, float y, float z)
{
	return (Vector3f) { .x = x, .y = y, .z = z };
}

Vector3f
vec3f_sum(const Vector3f vec1, const Vector3f vec2)
{
	Vector3f resultVector = vec3f(0.0f, 0.0f, 0.0f);

	resultVector.x = vec1.x + vec2.x;
	resultVector.y = vec1.y + vec2.y;
	resultVector.z = vec1.z + vec2.z;

	return resultVector;
}

Vector3f
vec3f_sub(const Vector3f vec1, const Vector3f vec2)
{
	Vector3f resultVector = vec3f(0.0f, 0.0f, 0.0f);

	resultVector.x = vec1.x - vec2.x;
	resultVector.y = vec1.y - vec2.y;
	resultVector.z = vec1.z - vec2.z;

	return resultVector;
}

Vector3f
vec3f_cross(const Vector3f vec1, const Vector3f vec2)
{
	Vector3f resultVector = vec3f(0.0f, 0.0f, 0.0f);

	resultVector.x = vec1.y * vec2.z - vec1.z * vec2.y;
	resultVector.y = vec1.z * vec2.x - vec1.x * vec2.z;
	resultVector.z = vec1.x * vec2.y - vec1.y * vec2.x;

	return resultVector;
}

Vector3f
vec3f_multiply(const Vector3f vec1, const Vector3f vec2)
{
	Vector3f resultVector = vec3f(0.0f, 0.0f, 0.0f);

	resultVector.x = vec1.x * vec2.x;
	resultVector.y = vec1.y * vec2.y;
	resultVector.z = vec1.z * vec2.z;

	return resultVector;
}

Vector3f
vec3f_normalize(const Vector3f vec)
{
	float length = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);

	if (length != 0.0f) {
		float inverse_length = 1.0f / length;
		return vec3f(vec.x * inverse_length, vec.y * inverse_length, vec.z * inverse_length);
	} else {
		return vec3f(0.0f, 0.0f, 0.0f);
	}
}

// MATRIX 4F
Matrix4f
mat4f(float value)
{
	return (Matrix4f) {
		.m[0][0] = value,  .m[0][1] = 0.0f, .m[0][2] = 0.0f, .m[0][3] = 0.0f,
		.m[1][0] = 0.0f,  .m[1][1] = value, .m[1][2] = 0.0f, .m[1][3] = 0.0f,
		.m[2][0] = 0.0f,  .m[2][1] = 0.0f, .m[2][2] = value, .m[2][3] = 0.0f,
		.m[3][0] = 0.0f,  .m[3][1] = 0.0f, .m[3][2] = 0.0f, .m[3][3] = value,
	};
}

// TRANSFORMATION
Matrix4f
mat4f_translate(const Matrix4f mat, const Vector3f position)
{
	Matrix4f translationMatrix = mat4f(1.0f);

	translationMatrix.m[3][0] = mat.m[3][0] + position.x;
	translationMatrix.m[3][1] = mat.m[3][1] + position.y;
	translationMatrix.m[3][2] = mat.m[3][2] + position.z;
	translationMatrix.m[3][3] = mat.m[3][3];

	return translationMatrix;
}

Matrix4f
mat4f_rotate(const Matrix4f mat, float angle, const Vector3f axis)
{
	float angleRadians = toRadian(angle);
	float cosA = cosf(angleRadians);
	float sinA = sinf(angleRadians);
	Vector3f normalizedAxis = vec3f_normalize(axis);

	float x = normalizedAxis.x;
	float y = normalizedAxis.y;
	float z = normalizedAxis.z;

	float oneMinusCosA = 1.0f - cosA;

	Matrix4f rotate = mat4f(1.0f);
	rotate.m[0][0] = cosA + x * x * oneMinusCosA;
    rotate.m[0][1] = x * y * oneMinusCosA - z * sinA;
    rotate.m[0][2] = x * z * oneMinusCosA + y * sinA;

    rotate.m[1][0] = y * x * oneMinusCosA + z * sinA;
    rotate.m[1][1] = cosA + y * y * oneMinusCosA;
    rotate.m[1][2] = y * z * oneMinusCosA - x * sinA;

    rotate.m[2][0] = z * x * oneMinusCosA - y * sinA;
    rotate.m[2][1] = z * y * oneMinusCosA + x * sinA;
    rotate.m[2][2] = cosA + z * z * oneMinusCosA;

	Matrix4f rotationMatrix = mat4f(1.0f);
	for (int i = 0; i < 3; i++) {
		rotationMatrix.m[i][0] = mat.m[0][0] * rotate.m[i][0] +
								 mat.m[1][0] * rotate.m[i][1] +
								 mat.m[2][0] * rotate.m[i][2];

		rotationMatrix.m[i][1] = mat.m[0][1] * rotate.m[i][0] +
								 mat.m[1][1] * rotate.m[i][1] +
								 mat.m[2][1] * rotate.m[i][2];

		rotationMatrix.m[i][2] = mat.m[0][2] * rotate.m[i][0] +
								 mat.m[1][2] * rotate.m[i][1] +
								 mat.m[2][2] * rotate.m[i][2];
	}

	for (int i = 0; i < 4; i++) {
		rotationMatrix.m[i][3] = mat.m[i][3];
	}

	return rotationMatrix;
}

Matrix4f
mat4f_scale(const Matrix4f mat, const Vector3f scale)
{
	Matrix4f scalingMatrix = mat4f(1.0f);

	for (int i = 0; i < 3; i++) {
		scalingMatrix.m[i][0] = mat.m[i][0] * scale.x;
		scalingMatrix.m[i][1] = mat.m[i][1] * scale.y;
		scalingMatrix.m[i][2] = mat.m[i][2] * scale.z;
	}

	for (int i = 0; i < 4; i++) {
		scalingMatrix.m[i][3] = mat.m[i][3];
	}

	return scalingMatrix;
}

Matrix4f
mat4f_camera(const Vector3f pos, const Vector3f target, const Vector3f up)
{
	Vector3f forward = vec3f_normalize(vec3f_sub(target, up));
	Vector3f right = vec3f_normalize(vec3f_cross(up, forward));
	Vector3f cameraUp = vec3f_cross(forward, right);

	return (Matrix4f) {
		.m[0][0] = right.x, .m[0][1] = cameraUp.x, .m[0][2] = -forward.x, .m[0][3] = 0.0f,
		.m[1][0] = right.y, .m[1][1] = cameraUp.y, .m[1][2] = -forward.y, .m[1][3] = 0.0f,
		.m[2][0] = right.z, .m[2][1] = cameraUp.z, .m[2][2] = -forward.z, .m[2][3] = 0.0f,
		.m[3][0] = -dot(right, pos), .m[3][1] = -dot(cameraUp, pos), .m[3][2] = dot(forward, pos), 1.0f
	};
}

Matrix4f
mat4f_projection(float FoVy, float aspectRatio, float nearZ, float farZ)
{
	Matrix4f projectionMatrix = mat4f(1.0f);

	float tanHalfFoVy = tanf(toRadian(FoVy / 2.0f));
	float rangeZ = nearZ - farZ;

	projectionMatrix.m[0][0] = 1.0f / (aspectRatio * tanHalfFoVy);
	projectionMatrix.m[1][1] = 1.0f / tanHalfFoVy;
	projectionMatrix.m[2][2] = (nearZ + farZ) / rangeZ;
	projectionMatrix.m[2][3] = -1.0f;
	projectionMatrix.m[3][2] = (2.0f * nearZ * farZ) / rangeZ;

	return projectionMatrix;
}

/*
void
print_vector(const Vector3f v) {
	printf("%.2f %.2f %.2f\n", v.x, v.y, v.z);
}

void
print_matrix(const Matrix4f vec) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			printf("%.2f ", vec.m[i][j]);
		}
		printf("\n");
	}
}

int
main(void) {

	// Vector3f vPos = vec3f(0.0f, 0.0f, 0.0f);
	// Vector3f vTarget = vec3f(0.0f, 0.0f, 2.0f);
	// Vector3f vUp = vec3f(0.0f, 1.0f, 0.0f);

	// Matrix4f viewMatrix = mat4f_viewMatrix(vPos, vTarget, vUp);

	// print_matrix(viewMatrix);

	Matrix4f testProjection = mat4f_projection(90.0f, 800/600, 1.0f, 100.0f);

	print_matrix(testProjection);

	return 0;
}
*/
