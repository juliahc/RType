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
enum BASIC3Anims
{
	BASIC3_STAYLEFT, BASIC3_RIGHTFOOT, BASIC3_NEUTRAL, BASIC3_LEFTFOOT, BASIC3_JUMPING, BASIC3_AIR
};
enum BASIC4Anims
{
	BASIC4_UP_LEFT1, BASIC4_UP_LEFT2, BASIC4_UP_LEFT3, BASIC4_UP_LEFT4, BASIC4_UP_LEFT5,
	BASIC4_UP_RIGHT5, BASIC4_UP_RIGHT4, BASIC4_UP_RIGHT3, BASIC4_UP_RIGHT2, BASIC4_UP_RIGHT1,
	BASIC4_DOWN_LEFT5, BASIC4_DOWN_LEFT4, BASIC4_DOWN_LEFT3, BASIC4_DOWN_LEFT2, BASIC4_DOWN_LEFT1,
	BASIC4_DOWN_RIGHT1, BASIC4_DOWN_RIGHT2, BASIC4_DOWN_RIGHT3, BASIC4_DOWN_RIGHT4, BASIC4_DOWN_RIGHT5
};
enum BOSSAnims
{
	BASIC, EGG1, EGG2, EGG3, ANGRY
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
			sprite->setNumberAnimations(6);
			
			sprite->setAnimationSpeed(BASIC3_STAYLEFT, 8);
			sprite->addKeyframe(BASIC3_STAYLEFT, glm::vec2(0.0f, 0.0f));
			
			sprite->setAnimationSpeed(BASIC3_RIGHTFOOT, 8);
			sprite->addKeyframe(BASIC3_RIGHTFOOT, glm::vec2(0.25f, 0.0f));
			
			sprite->setAnimationSpeed(BASIC3_NEUTRAL, 8);
			sprite->addKeyframe(BASIC3_NEUTRAL, glm::vec2(0.5f, 0.0f));
			
			sprite->setAnimationSpeed(BASIC3_LEFTFOOT, 8);
			sprite->addKeyframe(BASIC3_LEFTFOOT, glm::vec2(0.75f, 0.0f));

			sprite->setAnimationSpeed(BASIC3_JUMPING, 8);
			sprite->addKeyframe(BASIC3_JUMPING, glm::vec2(0.25f, 0.5f));

			sprite->setAnimationSpeed(BASIC3_AIR, 8);
			sprite->addKeyframe(BASIC3_AIR, glm::vec2(0.0f, 0.5f));
			break;
		case BASIC4:
			sprite->setNumberAnimations(20);

			//LEFT UP
			sprite->setAnimationSpeed(BASIC4_UP_LEFT1, 8);
			sprite->addKeyframe(BASIC4_UP_LEFT1, glm::vec2(0.0f, 0.0f));

			sprite->setAnimationSpeed(BASIC4_UP_LEFT2, 8);
			sprite->addKeyframe(BASIC4_UP_LEFT2, glm::vec2(0.2f, 0.0f));

			sprite->setAnimationSpeed(BASIC4_UP_LEFT3, 8);
			sprite->addKeyframe(BASIC4_UP_LEFT3, glm::vec2(0.4f, 0.0f));

			sprite->setAnimationSpeed(BASIC4_UP_LEFT4, 8);
			sprite->addKeyframe(BASIC4_UP_LEFT4, glm::vec2(0.6f, 0.0f));

			sprite->setAnimationSpeed(BASIC4_UP_LEFT5, 8);
			sprite->addKeyframe(BASIC4_UP_LEFT5, glm::vec2(0.8f, 0.0f));

			//RIGHT UP
			sprite->setAnimationSpeed(BASIC4_UP_RIGHT5, 8);
			sprite->addKeyframe(BASIC4_UP_RIGHT5, glm::vec2(0.0f, 0.25f));
			
			sprite->setAnimationSpeed(BASIC4_UP_RIGHT4, 8);
			sprite->addKeyframe(BASIC4_UP_RIGHT4, glm::vec2(0.2f, 0.25f));
			
