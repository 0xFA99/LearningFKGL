#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glowMath.h"
#include "shader.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main(void)
{
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: Could not initialize GLFW.\n");
		exit(1);
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *const window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "ERROR: Could not create a window.\n");
		glfwTerminate();
		exit(1);
	}

	glfwMakeContextCurrent(window);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "ERROR: Could not initialize GLEW.\n");
		glfwTerminate();
		exit(-1);
	}

	float vertices[] = {
		 0.0f, -0.5f,  0.0f,	1.0f, 0.0f, 0.0f,
		 0.0f,  0.5f,  0.0f, 	0.0f, 1.0f, 0.0f,

		-0.5f, -0.5f,  0.0f,	0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.0f, 	0.0f, 0.0f, 1.0f,

		 0.0f, -0.5f,  0.5f,	0.0f, 0.0f, 1.0f,
		 0.0f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f
	};

	unsigned int indices[] = {
		2, 0, 1,
		3, 0, 1,
		4, 0, 1,
		5, 0, 1
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * sizeof(float), vertices, GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	const char *vertexShaderSource = readGLSLFile("vertexShader.glsl");
	const char *fragmentShaderSource = readGLSLFile("fragmentShader.glsl");

	unsigned int compiledVertex = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
	unsigned int compiledFragment = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, compiledVertex);
	glAttachShader(shaderProgram, compiledFragment);
	glLinkProgram(shaderProgram);

	glDeleteShader(compiledVertex);
	glDeleteShader(compiledFragment);

	int modelMatrixLocation = glGetUniformLocation(shaderProgram, "ModelMatrix");
	int viewMatrixLocation = glGetUniformLocation(shaderProgram, "ViewMatrix");
	int projectionMatrixLocation = glGetUniformLocation(shaderProgram, "ProjectionMatrix");

	glEnableVertexAttribArray(0);

	while (!glfwWindowShouldClose(window)) {

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

        // Model
		Matrix4f modelMatrix = mat4f(1.0f);
		modelMatrix = mat4f_translate(modelMatrix, vec3f(0.0f, 0.0f, 0.0f));
		modelMatrix = mat4f_rotate(modelMatrix, (float)glfwGetTime() * 90.0f, vec3f(0.0f, 1.0f, 0.0f));
		modelMatrix = mat4f_scale(modelMatrix, vec3f(1.0f, 1.0f, 1.0f));

		// View
		Matrix4f viewMatrix = mat4f(1.0f);
		viewMatrix = mat4f_camera(vec3f(0.0f, 0.0f, 1.0f), vec3f(0.0f, 0.0f, -1.0f), vec3f(0.0f, 1.0f, 0.0f));

		// Projection
		Matrix4f projectionMatrix = mat4f(1.0f);
		projectionMatrix = mat4f_projection(45.0f, SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 100.0f);

		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix.m[0][0]);
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix.m[0][0]);
		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix.m[0][0]);

		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(shaderProgram);

	return 0;

}
