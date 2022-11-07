#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"

#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 2
#define INIT_PLAYER_Y_TILES 16

#define INIT_BASIC_ENEMY_X_TILES 15
#define INIT_BASIC_ENEMY_Y_TILES 24

enum menuButtons {PLAY_BT, CONTINUE_BT, RESTART_BT, INSTRUCTIONS_BT, CREDITS_BT};

Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}


/* INIT */

void Scene::init()
{
	//Shaders
	initShaders();

	//Subinits
	initGame();
	initMenu();
	initInstructions();
	initCredits();
	initTransition();
	initGameover();
	initReady();
	initTheEnd();
	
	//Projections
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	gameProjection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);

	bossfight = -1;
	currentTime = 0.0f;
}

//Subinits

void Scene::initMenu()
{
	glm::vec2 texCoords[2];
	glm::vec2 geomBackground[2] = { glm::vec2(0.f, 0.f), glm::vec2(384.f, 256.f) };
	glm::vec2 geomMenuButton[2] = { glm::vec2(0.f, 0.f), glm::vec2(170.f, 22.f) };


	texCoords[0] = glm::vec2(0.5f, 0.f); texCoords[1] = glm::vec2(1.f, 0.2f); //play
	menuTexQuad[0] = TexturedQuad::createTexturedQuad(geomMenuButton, texCoords, texProgram);
	texCoords[0] = glm::vec2(0.5f, 0.2f); texCoords[1] = glm::vec2(1.f, 0.4f); //continue
	menuTexQuad[1] = TexturedQuad::createTexturedQuad(geomMenuButton, texCoords, texProgram);
	texCoords[0] = glm::vec2(0.5f, 0.4f); texCoords[1] = glm::vec2(1.f, 0.6f); //restart
	menuTexQuad[2] = TexturedQuad::createTexturedQuad(geomMenuButton, texCoords, texProgram);
	texCoords[0] = glm::vec2(0.5f, 0.6f); texCoords[1] = glm::vec2(1.f, 0.8f); //instructions
	menuTexQuad[3] = TexturedQuad::createTexturedQuad(geomMenuButton, texCoords, texProgram);
	texCoords[0] = glm::vec2(0.5f, 0.8f); texCoords[1] = glm::vec2(1.f, 1.f); //credits
	menuTexQuad[4] = TexturedQuad::createTexturedQuad(geomMenuButton, texCoords, texProgram);
	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(0.5f, 0.2f); //play - selected
	menuTexQuad[5] = TexturedQuad::createTexturedQuad(geomMenuButton, texCoords, texProgram);
	texCoords[0] = glm::vec2(0.f, 0.2f); texCoords[1] = glm::vec2(0.5f, 0.4f); //continue - selected
	menuTexQuad[6] = TexturedQuad::createTexturedQuad(geomMenuButton, texCoords, texProgram);
	texCoords[0] = glm::vec2(0.f, 0.4f); texCoords[1] = glm::vec2(0.5f, 0.6f); //restart - selected
	menuTexQuad[7] = TexturedQuad::createTexturedQuad(geomMenuButton, texCoords, texProgram);
	texCoords[0] = glm::vec2(0.f, 0.6f); texCoords[1] = glm::vec2(0.5f, 0.8f); //instructions - selected
	menuTexQuad[8] = TexturedQuad::createTexturedQuad(geomMenuButton, texCoords, texProgram);
	texCoords[0] = glm::vec2(0.f, 0.8f); texCoords[1] = glm::vec2(0.5f, 1.f); //credits - selected
	menuTexQuad[9] = TexturedQuad::createTexturedQuad(geomMenuButton, texCoords, texProgram);

	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.f, 1.f); //background
	menuBackground = TexturedQuad::createTexturedQuad(geomBackground, texCoords, texProgram);
	// Load textures
	menuTexs[0].loadFromFile("images/menu/menuOptions.png", TEXTURE_PIXEL_FORMAT_RGBA);
	menuTexs[1].loadFromFile("images/menu/menuBackground.png", TEXTURE_PIXEL_FORMAT_RGBA);
}

void Scene::initGame()
{
	count = 0;
	
	//background
	glm::vec2 texCoords[2];
	glm::vec2 geomBack[2] = { glm::vec2(0.f, 0.f), glm::vec2(3840.f, 256.f) };
	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.f, 1.f);
	gameBackground = TexturedQuad::createTexturedQuad(geomBack, texCoords, texProgram);
	gameBackTex.loadFromFile("images/gameBackground.png", TEXTURE_PIXEL_FORMAT_RGBA);
	
	//map
	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgramGame);
	
	//player
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgramGame, true);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	
	//force
	force = new Force();
	force->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgramGame);
	force->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize() + player->getSize().x, INIT_PLAYER_Y_TILES * map->getTileSize() + 1));
	force->setTileMap(map);
	
	//enemies
	initEnemies();
	
	//heart
	glm::vec2 geomHeart[2] = { glm::vec2(0.f, 0.f), glm::vec2(13.f, 10.f) };
	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.f, 1.f);
	heart = TexturedQuad::createTexturedQuad(geomHeart, texCoords, texProgramGame);
	heartTex.loadFromFile("images/heart_13x10.png", TEXTURE_PIXEL_FORMAT_RGBA);

	//Tokens
	for (int i = 0; i < sizeof(showTokens); i++) showTokens[i] = false; //TODO: set to false
	glm::vec2 geomToken[2] = { glm::vec2(0.f, 0.f), glm::vec2(10.f, 10.f) };
	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(0.5f, 0.5f);
	upgradeTokens[0] = TexturedQuad::createTexturedQuad(geomToken, texCoords, texProgramGame); //Token force
	texCoords[0] = glm::vec2(0.5f, 0.f); texCoords[1] = glm::vec2(1.f, 0.5f);
	upgradeTokens[1] = TexturedQuad::createTexturedQuad(geomToken, texCoords, texProgramGame); //Token upgrade 1
	texCoords[0] = glm::vec2(0.f, 0.5f); texCoords[1] = glm::vec2(0.5f, 1.f);
	upgradeTokens[2] = TexturedQuad::createTexturedQuad(geomToken, texCoords, texProgramGame); //Token upgrade 2
	upgradeTokensTex.loadFromFile("images/force/upgradeTokens10x10.png", TEXTURE_PIXEL_FORMAT_RGBA);
}

void Scene::initInstructions()
{
	glm::vec2 texCoords[2];
	glm::vec2 geomBackground[2] = { glm::vec2(0.f, 0.f), glm::vec2(384.f, 256.f) };

	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.f, 1.f); //background
	instructionsBackground = TexturedQuad::createTexturedQuad(geomBackground, texCoords, texProgram);

	instructionsTex.loadFromFile("images/instructions.png", TEXTURE_PIXEL_FORMAT_RGBA);
}

