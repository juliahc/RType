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


void Scene::init()
{
	initShaders();
	glm::vec2 texCoords[2];
	
	glm::vec2 geomBack[2] = { glm::vec2(0.f, 0.f), glm::vec2(3840.f, 256.f) };
	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.f, 1.f);
	gameBackground = TexturedQuad::createTexturedQuad(geomBack, texCoords, texProgram);
	gameBackTex.loadFromFile("images/gameBackground.png", TEXTURE_PIXEL_FORMAT_RGBA);
	
	/* GAME */
	
	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgramGame);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgramGame);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	initEnemies();

	glm::vec2 geomHeart[2] = { glm::vec2(0.f, 0.f), glm::vec2(13.f, 10.f) };
	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.f, 1.f); //play
	heart = TexturedQuad::createTexturedQuad(geomHeart, texCoords, texProgramGame);
	heartTex.loadFromFile("images/heart_13x10.png", TEXTURE_PIXEL_FORMAT_RGBA);

	currentTime = 0.0f;

	/* MENU */
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
	
	
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	gameProjection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

void Scene::restartGame() {
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgramGame);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	playerShots.clear();
	enemyShots.clear();
	enemies.clear();
	activeEnemies.clear();
	boomEnemies.clear();
	screenMovement = 0;
	screenExtraPosition = 0;
	enemyGenerator = 0;
	
	initEnemies();

	gameProjection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

void Scene::initEnemies() {
	vector<pair<Enemies, glm::ivec2>> enemyPositions = { {make_pair(BASIC1, glm::ivec2 {400, 5 * map->getTileSize()})}, {make_pair(BASIC1, glm::ivec2 {410, 5 * map->getTileSize()})}, {make_pair(BASIC1, glm::ivec2 {420, 5 * map->getTileSize()})}, {make_pair(BASIC2, glm::ivec2 {440, 210})}, {make_pair(BASIC1, glm::ivec2 {460, 210})}};
	for (auto pos : enemyPositions) {
		Enemy* enemy = new Enemy();
		enemy->init(glm::ivec2(SCREEN_X, SCREEN_Y), pos.first, texProgramGame);
		enemy->setPosition(glm::vec2(pos.second.x, pos.second.y));
		enemy->setTileMap(map);
		enemies.insert(make_pair(pos.second.x, *enemy));
	}
}

void Scene::updateGame(int deltaTime)
{
	currentTime += deltaTime;

	if (!player->died()) {
		//screen movement
		if (screenMovement == 2) {
			glm::ivec2 posPlayer = player->getPosition();
			posPlayer.x++;
			player->setPosition(posPlayer);
			screenExtraPosition += 1;
			gameProjection = glm::ortho(0.f + screenExtraPosition, float(SCREEN_WIDTH - 1) + screenExtraPosition, float(SCREEN_HEIGHT - 1), 0.f);
			screenMovement = -1;
		}
		++screenMovement;

		//Enemies
		createEnemies();
		for (Enemy* enemy : activeEnemies) enemy->update(deltaTime);

		//Booming enemies
		vector<Enemy*> enemyErase;
		for (Enemy* enemy : boomEnemies) {
			enemy->update(deltaTime);
			if (enemy->boomFinished()) enemyErase.push_back(enemy);
		}
		for (Enemy* enemy : enemyErase) boomEnemies.erase(enemy);

		//Player
		if (Game::instance().getKey('s') == RELEASE) addPlayerShot();
		player->update(deltaTime, screenExtraPosition);

		//Shots
		vector<Shot*> erase;
		for (Shot* shot : playerShots) {
			shot->update(deltaTime);
			if (!inScreen(shot->getPosition(), shot->getSize())) erase.push_back(shot);
		}
		for (Shot* shot : erase) playerShots.erase(shot);

		//Check collisions
		checkCollisions();

		
	}
	else {
		if (!player->boomFinished()) player->update(deltaTime, screenExtraPosition);
		else {
			lifes--;
			if (lifes > 0) restartGame();
			else {
				Game::instance().setState(MENU);
				restartGame();
				menuType = INITIAL;
				menuState = 1;
				lifes = 3;
			}
		}
	}
}
	
void Scene::createEnemies() {
	int acutalPosition = (SCREEN_WIDTH - 1) + screenExtraPosition;
	auto it = enemies.find(acutalPosition);
	if (it != enemies.end()) {
		activeEnemies.insert(&it->second);
	}
}

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

void Scene::updateInstructions(int deltaTime) {
	currentTime += deltaTime;
}

void Scene::updateCredits(int deltaTime) {
	currentTime += deltaTime;
}

void Scene::renderGame()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	modelview = glm::mat4(1.0f);
	modelview = glm::translate(modelview, glm::vec3(0.f - screenExtraPosition, 0.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	gameBackground->render(gameBackTex);

	texProgramGame.use();
	texProgramGame.setUniformMatrix4f("projection", gameProjection);
	texProgramGame.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	
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


	player->render();
	for (Shot* shot : playerShots) shot->render();
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

}

void Scene::renderCredits() {

}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgramGame.init();
	texProgram.addShader(vShader);
	texProgramGame.addShader(vShader);
	texProgram.addShader(fShader);
	texProgramGame.addShader(fShader);
	texProgram.link();
	texProgramGame.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	if (!texProgramGame.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgramGame.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	texProgramGame.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
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
	addShot(spriteFolder, velocity, posShot, size, sizeInSpriteSheet, damage, true);

	//Reset shot charge
	player->setShotCharge(1);
}

void Scene::addShot(string& spriteFolder, const glm::ivec2& velocity, glm::ivec2& pos, const glm::ivec2& size, const glm::vec2& sizeInSpriteSheet, const int& damage, bool fromPlayer)
{
	Shot* shot = new Shot();
	shot->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgramGame, spriteFolder, velocity, size, sizeInSpriteSheet, damage);
	shot->setPosition(glm::vec2(pos.x, pos.y));
	shot->setTileMap(map);

	if (fromPlayer) playerShots.insert(shot);
}

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
	glm::ivec2 playerPos, playerSize, enemyPos, enemySize, shotPos, shotSize;
	playerPos = player->getPosition();
	playerSize = glm::ivec2(24, 15);
	
	//Player & enemies
	for (Enemy* enemy : activeEnemies) {
		enemyPos = enemy->getPosition();
		enemySize = enemy->getSize();
		if (isCollision(playerPos, playerSize, enemyPos, enemySize)) player->collision();
	}

	//Player & enemy shots
	for (Shot* shot : enemyShots) {
		shotPos = shot->getPosition();
		shotSize = shot->getSize();
		if (isCollision(playerPos, playerSize, shotPos, shotSize)) player->collision();
	}

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
				shotErase.push_back(shot); //TODO: check shot`s charge
			}
		}
	}
	for (Enemy* enemy : enemyErase) {
		enemy->collision();
		boomEnemies.insert(enemy);
		activeEnemies.erase(enemy);
	}
	for (Shot* shot : shotErase) playerShots.erase(shot);
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

