#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Force.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

void Force::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	tileMapDispl = tileMapPos;

	sizeForces[0] = glm::ivec2(12, 12);
	sizeForces[1] = glm::ivec2(15, 12);
	sizeForces[2] = glm::ivec2(16, 16);

	sizeForce = sizeForces[0];

	spritesheets[0].loadFromFile("images/force/force.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheets[1].loadFromFile("images/force/force1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheets[2].loadFromFile("images/force/force2.png", TEXTURE_PIXEL_FORMAT_RGBA);

	for (int i = 0; i < 3; i++) {
		sprites[i] = Sprite::createSprite(sizeForce, glm::vec2(0.25, 1.f), &spritesheets[i], &shaderProgram);
		sprites[i]->setNumberAnimations(4);
		sprites[i]->setAnimationSpeed(0, 8);
			sprites[i]->addKeyframe(0, glm::vec2(0.f, 0.f));
			sprites[i]->setAnimationSpeed(1, 8);
			sprites[i]->addKeyframe(1, glm::vec2(0.25f, 0.f));
			sprites[i]->setAnimationSpeed(2, 8);
			sprites[i]->addKeyframe(2, glm::vec2(0.50f, 0.f));
			sprites[i]->setAnimationSpeed(3, 8);
			sprites[i]->addKeyframe(3, glm::vec2(0.75f, 0.f));
		sprites[i]->changeAnimation(0);
		sprites[i]->setPosition(glm::vec2(float(tileMapDispl.x + posForce.x), float(tileMapDispl.y + posForce.y)));
	}

	active = false;
	
	
	state = INACTIVE;
	type = 0;

	attached = NOATTACHED;

	posForce.x = 0;
	posForce.y = (SCREEN_HEIGHT / 2) - (sizeForce.y / 2);

	leftLine = SCREEN_WIDTH / 4;
	rightLine = (SCREEN_WIDTH / 4) * 3;

	for (int i = 0; i < 30; i++) lastPlayerPos.push(glm::ivec2(0,0));

	vel = glm::ivec2(1,1);
}

void Force::update(int deltaTime, const glm::ivec2& posPlayer, const int screenPosX)
{
	sprites[0]->update(deltaTime);
	sprites[1]->update(deltaTime);
	sprites[2]->update(deltaTime);

	if (state == INACTIVE) {
		posForce.x = screenPosX;
		posForce.y = (SCREEN_HEIGHT / 2) - (sizeForce.y / 2);
	}
	else if (state == INIT) {
		posForce.x += 2;
		if (posForce.x >= ((SCREEN_WIDTH / 2) + screenPosX - (sizeForce.x / 2)) ) {
			state = UP;
		}
	}
	else if (attached == FRONT) {
		//Position to be in front of player
		posForce = posPlayer;
		posForce.x += 24;
		posForce.y += 1;
		if (type == 2) posForce.x -= 3;
	}
	else if (attached == BOTTOM) {
		//Position to be in the bottom of player
		posForce = posPlayer;
		posForce.x -= sizeForce.x;
		posForce.y += 1;
	}
	else {
		leftLine = (SCREEN_WIDTH / 4) + screenPosX;
		rightLine = ((SCREEN_WIDTH / 4) * 3) + screenPosX;
		nextPosition(screenPosX);
	}

	lastPlayerPos.push(posPlayer);
	lastPlayerPos.pop();

	if (deltaTime%7 == 0) {
		if (animation == 3) animation = 0;
		else animation++;
		sprites[type]->changeAnimation(animation);
	}
	if (Game::instance().getKey('a')) {
		int a = 0;
	}
	sprites[type]->setPosition(glm::vec2(float(tileMapDispl.x + posForce.x), float(tileMapDispl.y + posForce.y)));
}

void Force::render()
{
	sprites[type]->render();
}

void Force::setPosition(const glm::vec2& pos)
{
	posForce = pos;
	sprites[type]->setPosition(glm::vec2(float(tileMapDispl.x + posForce.x), float(tileMapDispl.y + posForce.y)));
}

void Force::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

glm::ivec2 Force::getPosition()
{
	return posForce;
}

glm::ivec2 Force::getSize()
{
	return sizeForce;
}

bool Force::isActive()
{
	return active;
}

void Force::setActive(bool newActive)
{
	active = newActive;
	if (active) state = INIT;
}

void Force::setAttached(string newAttached)
{
	if (newAttached == "bottom") attached = BOTTOM;
	else if (newAttached == "front") attached = FRONT;
}

void Force::setType(int newType)
{
	if (newType == 0 || newType == 1 || newType == 2) type = newType;
}

int Force::getWidth()
{
	if (attached == FRONT) {
		switch (type)
		{
			case 0:
				 return 12;
				 break;
			case 1:
				return 12;
				break;
			case 2:
				return 10;
				break;
		}
	}
	return 0;
}

void Force::nextPosition(const int screenPosX)
{
	glm::ivec2 posPlayer = lastPlayerPos.front();
	bool playerLeft = posPlayer.x <= (screenPosX + (SCREEN_WIDTH / 2));
	bool forceLeft = posForce.x <= (screenPosX + (SCREEN_WIDTH / 2));
	
	if (playerLeft) { //Player left
		if (forceLeft) { //Force left
			//Anar cap a la dreta
			posForce.x += vel.x;
		}
		else { //Force right
			if (posForce.x >= rightLine) { //Force in leftLine
				//mirar y
				if (posPlayer.y > posForce.y) posForce.y += vel.y;
				else if (posPlayer.y < posForce.y) posForce.y -= vel.y;
			} 
			else { //Force not in leftLine
				posForce.x += vel.x;
			}
		}
	}
	else { //Player right
		if (forceLeft) { //Force left
			if (posForce.x <= leftLine) { //Force in leftLine
				//mirar y
				if (posPlayer.y > posForce.y) posForce.y += vel.y;
				else if (posPlayer.y < posForce.y) posForce.y -= vel.y;
			}
			else { //Force not in leftLine
				posForce.x -= vel.x;
			}
		}
		else { //Force right
			//Anar cap a l'a dreta'esquerra
			posForce.x -= vel.x;
		}
	}
}