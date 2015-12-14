#include "SheetSprite.h"
//this is for drawing our arrays

SheetSprite::SheetSprite(GLuint* textureID, float spriteX, float spriteY, size_t spriteNum) :
texture(textureID), sheetX(spriteX), sheetY(spriteY), spriteNum(spriteNum)
{
	float u = (float)(((int)spriteNum) % int(sheetX)) / (float)sheetX;
	float v = (float)(((int)spriteNum) / int(sheetY)) / (float)sheetY;


}


Texture::Texture(GLuint &tex) :
texture(tex)
{}
Texture::Texture(GLuint &tex, unsigned int height, unsigned int width):
texture(tex)
{}
