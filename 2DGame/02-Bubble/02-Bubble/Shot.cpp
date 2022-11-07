#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Shot.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4



void Shot::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const string& spriteFolder, const glm::ivec2& shotVelocity, const glm::ivec2& size, const glm::vec2& sizeInSpriteSheet, const int& damage, const int upgrade, const int subtype)
{
	category = upgrade;
	if (upgrade == 1) upgrade1 = true;
	else if (upgrade == 2) upgrade2 = true;

	else if (upgrade >= 3) {
		bossShot = true;
		//Boss is shooting
		sizeShot = size;
		shotDamage = damage;
		velocity = shotVelocity;
		switch (upgrade) {
		case 3:
			firstElectric = true;
			//First electric shot
			spritesheet.loadFromFile(spriteFolder, TEXTURE_PIXEL_FORMAT_RGBA);
			sprite = Sprite::createSprite(sizeShot, sizeInSpriteSheet, &spritesheet, &shaderProgram);
			sprite->setNumberAnimations(1);
			sprite->setAnimationSpeed(0, 8);
			sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
			sprite->changeAnimation(0);
			state = 0;
			break;

		case 4:
			electrics = true;
			//Other electric shots
			spritesheet.loadFromFile(spriteFolder, TEXTURE_PIXEL_FORMAT_RGBA);
			sprite = Sprite::createSprite(sizeShot, sizeInSpriteSheet, &spritesheet, &shaderProgram);
			sprite->setNumberAnimations(5);

			sprite->setAnimationSpeed(0, 8);
			sprite->addKeyframe(0, glm::vec2(0.2f, 0.f));

			sprite->setAnimationSpeed(1, 8);
			sprite->addKeyframe(1, glm::vec2(0.4f, 0.f));

			sprite->setAnimationSpeed(2, 8);
			sprite->addKeyframe(2, glm::vec2(0.6f, 0.f));

			sprite->setAnimationSpeed(3, 8);
			sprite->addKeyframe(3, glm::vec2(0.8f, 0.f));


			sprite->setAnimationSpeed(4, 8);
			sprite->addKeyframe(4, glm::vec2(0.f, 0.5f));

			sprite->changeAnimation(0);
			state = 0;
			break;
		case 5:
			//Egg shot
			egg = true;
			eggStartY = subtype;

			spritesheet.loadFromFile(spriteFolder, TEXTURE_PIXEL_FORMAT_RGBA);
			sprite = Sprite::createSprite(sizeShot, sizeInSpriteSheet, &spritesheet, &shaderProgram);
			sprite->setNumberAnimations(2);

			sprite->setAnimationSpeed(0, 8);
			sprite->addKeyframe(0, glm::vec2(0.f, 0.f));

			sprite->setAnimationSpeed(1, 8);
			sprite->addKeyframe(1, glm::vec2(0.5f, 0.f));

			sprite->changeAnimation(0);
			state = 0;
			break;
			
		case 6:
			//Beam shot
			beam = true;
			spritesheet.loadFromFile(spriteFolder, TEXTURE_PIXEL_FORMAT_RGBA);
			sprite = Sprite::createSprite(sizeShot, sizeInSpriteSheet, &spritesheet, &shaderProgram);
			sprite->setNumberAnimations(1);
			sprite->setAnimationSpeed(0, 8);
			sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
			sprite->changeAnimation(0);
			break;
		}
	}

	shaderProgramShot = shaderProgram;

	if (category< 1) {
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
		type = 1;
		state = subtype;		 //  0 --> /    1 --> \    2 --> /\    3 --> -
		shotDamage = damage;
		sizeShot = glm::ivec2(16, 17);
		if (shotVelocity.x > 0) directionX = RIGHT;
		else directionX = LEFT;
		if (state == 0) directionY = UP;
		else directionY = DOWN;
		
		spritesheet.loadFromFile("images/force/upgrade1_shots.png", TEXTURE_PIXEL_FORMAT_RGBA);
		sprite = Sprite::createSprite(sizeShot, glm::vec2(0.5f, 0.25f), &spritesheet, &shaderProgram);
		sprite->setNumberAnimations(7);

			sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
			sprite->setAnimationSpeed(0, 8);

			sprite->addKeyframe(1, glm::vec2(0.5f, 0.f));
			sprite->setAnimationSpeed(1, 8);

			sprite->addKeyframe(2, glm::vec2(0.f, 0.25f));
			sprite->setAnimationSpeed(2, 8);

			sprite->addKeyframe(3, glm::vec2(0.5f, 0.25f));
			sprite->setAnimationSpeed(3, 8);

			sprite->addKeyframe(4, glm::vec2(0.f, 0.5f));
			sprite->setAnimationSpeed(4, 8);

			sprite->addKeyframe(5, glm::vec2(0.5f, 0.5f));
			sprite->setAnimationSpeed(5, 8);

			sprite->addKeyframe(6, glm::vec2(0.f, 0.75f));
			sprite->setAnimationSpeed(6, 8);

		sprite->changeAnimation(state);

		velocity = glm::ivec2(0, 0);
	}
	else if (upgrade2) {
		type = 2;
		state = 0;
		initState = true;
		shotDamage = damage;
		sizeShot = glm::ivec2(24,24);
		string img;
		if (shotVelocity.x > 0) {
			directionX = RIGHT;
			img = "images/force/upgrade2_shots_init.png";
		}
		else {
			directionX = LEFT;
			img = "images/force/upgrade2_shots_init_left.png";
		}
		velocity = glm::ivec2(0, 0);

		spritesheet.loadFromFile(img, TEXTURE_PIXEL_FORMAT_RGBA);
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

	if (upgrade1) {
		glm::ivec2 auxPos;
		bool up, down, left, right;
		switch (state) {
		case 0:
			//init
			up = down = left = right = false;
			//velocity
			if (directionX == RIGHT) velocity = glm::ivec2(8, -8);
			else if (directionX == LEFT) velocity = glm::ivec2(-8, 8);
			//check collisions
			auxPos = posShot;
			auxPos.x += velocity.x;
			if (map->collisionMoveRight(auxPos, sizeShot)) right = true;
			else if (map->collisionMoveLeft(auxPos, sizeShot)) left = true;
			else {
				auxPos.y += velocity.y;
				if (map->collisionMoveUp(auxPos, sizeShot, &auxPos.y)) up = true;
				else if (map->collisionMoveDown(auxPos, sizeShot, &auxPos.y)) down = true;
			}
			//collision treatment
			if (right) {
				auxPos.x += velocity.x;
				while (map->collisionMoveRight(auxPos, sizeShot)) auxPos.x--;
				velocity = glm::ivec2(0, 0);
				int move = auxPos.x - posShot.x;
				posShot.y = posShot.y + move - 7;
				posShot.x = auxPos.x;
				state = 5;
				initState = true;
				sprite->changeAnimation(state);
			}
			else if (up) {
				velocity = glm::ivec2(0, 0);
				int move = posShot.y - auxPos.y;
				posShot.y = auxPos.y;
				posShot.x = posShot.x + 8 - move;
				state = 2;
				initState = true;
				sprite->changeAnimation(state);
			}
			else if (left) {
				auxPos.x += velocity.x;
				while (map->collisionMoveLeft(auxPos, sizeShot)) auxPos.x++;
				velocity = glm::ivec2(0, 0);
				int move = posShot.x - auxPos.x;
				posShot.y = posShot.y + move + 7;
				posShot.x = auxPos.x;
				state = 5;
				initState = true;
				sprite->changeAnimation(state);
			}
			else if (down) {
				velocity = glm::ivec2(0, 0);
				int move = auxPos.y - posShot.y;
				posShot.y = auxPos.y;
				posShot.x = posShot.x + 8 - move;
				state = 3;
				initState = true;
				sprite->changeAnimation(state);
			}
			break;
		case 1:
			up = down = left = right = false;
			if (directionX == RIGHT) velocity = glm::ivec2(8, 8);
			else if (directionX == LEFT) velocity = glm::ivec2(-8, -8);

			auxPos = posShot;
			auxPos.x += velocity.x;
			if (map->collisionMoveRight(auxPos, sizeShot)) right = true;
			else if (map->collisionMoveLeft(auxPos, sizeShot)) left = true;
			else {
				auxPos.y += velocity.y;
				if (map->collisionMoveUp(auxPos, sizeShot, &auxPos.y)) up = true;
				else if (map->collisionMoveDown(auxPos, sizeShot, &auxPos.y)) down = true;
			}

			if (right) {
				auxPos.x += velocity.x;
				while (map->collisionMoveRight(auxPos, sizeShot)) auxPos.x--;
				velocity = glm::ivec2(0, 0);
				int move = auxPos.x - posShot.x;
				posShot.y = posShot.y + move + 7;
				posShot.x = auxPos.x;
				state = 5;
				initState = true;
				sprite->changeAnimation(state);
			}
			else if (up) {
				velocity = glm::ivec2(0, 0);
				int move = posShot.y - auxPos.y;
				posShot.y = auxPos.y;
				posShot.x = posShot.x - 8 - move;
				state = 2;
				initState = true;
				sprite->changeAnimation(state);
			}
			else if (left) {
				auxPos.x += velocity.x;
				while (map->collisionMoveLeft(auxPos, sizeShot)) auxPos.x++;
				velocity = glm::ivec2(0, 0);
				int move = posShot.x - auxPos.x;
				posShot.y = posShot.y - move - 7;
				posShot.x = auxPos.x;
				state = 4;
				initState = true;
				sprite->changeAnimation(state);
			}
			else if (down) {
				velocity = glm::ivec2(0, 0);
				int move = auxPos.y - posShot.y;
				posShot.y = auxPos.y;
				posShot.x = posShot.x + 8 + move;
				state = 3;
				initState = true;
				sprite->changeAnimation(state);
			}
			break;
		case 2:
			//sprite->changeAnimation(2);
			if (initState) initState = false;
			else {
				velocity = glm::ivec2(0, 0);
				directionY = DOWN;
				posShot.y += 4;
				if (directionX == RIGHT) {
					state = 1;
					posShot.x += 12;
				}
				else {
					state = 0;
					posShot.x -= 12;
				}
				sprite->changeAnimation(state);
			}
			break;
		case 3:
			//sprite->changeAnimation(3);
			if (initState) initState = false;
			else {
				//direction = LEFT;
				velocity = glm::ivec2(0, 0);
				directionY = UP;
				posShot.y -= 4;
				if (directionX == RIGHT) {
					state = 0;
					posShot.x += 12;
				}
				else {
					state = 1;
					posShot.x -= 12;
				}
				sprite->changeAnimation(state);
			}
			break;
		case 4:
			if (initState) initState = false;
			else {
				velocity = glm::ivec2(0, 0);
				directionX = RIGHT;
				posShot.x += 4;
				if (directionY == UP) {
					state = 0;
					posShot.y -= 12;
				}
				else {
					state = 1;
					posShot.y += 12;
				}
				sprite->changeAnimation(state);
			}
			break;
		case 5:
			if (initState) initState = false;
			else {
				velocity = glm::ivec2(0, 0);
				directionX = LEFT;
				posShot.x -= 4;
				if (directionY == UP) {
					state = 1;
					posShot.y -= 12;
				}
				else {
					state = 0;
					posShot.y += 12;
				}
				sprite->changeAnimation(state);
			}
			break;
		case 6:
			if (directionX == RIGHT) velocity = glm::ivec2(16, 0);
			else velocity = glm::ivec2(-16, 0);
			break;
		}
		/*posShot.x += velocity.x;
		posShot.y += velocity.y;*/
	}
	else if (upgrade2) {
		if (initState) {
			if (state <= 2) {
				if (directionX == RIGHT) posShot.x = forcePos.x + 3;
				else posShot.x = forcePos.x - 14;
				posShot.y = forcePos.y - forceSize.y / 2 + 1;
			}
			else if (state > 4) {
				initState = false;
				state = 0;
				//New sprite
				sizeShot = glm::ivec2(24, 16);
				string img;
				if (directionX == RIGHT) img = "images/force/upgrade2_shots.png";
				else img = "images/force/upgrade2_shots_left.png";

				if (directionX == RIGHT) posShot.x += 8;
				else posShot.x -= 8;
				posShot.y += 4;

				spritesheet.loadFromFile(img, TEXTURE_PIXEL_FORMAT_RGBA);
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
					if (state == 3 || state == 4) {
						if (directionX == RIGHT) posShot.x += 8;
						else posShot.x -= 8;
					}
					if (state != 5) sprite->changeAnimation(state);
				}
			}
		}
		else {
			if (count % 5 == 0) {
				sprite->changeAnimation(state);
				if (state == 0) {
					state = 1;
					if (directionX == RIGHT) posShot.x += 8;
					else posShot.x -= 8;
				}
				else if (state == 1) {
					state = 0;
					if (directionX == RIGHT) posShot.x += 8;
					else posShot.x -= 8;
				}
			}
		}
		/*posShot.x += velocity.x;
		posShot.y += velocity.y;*/
	}
	//Boos updates
	else if (bossShot) {
		if (category == 3 || category == 4) {
			++pause;
			if (category == 4) {
				//Animation
				++state;
				if (state == 5) state = 0;
				sprite->changeAnimation(state);
			}
			if (pause == 30) {
				velocity.x = 0;
				velocity.y = 0;
			}
			if (pause == 65) velocity.x = -1;
		}
		else if (category == 5) {
			//Egg animation
			if (pause == 5) {
				pause = 0;
				++state;
				if (state != 1) state = 0;
				sprite->changeAnimation(state);
			}
			else {
				pause++;
			}
			velocity = calcVelocity();
		}

		if ((category == 3 || category == 4) && pause < 30) {
			if (pause % 2 == 0) {
				posShot.x += velocity.x;
				posShot.y += velocity.y;
			}
		}
		else {
			posShot.x += velocity.x;
			posShot.y += velocity.y;
		}

		if (category == 5 && posShot == finalEggPosition) {
			horizontalMovement = true;
		}

	}
	else {
		posShot.x += velocity.x;
		posShot.y += velocity.y;
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posShot.x), float(tileMapDispl.y + posShot.y)));
	}
}

