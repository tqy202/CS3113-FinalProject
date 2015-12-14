#pragma once

#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>

class SheetSprite {
public:
	SheetSprite();
	SheetSprite(GLuint* textureID, float spriteX, float spriteY, size_t spriteNum);
	GLuint* texture;
	float u;
	float v;
	float sheetX;
	float sheetY;
	size_t spriteNum;
};

class Texture{
public:
	Texture(GLuint &tex);
	Texture(GLuint &tex, unsigned int height, unsigned int width);

private:
	std::vector<SheetSprite*> sheetSprites;
	GLuint texture;
};
