#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Shot.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};


void Shot::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const string& spriteFolder, const glm::ivec2& shotVelocity, const glm::ivec2& size, const glm::vec2& sizeInSpriteSheet, const int& damage)
{
	sizeShot = size;
	shotDamage = damage;

	spritesheet.loadFromFile(spriteFolder, TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(sizeShot, sizeInSpriteSheet, &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);
		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posShot.x), float(tileMapDispl.y + posShot.y)));

	velocity = shotVelocity;
}

void Shot::update(int deltaTime)
{
	sprite->update(deltaTime);

	posShot.x += velocity.x;
	posShot.y += velocity.y;

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posShot.x), float(tileMapDispl.y + posShot.y)));
}

void Shot::render()
{
	sprite->render();
}

void Shot::setPosition(const glm::vec2& pos)
{
	posShot = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posShot.x), float(tileMapDispl.y + posShot.y)));
}

void Shot::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

glm::ivec2 Shot::getPosition()
{
	return posShot;
}

glm::ivec2 Shot::getSize()
{
	return sizeShot;
}

int Shot::getDamage()
{
	return shotDamage;
}