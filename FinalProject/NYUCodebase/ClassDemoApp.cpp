#include "ClassDemoApp.h"
#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif
GLuint* LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);
	GLuint* textureID = new GLuint();
	glGenTextures(1, textureID);
	glBindTexture(GL_TEXTURE_2D, *textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SDL_FreeSurface(surface);
	return textureID;
}
ClassDemoApp::ClassDemoApp() {
	Setup();
}
void ClassDemoApp::Setup() {
	// SDL and OpenGL initialization
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
	glewInit();
#endif
	lastFrameTicks = 0.0f;
	glViewport(0, 0, 800, 600);
	program = new ShaderProgram(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");
	projectionMatrix.setOrthoProjection(-1.33f, 1.33f, -1.0f, 1.0f, -1.0f, 1.0f);
	done = false;
	textures.push_back(LoadTexture("pixel_font.png"));
	textures.push_back(LoadTexture("sprites.png"));
	state = STATE_GAME;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	players.push_back(new Entity(-0.25, -0.9 ,0.1,.1,nullptr));
	players.push_back(new Entity(0.25, -0.9, 0.1, .1, nullptr));
}

ClassDemoApp::~ClassDemoApp() {
	clear();
	SDL_Quit();
}

void ClassDemoApp::DrawText(int fontTexture, std::string text, float size, float spacing) {
	float texture_size = 1.0 / 16.0f;
	std::vector<float> vertexData;
	std::vector<float> texCoordData;
	for (size_t i = 0; i < text.size(); i++) {
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;
		vertexData.insert(vertexData.end(), {
			((size + spacing) * i) + (-0.5f * size), 0.5f * size,
			((size + spacing) * i) + (-0.5f * size), -0.5f * size,
			((size + spacing) * i) + (0.5f * size), 0.5f * size,
			((size + spacing) * i) + (0.5f * size), -0.5f * size,
			((size + spacing) * i) + (0.5f * size), 0.5f * size,
			((size + spacing) * i) + (-0.5f * size), -0.5f * size,
		});
		texCoordData.insert(texCoordData.end(), {
			texture_x, texture_y,
			texture_x, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x + texture_size, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x, texture_y + texture_size,
		});
	}
	glUseProgram(program->programID);
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
	glEnableVertexAttribArray(program->positionAttribute);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
	glEnableVertexAttribArray(program->texCoordAttribute);
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glDrawArrays(GL_TRIANGLES, 0, text.size() * 6);
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}

void ClassDemoApp::Render() {
	program->setModelMatrix(modelMatrix);
	program->setProjectionMatrix(projectionMatrix);
	program->setViewMatrix(viewMatrix);
	glClear(GL_COLOR_BUFFER_BIT);
	switch (state) {
	case STATE_MENU:
		RenderMenu();
		break;
	case STATE_GAME:
		RenderGame();
		break;
	case STATE_END:
		break;
	}
	SDL_GL_SwapWindow(displayWindow);
}

void ClassDemoApp::ProcessEvents() {
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		if (keys[SDL_SCANCODE_UP]){ 
			players[PLAYER_2]->y(PLAYERSPEED);
			if (players[PLAYER_2]->y() + (players[PLAYER_2]->height/2 + .001) > orthMaxY){				
				players[PLAYER_2]->y(orthMaxY - (players[PLAYER_2]->height / 2 + 0.001), true);
			}

		}

		if (keys[SDL_SCANCODE_DOWN]){
			players[PLAYER_2]->y(-1 * PLAYERSPEED);
			if (players[PLAYER_2]->y() - (players[PLAYER_2]->height / 2 + .001) < orthMinY){
				players[PLAYER_2]->y(orthMinY + (players[PLAYER_2]->height / 2 + 0.001), true);
			}
		}

		if (keys[SDL_SCANCODE_LEFT]){
			players[PLAYER_2]->x(-1 * PLAYERSPEED);
			if (players[PLAYER_2]->x() + (players[PLAYER_2]->width / 2 + .001) < 0){
				players[PLAYER_2]->x(orthMaxX - (players[PLAYER_2]->width / 2 + 0.001), true);
			}
		}

		if (keys[SDL_SCANCODE_RIGHT]){
			players[PLAYER_2]->x(PLAYERSPEED);
			if (players[PLAYER_2]->x() - (players[PLAYER_2]->width / 2 + .001) > orthMaxX){
				players[PLAYER_2]->x(0 + (players[PLAYER_2]->width / 2 + 0.001), true);
			}
		}

		if (keys[SDL_SCANCODE_0]){ players[PLAYER_2]->shootBullet(bullets);}
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		if (keys[SDL_SCANCODE_W]){
			players[PLAYER_1]->y(PLAYERSPEED);
			if (players[PLAYER_1]->y() + (players[PLAYER_1]->height / 2 + .001) > orthMaxY){
				players[PLAYER_1]->y(orthMaxY - (players[PLAYER_1]->height / 2 + 0.001), true);
			}
		}

		if (keys[SDL_SCANCODE_S]){
			players[PLAYER_1]->y(-1*PLAYERSPEED);
			if (players[PLAYER_1]->y() - (players[PLAYER_1]->height / 2 + .001) < orthMinY){
				players[PLAYER_1]->y(orthMinY + (players[PLAYER_1]->height / 2 + 0.001), true);
			}
		}

		if (keys[SDL_SCANCODE_A]){
			players[PLAYER_1]->x(-1*PLAYERSPEED);
			if (players[PLAYER_1]->x() - (players[PLAYER_1]->width / 2 + .001) < orthMinX){
				players[PLAYER_1]->x(0 - (players[PLAYER_1]->width / 2 + 0.001), true);
			}
		}

		if (keys[SDL_SCANCODE_D]){
			players[PLAYER_1]->x(PLAYERSPEED);
			if (players[PLAYER_1]->x() + (players[PLAYER_1]->width / 2 + .001) > 0){
				players[PLAYER_1]->x(orthMinX + (players[PLAYER_1]->width / 2 + 0.001), true);
			}
		}

		if (keys[SDL_SCANCODE_SPACE]){ players[PLAYER_1]->shootBullet(bullets); }
	}
}

