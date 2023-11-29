#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "mymath.h"
#include "transform.h"
#include "shader.h"
#include "texture.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

typedef struct {
    unsigned int textureID;
    Vector2i size;
    Vector2i bearing;
    unsigned int advance;
} Character;

typedef struct {
    char charIndex;
    Character character;
} Characters;

#define CHARACTERS_CAP 128
Characters characters[CHARACTERS_CAP] = {0};

GLFWwindow *window = NULL;

FT_Library ftLibrary;
FT_Face ftFace;

unsigned int VAO, VBO, EBO;
unsigned int VAO_Font, VBO_Font;

Shader ObjectShader;
Shader FontShader;
Texture newTexture;

void renderText(Shader *shader, const char *text, float x, float y, float scale, Vector3f color);

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

	// glEnable(GL_DEPTH_TEST);
	// glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "ERROR: Failed to initialize GLEW.\n");
		glfwTerminate();
		exit(1);
	}

	float vertices[] = {
		-0.5f, -0.5f,  0.5f,	0.33f, 0.25f,
		 0.5f, -0.5f,  0.5f,	0.66f, 0.25f,
		 0.5f,  0.5f,  0.5f,	0.66f, 0.50f,
		-0.5f,  0.5f,  0.5f,	0.33f, 0.50f,

		 0.5f, -0.5f,  0.5f,	1.00f, 0.50f,
		 0.5f, -0.5f, -0.5f,	1.00f, 0.75f,
		 0.5f,  0.5f, -0.5f,	0.66f, 0.75f,
		 0.5f,  0.5f,  0.5f,	0.66f, 0.50f,

		-0.5f,  0.5f,  0.5f,	0.33f, 0.50f,
		 0.5f,  0.5f,  0.5f,	0.66f, 0.50f,
		 0.5f,  0.5f, -0.5f,	0.66f, 0.75f,
		-0.5f,  0.5f, -0.5f,	0.33f, 0.75f,

		-0.5f, -0.5f, -0.5f,	0.00f, 0.75f,
		-0.5f, -0.5f,  0.5f,	0.00f, 0.50f,
		-0.5f,  0.5f,  0.5f,	0.33f, 0.50f,
		-0.5f,  0.5f, -0.5f,	0.33f, 0.75f,

		 0.5f, -0.5f, -0.5f,	0.66f, 1.00f,
		-0.5f, -0.5f, -0.5f,	0.33f, 1.00f,
		-0.5f,  0.5f, -0.5f,	0.33f, 0.75f,
		 0.5f,  0.5f, -0.5f,	0.66f, 0.75f,

	    -0.5f, -0.5f, -0.5f,	0.33f, 0.00f,
		 0.5f, -0.5f, -0.5f,	0.66f, 0.00f,
		 0.5f, -0.5f,  0.5f,    0.66f, 0.25f,
		-0.5f, -0.5f,  0.5f,	0.33f, 0.25f
	};

	unsigned int indices[] = {
		 0,  1,  2,      2,  3,  0,		// Front
		 4,  5,  6,      6,  7,  4,		// Right
		 8,  9, 10,     10, 11,  8,		// Top
		12, 13, 14,     14, 15, 12,		// Left
		16, 17, 18,		18, 19, 16,		// Back
		20, 21, 22,		22, 23, 20		// Bottom
	};

    createShader(&FontShader, "shaders/text_vertex.glsl", "shaders/text_fragment.glsl");
    glUseProgram(FontShader.programID);
    Matrix4f orthoMatrix = ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);
    shaderUniformMat4f(&FontShader, "projection", orthoMatrix);
    /*
	createShader(&ObjectShader, "shaders/texture_vertex.glsl", "shaders/texture_fragment.glsl");

	createTexture(&newTexture, "textures/grass_block_tex.png");

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	shaderUniformInt(&ObjectShader, "texture1", 0);
    */
	if (FT_Init_FreeType(&ftLibrary)) {
		fprintf(stderr, "ERROR: Failed to init FreeType.\n");
		exit(1);
	}

	if (FT_New_Face(ftLibrary, "./fonts/lemon.ttf", 0, &ftFace)) {
		fprintf(stderr, "ERROR: Failed to opened and read font.\n");
		exit(1);
	}
  
    FT_Set_Pixel_Sizes(ftFace, 0, 48);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; ++c) {
        if (FT_Load_Char(ftFace, c, FT_LOAD_RENDER)) {
            fprintf(stderr, "ERROR: Failed to load Glyph.\n");
            continue;
        }

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                ftFace->glyph->bitmap.width,
                ftFace->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                ftFace->glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character newCharacter = {
            texture,
            vec2i(ftFace->glyph->bitmap.width, ftFace->glyph->bitmap.rows),
            vec2i(ftFace->glyph->bitmap_left, ftFace->glyph->bitmap_top),
            (unsigned int)ftFace->glyph->advance.x
        };

        characters[c].charIndex = c;
        characters[c].character = newCharacter;
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    FT_Done_Face(ftFace);
    FT_Done_FreeType(ftLibrary);

    glGenVertexArrays(1, &VAO_Font);
    glGenBuffers(1, &VBO_Font);

    glBindVertexArray(VAO_Font);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Font);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT);

        /*
		glUseProgram(ObjectShader.programID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, newTexture.textureID);
		glBindVertexArray(VAO);

		Matrix4f modelMatrix = mat4f(1.0f);
		modelMatrix = translate(modelMatrix, vec3f(0.0f, 0.0f, 0.0f));
		modelMatrix = rotate(modelMatrix, (float)glfwGetTime(), vec3f(0.0f, 1.0f, 0.0f));
		modelMatrix = scale(modelMatrix, vec3f(1.0f, 1.0f, 1.0f));

		Matrix4f viewMatrix = mat4f(1.0f);
		viewMatrix = view(vec3f(0.0f, 0.0f, 2.0f), vec3f(0.0f, 0.0f, 0.0f), vec3f(0.0f, 1.0f, 0.0f));

		Matrix4f projectionMatrix = mat4f(1.0f);
		projectionMatrix = perspective(90.0f, (float)WINDOW_WIDTH / WINDOW_HEIGHT, 1.0f, 100.0f);

		shaderUniformMat4f(&ObjectShader, "ModelMatrix", modelMatrix);
		shaderUniformMat4f(&ObjectShader, "ViewMatrix", viewMatrix);
		shaderUniformMat4f(&ObjectShader, "ProjectionMatrix", projectionMatrix);

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
		glUseProgram(0);
        */

        // FONT
        renderText(&FontShader, "Hello World", 25.0f, 25.0f, 1.0f, vec3f(0.5f, 0.8f, 0.2f));

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void
renderText(Shader *shader, const char *text, float x, float y, float scale, Vector3f color)
{
    glUseProgram(shader->programID);
    shaderUniformVec3f(shader, "textColor", color); 
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO_Font);

    for (size_t c = 0; c < strlen(text); ++c) {
        Character ch = characters[c].character;

        float xPos = x + ch.bearing.x * scale;
        float yPos = y - (ch.size.y - ch.bearing.y) * scale;
        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        float vertices[6][4] = {
            { xPos,     yPos + h,   0.0f, 0.0f },
            { xPos,     yPos,       0.0f, 1.0f },
            { xPos + w, yPos,       1.0f, 1.0f },

            { xPos,     yPos + h,   0.0f, 0.0f },
            { xPos + w, yPos,       1.0f, 1.0f },
            { xPos + w, yPos + h,   1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_Font);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch.advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

