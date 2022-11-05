#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"

#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 4

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
	
	//Projections
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	gameProjection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);

	
	currentTime = 0.0f;
}

//Subinits

void Scene::initMenu()
{
	glm::vec2 texCoords[2];
	glm::vec2 geomBackground[2] = { glm::vec2(0.f, 0.f), glm::vec2(384.f, 256.f) };
	glm::vec2 geomMenuButton[2] = { glm::vec2(0.f, 0.f), glm::vec2(168.f, 23.f) };


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
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgramGame);
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
	for (int i = 0; i < sizeof(showTokens); i++) showTokens[i] = true; //TODO: set to false
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

}

void Scene::initCredits()
{

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
        {make_pair(BASIC2, glm::ivec2 {402, 220})},
        {make_pair(BASIC1, glm::ivec2 {403, 150})},
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

	if (!player->died()) {
		updateGameBackground(deltaTime);
		updateGameEnemies(deltaTime);
		updateGamePlayer(deltaTime);
		updateGameForce(deltaTime);
		updateGameShots(deltaTime);
		checkCollisions();
	}
	else {
		if (!player->boomFinished()) player->update(deltaTime, screenExtraPosition, force->getWidth()); //Player explosion animation
		else {
			lifes--;
			if (lifes > 0) restartGame(); //Play again with one less life
			else {
				//Game over, player lifes == 0
				Game::instance().setState(GAMEOVER);
				restartGame();
			}
		}
	}
}

void Scene::updateInstructions(int deltaTime) {
	currentTime += deltaTime;
}

void Scene::updateCredits(int deltaTime) {
	currentTime += deltaTime;
}

void Scene::updateTransition(int deltaTime)
{
	transitionCount++;

	int middle = 80;
	int max = 160;

	if (transitionCount >= max) {
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

//Subupdates

void Scene::updateGameBackground(int deltaTime)
{
	if (screenMovement == 2) {
		//update player position
		glm::ivec2 posPlayer = player->getPosition();
		posPlayer.x++;
		player->setPosition(posPlayer);

		//update force position
		glm::ivec2 posForce = force->getPosition();
		posForce.x++;
		force->setPosition(posForce);

		//update background position
		screenExtraPosition += 1;
		gameProjection = glm::ortho(0.f + screenExtraPosition, float(SCREEN_WIDTH - 1) + screenExtraPosition, float(SCREEN_HEIGHT - 1), 0.f);
		screenMovement = -1;
	}
	++screenMovement;
}

void Scene::updateGameEnemies(int deltaTime) {
	//Enemies
	createEnemies();
	for (Enemy* enemy : activeEnemies) {
		enemy->update(deltaTime);
		if (enemy->isShooting()) {
			//Shot
			addShot(enemy->getShotSprite(), enemy->getShotVelocity(player->getPosition()), enemy->getPosition(), enemy->getShotSize(), enemy->getShotSizeInSpriteSheet(), 1, false, 0);
			enemy->enemyAlreadyAttacked();
		}
	}

	//Booming enemies
	vector<Enemy*> enemyErase;
	for (Enemy* enemy : boomEnemies) {
		enemy->update(deltaTime);
		if (enemy->boomFinished()) enemyErase.push_back(enemy);
	}
	for (Enemy* enemy : enemyErase) boomEnemies.erase(enemy);
}

void Scene::updateGamePlayer(int deltaTime)
{
	//If "s" released, add shot with damage > 1
	if (Game::instance().getKey('s') == PRESS || (Game::instance().getKey('s') == RELEASE && player->getShotCharge() > 1)) addPlayerShot();

	//TODO: if s "press" add shot with damage = 1
	
	//Player update
	player->update(deltaTime, screenExtraPosition, force->getWidth());
}

void Scene::updateGameForce(int deltaTime)
{
	//Tokens TODO: que apareguin quan mor enemic
	//for (int i = 0; i < sizeof(showTokens); i++) showTokens[i] = true;
	tokenPositions[0] = glm::vec3(200.f, 50.f, 0.f);
	tokenPositions[1] = glm::vec3(400.f, 10.f, 0.f);
	tokenPositions[2] = glm::vec3(400.f, 200.f, 0.f);
	
	//Force update
	force->update(currentTime, player->getPosition(), screenExtraPosition);
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
		if (!inScreen(shot->getPosition(), shot->getSize())) erase.push_back(shot);
	}
	for (Shot* shot : erase) enemyShots.erase(shot);
}

//Other subupdate functions

void Scene::createEnemies() {
	int acutalPosition = (SCREEN_WIDTH - 1) + screenExtraPosition;
	auto it = enemies.find(acutalPosition);
	if (it != enemies.end()) {
		activeEnemies.insert(&it->second);
	}
}

void Scene::restartGame() {
	//restart player
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgramGame);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	//restart force
	force = new Force();
	force->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgramGame);
	force->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize() + player->getSize().x, INIT_PLAYER_Y_TILES * map->getTileSize() + 1));
	force->setTileMap(map);

	playerShots.clear();
	enemyShots.clear();
	enemies.clear();
	activeEnemies.clear();
	boomEnemies.clear();
	screenMovement = 0;
	screenExtraPosition = 0;
	enemyGenerator = 0;
	count = 0;
	lastUpgrade2Shot = 0;
	for (int i = 0; i < sizeof(showTokens); i++) showTokens[i] = true; //TODO: set to false
	
	initEnemies();

	gameProjection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
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
	addShot(spriteFolder, velocity, posShot, size, sizeInSpriteSheet, damage, true, 0);

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
		addShot(spriteFolder, velocity, posShot, size, sizeInSpriteSheet, damage, true, 0);
	}
	if (nbShots == 2 || nbShots == 4) {
		//Diagonal shots
		size = glm::ivec2(8, 6);

		spriteFolder = "images/force/diagonal_front_up.png";
		velocity = velocities[0];
		addShot(spriteFolder, velocity, posShot, size, sizeInSpriteSheet, damage, true, 0);

		spriteFolder = "images/force/diagonal_front_down.png";
		velocity = velocities[1];
		addShot(spriteFolder, velocity, posShot, size, sizeInSpriteSheet, damage, true, 0);
	}
	if (nbShots == 4) {
		size = glm::ivec2(3, 8);

		auxPos = posShot;
		auxPos.x -= forceSize.x / 2;
		auxPos.y -= forceSize.y / 2;
		spriteFolder = "images/force/up_shot.png";
		velocity = velocities[2];
		addShot(spriteFolder, velocity, auxPos, size, sizeInSpriteSheet, damage, true, 0);

		auxPos = posShot;
		auxPos.x -= forceSize.x / 2;
		auxPos.y += forceSize.y / 2;
		spriteFolder = "images/force/down_shot.png";
		velocity = velocities[3];
		addShot(spriteFolder, velocity, auxPos, size, sizeInSpriteSheet, damage, true, 0);
	}
	if (nbShots == 6) {
		//Upgrade 2
		if (count - lastUpgrade2Shot > 30) {
			lastUpgrade2Shot = count;
			auxPos = posShot;
			auxPos.x -= forceSize.x + 3;
			damage = 3;
			addShot(spriteFolder, velocity, auxPos, size, sizeInSpriteSheet, damage, true, 2);
		}
	}
}


