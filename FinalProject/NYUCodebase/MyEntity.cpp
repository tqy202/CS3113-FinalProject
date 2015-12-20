#include "MyEntity.h"
#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

Entity::Entity(float x, float y, float wid, float hei, SheetSprite* tex = nullptr) :
xpos(x), ypos(y), width(wid), height(hei), sprite(tex), angle(0), velocity(0)
{
	modelMatrix.identity();
	modelMatrix.Translate(xpos, ypos, 0);
}
Entity::Entity(float x, float y, float wid, float hei, float ang, float vel, SheetSprite* tex = nullptr) :
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
	return false;
}

void Entity::Render(ShaderProgram *program){
	/*float vertex[] = { xpos - (.5 * width), ypos + (.5 * height),
		xpos - (.5 * width), ypos - (.5 * height),
		xpos + (.5 * width), ypos - (.5 * height),
		xpos + (.5 * width), ypos - (.5 * height),
		xpos + (.5 * width), ypos + (.5 * height),
		xpos - (.5 * width), ypos + (.5 * height)
	};*/
	float vertex[] = {  - (.5 * width),  + (.5 * height),
		 -(.5 * width),  - (.5 * height),
		 + (.5 * width),  - (.5 * height),
		 + (.5 * width),  - (.5 * height),
		 + (.5 * width),  + (.5 * height),
		 - (.5 * width),  + (.5 * height)
	};
	
	glUseProgram(program->programID);
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertex);
	glEnableVertexAttribArray(program->positionAttribute);
	if (sprite != nullptr){
		//float texCoordData[] = {};
		//glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData);
		//glEnableVertexAttribArray(program->texCoordAttribute)
		
		float u = sprite->u;
		float v = sprite->v;
		float spriteWidth = 1.0 / (float)16;
		float spriteHeight = 1.0 / (float)16;


		GLfloat texCoords[] = { u, v + spriteHeight, u + spriteWidth, v, u, v, u + spriteWidth, v, u, v + spriteHeight, u + spriteWidth, v + spriteHeight };
		glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glEnableVertexAttribArray(program->texCoordAttribute);
		glBindTexture(GL_TEXTURE_2D, *sprite->texture);

	}
	program->setModelMatrix(modelMatrix);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(program->positionAttribute);
	if (sprite == nullptr)
		glDisableVertexAttribArray(program->texCoordAttribute);


}
bool Entity::Update(float elapsed){
	float changeX = velocity * cos(angle*PI/180.0) * elapsed;
	float changeY = velocity * sin(angle*PI / 180.0) * elapsed;
	xpos += changeX;
	ypos += changeY;
	//modelMatrix.Rotate(0.2 * 360);
	modelMatrix.Translate(changeX, changeY, 0);
	modelMatrix.Rotate(0.1);
	
	/*if (this->ypos > orthMaxY){
		this->angle *= -1;
		this->xpos *= -1;
		this->velocity *= -1;
	}
	else if (this->ypos < orthMinY){
		delete this;
		return true;
	}*/
	return false;
}
//acessors
float Entity::x(){ return xpos; }
float Entity::y(){ return ypos; }
const Matrix& Entity::getMatrix(){ return modelMatrix; }

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

void Entity::shootBullet(std::list<Entity*>& bullets){
	bullets.push_back(new Entity(xpos, ypos + (height / 2) + 0.1, .01, .01, 90, 10));
}
