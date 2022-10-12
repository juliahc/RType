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
	
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if(Game::instance().getSpecialKey(GLUT_KEY_UP))
	{
		if(sprite->animation() != MOVE_UP)
			sprite->changeAnimation(MOVE_UP);
		posPlayer.y -= 2;
		if(map->collisionMoveUp(posPlayer, glm::ivec2(24, 15), &posPlayer.y))
		{
			posPlayer.y += 2;
			sprite->changeAnimation(NORMAL);
		}
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_DOWN))
	{
		if(sprite->animation() != MOVE_DOWN)
			sprite->changeAnimation(MOVE_DOWN);
		posPlayer.y += 2;
		if(map->collisionMoveDown(posPlayer, glm::ivec2(24, 15), &posPlayer.y))
		{
			posPlayer.y -= 2;
			sprite->changeAnimation(NORMAL);
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if (sprite->animation() != NORMAL)
			sprite->changeAnimation(NORMAL);
		posPlayer.x -= 2;
		if (map->collisionMoveLeft(posPlayer, glm::ivec2(24, 15)))
			posPlayer.x += 2;
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if (sprite->animation() != NORMAL)
			sprite->changeAnimation(NORMAL);
		posPlayer.x += 2;
		if (map->collisionMoveRight(posPlayer, glm::ivec2(24, 15)))
			posPlayer.x -= 2;
	}
	else
	{
		if (sprite->animation() == MOVE_UP || sprite->animation() == MOVE_DOWN)
			sprite->changeAnimation(NORMAL);
	}	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}




