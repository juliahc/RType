#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Force.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};


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


}

void Force::update(int deltaTime, const glm::ivec2& posPlayer)
{
	sprite->update(deltaTime);

	if (active) {
		posForce = posPlayer;
		posForce.x += 24;
		posForce.y += 1;
	}

	if (deltaTime%7 == 0) {
		if (animation == 3) animation = 0;
		else animation++;
		sprite->changeAnimation(animation);
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posForce.x), float(tileMapDispl.y + posForce.y)));
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
