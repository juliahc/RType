#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Enemy.h"
#include "Game.h"

enum BASIC1Anims
{
	BASIC1_STANDARD, BASIC1_ROTATE_UP_1, BASIC1_ROTATE_UP_2, BASIC1_ROTATE_DOWN_1, BASIC1_ROTATE_DOWN_2, BASIC1_FULL_ROTATION
};

enum BASIC2Anims
{
	BASIC2_STAYLEFT, BASIC2_LEFT_UP1, BASIC2_LEFT_UP2, BASIC2_LEFT_UP3, BASIC2_STAYRIGHT, BASIC2_RIGHT_UP1, BASIC2_RIGHT_UP2, BASIC2_RIGHT_UP3, BASIC2_UP
};


void Enemy::init(const glm::ivec2& tileMapPos, Enemies enemy, ShaderProgram& shaderProgram)
{
	myType = enemy;
	bJumping = false;
	spritesheet.loadFromFile(spriteFiles[enemy], TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(enemySizes[enemy], spriteSheetSize[enemy], &spritesheet, &shaderProgram);
	
	switch (enemy) {
		case BASIC1:
			sprite->setNumberAnimations(6);

			sprite->setAnimationSpeed(BASIC1_STANDARD, 8);
			sprite->addKeyframe(BASIC1_STANDARD, glm::vec2(0.f, 0.f));

			sprite->setAnimationSpeed(BASIC1_ROTATE_UP_1, 8);
			sprite->addKeyframe(BASIC1_ROTATE_UP_1, glm::vec2(0.25f, 0.f));

			sprite->setAnimationSpeed(BASIC1_ROTATE_UP_2, 8);
			sprite->addKeyframe(BASIC1_ROTATE_UP_2, glm::vec2(0.5f, 0.f));

			sprite->setAnimationSpeed(BASIC1_ROTATE_DOWN_1, 8);
			sprite->addKeyframe(BASIC1_ROTATE_DOWN_1, glm::vec2(0.25f, 0.5f));

			sprite->setAnimationSpeed(BASIC1_ROTATE_DOWN_2, 8);
			sprite->addKeyframe(BASIC1_ROTATE_DOWN_2, glm::vec2(0.5f, 0.5f));

			sprite->setAnimationSpeed(BASIC1_FULL_ROTATION, 8);
			sprite->addKeyframe(BASIC1_FULL_ROTATION, glm::vec2(0.75f, 0.f));
			break;
		case BASIC2:
			sprite->setNumberAnimations(9);

			sprite->setAnimationSpeed(BASIC2_STAYLEFT, 8);
			sprite->addKeyframe(BASIC2_STAYLEFT, glm::vec2(0.0f, 0.0f));

			sprite->setAnimationSpeed(BASIC2_LEFT_UP1, 8);
			sprite->addKeyframe(BASIC2_LEFT_UP1, glm::vec2(0.2f, 0.f));

			sprite->setAnimationSpeed(BASIC2_LEFT_UP2, 8);
			sprite->addKeyframe(BASIC2_LEFT_UP2, glm::vec2(0.4f, 0.f));

			sprite->setAnimationSpeed(BASIC2_LEFT_UP3, 8);
			sprite->addKeyframe(BASIC2_LEFT_UP3, glm::vec2(0.6f, 0.f));

			sprite->setAnimationSpeed(BASIC2_STAYRIGHT, 8);
			sprite->addKeyframe(BASIC2_STAYRIGHT, glm::vec2(0.f, 0.5f));
			
			sprite->setAnimationSpeed(BASIC2_RIGHT_UP1, 8);
			sprite->addKeyframe(BASIC2_RIGHT_UP1, glm::vec2(0.2f, 0.5f));

			sprite->setAnimationSpeed(BASIC2_RIGHT_UP2, 8);
			sprite->addKeyframe(BASIC2_RIGHT_UP2, glm::vec2(0.4f, 0.5f));

			sprite->setAnimationSpeed(BASIC2_RIGHT_UP3, 8);
			sprite->addKeyframe(BASIC2_RIGHT_UP3, glm::vec2(0.6f, 0.5f));
			
			sprite->setAnimationSpeed(BASIC2_UP, 8);
			sprite->addKeyframe(BASIC2_UP, glm::vec2(0.8f, 0.f));
			break;
		case BASIC3:
			break;
		case BASIC4:
			break;
	}
	
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Enemy::update(int deltaTime)
{
	sprite->update(deltaTime);
	++timeLastRotationAnim;
	// update the position of the enemy according to it's type
	int random_number;
	switch (myType) {
	case BASIC1:
		if (up) {
			if (lastRotationAnim <= 2 && timeLastRotationAnim > 20) {
				if (lastRotationAnim == 2) lastRotationAnim = 4;
				sprite->changeAnimation(++lastRotationAnim);
				timeLastRotationAnim = 0;
			}
			posEnemy.y -= 1;
			if (checkCollisionUp()) {
				posEnemy.y += 1;
				up = false;
				lastRotationAnim = 2;
				sprite->changeAnimation(BASIC1_STANDARD);
			}

		}
		else {
			if (lastRotationAnim <= 4 && timeLastRotationAnim > 20) {
				sprite->changeAnimation(++lastRotationAnim);
				timeLastRotationAnim = 0;
			}
			posEnemy.y += 1;
			if (checkCollisionDown()) {
				posEnemy.y -= 1;
				up = true;
				lastRotationAnim = 0;
				sprite->changeAnimation(BASIC1_STANDARD);
			}
		}

		if (!right) {
			posEnemy.x -= 2;
			if (checkCollisionLeft()) {
				posEnemy.x += 2;
				right = true;
			}
		}
		else {
			posEnemy.x += 2;
			if (checkCollisionRight()) {
				posEnemy.x -= 2;
				right = false;
			}
		}

		break;
	case BASIC2:
		// with a 10% chance, the enemy will attack
		random_number = rand();
		if (!attacking && (random_number % 100 < 1)) {
			attacking = true;
			if (right) {
				sprite->changeAnimation(BASIC2_RIGHT_UP1);
				lastRotationAnim = BASIC2_RIGHT_UP1;
			}
			else {
				sprite->changeAnimation(++lastRotationAnim);
			}
			break;
		}
		else if (attacking) {
			if (timeLastRotationAnim > 40) {
				if (right) {
					if (lastRotationAnim < BASIC2_UP) {
						sprite->changeAnimation(++lastRotationAnim);
					}
					else {
						attacking = false;
						sprite->changeAnimation(BASIC2_STAYRIGHT);
						lastRotationAnim = BASIC2_STAYRIGHT;
					}
				}
				else {
					if (lastRotationAnim < BASIC2_LEFT_UP3) {
						sprite->changeAnimation(++lastRotationAnim);
					}
					else {
						attacking = false;
						sprite->changeAnimation(BASIC2_STAYLEFT);
						lastRotationAnim = BASIC2_STAYLEFT;
					}
				}
				timeLastRotationAnim = 0;
				break;
			}
			else ++timeLastRotationAnim;
		}
		if (!attacking) {
			if (!right) {
				posEnemy.x -= 1;
				if (checkCollisionLeft()) {
					posEnemy.x += 2;
					right = true;
					sprite->changeAnimation(BASIC2_STAYRIGHT);
					lastRotationAnim = BASIC2_STAYRIGHT;
					break;
				}
				posEnemy.y += 4;
				posEnemy.x -= 15;
				if (!checkCollisionDown()) {
					posEnemy.x += 16;
					right = true;
					sprite->changeAnimation(BASIC2_STAYRIGHT);
					lastRotationAnim = BASIC2_STAYRIGHT;
					posEnemy.y -= 4;
					break;
				}
				posEnemy.x += 15;
			}
			else {
				posEnemy.x += 1;
				if (checkCollisionRight()) {
					posEnemy.x -= 1;
					right = false;
					sprite->changeAnimation(BASIC2_STAYLEFT);
					lastRotationAnim = BASIC2_STAYLEFT;
					break;
				}
				posEnemy.y += 4;
				posEnemy.x += 15;
				if (!checkCollisionDown()) {
					posEnemy.x -= 16;
					right = false;
					sprite->changeAnimation(BASIC2_STAYLEFT);
					lastRotationAnim = BASIC2_STAYLEFT;
					posEnemy.y -= 4;
					break;
				}
				posEnemy.x -= 15;
			}
		}
		break;
	case BASIC3:
		break;
	case BASIC4:
		break;
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

bool Enemy::checkCollisionRight() {
	if (map->collisionMoveRight(posEnemy, enemySizes[myType])) return true;
	return false;
}

bool Enemy::checkCollisionLeft() {
	if (map->collisionMoveLeft(posEnemy, enemySizes[myType])) return true;
	return false;
}
bool Enemy::checkCollisionUp() {
	if (map->collisionMoveUp(posEnemy, enemySizes[myType], &posEnemy.y)) return true;
	return false;
}
bool Enemy::checkCollisionDown() {
	if (map->collisionMoveDown(posEnemy, enemySizes[myType], &posEnemy.y)) return true;
	return false;
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




