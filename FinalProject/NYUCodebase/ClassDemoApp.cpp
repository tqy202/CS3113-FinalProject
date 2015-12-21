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
	// SDL and OpenGL initialization
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Whoops", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	glViewport(0, 0, 800, 600);
#ifdef _WINDOWS
	glewInit();
#endif
	//Setup();
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	program = new ShaderProgram(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");
	programT =  new ShaderProgram(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	textures.push_back(LoadTexture("pixel_font.png"));
	textures.push_back(LoadTexture("Title.png"));
	sounds.push_back(Mix_LoadWAV("boom.wav"));
	sounds.push_back(Mix_LoadWAV("spawn.wav"));
	sounds.push_back(Mix_LoadWAV("win.wav"));
	sounds.push_back(Mix_LoadWAV("mainGame.wav"));
	sounds.push_back(Mix_LoadWAV("meow.wav"));
	musics.push_back(Mix_LoadMUS("8bitmusicloop.mp3"));
	UI.push_back(new Entity(0, 0, 2, 4, new SheetSprite(textures[1], 1, 3, 1)));
	state = STATE_MENU;
	lastFrameTicks = (float)SDL_GetTicks() / 1000.0f;
	Mix_PlayMusic(musics[0], -1);
}
void ClassDemoApp::Setup() {
	projectionMatrix.setOrthoProjection(orthMinX, orthMaxX, orthMinY, orthMaxY, -1.0f, 1.0f);
	viewMatrix.identity();
	modelMatrix.identity();
	done = false;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	players.push_back(new Entity(-0.25, -0.9 ,0.1, NORMAL_PLAYER_HEIGHT,nullptr));
	players.push_back(new Entity(0.25, -0.9, 0.1, NORMAL_PLAYER_HEIGHT, nullptr));
	UI.push_back(new Entity(0,0,0.01,2*orthMaxY, nullptr));

	level = 1;
	blocksLeft = 50;
	elapsedBuffer = 0.0f;
	
	srand(time(NULL));

	lastFrameTicks = (float)SDL_GetTicks() / 1000.0f;
	
}

ClassDemoApp::~ClassDemoApp() {
	clear();
	for (GLuint* tex : textures) { delete tex; }
	textures.clear();
	for (Mix_Chunk* tex : sounds) { Mix_FreeChunk(tex); }
	sounds.clear();
	for (Mix_Music* tex : musics) { Mix_FreeMusic(tex); }
	musics.clear();
	delete(program);
	delete(programT);
	SDL_Quit();
}

//void ClassDemoApp::DrawText(GLuint& fontTexture, std::string text, float size, float spacing, float location) {
	/*float texture_size = 1.0 / 16.0f;
	std::vector<float> vertexData;
	std::vector<float> texCoordData;
	for (size_t i = 0; i < text.size(); i++) {
		float //texture_x 
			u = (float)(((int)text[i]) % 16) / 16.0f;
		float //texture_y 
			v = (float)(((int)text[i]) / 16) / 16.0f;
		vertexData.insert(vertexData.end(), {
			((size + spacing) * i) + (-0.5f * size), 0.5f * size,
			((size + spacing) * i) + (-0.5f * size), -0.5f * size,
			((size + spacing) * i) + (0.5f * size), 0.5f * size,
			((size + spacing) * i) + (0.5f * size), -0.5f * size,
			((size + spacing) * i) + (0.5f * size), 0.5f * size,
			((size + spacing) * i) + (-0.5f * size), -0.5f * size,
		});
		texCoordData.insert(texCoordData.end(), { u, v + texture_size, u + texture_size, v, u, v, u + texture_size, v, u, v + texture_size, u + texture_size, v + texture_size });
		/*texCoordData.insert(texCoordData.end(), {
			texture_x, texture_y,
			texture_x, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x + texture_size, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x, texture_y + texture_size,
			});*//*
	}
	glUseProgram(program->programID);
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
	glEnableVertexAttribArray(program->positionAttribute);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
	glEnableVertexAttribArray(program->texCoordAttribute);
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glDrawArrays(GL_TRIANGLES, 0, text.size() * 6);
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);*/

	/*for (size_t x = 0; x < text.size(); x++){
		int index = text.at(x);
		float height = 0.15;
		float width = 0.15;
		float g = ((float)x + 1) * 2.0 / ((float)text.size() + 1) - 1.0;
		float u = (float)(((int)index) % 16) / (float)16;
		float v = (float)(((int)index) / 16) / (float)16;
		float spriteWidth = 1.0 / (float)16;
		float spriteHeight = 1.0 / (float)16;

		float vertices[] = { -1 * width, -1 * height,
			width, height,
			-1 * width, height,
			width, height,
			-1 * width, -1 * height,
			width, -1 * height };
		glVertexAttribPointer(programT->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
		glEnableVertexAttribArray(programT->positionAttribute);

		GLfloat texCoords[] = { u, v + spriteHeight, u + spriteWidth, v, u, v, u + spriteWidth, v, u, v + spriteHeight, u + spriteWidth, v + spriteHeight };
		glVertexAttribPointer(programT->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glEnableVertexAttribArray(programT->texCoordAttribute);
		glBindTexture(GL_TEXTURE_2D, fontTexture);

		modelMatrix.identity();
		modelMatrix.Translate(g, location, 0.0);
		programT->setModelMatrix(modelMatrix);
		programT->setProjectionMatrix(projectionMatrix);
		programT->setViewMatrix(viewMatrix);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(programT->positionAttribute);
		glDisableVertexAttribArray(programT->texCoordAttribute);
	}*/
void ClassDemoApp::DrawTextBox(GLuint& fontTexture, std::string text, float height, float width, float spacing, float xpos, float ypos, TextFocal focal = CENTER){
	float charWidth = (width - (spacing * (float)text.size())) / (float)text.size();
	float currentX;
	switch (focal){
	case LEFT:
		currentX = charWidth / 2.0f + xpos;
		break;
	case RIGHT:
		currentX = xpos - (charWidth * ((float)text.size())) + charWidth / 2.0f;
		break;
	case CENTER:
	default:
		currentX = xpos - (charWidth * ((float)text.size() / 2.0f)) + charWidth / 2.0f;
	}
	for (size_t x = 0; x < text.size(); x++){
		char index = text.at(x);
		DrawChar(fontTexture, index, height, charWidth, currentX, ypos);
		currentX += charWidth + spacing;
	}
}

void ClassDemoApp::DrawTextChar(GLuint& fontTexture, std::string text, float height, float width, float spacing, float xpos, float ypos, TextFocal focal = CENTER){
	float currentX;
	switch (focal){
	case LEFT:
		currentX = width / 2.0f + xpos;
		break;
	case RIGHT:
		currentX = xpos - ((width + spacing) * ((float)text.size())) + width / 2.0f;
		break;
	case CENTER:
	default:
		currentX = xpos - ((width + spacing) * ((float)text.size() / 2.0f)) + width / 2.0f;
	}
	for (size_t x = 0; x < text.size(); x++){
		char index = text.at(x);
		DrawChar(fontTexture, index, height, width, currentX, ypos);
		currentX += width + spacing;
	}
}

void ClassDemoApp::DrawChar(GLuint& fontTexture, char index, float height, float width, float xpos, float ypos){
	float u = (float)(((int)index) % 16) / (float)16;
	float v = (float)(((int)index) / 16) / (float)16;
	float spriteWidth = 1.0 / (float)16;
	float spriteHeight = 1.0 / (float)16;
	float heightH = height *.5;
	float widthH = width *.5;

	/*float vertices[] = { xpos - (.5 * width), ypos + (.5 * height),
		xpos - (.5 * width), ypos - (.5 * height),
		xpos + (.5 * width), ypos - (.5 * height),
		xpos + (.5 * width), ypos - (.5 * height),
		xpos + (.5 * width), ypos + (.5 * height),
		xpos - (.5 * width), ypos + (.5 * height) };*/
	float vertices[] = { xpos - (.5 * width), ypos - (.5 * height),
		xpos + (.5 * width), ypos + (.5 * height),
		xpos - (.5 * width), ypos + (.5 * height),
		xpos + (.5 * width), ypos + (.5 * height),
		xpos - (.5 * width), ypos - (.5 * height),
		xpos + (.5 * width), ypos - (.5 * height) };

	glVertexAttribPointer(programT->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(programT->positionAttribute);
	
	GLfloat texCoords[] = { u, v + spriteHeight, u + spriteWidth, v, u, v, u + spriteWidth, v, u, v + spriteHeight, u + spriteWidth, v + spriteHeight };
	glVertexAttribPointer(programT->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(programT->texCoordAttribute);
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	
	/*modelMatrix.identity();
	modelMatrix.Translate(xpos, ypos, 0.0);
	programT->setModelMatrix(modelMatrix);*/
	programT->setProjectionMatrix(projectionMatrix);
	programT->setViewMatrix(viewMatrix);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(programT->positionAttribute);
	glDisableVertexAttribArray(programT->texCoordAttribute);
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
	case STATE_LEVEL_TRANSITION:
		RenderGame();
		break;
	case STATE_END:
		//changes
		if (player1Wins){ win(); }
		else{ lose(); }
		DrawTextChar(*(textures[0]), "Press [Enter] to return to menu", 0.1, 0.1, -0.05f, 0, -.1);
		// changes
		break;
	}
	SDL_GL_SwapWindow(displayWindow);
}

void ClassDemoApp::ProcessEvents(float elasped) {
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}

		keys = SDL_GetKeyboardState(NULL);
		switch (state) {
		case STATE_MENU:
			if (keys[SDL_SCANCODE_RETURN]){
				//Mix_PlayChannel(-1, sounds[sounds.size() - 1], -1);
				state = STATE_GAME;
				clear();
				Setup();
			}
			else if (keys[SDL_SCANCODE_ESCAPE]){
				done = true;
			}
			break;
		case STATE_GAME:
		case STATE_LEVEL_TRANSITION:
			if (keys[SDL_SCANCODE_UP]){
				players[PLAYER_2]->y(PLAYERSPEED * elasped);
				if (players[PLAYER_2]->y() + (players[PLAYER_2]->height / 2 + .001) > orthMaxY){
					players[PLAYER_2]->y(orthMaxY - (players[PLAYER_2]->height / 2 + 0.001), true);
				}

			}

			else if (keys[SDL_SCANCODE_DOWN]){
				players[PLAYER_2]->y(-1 * PLAYERSPEED * elasped);
				if (players[PLAYER_2]->y() - (players[PLAYER_2]->height / 2 + .001) < orthMinY){
					players[PLAYER_2]->y(orthMinY + (players[PLAYER_2]->height / 2 + 0.001), true);
				}
			}

			if (keys[SDL_SCANCODE_LEFT]){
				players[PLAYER_2]->x(-1 * PLAYERSPEED * elasped);
				players[PLAYER_2]->height = NORMAL_PLAYER_HEIGHT - .01;
				if (players[PLAYER_2]->x() - (players[PLAYER_2]->width / 2 - .001) < 0){
					players[PLAYER_2]->x(orthMaxX - (players[PLAYER_2]->width / 2 + 0.001), true);
				}
			}

			else if (keys[SDL_SCANCODE_RIGHT]){
				players[PLAYER_2]->x(PLAYERSPEED * elasped);
				players[PLAYER_2]->height = NORMAL_PLAYER_HEIGHT - .01;
				if (players[PLAYER_2]->x() + (players[PLAYER_2]->width / 2 + .001) > orthMaxX){
					players[PLAYER_2]->x(0 + (players[PLAYER_2]->width / 2 + 0.001), true);
				}
			}
			else{
				players[PLAYER_2]->height = NORMAL_PLAYER_HEIGHT;
			}


			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			if (keys[SDL_SCANCODE_W]){
				players[PLAYER_1]->y(PLAYERSPEED * elasped);
				if (players[PLAYER_1]->y() + (players[PLAYER_1]->height / 2 + .001) > orthMaxY){
					players[PLAYER_1]->y(orthMaxY - (players[PLAYER_1]->height / 2 + 0.001), true);
				}
			}

			else if (keys[SDL_SCANCODE_S]){
				players[PLAYER_1]->y(-1 * PLAYERSPEED * elasped);
				if (players[PLAYER_1]->y() - (players[PLAYER_1]->height / 2 + .001) < orthMinY){
					players[PLAYER_1]->y(orthMinY + (players[PLAYER_1]->height / 2 + 0.001), true);
				}
			}

			if (keys[SDL_SCANCODE_A]){
				players[PLAYER_1]->x(-1 * PLAYERSPEED * elasped);
				players[PLAYER_1]->height = NORMAL_PLAYER_HEIGHT - .01;
				if (players[PLAYER_1]->x() - (players[PLAYER_1]->width / 2 + .001) < orthMinX){
					players[PLAYER_1]->x(0 - (players[PLAYER_1]->width / 2 + 0.001), true);
				}
			}

			else if (keys[SDL_SCANCODE_D]){
				players[PLAYER_1]->x(PLAYERSPEED * elasped);
				players[PLAYER_1]->height = NORMAL_PLAYER_HEIGHT - .01;
				if (players[PLAYER_1]->x() + (players[PLAYER_1]->width / 2 + .001) > 0){
					players[PLAYER_1]->x(orthMinX + (players[PLAYER_1]->width / 2 + 0.001), true);
				}
			}
			else{
				players[PLAYER_1]->height = NORMAL_PLAYER_HEIGHT;
			}
			break;
		case STATE_END:
			if (keys[SDL_SCANCODE_RETURN]){
				state = STATE_MENU;
			}
			break;
		}
	}
}