void Scene::initCredits()
{
	glm::vec2 texCoords[2];
	glm::vec2 geomBackground[2] = { glm::vec2(0.f, 0.f), glm::vec2(384.f, 256.f) };

	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.f, 1.f); //background
	creditsBackground = TexturedQuad::createTexturedQuad(geomBackground, texCoords, texProgram);

	creditsTex.loadFromFile("images/credits.png", TEXTURE_PIXEL_FORMAT_RGBA);
}

void Scene::initTransition()
{
	quad = Quad::createQuad(0.f, 0.f, 384.f, 256.f, simpleProgram);
}

void Scene::initGameover()
{
	glm::vec2 texCoords[2];
	glm::vec2 geomGameOver[2] = { glm::vec2(0.f, 0.f), glm::vec2(144.f, 14.f) };
	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.f, 1.f); //play
	gameOver = TexturedQuad::createTexturedQuad(geomGameOver, texCoords, texProgram);
	gameOverTex.loadFromFile("images/gameOver.png", TEXTURE_PIXEL_FORMAT_RGBA);
}

void Scene::initReady()
{
	glm::vec2 texCoords[2];
	glm::vec2 geomReady[2] = { glm::vec2(0.f, 0.f), glm::vec2(92.f, 14.f) };
	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.f, 1.f);
	ready = TexturedQuad::createTexturedQuad(geomReady, texCoords, texProgram);
	readyTex.loadFromFile("images/ready.png", TEXTURE_PIXEL_FORMAT_RGBA);
}

void Scene::initTheEnd()
{
	glm::vec2 texCoords[2];
	glm::vec2 geomTheEnd[2] = { glm::vec2(0.f, 0.f), glm::vec2(111.f, 14.f) };
	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.f, 1.f);
	theEnd = TexturedQuad::createTexturedQuad(geomTheEnd, texCoords, texProgram);
	theEndTex.loadFromFile("images/theEnd.png", TEXTURE_PIXEL_FORMAT_RGBA);
}

//Other subinit functions

void Scene::initShaders()
{
	Shader vShader, fShader;

	//Simple program
	vShader.initFromFile(VERTEX_SHADER, "shaders/simple.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/simple.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	simpleProgram.init();
	simpleProgram.addShader(vShader);
	simpleProgram.addShader(fShader);
	simpleProgram.link();
	if (!simpleProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << simpleProgram.log() << endl << endl;
	}
	simpleProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();

	//texProgram
	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");

	//TexProgramGame
	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgramGame.init();
	texProgramGame.addShader(vShader);
	texProgramGame.addShader(fShader);
	texProgramGame.link();
	if (!texProgramGame.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgramGame.log() << endl << endl;
	}
	texProgramGame.bindFragmentOutput("outColor");
}

void Scene::initEnemies() {
	vector<pair<Enemies, glm::ivec2>> enemyPositions = { 
		{make_pair(BASIC1, glm::ivec2 {480, 140})},
		{make_pair(BASIC1, glm::ivec2 {488, 140})},
		{make_pair(BASIC1, glm::ivec2 {496, 140})},
		{make_pair(BASIC1, glm::ivec2 {504, 140})},
		{make_pair(BASIC1, glm::ivec2 {512, 140})},
		{make_pair(BASIC1, glm::ivec2 {520, 140})},


		{make_pair(BASIC2, glm::ivec2 {77 * 8, 25 * 8 + 4})},
		{make_pair(BASIC2, glm::ivec2 {87 * 8, 25*8 + 4})},
		{make_pair(BASIC1, glm::ivec2 {96*8 + 2, 140})},
		{make_pair(BASIC1, glm::ivec2 {96*8 + 10, 140})},
		{make_pair(BASIC1, glm::ivec2 {96*8 + 18, 140})},


		{make_pair(BASIC3, glm::ivec2 {117 * 8, 8 * 24})},


		{make_pair(BASIC4, glm::ivec2 {124 * 8, 8 * 24})},
		{make_pair(BASIC4, glm::ivec2 {124*8 + 1, 8 * 8})},

		{make_pair(BASIC4, glm::ivec2 {130 * 8, 8 * 24})},
		{make_pair(BASIC4, glm::ivec2 {130 * 8 + 1, 8 * 8})},

		{make_pair(BASIC4, glm::ivec2 {137 * 8 -2, 8 * 24})},
		{make_pair(BASIC4, glm::ivec2 {137 * 8 -3, 8 * 8})},


		{make_pair(BASIC1, glm::ivec2 {150 * 8, 140})},
		{make_pair(BASIC1, glm::ivec2 {150 * 8 + 8, 140})},
		{make_pair(BASIC1, glm::ivec2 {150 * 8 + 16, 140})},
		{make_pair(BASIC1, glm::ivec2 {150 * 8 + 24, 140})},


		{make_pair(BASIC2, glm::ivec2 {173 * 8, 25 * 8 + 4})},
		{make_pair(BASIC1, glm::ivec2 {182 * 8, 140})},
		{make_pair(BASIC1, glm::ivec2 {182 * 8 + 8, 140})},
		{make_pair(BASIC1, glm::ivec2 {182 * 8 + 16, 140})},


		{make_pair(BASIC3, glm::ivec2 {208 * 8, 8 * 24})},
		{make_pair(BASIC1, glm::ivec2 {210 * 8, 140})},
		{make_pair(BASIC1, glm::ivec2 {210 * 8 + 8, 140})},
		{make_pair(BASIC1, glm::ivec2 {210 * 8 + 16, 140})},

		
		{make_pair(BASIC4, glm::ivec2 {224 * 8, 8 * 23})},
		{make_pair(BASIC4, glm::ivec2 {224 * 8 - 1, 8 * 9})},

		{make_pair(BASIC4, glm::ivec2 {228 * 8, 8 * 23})},
		{make_pair(BASIC4, glm::ivec2 {228 * 8 - 1, 8 * 9})},

		{make_pair(BASIC4, glm::ivec2 {232 * 8, 8 * 23})},
		{make_pair(BASIC4, glm::ivec2 {232 * 8 - 1, 8 * 9})},

		{make_pair(BASIC4, glm::ivec2 {236 * 8, 8 * 23})},
		{make_pair(BASIC4, glm::ivec2 {236 * 8 - 1, 8 * 9})},

		{make_pair(BASIC4, glm::ivec2 {240 * 8, 8 * 23})},
		{make_pair(BASIC4, glm::ivec2 {240 * 8 - 1, 8 * 9})},


		{make_pair(BASIC2, glm::ivec2 {271 * 8, 25 * 8 + 4})},

		{make_pair(BASIC1, glm::ivec2 {273 * 8, 140})},
		{make_pair(BASIC1, glm::ivec2 {273 * 8 + 8, 140})},
		{make_pair(BASIC1, glm::ivec2 {273 * 8 + 16, 140})},
		{make_pair(BASIC1, glm::ivec2 {273 * 8 + 24, 140})},
		{make_pair(BASIC1, glm::ivec2 {273 * 8 + 32, 140})},

		{make_pair(BASIC1, glm::ivec2 {282 * 8, 140})},
		{make_pair(BASIC1, glm::ivec2 {282 * 8 + 8, 140})},
		{make_pair(BASIC1, glm::ivec2 {282 * 8 + 16, 140})},
		{make_pair(BASIC1, glm::ivec2 {282 * 8 + 24, 140})},
		{make_pair(BASIC1, glm::ivec2 {282 * 8 + 32, 140})},

		{make_pair(BASIC4, glm::ivec2 {273 * 8 + 1, 5 * 8})},
		{make_pair(BASIC4, glm::ivec2 {273 * 8 + 2, 8 * 27})},

		{make_pair(BASIC4, glm::ivec2 {281 * 8 + 1, 5 * 8})},
		{make_pair(BASIC4, glm::ivec2 {281 * 8 + 2, 8 * 27})},

		{make_pair(BASIC4, glm::ivec2 {295 * 8, 5 * 8})},
		{make_pair(BASIC4, glm::ivec2 {295 * 8 + 1, 8 * 27})},

		{make_pair(BASIC4, glm::ivec2 {299 * 8, 5 * 8})},
		{make_pair(BASIC4, glm::ivec2 {299 * 8 + 1, 8 * 27})},


		{make_pair(BOSS, glm::ivec2 {8 * 348, 8 * 4 + 1})},


		/*{make_pair(BASIC2, glm::ivec2 {403, 220})},
		{make_pair(BASIC1, glm::ivec2 {408, 150})},
		{make_pair(BASIC1, glm::ivec2 {413, 150})},
		{make_pair(BASIC1, glm::ivec2 {418, 150})},
		{make_pair(BASIC3, glm::ivec2 {404, 8 * 27})},
		{make_pair(BASIC4, glm::ivec2 {436, 8 * 27})},
		{make_pair(BASIC4, glm::ivec2 {452, 8 * 27})},
		{make_pair(BASIC4, glm::ivec2 {468, 8 * 5})},
		{make_pair(BASIC4, glm::ivec2 {484, 8 * 5})},
		{make_pair(BASIC4, glm::ivec2 {500, 8 * 5})},*/
	};
	for (auto pos : enemyPositions) {
		Enemy* enemy = new Enemy();
		enemy->init(glm::ivec2(SCREEN_X, SCREEN_Y), pos.first, texProgramGame);
		enemy->setPosition(glm::vec2(pos.second.x, pos.second.y));
		enemy->setTileMap(map);
		enemies.insert(make_pair(pos.second.x, *enemy));
	}
}


/* UPDATES */

void Scene::updateMenu(int deltaTime) {
	int nbButtons;
	if (menuType == INITIAL) nbButtons = 3;
	else nbButtons = 4;

	if (Game::instance().getSpecialKey(GLUT_KEY_UP) == RELEASE) {
		if (menuState == 1) menuState = nbButtons;
		else menuState -= 1;
	} else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN) == RELEASE) {
		if (menuState == nbButtons) menuState = 1;
		else menuState += 1;
	}
	else if (Game::instance().getKey(13) == PRESS) {
		GameState newState;
		if (menuType == INITIAL) {
			if (menuState == 1) newState = GAME;
			else if (menuState == 2) newState = INSTRUCTIONS;
			else if (menuState == 3) newState = CREDITS;
		}
		else {
			if (menuState == 1) newState = GAME;
			else if (menuState == 2) {
				restartGame();
				newState = GAME;
			}
			else if (menuState == 3) newState = INSTRUCTIONS;
			else if (menuState == 4) newState = CREDITS;
		}
		Game::instance().setState(newState);
	}

	currentTime += deltaTime;
}