			sprite->setAnimationSpeed(BASIC4_UP_RIGHT3, 8);
			sprite->addKeyframe(BASIC4_UP_RIGHT3, glm::vec2(0.4f, 0.25f));
			
			sprite->setAnimationSpeed(BASIC4_UP_RIGHT2, 8);
			sprite->addKeyframe(BASIC4_UP_RIGHT2, glm::vec2(0.6f, 0.25f));
			
			sprite->setAnimationSpeed(BASIC4_UP_RIGHT1, 8);
			sprite->addKeyframe(BASIC4_UP_RIGHT1, glm::vec2(0.8f, 0.25f));

			//LEFT DOWN
			sprite->setAnimationSpeed(BASIC4_DOWN_LEFT5, 8);
			sprite->addKeyframe(BASIC4_DOWN_LEFT5, glm::vec2(0.0f, 0.5f));

			sprite->setAnimationSpeed(BASIC4_DOWN_LEFT4, 8);
			sprite->addKeyframe(BASIC4_DOWN_LEFT4, glm::vec2(0.2f, 0.5f));

			sprite->setAnimationSpeed(BASIC4_DOWN_LEFT3, 8);
			sprite->addKeyframe(BASIC4_DOWN_LEFT3, glm::vec2(0.4f, 0.5f));

			sprite->setAnimationSpeed(BASIC4_DOWN_LEFT2, 8);
			sprite->addKeyframe(BASIC4_DOWN_LEFT2, glm::vec2(0.6f, 0.5f));

			sprite->setAnimationSpeed(BASIC4_DOWN_LEFT1, 8);
			sprite->addKeyframe(BASIC4_DOWN_LEFT1, glm::vec2(0.8f, 0.5f));


			//RIGHT DOWN
			sprite->setAnimationSpeed(BASIC4_DOWN_RIGHT1, 8);
			sprite->addKeyframe(BASIC4_DOWN_RIGHT1, glm::vec2(0.0f, 0.75f));

			sprite->setAnimationSpeed(BASIC4_DOWN_RIGHT2, 8);
			sprite->addKeyframe(BASIC4_DOWN_RIGHT2, glm::vec2(0.2f, 0.75f));

			sprite->setAnimationSpeed(BASIC4_DOWN_RIGHT3, 8);
			sprite->addKeyframe(BASIC4_DOWN_RIGHT3, glm::vec2(0.4f, 0.75f));

			sprite->setAnimationSpeed(BASIC4_DOWN_RIGHT4, 8);
			sprite->addKeyframe(BASIC4_DOWN_RIGHT4, glm::vec2(0.6f, 0.75f));

			sprite->setAnimationSpeed(BASIC4_DOWN_RIGHT5, 8);
			sprite->addKeyframe(BASIC4_DOWN_RIGHT5, glm::vec2(0.8f, 0.75f));

			break;
		case BOSS:
			sprite->setNumberAnimations(5);
			//BASIC, EGG1, EGG2, EGG3, ANGRY
			
			sprite->setAnimationSpeed(BASIC, 8);
			sprite->addKeyframe(BASIC, glm::vec2(0.0f, 0.0f));

			sprite->setAnimationSpeed(EGG1, 8);
			sprite->addKeyframe(EGG1, glm::vec2(0.2f, 0.0f));

			sprite->setAnimationSpeed(EGG2, 8);
			sprite->addKeyframe(EGG2, glm::vec2(0.4f, 0.0f));

			sprite->setAnimationSpeed(EGG3, 8);
			sprite->addKeyframe(EGG3, glm::vec2(0.6f, 0.0f));

