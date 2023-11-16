#include <stdio.h>
#include <stdlib.h>

#include "shader.h"

const char *readGLSLFile(const char *filename)
{
	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		fprintf(stderr, "ERROR: opening GLSL file: %s\n", filename);
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	long file_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	char *buffer = (char *)malloc(file_size + 1);
	if (buffer == NULL) {
		fclose(file);
		fprintf(stderr, "ERROR: Memory allocation error\n");
		return NULL;
	}

	size_t bytes_read = fread(buffer, 1, file_size, file);
	if (bytes_read != file_size) {
		fclose(file);
		free(buffer);
		fprintf(stderr, "ERROR: Reading GLSL file.\n");
		return NULL;
	}

	buffer[file_size] = '\0';

	fclose(file);
	return buffer;
}

unsigned int compileShader(GLenum shaderType, const char *shaderCode)
{
    unsigned int shaderID = glCreateShader(shaderType);

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

unsigned int linkShaders(unsigned int vertexShader, unsigned int fragmentShader)
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

/*
int main() {
	const char *vertexShader = readGLSLFile("vertexShader.glsl");
	const char *fragmentShader = readGLSLFile("fragmentShader.glsl");

	unsigned int vertexCompiled = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fragmentCompiled = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	return 0;
}
*/
