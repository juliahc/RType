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
	int starterType = 0;
	
	tileMapDispl = tileMapPos;

	sizeForces[0] = glm::ivec2(12, 12);
	sizeForces[1] = glm::ivec2(15, 12);
	sizeForces[2] = glm::ivec2(16, 16);
	sizeForces[3] = glm::ivec2(15, 12);

	sizeForce = sizeForces[starterType];
	type = starterType;

	spritesheets[0].loadFromFile("images/force/force.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheets[1].loadFromFile("images/force/force1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheets[2].loadFromFile("images/force/force2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheets[3].loadFromFile("images/force/force1Bottom.png", TEXTURE_PIXEL_FORMAT_RGBA);

	for (int i = 0; i < 4; i++) {
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

	glm::ivec2 initialPos = posForce;

	if (Game::instance().getKey('f') == PRESS) {
		active = true;
		state = ACTIVE;
	}

	if (state == INACTIVE) {
		posForce.x = screenPosX;
		posForce.y = (SCREEN_HEIGHT / 2) - (sizeForce.y / 2);
	}
	else if (state == INIT) {
		posForce.x += 2;
		if (posForce.x >= ((SCREEN_WIDTH / 2) + screenPosX - (sizeForce.x / 2)) ) {
			state = ACTIVE;
		}
	}
	else if (state == PUSHED_LEFT || state == PUSHED_RIGHT) {
		nextPositionPushed(screenPosX);
	}
	else if (Game::instance().getKey('z') == PRESS && (attached == FRONT || attached == BOTTOM)) {
		if (attached == FRONT) state = PUSHED_RIGHT;
		else {
			state = PUSHED_LEFT;
			if (type == 3) type = 1;
		}
		attached = NOATTACHED;
	}
	else if (attached == FRONT) {
		//Position to be in front of player
		posForce = posPlayer;
		posForce.x += 24;
		posForce.y += 1;
		if (type == 1) posForce.y -= 2;
		if (type == 2) {
			posForce.x -= 3;
			posForce.y -= 2;
		}
	}
	else if (attached == BOTTOM) {
		//Position to be in the bottom of player
		posForce = posPlayer;
		posForce.x -= sizeForce.x;
		//posForce.y += 1;
		if (type == 1 || type == 3) posForce.y -= 1;
		if (type == 3) posForce.x += 4;
		if (type == 2) {
			posForce.x += 5;
			posForce.y -= 1;
		}
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
	

	//check collisions
	if (map->collisionMoveLeft(posForce, sizeForce)) {
		posForce.x++;
		while (map->collisionMoveLeft(posForce, sizeForce))posForce.x++;
	}
	else if (map->collisionMoveRight(posForce, sizeForce)) {
		posForce.x--;
		while (map->collisionMoveLeft(posForce, sizeForce))posForce.x--;
	}
	if (!map->collisionMoveUp(posForce, sizeForce, &posForce.y)) map->collisionMoveDown(posForce, sizeForce, &posForce.y);
	


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

bool Force::isAttached() {
	return !(attached == NOATTACHED);
}

bool Force::isAttachedFront() {
	return attached == FRONT;
}

void Force::setActive(bool newActive)
{
	active = newActive;
	if (active) state = INIT;
}

void Force::setAttached(string newAttached)
{
	if (state != PUSHED_LEFT && state != PUSHED_RIGHT) {
		if (newAttached == "bottom") {
			attached = BOTTOM;
			if (type == 1) type = 3;
		}
		else if (newAttached == "front") attached = FRONT;
	}
}

void Force::setType(int newType)
{
	if (newType == 0 || newType == 1 || newType == 2) {
		type = newType;
		if (type == 1 && attached == BOTTOM) type = 3;
		sizeForce = sizeForces[type];
	}
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
			if (posPlayer.y > posForce.y) posForce.y += vel.y;
			else if (posPlayer.y < posForce.y) posForce.y -= vel.y;
		}
		else { //Force right
			if (posForce.x == rightLine) { //Force in rightLine
				//mirar y
				if (posPlayer.y > posForce.y) posForce.y += vel.y;
				else if (posPlayer.y < posForce.y) posForce.y -= vel.y;
			} 
			else if (posForce.x > rightLine) { //Force passed the rightLine
				posForce.x -= vel.x;
				if (posPlayer.y > posForce.y) posForce.y += vel.y;
				else if (posPlayer.y < posForce.y) posForce.y -= vel.y;
			}
			else { //Force not in leftLine
				posForce.x += vel.x;
				if (posPlayer.y > posForce.y) posForce.y += vel.y;
				else if (posPlayer.y < posForce.y) posForce.y -= vel.y;
			}
		}
	}
	else { //Player right
		if (forceLeft) { //Force left
			if (posForce.x == leftLine) { //Force in leftLine
				//mirar y
				if (posPlayer.y > posForce.y) posForce.y += vel.y;
				else if (posPlayer.y < posForce.y) posForce.y -= vel.y;
			}
			else if (posForce.x < leftLine) {
				posForce.x += vel.x;
				if (posPlayer.y > posForce.y) posForce.y += vel.y;
				else if (posPlayer.y < posForce.y) posForce.y -= vel.y;
			}
			else { //Force not in leftLine
				posForce.x -= vel.x;
				if (posPlayer.y > posForce.y) posForce.y += vel.y;
				else if (posPlayer.y < posForce.y) posForce.y -= vel.y;
			}
		}
		else { //Force right
			//Anar cap a l'a dreta'esquerra
			posForce.x -= vel.x;
			if (posPlayer.y > posForce.y) posForce.y += vel.y;
			else if (posPlayer.y < posForce.y) posForce.y -= vel.y;
		}
	}
}

void Force::nextPositionPushed(const int screenPosX)
{
	if (state == PUSHED_RIGHT) {
		int maxX = SCREEN_WIDTH + screenPosX - sizeForce.x - 1;
		int space = maxX - posForce.x;
		if (space < 6 && space > 0) posForce.x += space;
		else if (posForce.x < maxX) posForce.x += 6;
		else state = ACTIVE;
		if (map->collisionMoveRight(posForce, sizeForce)) {
			posForce.x--;
			while (map->collisionMoveRight(posForce, sizeForce)) posForce.x--;
			state = ACTIVE;
		}
	} 
	else {
		int minX = screenPosX;
		int space = posForce.x - minX;
		if (space < 6 && space > 0) posForce.x -= space;
		else if (posForce.x > minX) posForce.x -= 6;
		else state = ACTIVE;
		if (map->collisionMoveRight(posForce, sizeForce)) {
			posForce.x++;
			while (map->collisionMoveRight(posForce, sizeForce)) posForce.x++;
			state = ACTIVE;
		}
	}
}

void Force::addShot(int& nbShots, glm::ivec2& posShot, glm::ivec2* velocities, bool& front)
{
	posShot = posForce;
	posShot.x += sizeForce.x;
	posShot.y += sizeForce.y / 2;

	if (attached == NOATTACHED) {
		if (type == 0) {
			nbShots = 1;
		}
		if (type == 1 || type == 2 || type == 3) {
			//Diagonal shots
			nbShots = 2;
			velocities[0] = glm::ivec2(4,-2);
			velocities[1] = glm::ivec2(4, 2);
		} 
		if (type == 2) {
			//Up and down shots
			nbShots = 4;
			velocities[2] = glm::ivec2(vel.x,-4);
			velocities[3] = glm::ivec2(vel.x, 4);
		}
	}
	else {
		nbShots = -1;
		if (type == 1 || type == 3) {
			//upgrade 1 shot
			nbShots = 5;
		}
		else if (type == 2) {
			//upgrade 2 shot
			nbShots = 6;
		}
		if (attached == FRONT)  velocities[0].x = 1;
		else velocities[0].x = -1;
	}

	front = attached == FRONT;
}

int Force::getType() {
	return type;
}