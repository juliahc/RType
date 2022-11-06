#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Shot.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4



void Shot::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const string& spriteFolder, const glm::ivec2& shotVelocity, const glm::ivec2& size, const glm::vec2& sizeInSpriteSheet, const int& damage, const int upgrade)
{
	if (upgrade == 1) upgrade1 = true;
	else if (upgrade == 2) upgrade2 = true;

	if (upgrade >= 3) {
		//Boss is shooting
		switch (upgrade) {
		case 3:
			//Electric shot
			spritesheet.loadFromFile(spriteFolder, TEXTURE_PIXEL_FORMAT_RGBA);
			sprite = Sprite::createSprite(sizeShot, sizeInSpriteSheet, &spritesheet, &shaderProgram);
			break;

		case 4:
			//Egg shot
			break;
			
		case 5:
			//Beam shot
			break;
		}
	}

	shaderProgramShot = shaderProgram;

	if (!upgrade1 && !upgrade2) {
		sizeShot = size;
		shotDamage = damage;

		spritesheet.loadFromFile(spriteFolder, TEXTURE_PIXEL_FORMAT_RGBA);
		sprite = Sprite::createSprite(sizeShot, sizeInSpriteSheet, &spritesheet, &shaderProgram);
		sprite->setNumberAnimations(1);
			sprite->setAnimationSpeed(0, 8);
			sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
		sprite->changeAnimation(0);

		velocity = shotVelocity;
	}
	else if (upgrade1) {

	}
	else if (upgrade2) {
		state = 0;
		initState = true;
		shotDamage = damage;
		sizeShot = glm::ivec2(24,24);

		spritesheet.loadFromFile("images/force/upgrade2_shots_init.png", TEXTURE_PIXEL_FORMAT_RGBA);
		sprite = Sprite::createSprite(sizeShot, glm::vec2(0.2f, 1.f), &spritesheet, &shaderProgram);
		sprite->setNumberAnimations(5);
		sprite->setAnimationSpeed(0, 8);

			sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
			sprite->setAnimationSpeed(0, 8);

			sprite->addKeyframe(1, glm::vec2(0.2f, 0.f));
			sprite->setAnimationSpeed(1, 8);

			sprite->addKeyframe(2, glm::vec2(0.4f, 0.f));
			sprite->setAnimationSpeed(2, 8);

			sprite->addKeyframe(3, glm::vec2(0.6f, 0.f));
			sprite->setAnimationSpeed(3, 8);

			sprite->addKeyframe(4, glm::vec2(0.8f, 0.f));
			sprite->setAnimationSpeed(4, 8);

		sprite->changeAnimation(state);

		velocity = glm::ivec2(0, 0);
	}

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posShot.x), float(tileMapDispl.y + posShot.y)));
	count = 0;
}

void Shot::update(int deltaTime, glm::ivec2 forcePos, glm::ivec2 forceSize, ShaderProgram& shaderProgram)
{
	count++;
	sprite->update(deltaTime);
	
	if (upgrade2) {
		if (initState) {
			if (state <= 2) {
				posShot.x = forcePos.x + 3;
				posShot.y = forcePos.y - forceSize.y / 2 + 1;
			}
			else if (state > 4) {
				initState = false;
				state = 0;
				//New sprite
				sizeShot = glm::ivec2(24, 16);
				posShot.x += 8;
				posShot.y += 4;

				spritesheet.loadFromFile("images/force/upgrade2_shots.png", TEXTURE_PIXEL_FORMAT_RGBA);
				sprite = Sprite::createSprite(sizeShot, glm::vec2(0.5f, 1.f), &spritesheet, &shaderProgram);
				sprite->setNumberAnimations(2);

				sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
				sprite->setAnimationSpeed(0, 8);

				sprite->addKeyframe(1, glm::vec2(0.5f, 0.f));
				sprite->setAnimationSpeed(1, 8);

				sprite->changeAnimation(state);
			}
			if (count % 5 == 0) {
				if (state < 5) {
					state++;
					if (state == 3 || state == 4) posShot.x += 8;
					if (state != 5) sprite->changeAnimation(state);
				}
			}
		}
		else {
			if (count % 5 == 0) {
				sprite->changeAnimation(state);
				if (state == 0) {
					state = 1;
					posShot.x += 8;
				}
				else if (state == 1) {
					state = 0;
					posShot.x += 8;
				}
			}
		}
		

		
	}

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