void Scene::updateGame(int deltaTime)
{
	count++;
	currentTime += deltaTime;

	if (bossfight > 0) --bossfight;

	if (!player->died()) {
		updateGameBackground(deltaTime);
		updateGameEnemies(deltaTime);
		glm::ivec2 p0 = player->getPosition();
		updateGamePlayer(deltaTime);
		glm::ivec2 p1 = player->getPosition();
		updateGameForce(deltaTime);
		updateGameShots(deltaTime);
		checkCollisions();
		//Check if player is out of screen
		if (!inScreen(player->getPosition(), player->getSize()) && !player->inInitAnimation()) {
			player->collision();
		}
		if (Game::instance().getKey('m') == PRESS) Game::instance().setState(MENU);
		for (int i = '1'; i <= '6'; ++i) {
			if (Game::instance().getKey(i) == PRESS) {
				// spawnEnemies(i);
				player->setPosition(glm::vec2(breakpoints[(i - '0') - 1], player->getPosition().y));
				screenExtraPosition = breakpoints[(i - '0') - 1];
				/*
				if (i == '6' && bossfight == 0) {
					player->setPosition(glm::vec2(330 * 8, player->getPosition().y));
					screenExtraPosition = 330 * 8;
				} else{
					if (bossfight == 0) bossfight = -1;
				}
				*/
			}
		}
	}
	else {
		if (!player->boomFinished()) player->update(deltaTime, screenExtraPosition, force->getWidth()); //Player explosion animation
		else {
			lifes--;
			if (lifes > 0) {
				Game::instance().setState(READY);
				//restartGame(); //Play again with one less life
			}
			else {
				//Game over, player lifes == 0
				Game::instance().setState(GAMEOVER);
				//restartGame();
			}
		}
	}
}

void Scene::updateInstructions(int deltaTime) {
	currentTime += deltaTime;

	if (Game::instance().getKey(13) == PRESS) {
		Game::instance().setState(MENU);
	}
}

void Scene::updateCredits(int deltaTime) {
	currentTime += deltaTime;

	if (Game::instance().getKey(13) == PRESS) {
		Game::instance().setState(MENU);
	}
}

void Scene::updateTransition(int deltaTime)
{
	transitionCount++;

	int middle = 80;
	int max = 160;
	if (transitionCount == 1) {
		int a = 0;
	}
	if (transitionCount < middle) {
		//Game before READY?
		//if (Game::instance().getNextState() == READY && player->inInitAnimation()) updateGame(deltaTime);

	}
	else if (transitionCount == middle) {
		if (Game::instance().getNextState() == GAME && !player->inInitAnimation()) restartGame();
	}
	else if (transitionCount > middle && transitionCount < max) {
		//Game init animation
		if (Game::instance().getNextState() == GAME && player->inInitAnimation()) updateGame(deltaTime);
		//Restart game after READY?
	}
	else if (transitionCount >= max) {
		transitionCount = 0;
		GameState nextState = Game::instance().getNextState();
		Game::instance().setState(nextState);
	}
}

