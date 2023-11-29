#include <stdio.h>
#include <stdlib.h>

#include "shader.h"

static int
getUniformLocation(unsigned int programId, const char *text)
{
	if (!programId) {
		fprintf(stderr, "ERROR: Shader program is empty.\n");
		return -1;
	}

	int location = glGetUniformLocation(programId, text);
	if (location == -1) {
		fprintf(stderr, "ERROR: Uniform %s is not found.\n", text);
		return -1;
	}

	return location;
}

static char*
readShaderFile(const char *filename)
{
	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		fprintf(stderr, "ERROR: Failed to open file: %s.\n", filename);
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	long file_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	char *buffer = (char *)malloc(file_size + 1);
	if (buffer == NULL) {
		fclose(file);
		fprintf(stderr, "ERROR: Failed to allocation memory.\n");
		return NULL;
	}

	size_t bytes_read = fread(buffer, 1, file_size, file);
	if (bytes_read != (size_t)file_size) {
		fclose(file);
		free(buffer);
		fprintf(stderr, "ERROR: Failed to reading file: %s.\n", filename);
		return NULL;
	}

	buffer[file_size] = '\0';
	fclose(file);

	return buffer;
}

static unsigned int
compileShader(GLenum type, const char *shaderCode)
{
	unsigned int shaderID = glCreateShader(type);

	glShaderSource(shaderID, 1, &shaderCode, NULL);
	glCompileShader(shaderID);

	{ // Check compilation status
		int success;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
			fprintf(stderr, "ERROR: Shader compilation error: %s.\n", infoLog);
		}
	}

	return shaderID;
}

static unsigned int
linkShaders(unsigned int vertexShader, unsigned int fragmentShader)
{
	unsigned int programShader = glCreateProgram();
	glAttachShader(programShader, vertexShader);
	glAttachShader(programShader, fragmentShader);
	glLinkProgram(programShader);

	{ // Check shader linking status
		int success;
		glGetProgramiv(programShader, GL_LINK_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetProgramInfoLog(programShader, 512, NULL, infoLog);
			fprintf(stderr, "ERROR: Shader program linking error: %s.\n", infoLog);
		}
	}

	return programShader;
}

void
createShader(Shader *shader, const char *vertexPath, const char *fragmentPath)
{
	const char *vertexShaderSource   = readShaderFile(vertexPath);
	const char *fragmentShaderSource = readShaderFile(fragmentPath);

	unsigned int compiledVertex	  = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
	unsigned int compiledFragment = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	shader->programID = linkShaders(compiledVertex, compiledFragment);

	glDeleteShader(compiledVertex);
	glDeleteShader(compiledFragment);
}

void
shaderUniformMat4f(Shader *shader, const char *text, Matrix4f mat)
{
	int location = getUniformLocation(shader->programID, text);

	glUniformMatrix4fv(location, 1, GL_FALSE, &mat.m[0][0]);
}

void
shaderUniformInt(Shader *shader, const char *text, int value)
{
	int location = getUniformLocation(shader->programID, text);

	glUniform1i(location, value);
}

void
shaderUniformVec3f(Shader *shader, const char *text, Vector3f color)
{
	int location = getUniformLocation(shader->programID, text);

	glUniform3f(location, color.x, color.y, color.z);
}
