#include "MyEntity.h"

Entity::Entity(float x, float y, float wid, float hei, SheetSprite* tex) :
xpos(x), ypos(y), width(wid), height(hei), sprite(tex), angle(0), velocity(0)
{
	modelMatrix.identity();
	modelMatrix.Translate(xpos, ypos, 0);
}
Entity::Entity(float x, float y, float wid, float hei, float ang, float vel, SheetSprite* tex) :
xpos(x), ypos(y), width(wid), height(hei), sprite(tex), angle(ang), velocity(vel)
{
	modelMatrix.identity();
	modelMatrix.Translate(xpos, ypos, 0);
}

bool Entity::collisionDetection(Entity* anEntity){
	// Assume the player entity is on the left side of the screen and will continue to stay there for the duration of the game. We can always change the position of the player and therefore this code sometime later.
	if (!(this->x()+ this->width / 2 < anEntity->x() - anEntity->width/ 2 || this->x() - this->width / 2 > anEntity->x() + anEntity->width / 2 || this->y() + this->height/ 2 < anEntity->y() - anEntity->height/ 2 || this->y() - this->height / 2 > anEntity->y() + anEntity->height/ 2)){
		return true;
	}
}

void Entity::Render(ShaderProgram *program){
	float vertex[] = { xpos - (.5 * width), ypos + (.5 * height),
		xpos - (.5 * width), ypos - (.5 * height),
		xpos + (.5 * width), ypos - (.5 * height),
		xpos + (.5 * width), ypos - (.5 * height),
		xpos + (.5 * width), ypos + (.5 * height),
		xpos - (.5 * width), ypos + (.5 * height)
	};
	//float texCoordData[] = {};
	glUseProgram(program->programID);
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertex);
	glEnableVertexAttribArray(program->positionAttribute);
	//glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData);
	//glEnableVertexAttribArray(program->texCoordAttribute);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(program->positionAttribute);
	//glDisableVertexAttribArray(program->texCoordAttribute);

}
void Entity::Update(float elapsed){
	float changeX = velocity * cos(angle*PI/180.0);
	float changeY = velocity * sin(angle*PI / 180.0);
	xpos += changeX;
	ypos += changeY;
	modelMatrix.Translate(changeX, changeY, 0);
}
//acessors
float Entity::x(){ return xpos; }
float Entity::y(){ return ypos; }

//mutators
void Entity::move(float newX, float newY){
	xpos = newX;
	ypos = newY;
	modelMatrix.identity();
	modelMatrix.Translate(xpos, ypos, 0);
}

void Entity::x(float distance, bool isNewCoord){
	if (isNewCoord)
		move(distance, ypos);
	else{
		xpos += distance;
		modelMatrix.Translate(distance, 0, 0);
	}
}
void Entity::y(float distance, bool isNewCoord){
	if (isNewCoord)
		move(xpos, distance);
	else{
		ypos += distance;
		modelMatrix.Translate(0, distance, 0);
	}
}