void Scene::updateGameOver(int deltaTime)
{
	gameOverCount++;

	if (gameOverCount >= 100) {
		gameOverCount = 0;
		Game::instance().setState(MENU);
		menuType = INITIAL;
		menuState = 1;
		lifes = 3;
	}
}

void Scene::updateReady(int deltaTime)
{
	readyCount++;

	if (readyCount >= 100) {
		readyCount = 0;
		Game::instance().setState(GAME);
	}
}

void Scene::updateTheEnd(int deltaTime) {
	theEndCount++;

	if (theEndCount >= 100) {
		theEndCount = 0;
		Game::instance().setState(MENU);
		menuType = INITIAL;
		menuState = 1;
		lifes = 3;
	}
}

//Subupdates

void Scene::updateGameBackground(int deltaTime)
{
	if (screenMovement == 2) {
		//update player position
		glm::ivec2 posPlayer = player->getPosition();
		glm::ivec2 posForce = glm::ivec2(0, 0), sizeForce = glm::ivec2(0, 0);
		if (force->isActive() && force->isAttachedFront()) {
			posForce = force->getPosition();
			posForce.x -= 1;
			sizeForce = force->getSize();
		}
		if (!map->collisionMoveRight(posPlayer + glm::ivec2(1,0), player->getSize() + glm::ivec2(sizeForce.x, 0))) {
			posPlayer.x++;
			player->setPosition(posPlayer);
		}

		//update force position
		posForce = force->getPosition();
		if (!map->collisionMoveRight(force->getPosition() + glm::ivec2(1, 0), force->getSize())) {
			posForce.x++;
			force->setPosition(posForce);
		}

		//update background position
		if (bossfight != 0) {
			screenExtraPosition += 1;
			gameProjection = glm::ortho(0.f + screenExtraPosition, float(SCREEN_WIDTH - 1) + screenExtraPosition, float(SCREEN_HEIGHT - 1), 0.f);
			screenMovement = -1;
		}
	}
	++screenMovement;
}

void Scene::updateGameEnemies(int deltaTime) {
	//Enemies
	createEnemies();
	for (Enemy* enemy : activeEnemies) {
		enemy->update(deltaTime, player->getPosition());
		if (enemy->isShooting()) {
			//Shot
			if (enemy->getType() == BOSS) {
				if (enemy->getNewShotType() == EGG) {
					createEggs(enemy);
				}
			}
			if (enemy->getType() == BOSS) addShot(enemy->getShotSprite(), enemy->getShotVelocity(), enemy->getShotPosition(), enemy->getShotSize(), enemy->getShotSizeInSpriteSheet(), enemy->getShotDamage(), false, (3 + enemy->getNewShotType()),	 0);
			else addShot(enemy->getShotSprite(), enemy->getShotVelocity(), enemy->getPosition(), enemy->getShotSize(), enemy->getShotSizeInSpriteSheet(), 1, false, -1, 0);
			enemy->enemyAlreadyAttacked();
		}
	}

	//Booming enemies
	vector<Enemy*> enemyErase;
	for (Enemy* enemy : boomEnemies) {
		enemy->update(deltaTime, player->getPosition());
		if (enemy->getType() == BOSS && enemy->boomFinished()) {
			Game::instance().setState(THEEND);
		}
		if (enemy->boomFinished()) enemyErase.push_back(enemy);
	}
	for (Enemy* enemy : enemyErase) boomEnemies.erase(enemy);
}

void Scene::updateGamePlayer(int deltaTime)
{
	//If "s" released, add shot with damage > 1
	if (Game::instance().getKey('s') == PRESS || (Game::instance().getKey('s') == RELEASE && player->getShotCharge() > 1)) {
		if (!player->inInitAnimation()) addPlayerShot();
	}
	
	//Player update
	player->update(deltaTime, screenExtraPosition, force->getWidth());

}

void Scene::updateGameForce(int deltaTime)
{
	//Force update
	force->update(currentTime, player->getPosition(), screenExtraPosition);

	//Check if is out of screen
	if (!inScreen(force->getPosition(), force->getSize())) {
		int type = force->getType();
		force = new Force();
		force->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgramGame);
		force->setPosition(glm::vec2(screenExtraPosition, 121));
		force->setTileMap(map);
		force->setActive(true);
		force->setType(type);
	}
}

void Scene::updateGameShots(int deltaTime) 
{
	vector<Shot*> erase;
	glm::ivec2 forcePos = force->getPosition();
	glm::ivec2 forceSize = force->getSize();
	//Player shots
	for (Shot* shot : playerShots) {
		
		//Update shots
		shot->update(deltaTime, forcePos, forceSize, texProgramGame);
		
		//Check shots that are out of the screen
		if (!inScreen(shot->getPosition(), shot->getSize())) erase.push_back(shot);
	}

	//Erase player shots that are out of the screen
	for (Shot* shot : erase) playerShots.erase(shot);

	//enemies shots
	for (Shot* shot : enemyShots) {
		shot->update(deltaTime, forcePos, forceSize, texProgramGame);
		if (shot->getCategory() != 5 && !inScreen(shot->getPosition(), shot->getSize())) erase.push_back(shot);
	}
	for (Shot* shot : erase) enemyShots.erase(shot);


	//Check if shots collision with map
	for (Shot* shot : boomShots) {
		shot->update(deltaTime, forcePos, forceSize, texProgramGame);
		if (shot->boomFinished()) erase.push_back(shot);
	}
	for (Shot* shot : erase) boomShots.erase(shot);
}

//Other subupdate functions

void Scene::createEnemies() {
	int acutalPosition = (SCREEN_WIDTH - 1) + screenExtraPosition;
	auto it = enemies.find(acutalPosition);
	if (it != enemies.end()) {
		activeEnemies.insert(&it->second);
		if (it->second.getType() == BOSS) bossfight = 340;
	}
}