void Shot::render()
{
	sprite->render();
}

void Shot::setPosition(const glm::vec2& pos)
{
	if (category == 5) {
		finalEggPosition = pos;
		posShot.x = pos.x + (rand() % 10);
		if (eggStartY) {
			//Top
			posShot.y = 240;
		}
		else {
			//Bottom
			posShot.y = 5;
		}
	}
	else {
		posShot = pos;
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posShot.x), float(tileMapDispl.y + posShot.y)));
	}
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

glm::ivec2 Shot::calcVelocity() {
	glm::ivec2 newVelocity = { 0.f, 0.f };
	if (horizontalMovement) {
		if (speedDelay > 5) {
			speedDelay = 0;
			if (!right) {
				newVelocity.x = 1;
				right = true;
			}
			else {
				newVelocity.x = -1;
				right = false;
			}
			if (up) {
				newVelocity.y = -1;
				up = false;
			}
			else {
				newVelocity.y = 1;
				up = true;
			}
		}
		else {
			++speedDelay;
		}
	}
	else if (speedDelay == 2) {
		speedDelay = 0;
		int differenceX = finalEggPosition.x - posShot.x;
		int differenceY = finalEggPosition.y - posShot.y;
		if (differenceX != 0) {
			if (abs(differenceX) > 20) newVelocity.x = (differenceX / abs(differenceX)) * 2;
			else newVelocity.x = (differenceX / abs(differenceX));
		}
		if (differenceY != 0) {
			if (abs(differenceY) > 20) newVelocity.y = (differenceY / abs(differenceY)) * 2;
			else newVelocity.y = (differenceY / abs(differenceY));
		}
	}
	else {
		++speedDelay;
	}

	return newVelocity;
}