void ClassDemoApp::Update(float elapsed) {
	switch (state){
	case STATE_MENU:
		break;
	case STATE_GAME:
		if (elapsedBuffer >= FIXED_TIMESTEP * 60.0 / (float)level){
			float dropRate = FIXED_TIMESTEP * 60.0 / (float)level;
			for (; elapsedBuffer >= dropRate; elapsedBuffer = elapsedBuffer - dropRate){
				if (bullets.size() >= MAX_BULLETS)
					break;
				float loc = randomX();
				float speed = ((float)level) * 0.2f * (float)(rand() % 300 + 800) * 0.001f;
				bullets.push_back(new Entity(loc, orthMaxY, 0.05, 0.05, -90, speed, nullptr));
				bullets.push_back(new Entity(-1 * loc, orthMaxY, 0.05, 0.05, -90, speed, nullptr));
				
				blocksLeft = blocksLeft - 1;
				if (blocksLeft <= 0){
					state = STATE_LEVEL_TRANSITION;
					break;
				}
			}
		}
	case STATE_LEVEL_TRANSITION:
		for (Entity* itr : bullets){
			itr->Update(elapsed);
		}
		UpdateGame();
		if (bullets.size() <= 0 && state == STATE_LEVEL_TRANSITION){
			state = STATE_GAME;
			level++;
			blocksLeft = level * 25 + 50;
			elapsedBuffer = 0.0f;
		}
		break;
	case STATE_END:

		break;
	}
	
}


