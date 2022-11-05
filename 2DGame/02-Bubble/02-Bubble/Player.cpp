#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum PlayerAnims { NORMAL, MOVE_UP, MOVE_DOWN };


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	invulnerable = true;
	tileMapDispl = tileMapPos;
	sizePlayer = glm::ivec2(24, 11);

	//Ship
	spritesheet.loadFromFile("images/ship/ships.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(sizePlayer, glm::vec2(0.20, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);	
		sprite->setAnimationSpeed(NORMAL, 8);
		sprite->addKeyframe(NORMAL, glm::vec2(0.4f, 0.f));				
		sprite->setAnimationSpeed(MOVE_UP, 8);
		sprite->addKeyframe(MOVE_UP, glm::vec2(0.8f, 0.f));		
		sprite->setAnimationSpeed(MOVE_DOWN, 8);
		sprite->addKeyframe(MOVE_DOWN, glm::vec2(0.f, 0.f));		
	sprite->changeAnimation(0);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	//Ship boom
	spritesheetBoom.loadFromFile("images/shipBoom.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spriteBoom = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.2, 1.f), &spritesheetBoom, &shaderProgram);
	spriteBoom->setNumberAnimations(5);		
		spriteBoom->setAnimationSpeed(0, 8);
		spriteBoom->addKeyframe(0, glm::vec2(0.f, 0.f));
		spriteBoom->setAnimationSpeed(1, 8);
		spriteBoom->addKeyframe(1, glm::vec2(0.2f, 0.f));
		spriteBoom->setAnimationSpeed(2, 8);
		spriteBoom->addKeyframe(2, glm::vec2(0.4f, 0.f));
		spriteBoom->setAnimationSpeed(3, 8);
		spriteBoom->addKeyframe(3, glm::vec2(0.6f, 0.f));
		spriteBoom->setAnimationSpeed(4, 8);
		spriteBoom->addKeyframe(4, glm::vec2(0.8f, 0.f));
	spriteBoom->changeAnimation(0);
	spriteBoom->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	//Shot Charge
	spritesheetCharge.loadFromFile("images/shotCharge.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spriteCharge = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.2, 1.f), &spritesheetCharge, &shaderProgram);
	spriteCharge->setNumberAnimations(5);
		spriteCharge->setAnimationSpeed(0, 8);
		spriteCharge->addKeyframe(0, glm::vec2(0.f, 0.f));
		spriteCharge->setAnimationSpeed(1, 8);
		spriteCharge->addKeyframe(1, glm::vec2(0.2f, 0.f));
		spriteCharge->setAnimationSpeed(2, 8);
		spriteCharge->addKeyframe(2, glm::vec2(0.4f, 0.f));
		spriteCharge->setAnimationSpeed(3, 8);
		spriteCharge->addKeyframe(3, glm::vec2(0.6f, 0.f));
		spriteCharge->setAnimationSpeed(4, 8);
		spriteCharge->addKeyframe(4, glm::vec2(0.8f, 0.f));
		spriteCharge->changeAnimation(0);
}

void Player::update(int deltaTime, int screenPosX, int forceWidth)
{
	if (!boom) {

		sprite->update(deltaTime);

		if (sprite->animation() == MOVE_UP || sprite->animation() == MOVE_DOWN) sprite->changeAnimation(NORMAL);

		//check UP, DOWN, LEFT, RIGHT keys
		if ((Game::instance().getSpecialKey(GLUT_KEY_UP) == PRESS) || (Game::instance().getSpecialKey(GLUT_KEY_UP) == REPEAT))
		{
			if (sprite->animation() != MOVE_UP) sprite->changeAnimation(MOVE_UP);
			posPlayer.y -= 2;
			if (map->collisionMoveUp(posPlayer, sizePlayer, &posPlayer.y))
			{
				//posPlayer.y += 2;
				//sprite->changeAnimation(NORMAL);
				collision();
			}
		}
		if ((Game::instance().getSpecialKey(GLUT_KEY_DOWN) == PRESS) || (Game::instance().getSpecialKey(GLUT_KEY_DOWN) == REPEAT))
		{
			if (sprite->animation() != MOVE_DOWN) sprite->changeAnimation(MOVE_DOWN);
			posPlayer.y += 2;
			if (map->collisionMoveDown(posPlayer, sizePlayer, &posPlayer.y))
			{
				//posPlayer.y -= 2;
				sprite->changeAnimation(NORMAL);
				collision();
			}
		}
		if ((Game::instance().getSpecialKey(GLUT_KEY_LEFT) == PRESS) || (Game::instance().getSpecialKey(GLUT_KEY_LEFT) == REPEAT))
		{
			if (sprite->animation() != NORMAL) sprite->changeAnimation(NORMAL);
			posPlayer.x -= 2;
			if (map->collisionMoveLeft(posPlayer, sizePlayer)) {
				posPlayer.x += 2;
				collision();
			}
			else if (posPlayer.x < screenPosX) posPlayer.x = screenPosX;
		}
		if ((Game::instance().getSpecialKey(GLUT_KEY_RIGHT) == PRESS) || (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) == REPEAT))
		{
			if (sprite->animation() != NORMAL) sprite->changeAnimation(NORMAL);
			posPlayer.x += 2;
			if (map->collisionMoveRight(posPlayer, sizePlayer)) {
				posPlayer.x -= 2;
				collision();
			}
			else if ((posPlayer.x + sizePlayer.x - 1) > (screenPosX + SCREEN_WIDTH - 1)) posPlayer.x = screenPosX + (SCREEN_WIDTH - 1) - sizePlayer.x;
		}


		//set position
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

		//Charge animation
		if (Game::instance().getKey('s') == REPEAT)
		{
			spriteCharge->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x + sizePlayer.x + forceWidth), float(tileMapDispl.y + posPlayer.y)));
			if (!charging) {
				charging = true;
				chargeAnimationChangeTime = 0;
				chargeAnimation = 2;
				spriteCharge->changeAnimation(0);
			}
			else {
				if (chargeAnimationChangeTime % 20 == 0 && shotCharge < 5) shotCharge++;
				if (chargeAnimationChangeTime % 7 == 0) {
					if (chargeAnimation < 4) chargeAnimation++;
					else {
						
						chargeAnimation = 0;
					}
					spriteCharge->changeAnimation(chargeAnimation);
				}
				chargeAnimationChangeTime++;
			}
		}
		else if (charging) charging = false;

		//If "g" key pressed --> invulnerability
		if (Game::instance().getKey('g') == PRESS) {
			invulnerable = !invulnerable;
		}
	}
	else {
		spriteBoom->update(deltaTime);
		if (boomAnimation < 5) {
			boomAnimation += 1;
			spriteBoom->changeAnimation(boomAnimation);
		}
		else boomFinish = true;
	}
}

void Player::render()
{
	if (!boom) {
		sprite->render();
		if (charging) spriteCharge->render();
	}
	else spriteBoom->render();
}


void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	spriteBoom->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Player::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Player::collision()
{
	if (!invulnerable) {
		spriteBoom->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
		boom = true;
	}
}

glm::ivec2 Player::getPosition()
{
	return posPlayer;
}

glm::ivec2 Player::getSize()
{
	return sizePlayer;
}

void Player::setShotCharge(int charge)
{
	shotCharge = charge;
}

int Player::getShotCharge()
{
	return shotCharge;
}

bool Player::died() {
	return boom;
}

bool Player::boomFinished()
{
	return boomFinish;
}