int Shot::getCategory() {
	return category;
}

int Shot::getType() {
	return type;
}

void Shot::collisionsUpgrade1(vector<glm::ivec2>& positions, vector<glm::ivec2>& sizes)
{
	switch (state)
	{
		case 0:
			positions.push_back(glm::ivec2(posShot.x, posShot.y + 14));
			sizes.push_back(glm::ivec2(3, 3));
			for (int i = 0; i < 5; i++) {
				positions.push_back(glm::ivec2(posShot.x + 2 + 3*i, posShot.y + 12 - 3*i));
				sizes.push_back(glm::ivec2(2, 2));
			}
			for (int i = 0; i < 4; i++) {
				positions.push_back(glm::ivec2(posShot.x + 4 + 3*i, posShot.y + 11 - 3*i));
				sizes.push_back(glm::ivec2(2, 2));
			}
			break;
		case 1:
			positions.push_back(glm::ivec2(posShot.x + 13, posShot.y + 14));
			sizes.push_back(glm::ivec2(3, 3));
			for (int i = 0; i < 5; i++) {
				positions.push_back(glm::ivec2(posShot.x + 12 - 3 * i, posShot.y + 12 - 3 * i));
				sizes.push_back(glm::ivec2(2, 2));
			}
			for (int i = 0; i < 4; i++) {
				positions.push_back(glm::ivec2(posShot.x + 10 - 3 * i, posShot.y + 11 - 3 * i));
				sizes.push_back(glm::ivec2(2, 2));
			}
			break;
		case 2:
			positions.push_back(glm::ivec2(posShot.x + 3, posShot.y));
			sizes.push_back(glm::ivec2(9, 4));
			break;
		case 3:
			positions.push_back(glm::ivec2(posShot.x + 3, posShot.y + 13));
			sizes.push_back(glm::ivec2(9, 4));
			break;
		case 4:
			positions.push_back(glm::ivec2(posShot.x, posShot.y + 4));
			sizes.push_back(glm::ivec2(4, 9));
			break;
		case 5:
			positions.push_back(glm::ivec2(posShot.x + 12, posShot.y + 4));
			sizes.push_back(glm::ivec2(4, 9));
			break;
		case 6:
			positions.push_back(glm::ivec2(posShot.x, posShot.y + 7));
			sizes.push_back(glm::ivec2(16,2));
			break;
		default:
			break;
	}
}