#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#include "mymath.h"

typedef struct {
	unsigned int programID;
} Shader;

void createShader(Shader *shader, const char *vertexPath, const char *fragmentPath);

void shaderUniformMat4f(Shader *shader, const char *text, Matrix4f mat);
void shaderUniformInt(Shader *shader, const char *text, int value);
void shaderUniformVec3f(Shader *shader, const char *text, Vector3f vec);

#endif
