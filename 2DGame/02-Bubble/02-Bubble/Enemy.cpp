#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Enemy.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum EnemyAnims
{
	BASIC, ROTATE_UP_1, ROTATE_UP_2, ROTATE_DOWN_1, ROTATE_DOWN_2, FULL_ROTATION
};


void Enemy::init(const glm::ivec2& tileMapPos, Enemies enemy, int rotationAnimations, ShaderProgram& shaderProgram)
{
	bJumping = false;
	rotationAnims = rotationAnimations;
	spritesheet.loadFromFile(spriteFiles[enemy], TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(enemySizes[enemy].first, enemySizes[enemy].second), glm::vec2(0.25, 0.5), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(6);

	sprite->setAnimationSpeed(BASIC, 8);
	sprite->addKeyframe(BASIC, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(ROTATE_UP_1, 8);
	sprite->addKeyframe(ROTATE_UP_1, glm::vec2(0.25f, 0.f));

	sprite->setAnimationSpeed(ROTATE_UP_2, 8);
	sprite->addKeyframe(ROTATE_UP_2, glm::vec2(0.5f, 0.f));

	sprite->setAnimationSpeed(ROTATE_DOWN_1, 8);
	sprite->addKeyframe(ROTATE_DOWN_1, glm::vec2(0.25f, 0.5f));

	sprite->setAnimationSpeed(ROTATE_DOWN_2, 8);
	sprite->addKeyframe(ROTATE_DOWN_2, glm::vec2(0.5f, 0.5f));

	sprite->setAnimationSpeed(FULL_ROTATION, 8);
	sprite->addKeyframe(FULL_ROTATION, glm::vec2(0.75f, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));

}

void Enemy::update(int deltaTime)
{
	sprite->update(deltaTime);
	++timeLastRotationAnim;
	if (up) {
		if (lastRotationAnim <= 2 && timeLastRotationAnim > 20) {
			if (lastRotationAnim == 2) lastRotationAnim = 4;
			sprite->changeAnimation(++lastRotationAnim);
			timeLastRotationAnim = 0;
		}
		posEnemy.y -= 1;
		if (map->collisionMoveUp(posEnemy, glm::ivec2(24, 23), &posEnemy.y) || posEnemy.y <= 350) {
			posEnemy.y += 1;
			up = false;
			lastRotationAnim = 2;
			sprite->changeAnimation(BASIC);
		}
		
	}
	else {
		if (lastRotationAnim <= 4 && timeLastRotationAnim > 20) {
			sprite->changeAnimation(++lastRotationAnim);
			timeLastRotationAnim = 0;
		}
		posEnemy.y += 1;
		if (map->collisionMoveDown(posEnemy, glm::ivec2(24, 23), &posEnemy.y) || posEnemy.y >= 400) {
			posEnemy.y -= 1;
			up = true;
			lastRotationAnim = 0;
			sprite->changeAnimation(BASIC);
		}
	}

	if (!right) {
		posEnemy.x -= 2;
		if (map->collisionMoveLeft(posEnemy, glm::ivec2(24, 15))) {
			posEnemy.x += 2;
			right = true;
		}
	}
	else {
		posEnemy.x += 2;
		if (map->collisionMoveRight(posEnemy, glm::ivec2(24, 15))) {
			posEnemy.x -= 2;
			right = false;
		}
	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Enemy::render()
{
	sprite->render();
}

void Enemy::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Enemy::setPosition(const glm::vec2& pos)
{
	posEnemy = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}




