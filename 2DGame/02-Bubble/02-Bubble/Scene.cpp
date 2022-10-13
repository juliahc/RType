#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"

#define SCREEN_X 32
#define SCREEN_Y 16



#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 25

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

void Scene::updateGame(int deltaTime)
{
	createEnemies();
	currentTime += deltaTime;
	player->update(deltaTime);
	for (Enemy* enemy : activeEnemies) enemy->update(deltaTime);
}

void Scene::createEnemies() {
	if (enemyGenerator == 121 || enemyGenerator == 0) {
		Enemy* newEnemy = new Enemy();
		if (activeEnemies.size() == 10) {
			enemyGenerator = 122;
			newEnemy->init(glm::ivec2(SCREEN_X, SCREEN_Y), BASIC2, texProgram);
			newEnemy->setPosition(glm::vec2(7 * map->getTileSize(), 7 * map->getTileSize()));
			newEnemy->setTileMap(map);
		}
		else {
			newEnemy->init(glm::ivec2(SCREEN_X, SCREEN_Y), BASIC1, texProgram);
			newEnemy->setPosition(glm::vec2(2 * map->getTileSize(), 2 * map->getTileSize()));
			newEnemy->setTileMap(map);
			enemyGenerator = 0;
		}
		activeEnemies.push_back(newEnemy);
	}
	++enemyGenerator;
}

void Scene::updateMenu(int deltaTime) {

	if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
		if (menuState == 1) menuState = 3;
		else menuState -= 1;
	} else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
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



