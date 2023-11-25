#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "mymath.h"
#include "transform.h"
#include "shader.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

GLFWwindow *window = NULL;

int
main(void)
{
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: Failed to Initialize GLFW.\n");
		exit(1);
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "ERROR: Failed to create window.\n");
		glfwTerminate();
		exit(1);
	}

	glfwMakeContextCurrent(window);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "ERROR: Failed to initialize GLEW.\n");
		glfwTerminate();
		exit(1);
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

	Shader newShader;
	createShader(&newShader, "shaders/basic_vertex.glsl", "shaders/basic_fragment.glsl");

	unsigned VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(newShader.programID);
		glBindVertexArray(VAO);

		Matrix4f modelMatrix = mat4f(1.0f);
		modelMatrix = translate(modelMatrix, vec3f(0.0f, 0.0f, 0.0f));
		modelMatrix = rotate(modelMatrix, (float)glfwGetTime(), vec3f(0.0f, 1.0f, 0.0f));
		modelMatrix = scale(modelMatrix, vec3f(1.0f, 1.0f, 1.0f));

		Matrix4f viewMatrix = mat4f(1.0f);
		viewMatrix = view(vec3f(0.0f, 0.0f, 1.0f), vec3f(0.0f, 0.0f, 0.0f), vec3f(0.0f, 1.0f, 0.0f));

		Matrix4f projectionMatrix = mat4f(1.0f);
		projectionMatrix = perspective(90.0f, (float)WINDOW_WIDTH / WINDOW_HEIGHT, 1.0f, 100.0f);

		shaderUniformMat4f(&newShader, "ModelMatrix", modelMatrix);
		shaderUniformMat4f(&newShader, "ViewMatrix", viewMatrix);
		shaderUniformMat4f(&newShader, "ProjectionMatrix", projectionMatrix);

		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glUseProgram(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