void Scene::restartGame() {
	//player position (checkpoints)
	int x = 0;
	if (lifes < 3) {
		glm::ivec2 posPlayer = player->getPosition();
		x = posPlayer.x;
		for (int i = 1; i < breakpoints.size(); i++) {
			if (x >= breakpoints[i - 1] && x < breakpoints[i]) x = breakpoints[i - 1];
		}
	}

	//restart player
	player = new Player();
	bool animation = lifes == 3;
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgramGame, animation);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize() + x, INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	//restart force
	force = new Force();
	force->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgramGame);
	force->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize() + x + player->getSize().x, INIT_PLAYER_Y_TILES * map->getTileSize() + 1));
	force->setTileMap(map);

	for (int i = 0; i < 3; ++i) showTokens[i] = false;
	bossfight = -1;

	playerShots.clear();
	enemyShots.clear();
	enemies.clear();
	activeEnemies.clear();
	boomEnemies.clear();
	screenMovement = 0;
	screenExtraPosition = 0 + x;
	enemyGenerator = 0;
	count = 0;
	lastUpgrade1Shot = 0;
	lastUpgrade2Shot = 0;
	
	initEnemies();

	gameProjection = glm::ortho(0.f, float(SCREEN_WIDTH - 1) + x, float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

void Scene::addPlayerShot()
{
	//Player info
	int charge = player->getShotCharge();
	glm::ivec2 posPlayer = player->getPosition();


	//Shot info
	int damage;
	string spriteFolder;
	glm::ivec2 velocity, posShot, size;
	glm::vec2 sizeInSpriteSheet;
	damage = charge;
	posShot = glm::ivec2(posPlayer.x + 16, posPlayer.y);
	sizeInSpriteSheet = glm::vec2(1, 1);

	switch (charge) {
		case 1:
			spriteFolder = "images/ship/shot.png";
			velocity = glm::ivec2(6.f, 0.f);
			size = glm::ivec2(8, 4);
			posShot = glm::ivec2(posPlayer.x + 16, posPlayer.y + 6);
			break;
		case 2:
			spriteFolder = "images/ship/chargedShots/shotCharged1.png";
			velocity = glm::ivec2(6.f, 0.f);
			size = glm::ivec2(15, 12);
			break;
		case 3:
			spriteFolder = "images/ship/chargedShots/shotCharged2.png";
			velocity = glm::ivec2(6.f, 0.f);
			size = glm::ivec2(28, 14);
			break;
		case 4:
			spriteFolder = "images/ship/chargedShots/shotCharged3.png";
			velocity = glm::ivec2(6.f, 0.f);
			size = glm::ivec2(44, 14);
			break;
		case 5:
			spriteFolder = "images/ship/chargedShots/shotCharged4.png";
			velocity = glm::ivec2(6.f, 0.f);
			size = glm::ivec2(60, 14);
			break;
		default:
			spriteFolder = "images/ship/shot.png";
			velocity = glm::ivec2(6.f, 0.f);
			size = glm::ivec2(8, 4);
			break;
	}
	
	//Add shot
	addShot(spriteFolder, velocity, posShot, size, sizeInSpriteSheet, damage, true, 0, 0);

	//Reset shot charge
	player->setShotCharge(1);

	//Add Force shot
	if(force->isActive() && Game::instance().getKey('s') == PRESS) addForceShot();
}

void Scene::addForceShot() {
	int nbShots; // 1 o 2 o 4
	int damage = 1;
	bool front;
	string spriteFolder;
	glm::ivec2 velocity, velocities[4], posShot, auxPos, size;
	glm::vec2 sizeInSpriteSheet;
	sizeInSpriteSheet = glm::vec2(1, 1);
	glm::ivec2 forceSize = force->getSize();
	
	force->addShot(nbShots, posShot, velocities, front);
	if (nbShots == 1) {
		//Simple shot
		spriteFolder = "images/ship/shot.png";
		velocity = glm::ivec2(6.f, 0.f);
		size = glm::ivec2(8, 4);
		addShot(spriteFolder, velocity, posShot, size, sizeInSpriteSheet, damage, true, 0, 0);
	}
	if (nbShots == 2 || nbShots == 4) {
		//Diagonal shots
		size = glm::ivec2(8, 6);

		spriteFolder = "images/force/diagonal_front_up.png";
		velocity = velocities[0];
		addShot(spriteFolder, velocity, posShot, size, sizeInSpriteSheet, damage, true, 0, 0);

		spriteFolder = "images/force/diagonal_front_down.png";
		velocity = velocities[1];
		addShot(spriteFolder, velocity, posShot, size, sizeInSpriteSheet, damage, true, 0, 0);
	}
	if (nbShots == 4) {
		size = glm::ivec2(3, 8);

		auxPos = posShot;
		auxPos.x -= forceSize.x / 2;
		auxPos.y -= forceSize.y / 2;
		spriteFolder = "images/force/up_shot.png";
		velocity = velocities[2];
		addShot(spriteFolder, velocity, auxPos, size, sizeInSpriteSheet, damage, true, 0, 0);

		auxPos = posShot;
		auxPos.x -= forceSize.x / 2;
		auxPos.y += forceSize.y / 2;
		spriteFolder = "images/force/down_shot.png";
		velocity = velocities[3];
		addShot(spriteFolder, velocity, auxPos, size, sizeInSpriteSheet, damage, true, 0, 0);
  }
	if (nbShots == 5) {
		//Upgrade 1
		if (count - lastUpgrade1Shot > 30) {
			lastUpgrade1Shot = count;
			auxPos = posShot;
			auxPos.x -= forceSize.x / 2;
			damage = 3;
			velocity = velocities[0];
			//Shot --> /
			auxPos.y -= 16;
			addShot(spriteFolder, velocity, auxPos, size, sizeInSpriteSheet, damage, true, 1, 0);
			//Shot --> -
			auxPos.y = posShot.y - 8;
			addShot(spriteFolder, velocity, auxPos, size, sizeInSpriteSheet, damage, true, 1, 6);
			//Shot --> \ .
			auxPos.y = posShot.y;
			addShot(spriteFolder, velocity, auxPos, size, sizeInSpriteSheet, damage, true, 1, 1);
		}
	}
	if (nbShots == 6) {
		//Upgrade 2
		if (count - lastUpgrade2Shot > 30) {
			lastUpgrade2Shot = count;
			auxPos = posShot;
			auxPos.x -= forceSize.x + 3;
			damage = 3;
			velocity = velocities[0];
			addShot(spriteFolder, velocity, auxPos, size, sizeInSpriteSheet, damage, true, 2, 0);
		}
	}
}

void Scene::addShot(string& spriteFolder, const glm::ivec2& velocity, glm::ivec2& pos, const glm::ivec2& size, const glm::vec2& sizeInSpriteSheet, const int& damage, bool fromPlayer, int upgrade, int subtype)
{
	Shot* shot = new Shot();
	shot->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgramGame, spriteFolder, velocity, size, sizeInSpriteSheet, damage, upgrade, subtype);
	shot->setPosition(glm::vec2(pos.x, pos.y));
	shot->setTileMap(map);

	if (fromPlayer) playerShots.insert(shot);
	else enemyShots.insert(shot);
}


/* RENDERS */

