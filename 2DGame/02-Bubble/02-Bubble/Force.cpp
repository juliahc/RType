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

	//Ship
	spritesheet.loadFromFile("images/force/force.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(12, 12), glm::vec2(0.25, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);
		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
		sprite->setAnimationSpeed(1, 8);
		sprite->addKeyframe(1, glm::vec2(0.25f, 0.f));
		sprite->setAnimationSpeed(2, 8);
		sprite->addKeyframe(2, glm::vec2(0.50f, 0.f));
		sprite->setAnimationSpeed(3, 8);
		sprite->addKeyframe(3, glm::vec2(0.75f, 0.f));
	sprite->changeAnimation(0);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posForce.x), float(tileMapDispl.y + posForce.y)));

	active = false;
	
	state = INACTIVE;
	type = UPGRADE_0;

	frontAttached = false;
	bottomAttached = false;

	posForce.x = 0;
	posForce.y = (SCREEN_HEIGHT / 2) - (12 / 2);
}

void Force::update(int deltaTime, const glm::ivec2& posPlayer, const int screenExtraPosition)
{
	sprite->update(deltaTime);

	if (state == INACTIVE) {
		posForce.x = 0;
		posForce.y = (SCREEN_HEIGHT / 2) - (12 / 2);
	}
	else if (state == INIT) {
		posForce.x += 2;
		if (posForce.x >= ((SCREEN_WIDTH / 2) - (12 / 2)) ) {
			state = UP;
		}
	}
	else if (frontAttached) {
		//Position to be in front of player
		posForce = posPlayer;
		posForce.x += 24;
		posForce.y += 1;
	}
	else if (bottomAttached) {
		//Position to be in the bottom of player
		posForce = posPlayer;
		posForce.x -= 12;
		posForce.y += 1;
	}
	else {

	}


	if (deltaTime%7 == 0) {
		if (animation == 3) animation = 0;
		else animation++;
		sprite->changeAnimation(animation);
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posForce.x + screenExtraPosition), float(tileMapDispl.y + posForce.y)));
}

void Force::render()
{
	sprite->render();
}

void Force::setPosition(const glm::vec2& pos)
{
	posForce = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posForce.x), float(tileMapDispl.y + posForce.y)));
}

void Force::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

glm::ivec2 Force::getPosition()
{
	return posForce;
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
