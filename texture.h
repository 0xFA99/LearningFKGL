#ifndef TEXTURE_H
#define TEXTURE_H

typedef struct {
	unsigned int textureID;
	int width;
	int height;
	int nrChannels;
} Texture;

void createTexture(Texture *tex, const char *texturePath);

#endif
