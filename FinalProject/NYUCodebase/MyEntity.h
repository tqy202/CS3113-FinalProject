#pragma once

#include "ShaderProgram.h"
#include "SheetSprite.h"
#include <math.h>
#include <iostream>
#include <list>

#ifndef PI
#define PI 3.14159265
#endif

#ifndef orthMaxX
#define orthMaxX 1.33
#endif

#ifndef orthMinX
#define orthMinX -1.33
#endif

#ifndef orthMaxY
#define orthMaxY 1.0
#endif

#ifndef orthMinY
#define orthMinY -1.0
#endif


class Entity {
public:
	Entity();
	Entity(float x, float y, float wid, float hei, SheetSprite* tex);
	Entity(float x, float y, float wid, float hei, float ang, float vel, SheetSprite* tex);
	bool Update(float elapsed);
	void Render(ShaderProgram *program);
	bool collisionDetection(Entity* anEntity);
	void shootBullet(std::list<Entity*>& bullets);
//acessors
	float x();
	float y();
	const Matrix& getMatrix();

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