bool ClassDemoApp::UpdateAndRender() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;
	ProcessEvents(elapsed);
	while (elapsed >= FIXED_TIMESTEP) {
		elapsedBuffer += FIXED_TIMESTEP;
		elapsed -= FIXED_TIMESTEP;
		Update(FIXED_TIMESTEP);
	}
	elapsedBuffer += elapsed;
	Update(elapsed);
	Render();
	return done;
}


//private functions
void ClassDemoApp::clear(){
	for (Entity* ent : players) { delete ent; }
	players.clear();
	for (Entity* ent : UI) { delete ent; }
	UI.clear();
	for (Entity* ent : bullets) { delete ent; }
	bullets.clear();
}

void ClassDemoApp::UpdateGame(){
	
	for (std::list<Entity*>::iterator itr = bullets.begin(); itr != bullets.end();){
		if (players[PLAYER_1]->collisionDetection(*itr)){
			delete *itr;
			itr = bullets.erase(itr);
			Mix_PlayChannel(-1, sounds[0], 0);
			//changes
			players[0]->width -= 0.02; // hardcoded numbers to subtract from the width of the block
			// changes
			if ((players[0]->width <= 0.001)){
				player1Wins = true;
				state = STATE_END;
				break;
			}
		}
		else if (players[PLAYER_2]->collisionDetection(*itr)){
			delete *itr;
			itr = bullets.erase(itr);
			Mix_PlayChannel(-1, sounds[0], 0);
			//changes 
			players[1]->width -= 0.02; // hardcoded numbers to subtract from the width of the block
			//changes
			if ((players[1]->width <= 0.001)){
				player1Wins = false;
				state = STATE_END;
				break;
			}
		}
		else if ((*itr)->y() < orthMinY - (*itr)->height/2){
			delete *itr;
			itr = bullets.erase(itr);
		}
		else{ itr++; }
		// changes
		// changes
	}

}