void Scene::addShot(string& spriteFolder, const glm::ivec2& velocity, glm::ivec2& pos, const glm::ivec2& size, const glm::vec2& sizeInSpriteSheet, const int& damage, bool fromPlayer, int upgrade)
{
	Shot* shot = new Shot();
	shot->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgramGame, spriteFolder, velocity, size, sizeInSpriteSheet, damage, upgrade);
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

	if (!player->died()) {

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

		//enemies
		for (Enemy* enemy : activeEnemies) enemy->render();
		for (Enemy* boomEnemy : boomEnemies) boomEnemy->render();
	}

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

}

void Scene::renderCredits() {

}

void Scene::renderTransition()
{
	
	int middle = 80;
	int max = 160;

	float alpha;

	if (transitionCount < middle) { //fadeOut
		GameState previousState = Game::instance().getPreviousState();
		if (previousState != NONE) Game::instance().renderState(previousState);
		alpha = float(transitionCount) / float(middle);
		int a = 0;
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
	
	//Player & enemies
	for (Enemy* enemy : activeEnemies) {
		enemyPos = enemy->getPosition();
		enemySize = enemy->getSize();
		if (isCollision(playerPos, playerSize, enemyPos, enemySize)) player->collision();
	}

	//(Player and Force)  and enemy shots
	vector<Shot*> erase;
	for (Shot* shot : enemyShots) {
		shotPos = shot->getPosition();
		shotSize = shot->getSize();
		if (isCollision(playerPos, playerSize, shotPos, shotSize)) player->collision();
		if (isCollision( forcePos, forceSize, shotPos, shotSize)) erase.push_back(shot);
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
			if (isCollision(enemyPos, enemySize, shotPos, shotSize)) {
				enemyErase.push_back(enemy);
				if (shot->getDamage() == 1) shotErase.push_back(shot);
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
	if (Game::instance().getKey('a') == PRESS) {
		int a = 0;
	}

	glm::ivec2 playerFrontPos, playerBottomPos, playerMidSize;
	playerMidSize = glm::ivec2(playerSize.x / 2, playerSize.y / 2);
	playerBottomPos = playerPos;
	playerFrontPos = glm::ivec2(playerPos.x + playerMidSize.x - 1, playerPos.y);
	

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