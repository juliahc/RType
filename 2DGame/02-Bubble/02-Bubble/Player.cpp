#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum PlayerAnims
{
	NORMAL, MOVE_UP, MOVE_DOWN
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;

	//Ship
	spritesheet.loadFromFile("images/spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(24, 15), glm::vec2(0.25, 0.5), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);
	
		sprite->setAnimationSpeed(NORMAL, 8);
		sprite->addKeyframe(NORMAL, glm::vec2(0.f, 0.f));
				
		sprite->setAnimationSpeed(MOVE_UP, 8);
		/*sprite->addKeyframe(MOVE_UP, glm::vec2(0.25f, 0.f));
		sprite->addKeyframe(MOVE_UP, glm::vec2(0.5f, 0.f));*/
		sprite->addKeyframe(MOVE_UP, glm::vec2(0.75f, 0.f));
		
		sprite->setAnimationSpeed(MOVE_DOWN, 8);
		/*sprite->addKeyframe(MOVE_DOWN, glm::vec2(0.25f, 0.5f));
		sprite->addKeyframe(MOVE_DOWN, glm::vec2(0.5f, 0.5f));*/
		sprite->addKeyframe(MOVE_DOWN, glm::vec2(0.75f, 0.5f));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	//Ship boom
	spritesheetBoom.loadFromFile("images/ship-boom.png", TEXTURE_PIXEL_FORMAT_RGBA);
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


}

void Player::update(int deltaTime)
{
	if (!boom) {
		sprite->update(deltaTime);
		if (Game::instance().getSpecialKey(GLUT_KEY_UP))
		{
			if (sprite->animation() != MOVE_UP)
				sprite->changeAnimation(MOVE_UP);
			posPlayer.y -= 2;
			if (map->collisionMoveUp(posPlayer, glm::ivec2(24, 15), &posPlayer.y))
			{
				posPlayer.y += 2;
				//sprite->changeAnimation(NORMAL);
				collision();
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
		{
			if (sprite->animation() != MOVE_DOWN)
				sprite->changeAnimation(MOVE_DOWN);
			posPlayer.y += 2;
			if (map->collisionMoveDown(posPlayer, glm::ivec2(24, 15), &posPlayer.y))
			{
				posPlayer.y -= 2;
				//sprite->changeAnimation(NORMAL);
				collision();
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
		{
			if (sprite->animation() != NORMAL)
				sprite->changeAnimation(NORMAL);
			posPlayer.x -= 2;
			if (map->collisionMoveLeft(posPlayer, glm::ivec2(24, 15))) {
				posPlayer.x += 2;
				collision();
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
		{
			if (sprite->animation() != NORMAL)
				sprite->changeAnimation(NORMAL);
			posPlayer.x += 2;
			if (map->collisionMoveRight(posPlayer, glm::ivec2(24, 15))) {
				posPlayer.x -= 2;
				collision();
			}
		}
		else
		{
			if (sprite->animation() == MOVE_UP || sprite->animation() == MOVE_DOWN)
				sprite->changeAnimation(NORMAL);
		}
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	}
	else {
		spriteBoom->update(deltaTime);
		if (boomAnimation < 5) {
			boomAnimation += 1;
			spriteBoom->changeAnimation(boomAnimation);
		}
	}
}

void Player::render()
{
	if (!boom) sprite->render();
	else {
		spriteBoom->render();
	}
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	spriteBoom->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Player::collision()
{
	spriteBoom->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	boom = true;
}

