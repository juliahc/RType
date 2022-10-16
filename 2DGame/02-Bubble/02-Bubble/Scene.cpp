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
	
	/* GAME */
	
	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	initEnemies();

	currentTime = 0.0f;

	/* MENU */
	glm::vec2 geomBackground[2] = { glm::vec2(0.f, 0.f), glm::vec2(640.f, 480.f) };
	glm::vec2 geomMenuButton[2] = { glm::vec2(0.f, 0.f), glm::vec2(300.f, 50.f) };
	glm::vec2 geomSmallMenuButton[2] = { glm::vec2(0.f, 0.f), glm::vec2(148.f, 50.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

	
	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.f, 1.f); //play
	menuTexQuad[0] = TexturedQuad::createTexturedQuad(geomMenuButton, texCoords, texProgram);
	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.f, 1.f); //instruction
	menuTexQuad[1] = TexturedQuad::createTexturedQuad(geomMenuButton, texCoords, texProgram);
	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.f, 1.f); //credits
	menuTexQuad[2] = TexturedQuad::createTexturedQuad(geomSmallMenuButton, texCoords, texProgram);
	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.f, 1.f); //resume
	menuTexQuad[3] = TexturedQuad::createTexturedQuad(geomMenuButton, texCoords, texProgram);
	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.f, 1.f); //background
	menuTexQuad[4] = TexturedQuad::createTexturedQuad(geomBackground, texCoords, texProgram);
	// Load textures
	menuTexs[0].loadFromFile("images/playButton.png", TEXTURE_PIXEL_FORMAT_RGBA);
	menuTexs[1].loadFromFile("images/instructionsButtonGrey.png", TEXTURE_PIXEL_FORMAT_RGBA);
	menuTexs[2].loadFromFile("images/creditsButtonGrey.png", TEXTURE_PIXEL_FORMAT_RGBA);
	menuTexs[3].loadFromFile("images/resumeButtonGrey.jpg", TEXTURE_PIXEL_FORMAT_RGB);
	menuTexs[4].loadFromFile("images/planetsMenu.png", TEXTURE_PIXEL_FORMAT_RGBA);
	
	
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

void Scene::initEnemies() {
	vector<pair<Enemies, glm::ivec2>> enemyPositions = { {make_pair(BASIC1, glm::ivec2 {400, 5 * map->getTileSize()})}, {make_pair(BASIC1, glm::ivec2 {410, 5 * map->getTileSize()})}, {make_pair(BASIC1, glm::ivec2 {420, 5 * map->getTileSize()})}, {make_pair(BASIC2, glm::ivec2 {440, 210})}, {make_pair(BASIC1, glm::ivec2 {460, 210})}};
	for (auto pos : enemyPositions) {
		Enemy* enemy = new Enemy();
		enemy->init(glm::ivec2(SCREEN_X, SCREEN_Y), pos.first, texProgram);
		enemy->setPosition(glm::vec2(pos.second.x, pos.second.y));
		enemy->setTileMap(map);
		enemies.insert(make_pair(pos.second.x, *enemy));
	}
}

void Scene::updateGame(int deltaTime)
{
	currentTime += deltaTime;

	//screen movement
	if (screenMovement == 2) {
		glm::ivec2 posPlayer = player->getPosition();
		posPlayer.x++;
		player->setPosition(posPlayer);
		screenExtraPosition += 1;
		projection = glm::ortho(0.f + screenExtraPosition, float(SCREEN_WIDTH - 1) + screenExtraPosition, float(SCREEN_HEIGHT - 1), 0.f);
		screenMovement = -1;
	}
	++screenMovement;
	
	//enemies
	createEnemies();
	for (Enemy* enemy : activeEnemies) enemy->update(deltaTime);

	//Player
	if (Game::instance().getKey('s') == RELEASE) addPlayerShot();
	player->update(deltaTime, screenExtraPosition);

	//Shots
	vector<Shot*> erase;
	for (Shot* shot : shots) {
		shot->update(deltaTime);
		if (!inScreen(shot->getPosition(), shot->getSize())) erase.push_back(shot);
	}
	for (Shot* shot : erase) shots.erase(shot);

	//Check collisions


}
	

void Scene::createEnemies() {
	int acutalPosition = (SCREEN_WIDTH - 1) + screenExtraPosition;
	auto it = enemies.find(acutalPosition);
	if (it != enemies.end()) {
		activeEnemies.push_back(&it->second);
	}
}

void Scene::updateMenu(int deltaTime) {

	if (Game::instance().getSpecialKey(GLUT_KEY_UP) == RELEASE) {
		if (menuState == 1) menuState = 3;
		else menuState -= 1;
	} else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN) == RELEASE) {
		if (menuState == 3) menuState = 1;
		else menuState += 1;
	} 

	menuTexs[0].loadFromFile("images/playButtonGrey.png", TEXTURE_PIXEL_FORMAT_RGBA);
	menuTexs[1].loadFromFile("images/instructionsButtonGrey.png", TEXTURE_PIXEL_FORMAT_RGBA);
	menuTexs[2].loadFromFile("images/creditsButtonGrey.png", TEXTURE_PIXEL_FORMAT_RGBA);
	menuTexs[3].loadFromFile("images/resumeButtonGrey.jpg", TEXTURE_PIXEL_FORMAT_RGB);

	switch (menuState)
	{
		case 1:
			menuTexs[0].loadFromFile("images/playButton.png", TEXTURE_PIXEL_FORMAT_RGBA);
			break;
		case 2:
			menuTexs[1].loadFromFile("images/instructionsButton.png", TEXTURE_PIXEL_FORMAT_RGBA);
			break;
		case 3:
			menuTexs[2].loadFromFile("images/creditsButton.png", TEXTURE_PIXEL_FORMAT_RGBA);
			break;
		default:
			break;
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
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	player->render();
	for (Shot* shot : shots) shot->render();
	for (Enemy* enemy : activeEnemies) enemy->render();
}

void Scene::renderMenu() {
	glm::mat4 modelview = glm::mat4(1.0f);

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	//Background
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	menuTexQuad[4]->render(menuTexs[4]);

	//Play button
	modelview = glm::translate(modelview, glm::vec3(170.f, 150.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	menuTexQuad[0]->render(menuTexs[0]);

	//Instructions button
	modelview = glm::translate(modelview, glm::vec3(0.f, 100.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	menuTexQuad[1]->render(menuTexs[1]);

	//Credits button
	modelview = glm::translate(modelview, glm::vec3(280.f, 150.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	menuTexQuad[2]->render(menuTexs[2]);
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
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
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
	addShot(spriteFolder, velocity, posShot, size, sizeInSpriteSheet, damage);

	//Reset shot charge
	player->setShotCharge(1);
}

void Scene::addShot(string& spriteFolder, const glm::ivec2& velocity, glm::ivec2& pos, const glm::ivec2& size, const glm::vec2& sizeInSpriteSheet, const int& damage)
{
	Shot* shot = new Shot();
	shot->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spriteFolder, velocity, size, sizeInSpriteSheet, damage);
	shot->setPosition(glm::vec2(pos.x, pos.y));
	shot->setTileMap(map);

	shots.insert(shot);
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
