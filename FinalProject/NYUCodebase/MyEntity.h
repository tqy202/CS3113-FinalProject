#pragma once

#include "ShaderProgram.h"
#include "SheetSprite.h"
#include <math.h>
#include <iostream>

#ifndef PI
#define PI 3.14159265
#endif


class Entity {
public:
	Entity();
	Entity(float x, float y, float wid, float hei, SheetSprite* tex = nullptr);
	Entity(float x, float y, float wid, float hei, float ang = 0, float vel = 0,SheetSprite* tex = nullptr);
	void Update(float elapsed);
	void Render(ShaderProgram *program);
//acessors
	float x();
	float y();
//mutators
	void move(float newX, float newY);
	void x(float distance, bool isNewCoord = false);
	void y(float distance, bool isNewCoord = false);

	SheetSprite* sprite;
	float width;
	float height;
	float angle;
	float velocity;
private:
	Matrix modelMatrix;
	float xpos;
	float ypos;
};