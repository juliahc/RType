#pragma once
#include <vector>
#include <utility>
#include "Entity.h"

class Enemy :
    public Entity
{
public:
	/*
	Basic functions
	*/
	void init(const glm::ivec2& tileMapPos, Enemies enemy, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

	glm::ivec2 getPosition();
	glm::ivec2 getSize();

	/*
	Boom animation
	*/
	void collision();
	bool died();
	bool boomFinished();

	/*
	Attacking
	*/
	bool isShooting();
	glm::ivec2 getShotSize();
	glm::ivec2 getShotVelocity(glm::ivec2 playerPosition);
	string getShotSprite();
	glm::vec2 getShotSizeInSpriteSheet();

	void enemyAlreadyAttacked();

private:
	/*
	Attributes for the IA
	*/

	//Movement
	bool right = false;
	bool up = true;

	//Attack player
	bool attacking = false;

	bool shooting = false;
	vector<glm::ivec2> shotSize = { glm::ivec2(6, 6), glm::ivec2(6, 6), glm::ivec2(1,1), glm::ivec2(1,1) };
	glm::ivec2 shotvelocity = glm::ivec2(-4, 0);
	vector<string> spriteShot = { "images/enemies/basic1/shot.png", "images/enemies/basic1/shot.png", "", "" };
	glm::vec2 shotSizeInSpriteSheet = glm::vec2(1, 1);

	int timesWithoutAttacking = 50;
	
	//Jump?
	bool bJumping = false;

	//Type of enemy
	Enemies myType;

	/*
	Game coords
	*/

	//Coords
	glm::ivec2 tileMapDispl, posEnemy;

	int jumpAngle, startY;

	/*
	Enemies sprites
	*/

	//Spritesheets
	Texture spritesheet, spritesheetBoom;
	Sprite *sprite, *spriteBoom;

	//Tilemap
	TileMap* map;

	//Sprites + sizes
	vector<string> spriteFiles{ "images/enemies/basic1/basic1.png", "images/enemies/basic2/basic2.png", "", "" };
	vector<glm::ivec2> enemySizes{ glm::ivec2(23, 24), glm::ivec2(30, 28), glm::ivec2(0, 0), glm::ivec2(0, 0) };
	vector<glm::vec2> spriteSheetSize{ glm::vec2(0.25f, 0.5f), glm::vec2(0.2f, 0.5f), glm::vec2(0., 0.), glm::vec2(0., 0.) };

	//Animation helpers
	int lastRotationAnim = 0;
	int actualAnimation = 0;

		//BASIC2 helper
	bool startWalking = false;

	int actualAttackAnimation = -1;
	int previousAttackAnimation = 0;

	int timeLastRotationAnim = -1;

	bool boom = false;
	int boomAnimation = -1;

	/*
	Functions for IA
	*/

	//Collisions
	bool checkCollisionRight();
	bool checkCollisionLeft();
	bool checkCollisionUp();
	bool checkCollisionDown();

	//Movement
	void moveEnemyHorizontally(int length, bool right);
	void moveEnemyVertically(int length, bool up);
	void restartWalking();

	//Attack player
	bool attack(int probability);
	void attackPlayer();

	//Animations
	void changeAnimation();

};

