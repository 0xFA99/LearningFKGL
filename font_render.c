#include "font_render.h"

#include <ft2build.h>
#include FT_FREETYPE_H

void
initFontRender(FT_Library *ft)
{
	if (FT_Init_FreeType(&ft)) {
		fprintf(stderr, "ERROR: Could not init FreeType library.\n");
		return;
	}
}

void
renderText(Shader *shader, const char *text, float x, float y, float scale, Vector3f color)
{
	glUseProgram(&shader->programID);
	shaderUniformVec3f(shader->programID, "textColor", color);
	glActiveTexture(GL_TEXTURE1);
	// TODO: VAO
	glBindVertexArray(VAO);

	for (int i = 0; i < strlen(text); i++) {
		// Character ch = c

		float xPos = x + ch.bearing.x * scale;
		float yPos = y - (ch.size.y - ch.bearing.y) * scale;

		float w = ch.size.x * scale;
		float h = ch.size.y * scale;

		float vertices[6][4] = {
			{ xPos,		yPos + h,	0.0f, 0.0f },
			{ xPos,		yPos,		0.0f, 1.0f },
			{ xPos + w,	yPos,		1.0f, 1.0f },

			{ xPos,		yPos + h,	0.0f, 0.0f },
			{ xPos + w,	yPos,		1.0f, 1.0f },
			{ xPos + w, yPos + h,	1.0f, 0.0f }
		};

		glBindTexture(GL_TEXTURE_2D, ch.textureID);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// 2^6 = 64
		x += (ch.advance >> 6) * scale;
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