			sprite->setAnimationSpeed(ANGRY, 8);
			sprite->addKeyframe(ANGRY, glm::vec2(0.8f, 0.0f));
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
void Enemy::update(int deltaTime, glm::ivec2 posPlayer)
{
	this->posPlayer = posPlayer;
	if (!boom) {

		//Incrememnt the frame counter for rotation animations
		++timeLastRotationAnim;

		switch (myType) {
		case BASIC1:
			//Check if the enemy have to attack the player. Probability 2%
			attacking = attack(5);
			if (attacking) attackPlayer();

			//Change animation
			changeAnimation();

			//Vertical movement
			if (up) moveEnemyVertically(1, true);
			else moveEnemyVertically(1, false);

			//Horizontal movement
			if (right) moveEnemyHorizontally(1, true);
			else moveEnemyHorizontally(1, false);
			break;

		case BASIC2:
			if (startWalking) {
				//restart Walking
				restartWalking();
			}
			else {
				if (!attacking) {
					//Check if the enemy have to attack the player. Probability 5%
					attacking = attack(10);
				}
				//Attack + animation
				if (attacking) attackPlayer();
				else {
					//Horizontal movement + Animations
					if (right) moveEnemyHorizontally(1, true);
					else moveEnemyHorizontally(1, false);
				}
			}

			break;

		case BASIC3:
			//Check if the enemy is jumping
			if (startJumping()) {
				//Check the jump states
				jumpEnemy();
				if (stopJumping) {
					//In case the enemy arrives to ground, stop jumping && change animation
					sprite->changeAnimation(BASIC3_STAYLEFT);
					lastRotationAnim = BASIC3_STAYLEFT;
					bJumping = false;
					stopJumping = false;
					timesWithoutJumping = 0;
					stay = true;
				}
				else {
					//If the enemy is jumping
					if (up) moveEnemyVertically(1, true);
					else moveEnemyVertically(1, false);	
				}
			}
			else {
				++timesWithoutJumping;
			}
			//Horizontal movement + Animations
			if (bJumping || (!bJumping && !stay)) moveEnemyHorizontally(1, false);
			else if (stay) {
				if (stayCounter > 60) {
					stay = false; 
					stayCounter = -1;
				}
				++stayCounter;
			}
			break;

		case BASIC4:
			//changeAnimation taking into account if the enemy is in the top side
			changeAnimation();
			//Check if the enemy have to attack
			attacking = attack(4);
			if (attacking) attackPlayer();
			break;

		case BOSS:
			//Change the animation of the boss
			changeAnimation();
			if (attack(0)) attackPlayer();
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
				posEnemy.x += length;
				if (checkCollisionRight()) {
					posEnemy.x -= length;
					this->right = false;
				}
			}
			else {
				posEnemy.x -= length;
				if (checkCollisionLeft()) {
					posEnemy.x += length;
					this->right = true;
				}
			}
			break;
		case BASIC2:
			if (right) {
				posEnemy.x += length;
				if (checkCollisionRight()) {
					posEnemy.x -= length;
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
				posEnemy.x -= length;
				if (checkCollisionLeft()) {
					posEnemy.x += length;
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
			break;
		case BASIC3:
			//Move the enemy horizontally
			posEnemy.x -= length;
			if (checkCollisionLeft()) {
				posEnemy.x += length;
				bJumping = true;
			}
			if (!bJumping) {
				posEnemy.y += 2;
				if (checkCollisionDown() && !stay && (rand() % 100) < 5 && (rand() % 100) < 5) {
					stay = true;
					posEnemy.y -= 2;
					return;
				}
				if (!checkCollisionDown()) -posEnemy.y;
				//Change animations
				changeAnimation();
			}
			break;
	}
}
void Enemy::moveEnemyVertically(int length, bool up) {
	switch (myType) {
		case BASIC1:
			if (up) {
				posEnemy.y -= length;
				if (checkCollisionUp() || posEnemy.y < 98) {
					posEnemy.y += length;
					this->up = false;
				}
			}
			else {
				posEnemy.y += length;
				if (checkCollisionDown() || posEnemy.y > 158) {
					posEnemy.y -= length;
					this->up = true;
				}
			}
			break;
		case BASIC3:
			if (bJumping) {
				if (timesToNextY > 10) {
					if (up) {
						posEnemy.y -= length;
						if (lastRotationAnim != BASIC3_AIR && lastRotationAnim != BASIC3_JUMPING) {
							lastRotationAnim = BASIC3_JUMPING;
							sprite->changeAnimation(lastRotationAnim);
						}
						else if (lastRotationAnim != BASIC3_AIR && timeLastRotationAnim > 10) {
							lastRotationAnim = BASIC3_AIR;
							sprite->changeAnimation(lastRotationAnim);
							timeLastRotationAnim = 0;
						}
					}
					else {
						posEnemy.y += length;
					}
					timesToNextY = 0;
				}
				++timesToNextY;
			}
			break;
	}
}
void Enemy::restartWalking() {
	if (timeLastRotationAnim > 15/*30*/) {
		--lastRotationAnim;
		if (!right && lastRotationAnim == BASIC2_RIGHT_UP3) {
			lastRotationAnim = BASIC2_LEFT_UP3;
		}
		if (lastRotationAnim == BASIC2_STAYLEFT || lastRotationAnim == BASIC2_STAYRIGHT) startWalking = false;
		sprite->changeAnimation(lastRotationAnim);
		actualAttackAnimation = lastRotationAnim;
		timeLastRotationAnim = 0;
	}
	//else ++timeLastRotationAnim;
}
void Enemy::changeAnimation() {
	int cannonDirection;
	switch (myType) {
		case BASIC1:
			if (lastRotationAnim <= 5 && timeLastRotationAnim > 3) {
				if (lastRotationAnim == 5) lastRotationAnim = -1;
				sprite->changeAnimation(++lastRotationAnim);
				timeLastRotationAnim = 0;
			}
			break;
		case BASIC3:
			if (timeLastRotationAnim > 15) {
				if (rightFoot) {
					++lastRotationAnim;
					if (lastRotationAnim == 4) {
						lastRotationAnim = 2;
						rightFoot = false;
					}
				}
				else {
					--lastRotationAnim;
					if (lastRotationAnim <= 0) {
						lastRotationAnim = 2;
						rightFoot = true;
					}
				}
				sprite->changeAnimation(lastRotationAnim);
				timeLastRotationAnim = 0;
			}
			else {
				++timeLastRotationAnim;
			}
			break;

		case BASIC4:
			cannonDirection = (posPlayer.x - posEnemy.x) / 38;
			if (lookingTop) {
				//Top
				if (cannonDirection < 0) {
					//Left
					cannonDirection = 4 - min(abs(cannonDirection), 4);
					sprite->changeAnimation(cannonDirection);
				}
				else {
					//Right
					cannonDirection = min(abs(cannonDirection), 4);
					sprite->changeAnimation(5+cannonDirection);
				}
			}
			else {
				//Bottom
				if (cannonDirection < 0) {
					//Left
					cannonDirection = 4 - min(abs(cannonDirection), 4);
					sprite->changeAnimation(15 + cannonDirection);
				}
				else {
					//Right
					cannonDirection = min(abs(cannonDirection), 4);
					sprite->changeAnimation(10 + cannonDirection);
				}
			}
			break;
			
		case BOSS:
			if (restartSprite) {
				if (timeLastRotationAnim > 50) {
					--lastRotationAnim;
					if (lastRotationAnim == 0) {
						restartSprite = false;
					}
					sprite->changeAnimation(lastRotationAnim);
					timeLastRotationAnim = 0;
				}
			}
			else if (!beam) {
				if (electricShots) {
					pendingBeam = true;
				}
				if (delayFirstAnimation < 500) delayFirstAnimation++;
				else {
					if (timeLastRotationAnim > 75/*150*/) {
						lastRotationAnim++;
						if (lastRotationAnim == 3) {
							startingBeam = true;
							beam = true;
						}
						sprite->changeAnimation(lastRotationAnim);
						timeLastRotationAnim = 0;
					}
					//timeLastRotationAnim++;
				}
			}
			break;
	}
}

bool Enemy::attack(int probability) {
	// Random number to decide actions
	int random_number = rand();
	switch (myType) {
		case BASIC1:
		case BASIC2:
		case BASIC4:
			if ((random_number % 100) < (probability % 101) && (rand() % 100 < (probability * 2) && timesWithoutAttacking > 49)) {
				timesWithoutAttacking = 0;
				return true;
			}
			timesWithoutAttacking++;
			return false;
			break;
		case BOSS:
			if (beam) return true;
			if (electricShots) {
				if (!numberElectricShots) return true;
				if (interval > 3) {
					interval = 0;
					return true;
				}
				else ++interval;
			}
			else {
				++startElectric;
				if (startElectric == 100) {
					electricShots = true;
					startElectric = 0;
				}
			}
			break;
	}
	return false;
}
void Enemy::attackPlayer() {
	switch (myType) {
		case BASIC1:
		case BASIC4:
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
				actualAttackAnimation = lastRotationAnim;
			}
			else if (lastRotationAnim == BASIC2_UP) {
				if (timeLastRotationAnim > 15/*30*/) {
					shooting = true;
					timeLastRotationAnim = 0;
				}
				//else ++timeLastRotationAnim;
				return;
			}
			else {
				if (timeLastRotationAnim > 15/*30*/) {
					++lastRotationAnim;
					if (!right && lastRotationAnim == BASIC2_STAYRIGHT) {
						lastRotationAnim = BASIC2_UP;
					}
					sprite->changeAnimation(lastRotationAnim);
					timeLastRotationAnim = 0;
					actualAttackAnimation = lastRotationAnim;
					break;
				}
				else ++timeLastRotationAnim;
			}
			break;

		case BOSS:
			if (beam) {
				if (startingBeam) {
					if (timeLastRotationAnim > 30) {
						startingBeam = false;
						timeLastRotationAnim = true;
					}
				}
				else if (beamShots < 1) {
					newShotType = BEAM;
					shooting = true;
					beamShots++;
				}
				else {
					beam = false;
					// Initialize eggs
					if (!eggShotDone) {
						newShotType = EGG;
						shooting = true;
					}
					else shooting = false;
					//Restart beam attributes
					beamShots = 0;
					delayFirstAnimation = 150;
					//Change the sprite animation to the default one
					restartSprite = true;
				}
			}
			else if (electricShots) {
				if (numberElectricShots < 10) {
					if (numberElectricShots) newShotType = ELECTRIC2;
					else newShotType = ELECTRIC1;
					shooting = true;
					numberElectricShots++;
				}
				else {
					if (numberElectricShots == 60) {
						lastElectricPosition++;
						if (lastElectricPosition == 3) lastElectricPosition = 0;
						numberElectricShots = 0;
						newShotType = ELECTRIC1;
					}
					else ++numberElectricShots;
				}
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
	if (myType == BASIC4) lookingTop = (posEnemy.y < (256/2)) ? false : true;
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
	if (myType != BOSS)	return shotSize[myType];
	return bossShotSize[max(0, newShotType -1)];
}
glm::ivec2 Enemy::getShotVelocity() {
	int xvel, yvel, differenceX, differenceY, threashholdX, threashholdY, denominator;
	switch (myType) {
		case BASIC1:
			threashholdX = 4;
			threashholdY = 3;
			denominator = 20;

			differenceY = posPlayer.y - posEnemy.y;
			yvel = ceil(differenceY / denominator);
			if (yvel < 0) yvel = max(yvel, -threashholdY);
			else yvel = min(yvel, threashholdY);

			xvel = posPlayer.x - posEnemy.x;
			if (xvel < 1) xvel = -threashholdX;
			else xvel = threashholdX;

			break;
		case BASIC2:
			threashholdX = 4;
			threashholdY = 3;
			denominator = 20;

			differenceX = posPlayer.x - posEnemy.x;
			differenceY = posPlayer.y - posEnemy.y;
			yvel = ceil(differenceY / denominator);
			xvel = ceil(differenceX / denominator);
			if (yvel < 0) yvel = max(yvel, -threashholdY);
			else yvel = min(yvel, threashholdY);

			if (xvel < 0) xvel = max(xvel, -threashholdX);
			else xvel = min(xvel, threashholdX);

			break;

		case BASIC4:
			threashholdX = 4;
			threashholdY = 3;
			denominator = 20;

			differenceX = posPlayer.x - posEnemy.x;
			differenceY = posPlayer.y - posEnemy.y;
			yvel = ceil(differenceY / denominator);
			xvel = ceil(differenceX / denominator);
			if (yvel < 0) yvel = max(yvel, -threashholdY);
			else yvel = min(yvel, threashholdY);

			if (xvel < 0) xvel = max(xvel, -threashholdX);
			else xvel = min(xvel, threashholdX);
			break;
		case BOSS:
			xvel = -3;
			yvel = 0;
			if (electricShots && !beam) {
				if (lastElectricPosition == 0) {
					//UP
					yvel = -2;
				}
				else if (lastElectricPosition == 1) {
					//MIDDLE
					yvel = 0;
				}
				else {
					//BOTTOM
					yvel = 2;
				}
			}
			break;
	}
	return glm::ivec2(xvel, yvel);
}
string Enemy::getShotSprite() {
	if (myType != BOSS) return spriteShot[myType];
	else return bossSpriteShot[max(0, newShotType - 1)];
}

void Enemy::enemyAlreadyAttacked() {
	this->shooting = false;
	switch (myType) {
		case BASIC2:
			attacking = false;
			startWalking = true;
			timeLastRotationAnim = 0;
			break;
		case BOSS:
			if (newShotType == EGG) eggShotDone = true;
	}
}
glm::vec2 Enemy::getShotSizeInSpriteSheet() {
	if (myType != BOSS) return shotSizeInSpriteSheet;
	else return bossShotsInSpriteSheet[max(0, newShotType - 1)];
}

void Enemy::jumpEnemy() {
	int jump = rand() % 130;
	jump = max(jump, 80);
	int finalY = startY - jump;
	if (up) {
		posEnemy.y -= 2;
		if (posEnemy.y > finalY && checkCollisionUp()) {
			// Enemy is going up, but there's a collision
			up = false;
			posEnemy.y += 2;
		}
		else if (posEnemy.y <= finalY) up = false;
	}
	else {
		posEnemy.y += 2;
		//If there is collision with the ground, stop jumping
		if (checkCollisionDown()) {
			posEnemy.y -= 1;
			stopJumping = true;
		}
	}
}
bool Enemy::startJumping() {
	if (bJumping) return true;
	else if (timesWithoutJumping > 150) {
		bJumping = true;
		up = true;
		stopJumping = false;
		startY = posEnemy.y;
		return true;
	}
	return false;
}

Enemies Enemy::getType() {
	return myType;
}
bool Enemy::reduceHP() {
	--health;
	if (health <= 0) return true;
	return false;
}

shotTypes Enemy::getNewShotType() {
	return this->newShotType;
}

glm::ivec2 Enemy::getShotPosition() {
	if (myType == BOSS) {
		if (this->getNewShotType() < 2) {
			//Electric Shots start in the head
			return glm::ivec2(posEnemy.x + 8, posEnemy.y + 67);
		}
		if (this->getNewShotType() == BEAM) {
			return glm::ivec2(posEnemy.x, posEnemy.y + 121);
		}
	}
	return posEnemy;
}

int Enemy::getShotDamage() {
	if (this->getNewShotType() == BEAM || this->getNewShotType() == EGG) return 2; //Force cannot avoid this damage
	return 1;
}

glm::ivec2 Enemy::getCheckboxSizeBoss() {
	switch (lastRotationAnim) {
		case EGG1:
			return glm::ivec2(23.f, 16.f);
			break;
		case EGG2:
			return glm::ivec2(23.f, 29.f);
			break;
		case EGG3:
			return glm::ivec2(24.f, 27.f);
			break;
		default: return glm::ivec2(1.f, 1.f);
	}
}

glm::ivec2 Enemy::getCheckboxPosBoss() {
	switch (lastRotationAnim) {
	case EGG1:
		return glm::ivec2(posEnemy.x + 7.f, posEnemy. y + 137.f);
		break;
	case EGG2:
	case EGG3:
		return glm::ivec2(posEnemy.x + 3.f, posEnemy.y +  122.f);
		break;
	default: return glm::ivec2(1.f, 1.f);
	}
}