void ClassDemoApp::Update(float elapsed) {
	if (state == 1){
		for (Entity* itr : bullets){
			itr->Update(elapsed);
		}
	}
}


bool ClassDemoApp::UpdateAndRender() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;
	ProcessEvents();
	Update(elapsed);
	Render();
	return done;
}


//private functions
void ClassDemoApp::clear(){
	for (Entity* ent : players) { delete ent; }
	players.clear();
	for (Entity* ent : entities) { delete ent; }
	entities.clear();
	for (Entity* ent : bullets) { delete ent; }
	bullets.clear();
	for (GLuint* tex : textures) { delete tex; }
	textures.clear();
}

void ClassDemoApp::UpdateGame(){
	for (std::list<Entity*>::iterator itr = bullets.begin(); itr != bullets.end(); itr++){
		if (players[0]->collisionDetection(*itr)){
			delete *itr;
			itr = bullets.erase(itr);
			itr--;
		}
		if (players[1]->collisionDetection(*itr)){
			delete *itr;
			itr = bullets.erase(itr);
			itr--;
		}
	}
}

void ClassDemoApp::RenderMenu(){
	modelMatrix.identity();
	modelMatrix.Translate(-0.69f, 0.7f, 0.0);
	//DrawText(fontTexture, "My SpaceInvaders Game", 0.13f, 0.0f);
}

void ClassDemoApp::RenderGame(){
	for (Entity* ent : bullets){
		ent->Render(program);
	}
	for (Entity* ent : players){
		ent->Render(program);
	}
	for (Entity* ent : entities){
		ent->Render(program);
	}
	program->setViewMatrix(viewMatrix);
}

void ClassDemoApp::win(){
	modelMatrix.identity();
	modelMatrix.Translate(-0.6f, 0.4f, 0.0);
	//DrawText(fontTexture, "You win", 0.13f, 0.0f);
}
void ClassDemoApp::lose(){
	modelMatrix.identity();
	modelMatrix.Translate(-0.6f, 0.4f, 0.0);
	//DrawText(fontTexture, "You lose", 0.13f, 0.0f);
}