void ClassDemoApp::RenderMenu(){
	modelMatrix.identity();
	modelMatrix.Translate(-0.6f, 0.4f, 0.0);
	//DrawText(*(textures[0]), "KITTY HELL", 0.13f, 0.0f,0.5);
	DrawTextChar(*(textures[0]), "RANDOM HELL", 0.2, 0.2, -.1, 0, 0.25);
	//DrawChar(*(textures[0]), 'b', 0.1, 0.1, .5, 0.5);
	DrawTextChar(*(textures[0]), "Press [Enter] to Start", 0.1, 0.1, -0.05f, 0, 0);
	DrawTextChar(*(textures[0]), "Press [ESC] to exit", 0.1, 0.1f, -0.05f, 0.0, -0.1);

	for (Entity* ent : UI){
		ent->Render(programT);
	}
	

}

void ClassDemoApp::RenderGame(){
	//switch (state){
	//case STATE_GAME:
		for (Entity* ent : bullets){
			ent->Render(program);
		}
		for (Entity* ent : players){
			ent->Render(program);
		}
	//default:
		for (Entity* ent : UI){
			ent->Render(program);
		}
		DrawTextChar(*(textures[0]), "LEVEL: " + std::to_string(level), 0.1, 0.1, -0.05, orthMinX, orthMaxY - 0.05, LEFT);
		DrawTextChar(*(textures[0]), "Blocks Left: " + std::to_string(blocksLeft), 0.1, 0.1, -0.05, 0, orthMaxY - 0.05, LEFT);
	//}
}

void ClassDemoApp::win(){
	modelMatrix.identity();
	modelMatrix.Translate(-0.6f, 0.4f, 0.0);
	DrawTextChar(*(textures[0]), "Player 1 wins", 0.2,0.2, -0.125f, 0.0f, 0.05f);
}
void ClassDemoApp::lose(){
	modelMatrix.identity();
	modelMatrix.Translate(-0.6f, 0.4f, 0.0);
	DrawTextChar(*(textures[0]), "Player 2 wins", 0.2, 0.2, -0.125f, 0.0f, 0.05f);
}

float ClassDemoApp::randomX(){
	float percent = (float) (rand() % 10000) ;
	return percent * 0.0001f * orthMaxX;
}

