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

	//Enemy boom
	spritesheetBoom.loadFromFile("images/enemies/enemyBoom.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spriteBoom = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.5, 0.5f), &spritesheetBoom, &shaderProgram);
	spriteBoom->setNumberAnimations(8);
	spriteBoom->setAnimationSpeed(0, 8);
	spriteBoom->addKeyframe(0, glm::vec2(0.f, 0.f));
	spriteBoom->setAnimationSpeed(1, 8);
	spriteBoom->addKeyframe(1, glm::vec2(0.5f, 0.f));
	spriteBoom->setAnimationSpeed(2, 8);
	spriteBoom->addKeyframe(2, glm::vec2(0.f, 0.f));
	spriteBoom->setAnimationSpeed(3, 8);
	spriteBoom->addKeyframe(3, glm::vec2(0.5f, 0.f));
	spriteBoom->setAnimationSpeed(4, 8);
	spriteBoom->addKeyframe(4, glm::vec2(0.f, 0.5f));
	spriteBoom->setAnimationSpeed(5, 8);
	spriteBoom->addKeyframe(5, glm::vec2(0.5f, 0.f));
	spriteBoom->setAnimationSpeed(6, 8);
	spriteBoom->addKeyframe(6, glm::vec2(0.0f, 0.f));
	spriteBoom->setAnimationSpeed(7, 8);
	spriteBoom->addKeyframe(7, glm::vec2(0.5f, 0.5f));

	
	
	sprite->changeAnimation(0);
	spriteBoom->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
	spriteBoom->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Enemy::update(int deltaTime)
{
	if (!boom) {

		//Incrememnt the frame counter for rotation animations
		++timeLastRotationAnim;

		switch (myType) {
		case BASIC1:
			//Check if the enemy have to attack the player. Probability 2%
			attacking = attack(2);
			if (attacking) attackPlayer();

			//Change animation
			changeAnimation();

			//Vertical movement
			if (up) moveEnemyVertically(1, true);
			else moveEnemyVertically(1, false);

			//Horizontal movement
			if (right) moveEnemyHorizontally(2, true);
			else moveEnemyHorizontally(2, false);
			break;
		case BASIC2:
			//Check if the enemy have to attack the player. Probability 5%
			attacking = attack(5);
			//Attack + animation
			if (attacking) attackPlayer();

			//Horizontal movement + Animations
			if (!attacking) {
				if (right) moveEnemyHorizontally(2, true);
				else moveEnemyHorizontally(2, false);
			}
			break;
		case BASIC3:
			break;
		case BASIC4:
			break;
		}
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
	}
	else {
		spriteBoom->update(deltaTime);
		if (boomAnimation < 8) {
			boomAnimation += 1;
			spriteBoom->changeAnimation(boomAnimation);
		}
	}
	sprite->update(deltaTime);
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

void Enemy::moveEnemyHorizontally(int length, bool right) {
	switch (myType) {
		case BASIC1:
			if (right) {
				posEnemy.x += 2;
				if (checkCollisionRight()) {
					posEnemy.x -= 2;
					this->right = false;
				}
			}
			else {
				posEnemy.x -= 2;
				if (checkCollisionLeft()) {
					posEnemy.x += 2;
					this->right = true;
				}
			}
			break;
		case BASIC2:
			if (right) {
				posEnemy.x += 1;
				if (checkCollisionRight()) {
					posEnemy.x -= 1;
					this->right = false;
					sprite->changeAnimation(BASIC2_STAYLEFT);
					lastRotationAnim = BASIC2_STAYLEFT;
					break;
				}
				posEnemy.y += 4;
				posEnemy.x += 15;
				if (!checkCollisionDown()) {
					posEnemy.x -= 16;
					this->right = false;
					sprite->changeAnimation(BASIC2_STAYLEFT);
					lastRotationAnim = BASIC2_STAYLEFT;
					posEnemy.y -= 4;
					break;
				}
				posEnemy.x -= 15;
			}
			else {
				posEnemy.x -= 1;
				if (checkCollisionLeft()) {
					posEnemy.x += 2;
					this->right = true;
					sprite->changeAnimation(BASIC2_STAYRIGHT);
					lastRotationAnim = BASIC2_STAYRIGHT;
					break;
				}
				posEnemy.y += 4;
				posEnemy.x -= 15;
				if (!checkCollisionDown()) {
					posEnemy.x += 16;
					this->right = true;
					sprite->changeAnimation(BASIC2_STAYRIGHT);
					lastRotationAnim = BASIC2_STAYRIGHT;
					posEnemy.y -= 4;
					break;
				}
				posEnemy.x += 15;
			}
	}
}

void Enemy::moveEnemyVertically(int length, bool up) {
	switch (myType) {
		case BASIC1:
			if (up) {
				posEnemy.y -= 1;
				if (checkCollisionUp() || posEnemy.y < 98) {
					posEnemy.y += 1;
					this->up = false;
				}
			}
			else {
				posEnemy.y += 1;
				if (checkCollisionDown() || posEnemy.y > 158) {
					posEnemy.y -= 1;
					this->up = true;
				}
			}
			break;
	}
}

bool Enemy::attack(int probability) {
	// Random number to decide actions
	int random_number = rand();
	if (random_number % 100 < probability % 101 && timesWithoutAttacking > 49) {
		timesWithoutAttacking = 0;
		return true;
	}
	timesWithoutAttacking++;
	return false;
}

void Enemy::attackPlayer() {
	switch (myType) {
		case BASIC1:
			shooting = true;
			break;
		case BASIC2:
			if (actualAttackAnimation == -1) {
				//First attack animation
				if (right) {
					sprite->changeAnimation(BASIC2_RIGHT_UP1);
					lastRotationAnim = BASIC2_RIGHT_UP1;
				}
				else {
					sprite->changeAnimation(++lastRotationAnim);
				}
			}
			else {
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
			break;
	}
}

void Enemy::changeAnimation() {
	switch (myType) {
		case BASIC1:
			if (lastRotationAnim <= 5 && timeLastRotationAnim > 5) {
				if (lastRotationAnim == 5) lastRotationAnim = -1;
				sprite->changeAnimation(++lastRotationAnim);
				timeLastRotationAnim = 0;
			}
			break;
	}
}

void Enemy::render()
{
	if (!boom) sprite->render();
	else spriteBoom->render();
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

glm::ivec2 Enemy::getPosition() 
{
	return posEnemy;
}

glm::ivec2 Enemy::getSize() 
{
	return enemySizes[myType];
}

void Enemy::collision()
{
	spriteBoom->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
	boom = true;
}

bool Enemy::died()
{
	return boom;
}

bool Enemy::boomFinished()
{
	return boomAnimation == 8;
}

/*
Attacking
*/
bool Enemy::isShooting() {
	return shooting;
}
glm::ivec2 Enemy::getShotSize() {
	return shotSize[myType];
}
glm::ivec2 Enemy::getShotVelocity(glm::ivec2 playerPosition) {
	int difference = playerPosition.y - posEnemy.y;
	int yvel = ceil(difference / 20);
	if (yvel < 0) yvel = max(yvel, -3);
	else yvel = min(yvel, 3);
	int xvel = playerPosition.x - posEnemy.x;
	if (xvel < 1) xvel = -4;
	else xvel = 4;
	return glm::ivec2(xvel, yvel);
}
string Enemy::getShotSprite() {
	return spriteShot[myType];
}

void Enemy::enemyAlreadyAttacked() {
	this->shooting = false;
}
glm::vec2 Enemy::getShotSizeInSpriteSheet() {
	return shotSizeInSpriteSheet;
}