void Scene::renderGame()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);


	//background
	modelview = glm::mat4(1.0f);
	modelview = glm::translate(modelview, glm::vec3(0.f - screenExtraPosition, 0.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	gameBackground->render(gameBackTex);

	texProgramGame.use();
	texProgramGame.setUniformMatrix4f("projection", gameProjection);
	texProgramGame.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	//map
	modelview = glm::mat4(1.0f);
	texProgramGame.setUniformMatrix4f("modelview", modelview);
	texProgramGame.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();

	//hearts
	modelview = glm::translate(modelview, glm::vec3(2.f + screenExtraPosition, 2.f, 0.f));
	texProgramGame.setUniformMatrix4f("modelview", modelview);
	heart->render(heartTex);

	if (lifes >= 2) {
		modelview = glm::translate(modelview, glm::vec3(15.f, 0.f, 0.f));
		texProgramGame.setUniformMatrix4f("modelview", modelview);
		heart->render(heartTex);
	}
	if (lifes >= 3) {
		modelview = glm::translate(modelview, glm::vec3(15.f, 0.f, 0.f));
		texProgramGame.setUniformMatrix4f("modelview", modelview);
		heart->render(heartTex);
	}

	//upgrade tokens
	for (int i = 0; i < sizeof(showTokens); i++) {
		if (showTokens[i]) {
			modelview = glm::mat4(1.0f);
			modelview = glm::translate(modelview, tokenPositions[i]);
			texProgramGame.setUniformMatrix4f("modelview", modelview);
			upgradeTokens[i]->render(upgradeTokensTex);
		}
	}

	//player and force
	player->render();
	if (force->isActive()) force->render();

	//shots
	for (Shot* shot : playerShots) {
		shot->render();
	}
	for (Shot* shot : enemyShots) shot->render();
	for (Shot* shot : boomShots) {
		shot->render();
	}


	//enemies
	for (Enemy* enemy : activeEnemies) enemy->render();
	for (Enemy* boomEnemy : boomEnemies) boomEnemy->render();

	if (Game::instance().getKey('m')) {
		Game::instance().setState(MENU);
		menuType = PLAYING;
		menuState = 1;
	}
}

void Scene::renderMenu() {
	vector<menuButtons> buttons = { PLAY_BT, INSTRUCTIONS_BT, CREDITS_BT};
	glm::mat4 modelview = glm::mat4(1.0f);

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	//Background
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	menuBackground->render(menuTexs[1]);
	
	//1st button ("Start playing" or "Continue")
	modelview = glm::translate(modelview, glm::vec3(107.f, 80.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	if (menuType == INITIAL) {
		if (menuState != 1) menuTexQuad[0]->render(menuTexs[0]); //start playing
		else menuTexQuad[5]->render(menuTexs[0]); //start playing - selected
	}
	else {
		if (menuState != 1) menuTexQuad[1]->render(menuTexs[0]); //continue
		else menuTexQuad[6]->render(menuTexs[0]); //continue - selected
	}
	
	//2nd button ("Instructions" or "Restart")
	if (menuType == INITIAL) {
		modelview = glm::translate(modelview, glm::vec3(0.f, 50.f, 0.f));
		texProgram.setUniformMatrix4f("modelview", modelview);
		if (menuState != 2) menuTexQuad[3]->render(menuTexs[0]); //instructions
		else menuTexQuad[8]->render(menuTexs[0]); //instructions - selected
	}
	else {
		modelview = glm::translate(modelview, glm::vec3(0.f, 30.f, 0.f));
		texProgram.setUniformMatrix4f("modelview", modelview);
		if (menuState != 2) menuTexQuad[2]->render(menuTexs[0]); //continue
		else menuTexQuad[7]->render(menuTexs[0]); //continue - selected
	}

	//3rd button ("" or "Instructions")
	if (menuType == PLAYING) {
		modelview = glm::translate(modelview, glm::vec3(0.f, 30.f, 0.f));
		texProgram.setUniformMatrix4f("modelview", modelview);
		if (menuState != 3) menuTexQuad[3]->render(menuTexs[0]); //instructions
		else menuTexQuad[8]->render(menuTexs[0]); //instructions - selected
	}
	
	//Credits button
	if (menuType == INITIAL) modelview = glm::translate(modelview, glm::vec3(133.f, 80.f, 0.f));
	else modelview = glm::translate(modelview, glm::vec3(123.f, 80.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	if ((menuType == INITIAL && menuState == 3) || (menuType == PLAYING && menuState == 4)) menuTexQuad[9]->render(menuTexs[0]);
	else menuTexQuad[4]->render(menuTexs[0]);
}

void Scene::renderInstructions() {
	glm::mat4 modelview = glm::mat4(1.0f);
	
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	//Background
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	instructionsBackground->render(instructionsTex);
}

void Scene::renderCredits() {
	glm::mat4 modelview = glm::mat4(1.0f);

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	//Background
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	creditsBackground->render(creditsTex);
}

void Scene::renderTransition()
{
	
	int middle = 80;
	int max = 160;

	float alpha;
	if (transitionCount == 1) {
		int a = 0;
	}
	if (transitionCount < middle) { //fadeOut
		GameState previousState = Game::instance().getPreviousState();
		if (previousState != NONE) Game::instance().renderState(previousState);
		alpha = float(transitionCount) / float(middle);
	}
	else if (transitionCount >= middle && transitionCount < max) { //fadeIn
		GameState nextState = Game::instance().getNextState();
		if (nextState != NONE) Game::instance().renderState(nextState);
		alpha = 1.f - float(transitionCount - middle)/ float(middle);
	}

	
	glm::mat4 modelview = glm::mat4(1.0f);

	simpleProgram.use();
	simpleProgram.setUniformMatrix4f("projection", projection);
	simpleProgram.setUniform4f("color", 0.f, 0.f, 0.f, alpha);

	modelview = glm::mat4(1.0f);
	simpleProgram.setUniformMatrix4f("modelview", modelview);
	quad->render();
}

void Scene::renderGameover()
{
	glm::mat4 modelview = glm::mat4(1.0f);
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	//game over
	modelview = glm::translate(modelview, glm::vec3(120.f, 121.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	gameOver->render(gameOverTex);
}

void Scene::renderReady()
{
	glm::mat4 modelview = glm::mat4(1.0f);
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	//ready
	modelview = glm::translate(modelview, glm::vec3(146.f, 121.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	ready->render(readyTex);
}

void Scene::renderTheEnd()
{
	glm::mat4 modelview = glm::mat4(1.0f);
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	//the end
	modelview = glm::translate(modelview, glm::vec3(137.f, 121.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	theEnd->render(theEndTex);
}

/* OTHERS */

//Collisions

bool Scene::inScreen(const glm::ivec2& pos, const glm::ivec2& size)
{
	glm::ivec2 vertex1, vertex2, vertex3, vertex4;
	vertex1 = glm::ivec2(pos.x, pos.y);
	vertex2 = glm::ivec2(pos.x + size.x - 1, pos.y);
	vertex3 = glm::ivec2(pos.x, pos.y + size.y - 1);
	vertex4 = glm::ivec2(pos.x + size.x - 1, pos.y + size.y - 1);

	vector<glm::ivec2> vertexs = { vertex1, vertex2, vertex3, vertex4 };

	int x0, xf, y0, yf;
	x0 = screenExtraPosition;
	xf = screenExtraPosition + SCREEN_WIDTH - 1;
	y0 = 0;
	yf = SCREEN_HEIGHT - 1;
	
	for (glm::ivec2 vertex : vertexs) {
		if (vertex.x >= x0 && vertex.x < xf && vertex.y >= y0 && vertex.y < yf) return true;
	}

	return false;
}

void Scene::checkCollisions()
{
	glm::ivec2 playerPos, playerSize, forcePos, forceSize, enemyPos, enemySize, shotPos, shotSize, tokenPos, tokenSize;
	playerPos = player->getPosition();
	playerSize = player->getSize();
	forcePos = force->getPosition();
	forceSize = force->getSize();
	
	//(Player and Force) & enemies
	vector<Enemy*> eraseByForce;
	for (Enemy* enemy : activeEnemies) {
		enemyPos = enemy->getPosition();
		enemySize = enemy->getSize();
		if (isCollision(playerPos, playerSize, enemyPos, enemySize)) {
			if (enemy->getType() == BOSS)
			{
				glm::ivec2 bossPosition = enemy->getPosition();
				vector<glm::ivec2> bossColiders = {
					glm::ivec2(bossPosition.x + 27, bossPosition.y + 0),
					glm::ivec2(bossPosition.x + 28, bossPosition.y + 12),
					glm::ivec2(bossPosition.x + 24, bossPosition.y + 31),
					glm::ivec2(bossPosition.x + 23, bossPosition.y + 36),
					glm::ivec2(bossPosition.x + 21, bossPosition.y + 37),
					glm::ivec2(bossPosition.x + 17, bossPosition.y + 40),
					glm::ivec2(bossPosition.x + 14, bossPosition.y + 47),
					glm::ivec2(bossPosition.x + 9, bossPosition.y + 51),
					glm::ivec2(bossPosition.x + 7, bossPosition.y + 58),
					glm::ivec2(bossPosition.x + 9, bossPosition.y + 68),
					glm::ivec2(bossPosition.x + 12, bossPosition.y + 74),
					glm::ivec2(bossPosition.x + 16, bossPosition.y + 78),
					glm::ivec2(bossPosition.x + 28, bossPosition.y + 82),
					glm::ivec2(bossPosition.x + 37, bossPosition.y + 90),
					glm::ivec2(bossPosition.x + 26, bossPosition.y + 95),
					glm::ivec2(bossPosition.x + 22, bossPosition.y + 102),
					glm::ivec2(bossPosition.x + 17, bossPosition.y + 106),
					glm::ivec2(bossPosition.x + 10, bossPosition.y + 115),
					glm::ivec2(bossPosition.x + 6, bossPosition.y + 127),
					glm::ivec2(bossPosition.x + 7, bossPosition.y + 149),
					glm::ivec2(bossPosition.x + 20, bossPosition.y + 158),
					glm::ivec2(bossPosition.x + 24, bossPosition.y + 164),
					glm::ivec2(bossPosition.x + 33, bossPosition.y + 171),
					glm::ivec2(bossPosition.x + 29, bossPosition.y + 177),
					glm::ivec2(bossPosition.x + 17, bossPosition.y + 200),
					glm::ivec2(bossPosition.x + 19, bossPosition.y + 194),
				};
				vector<glm::ivec2> bossColidersSizes = { 
					glm::ivec2(12,12),
					glm::ivec2(12,20),
					glm::ivec2(15,5),
					glm::ivec2(5,5),
					glm::ivec2(3,4),
					glm::ivec2(5,7),
					glm::ivec2(7,5),
					glm::ivec2(7,7),
					glm::ivec2(9,10),
					glm::ivec2(7,6),
					glm::ivec2(6,4),
					glm::ivec2(7,6),
					glm::ivec2(15,8),
					glm::ivec2(6,7),
					glm::ivec2(11,8),
					glm::ivec2(5,5),
					glm::ivec2(6,9),
					glm::ivec2(9,12),
					glm::ivec2(13,22),
					glm::ivec2(14,9),
					glm::ivec2(12,6),
					glm::ivec2(10,7),
					glm::ivec2(7,6),
					glm::ivec2(6,18),
					glm::ivec2(9,7),
					glm::ivec2(12,6)
				};
				for (int i = 0; i < bossColiders.size(); ++i) {
					if (isCollision(playerPos, playerSize, bossColiders[i], bossColidersSizes[i])) {
						player->collision();
						break;
					}
				}
			}
			else player->collision();
		}
		if (isCollision(forcePos, forceSize, enemyPos, enemySize)) {
			if (enemy->getType() != BOSS )eraseByForce.push_back(enemy);
		}
		if (isCollision(playerPos, playerSize, enemyPos, enemySize)) player->collision();
		if (force->isActive() && isCollision(forcePos, forceSize, enemyPos, enemySize)) eraseByForce.push_back(enemy);
	}
	for (Enemy* enemy : eraseByForce) {
		enemy->collision();
		boomEnemies.insert(enemy);
		activeEnemies.erase(enemy);
	}

	//(Player and Force) and enemy shots
	vector<Shot*> erase;
	for (Shot* shot : enemyShots) {
		shotPos = shot->getPosition();
		shotSize = shot->getSize();
		if (isCollision(playerPos, playerSize, shotPos, shotSize)) player->collision();
		if (isCollision(forcePos, forceSize, shotPos, shotSize) && shot->getDamage() <= 1) erase.push_back(shot);
	}
	for (Shot* shot : erase) enemyShots.erase(shot);

	//Enemies & player shots
	vector<Enemy*> enemyErase;
	vector<Shot*> shotErase;
	for (Enemy* enemy : activeEnemies) {
		enemyPos = enemy->getPosition();
		enemySize = enemy->getSize();
		for (Shot* shot : playerShots) {
			shotPos = shot->getPosition();
			shotSize = shot->getSize();
			if (shot->getType() != 1) {
				bool isDead = true;
				bool isBoss = false;
				if (enemy->getType() == BOSS) {
					isBoss = true;
					enemySize = enemy->getCheckboxSizeBoss();
					enemyPos = enemy->getCheckboxPosBoss();
				}
				if (isCollision(enemyPos, enemySize, shotPos, shotSize)) {
					if (isBoss) isDead = enemy->reduceHP(shot->getDamage());
					if (isDead) {
						enemyErase.push_back(enemy);
						if (enemy->getType() == BASIC3) {
							//Show upgrades
							int forceUpgrade;
							if (!force->isActive() && !showTokens[0]) {
								forceUpgrade = 0;
							}
							else if (force->isActive() && force->getType() == 0) {
								forceUpgrade = 1;
							}
							else {
								forceUpgrade = 2;
							}
							showTokens[forceUpgrade] = true;
							tokenPositions[forceUpgrade] = glm::vec3(enemy->getPosition(), 0);
						}
						enemyErase.push_back(enemy);
					}
					if (shot->getDamage() == 1) shotErase.push_back(shot);
					else if (isBoss) shotErase.push_back(shot);
				}
			}
			else {
				vector<glm::ivec2> positions, sizes;
				shot->collisionsUpgrade1(positions, sizes);
				bool isBoss = false;
				bool isDead = true;
				if (enemy->getType() == BOSS) {
					isBoss = true;
					enemySize = enemy->getCheckboxSizeBoss();
					enemyPos = enemy->getCheckboxPosBoss();
				}
				for (int i = 0; i < positions.size(); i++) {
					if (isCollision(enemyPos, enemySize, positions[i], sizes[i])) {
						if (isBoss) isDead = enemy->reduceHP(shot->getDamage());
						if (isDead) {
							enemyErase.push_back(enemy);
							if (enemy->getType() == BASIC3) {
								//Show upgrades
								int forceUpgrade;
								if (!force->isActive() && !showTokens[0]) {
									forceUpgrade = 0;
								}
								else if (force->isActive() && force->getType() == 0) {
									forceUpgrade = 1;
								}
								else {
									forceUpgrade = 2;
								}
								showTokens[forceUpgrade] = true;
								tokenPositions[forceUpgrade] = glm::vec3(enemy->getPosition(), 0);
							}
							enemyErase.push_back(enemy);
						}
						if (shot->getDamage() == 1) shotErase.push_back(shot);
						else if (isBoss) shotErase.push_back(shot);
					}
				}
			}
		}
	}
	for (Enemy* enemy : enemyErase) {
		enemy->collision();
		boomEnemies.insert(enemy);
		activeEnemies.erase(enemy);
	}
	for (Shot* shot : shotErase) playerShots.erase(shot);

	//Player & upgradeTokens
	tokenSize = glm::ivec2(10, 10);
	for (int i = 0; i < sizeof(showTokens); i++) {
		if (showTokens[i]) {
			tokenPos = glm::ivec2(tokenPositions[i].x, tokenPositions[i].y);
			if (isCollision(playerPos, playerSize, tokenPos, tokenSize)) {
				showTokens[i] = false;
				if (i == 0) {
					//force
					force->setActive(true);
				}
				else if (i == 1) {
					//upgrade 1
					force->setType(1);
				}
				else if (i == 2) {
					//upgrade 3
					force->setType(2);
				}
			}
		}
	}

	//Player & force
	glm::ivec2 playerFrontPos, playerBottomPos, playerMidSize;
	playerMidSize = glm::ivec2(playerSize.x / 2, playerSize.y / 2);
	playerBottomPos = playerPos;
	playerFrontPos = glm::ivec2(playerPos.x + playerMidSize.x - 1, playerPos.y);

	//Shots with map
	vector<Shot*> boomPlayer;
	for (Shot* shot : playerShots) {
		glm::ivec2 shotSize = shot->getSize(), shotPos = shot->getPosition();
		if (map->collisionMoveLeft(shotPos, shotSize)) boomPlayer.push_back(shot);
		else if (map->collisionMoveRight(shotPos, shotSize)) boomPlayer.push_back(shot);
		else if (map->collisionMoveUp(shotPos, shotSize, 0)) boomPlayer.push_back(shot);
		else if (map->collisionMoveDown(shotPos, shotSize, 0)) boomPlayer.push_back(shot);
	}
	for (Shot* shot : boomPlayer) {
		shot->shotBoom(texProgramGame);
		boomShots.insert(shot);
		playerShots.erase(shot);
	}
	vector<Shot*> boomEnemies;
	for (Shot* shot : enemyShots) {
		glm::ivec2 shotSize = shot->getSize(), shotPos = shot->getPosition();
		if (map->collisionMoveLeft(shotPos, shotSize)) boomEnemies.push_back(shot);
		else if (map->collisionMoveRight(shotPos, shotSize)) boomEnemies.push_back(shot);
		else if (map->collisionMoveUp(shotPos, shotSize, 0)) boomEnemies.push_back(shot);
		else if (map->collisionMoveDown(shotPos, shotSize, 0)) boomEnemies.push_back(shot);
	}
	for (Shot* shot : boomEnemies) {
		shot->shotBoom(texProgramGame);
		boomShots.insert(shot);
		enemyShots.erase(shot);
	}
	

	forcePos = force->getPosition();
	forceSize = force->getSize();
	if (isCollision(playerBottomPos, playerMidSize, forcePos, forceSize)) {
		force->setAttached("bottom");
	}
	else if (isCollision(playerFrontPos, playerMidSize, forcePos, forceSize)) {
		force->setAttached("front");
	}
}

bool Scene::isCollision(const glm::ivec2& pos1, const glm::ivec2& size1, const glm::ivec2& pos2, const glm::ivec2& size2)
{
	int minx1, miny1, maxx1, maxy1, minx2, miny2, maxx2, maxy2;

	minx1 = pos1.x;
	miny1 = pos1.y;
	maxx1 = pos1.x + size1.x - 1;
	maxy1 = pos1.y + size1.y - 1;
	
	minx2 = pos2.x;
	miny2 = pos2.y;
	maxx2 = pos2.x + size2.x - 1;
	maxy2 = pos2.y + size2.y - 1;

	if (((minx1 < maxx2) && (minx2 < maxx1)) && ((miny1 < maxy2) && (miny2 < maxy1))) return true;
	return false;
}

void Scene::createEggs(Enemy* boss) {
	glm::ivec2 bossPosition = boss->getPosition();
	vector<glm::ivec2> eggPositions = { 
		glm::ivec2(bossPosition.x - 30, bossPosition.y + 2),
		glm::ivec2(bossPosition.x - 60, bossPosition.y + 4),
		glm::ivec2(bossPosition.x - 90, bossPosition.y + 6),
		glm::ivec2(bossPosition.x - 120, bossPosition.y + 15),
		glm::ivec2(bossPosition.x - 150, bossPosition.y + 24),
		glm::ivec2(bossPosition.x - 165, bossPosition.y + 54),
		glm::ivec2(bossPosition.x - 165, bossPosition.y + 88),
		glm::ivec2(bossPosition.x - 165, bossPosition.y + 122),
		glm::ivec2(bossPosition.x - 150, bossPosition.y + 152),
		glm::ivec2(bossPosition.x - 120, bossPosition.y + 165),
		glm::ivec2(bossPosition.x - 90, bossPosition.y + 170),
		glm::ivec2(bossPosition.x - 60, bossPosition.y + 176),
		glm::ivec2(bossPosition.x - 30, bossPosition.y + 178),
		glm::ivec2(bossPosition.x, bossPosition.y + 180),
	};
 	for (int i = 0; i < 14; ++i) {
		int yheight = ((eggPositions[i]).y > 135) ? 1 : 0;
		addShot(boss->getShotSprite(), boss->getShotVelocity(), eggPositions[i], boss->getShotSize(), boss->getShotSizeInSpriteSheet(), boss->getShotDamage(), false, (3 + boss->getNewShotType()), yheight);
	}
}