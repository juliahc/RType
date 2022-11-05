#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"

void Game::init()
{
	bPlay = true;
	previousState = NONE;
	state = TRANSITION;
	nextState = MENU;;
	glClearColor(0.f, 0.f, 0.f, 1.f);
	scene.init();
	for (int key : keysInt) key = 0;
}

bool Game::update(int deltaTime)
{
	//Process key input
	processInput();

	//Game loop
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
		case TRANSITION:
			scene.updateTransition(deltaTime);
			break;
		case GAMEOVER:
			scene.updateGameOver(deltaTime);
			break;
	}

	return bPlay;
}

void Game::renderState(GameState state) {
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
		case TRANSITION:
			scene.renderTransition();
			break;
		case GAMEOVER:
			scene.renderGameover();
	}
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderState(state);
}

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;

	keysBool[key] = true;
}

void Game::keyReleased(int key)
{
	keysBool[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeysBool[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeysBool[key] = false;
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

void Game::setState(GameState newState)
{
	previousState = state;
	
	if (state != TRANSITION) {
		state = TRANSITION;
		nextState = newState;
	}
	else {
		state = newState;
		nextState = NONE;
	}
}

GameState Game::getPreviousState()
{
	return previousState;
}

GameState Game::getNextState()
{
	return nextState;
}

void Game::processInput() 
{
	//Process keys
	for (int i = 0; i < sizeof(keysBool); ++i) {
		if (keysBool[i]) { //i key is pressed
			if ((keys[i] == IDLE && keysInt[i] == 0) || keys[i] == RELEASE) keys[i] = PRESS;
			else if (keys[i] == PRESS) keys[i] = IDLE;
			else if (keys[i] == IDLE && keysInt[i] > nbFramesRepeat) keys[i] = REPEAT;
			keysInt[i]++;
		}
		else { //i key is not pressed
			if (keys[i] == PRESS || keys[i] == REPEAT || (keys[i] == IDLE && keysInt[i] > 0)) {
				keysInt[i] = 0;
				keys[i] = RELEASE;
			}
			else if (keys[i] == RELEASE) keys[i] = IDLE;
		}
	}

	//Process special keys
	for (int i = 0; i < sizeof(specialKeysBool); ++i) {
		if (specialKeysBool[i]) { //i key is pressed
			if (specialKeys[i] == IDLE || specialKeys[i] == RELEASE) specialKeys[i] = PRESS;
			else if (specialKeys[i] == PRESS) specialKeys[i] = REPEAT;
		}
		else { //i key is not pressed
			if (specialKeys[i] == PRESS || specialKeys[i] == REPEAT) specialKeys[i] = RELEASE;
			else if (specialKeys[i] == RELEASE) specialKeys[i] = IDLE;
		}
	}

}

