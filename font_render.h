#ifndef FONT_RENDER_H
#define FONT_RENDER_H

#include "mymath.h"
#include "shader.h"

typedef struct {
	unsigned int textureID;
	Vector2i size;
	Vector2i bearing;
	unsigned int advance;
} Character;

#ifndef CHAR_ASCII_LIMIT
#define CHAR_ASCII_LIMIT 128
typedef struct {
	char *font_name;
	Character character[CHAR_ASCII_LIMIT];
} Characters;

void initFontRender(const char *fontPath);
void renderText(Shader *shader, const char *text, float x, float y, float scale, Vector3f color);

#endif

