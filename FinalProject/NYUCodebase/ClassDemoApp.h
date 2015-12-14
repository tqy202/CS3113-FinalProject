#pragma once

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <vector>
#include <list>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <algorithm>

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Matrix.h"
#include "ShaderProgram.h"
#include "SheetSprite.h"
#include "MyEntity.h"


#ifndef PLAYERSPEED
#define PLAYERSPEED .01
#endif
#ifndef FIXED_TIMESTEP
#define FIXED_TIMESTEP 0.0166666f
#endif

enum LocationOfPlayah{ PLAYER_1, PLAYER_2 };
enum GameState { STATE_MENU, STATE_GAME, STATE_END };
GLuint* LoadTexture(const char *image_path);
class ClassDemoApp {
public:
	ClassDemoApp();
	~ClassDemoApp();
	void Setup();
	void ProcessEvents();
	bool UpdateAndRender();
	void Render();
	void Update(float elapsed);
private:
	float lastFrameTicks;
	SDL_Window* displayWindow;
	Matrix projectionMatrix;
	Matrix modelMatrix;
	Matrix viewMatrix;
	ShaderProgram *program;
	ShaderProgram *programT;
	SDL_Event event;
	size_t level;
	size_t blocksLeft;

	bool done;
	bool player1Wins;
	int state;
	const Uint8 *keys;// = SDL_GetKeyboardState(NULL);
	
	//bool amAlive;
	//Entity* player;
	std::vector<Entity*> players;
	std::list<Entity*> UI;
	std::list<Entity*> bullets;
	std::vector<GLuint*> textures;
	std::vector<Mix_Chunk*> sounds;

	void clear();
	void RenderMenu();
	void RenderGame();
	void win();
	void lose();
	void UpdateGame();
	float randomX();
	void DrawText(GLuint& fontTexture, std::string text, float size, float spacing, float location);

};