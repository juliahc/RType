#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"

void Game::init()
{
	bPlay = true;
	state = GAME;
	glClearColor(0.f, 0.f, 0.f, 1.f);
	scene.init();
}

bool Game::update(int deltaTime)
{
	switch (state) {
		case MENU:
			scene.updateMenu(deltaTime);
			break;
		case GAME:
			scene.updateGame(deltaTime);
			break;
		case CREDITS:
			scene.updateCredits(deltaTime);
			break;
		case INSTRUCTIONS:
			scene.updateInstructions(deltaTime);
			break;
	}

	for (int i = 0; i < 256; i++) {
		if (keys[i] == RELEASE) keys[i] = IDLE;
		if (specialKeys[i] == RELEASE) specialKeys[i] = IDLE;
	}
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch (state) {
		case MENU:
			scene.renderMenu();
			break;
		case GAME:
			scene.renderGame();
			break;
		case CREDITS:
			scene.renderCredits();
			break;
		case INSTRUCTIONS:
			scene.renderInstructions();
			break;
	}
}

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;
	if (keys[key] == IDLE || keys[key] == RELEASE) keys[key] = PRESS;
	else if (keys[key] == PRESS) keys[key] = REPEAT;
}

void Game::keyReleased(int key)
{
	keys[key] = RELEASE;
}

void Game::specialKeyPressed(int key)
{
	if (specialKeys[key] == IDLE || specialKeys[key] == RELEASE) specialKeys[key] = PRESS;
	else if (specialKeys[key] == PRESS) specialKeys[key] = REPEAT;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = RELEASE;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

keyState Game::getKey(int key) const
{
	return keys[key];
}

keyState Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}





