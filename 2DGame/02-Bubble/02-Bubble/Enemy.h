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
	void update(int deltaTime, glm::ivec2 posPlayer);
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
	glm::ivec2 getShotVelocity();
	string getShotSprite();
	glm::vec2 getShotSizeInSpriteSheet();

	void enemyAlreadyAttacked();

	/*
	BOSS FUNCTIONS
	*/
	Enemies getType();
	bool reduceHP();
	shotTypes getNewShotType();
	glm::ivec2 getShotPosition();
	int getShotDamage();

private:

	/*
	Attributes for the Boss
	*/
	int delayFirstAnimation = 0;

	shotTypes newShotType = EGG;

	int health = 10;
	bool electricShots = false;
	bool eggs = false;
	bool beam = false;
	//Electric shot helpers
	int startElectric = 0;
	int numberElectricShots = 0;
	int lastElectricPosition = 0; // 0 == UP, 1 == MIDDLE, 2 == BOTTOM
	int interval = 0;

	//Beam shot helper
	int beamShots = 0;
	bool pendingBeam = false;

	//Egg shot
	bool eggShotDone = false;

	vector<glm::vec2> bossShotSize = { glm::vec2(15, 14), glm::vec2(28, 30), glm::vec2(82,26) };
	vector<string> bossSpriteShot = { "images/enemies/boss/shots/electric.png", "images/enemies/boss/shots/eggs.png", "images/enemies/boss/shots/beam.png" };
	vector<glm::vec2> bossShotsInSpriteSheet = { glm::vec2(0.2f, 0.5f), glm::vec2(0.5f, 1.f), glm::vec2(1.f, 1.f)};
	
	/*
	Attributes for the IA
	*/

	//Movement
	bool right = false;
	bool up = true;
	bool stay = false;
	int stayCounter = 0;
	
	bool rightFoot = true; // Helper for the BASIC3 movement animation
	bool lookingTop = false; // Helper for the BASIC4 movement animation
	
	//Attack player
	glm::ivec2 posPlayer;
	bool attacking = false;

	bool shooting = true;
	vector<glm::ivec2> shotSize = { glm::ivec2(6, 6), glm::ivec2(6, 6), glm::ivec2(1,1), glm::ivec2(6,6) };
	glm::ivec2 shotvelocity = glm::ivec2(-4, 0);
	vector<string> spriteShot = { "images/enemies/basic1/shot.png", "images/enemies/basic1/shot.png", "", "images/enemies/basic1/shot.png" };
	glm::vec2 shotSizeInSpriteSheet = glm::vec2(1, 1);

	int timesWithoutAttacking = 50;
	
	//Jump
	bool bJumping = false;
	bool stopJumping = false;
	int startY = 0;
	int timesToNextY = 0;
	int timesWithoutJumping = 0;

	//Type of enemy
	Enemies myType;

	/*
	Game coords
	*/

	//Coords
	glm::ivec2 tileMapDispl, posEnemy;

	/*
	Enemies sprites
	*/

	//Spritesheets
	Texture spritesheet, spritesheetBoom;
	Sprite *sprite, *spriteBoom;

	//Tilemap
	TileMap* map;

	//Sprites + sizes
	vector<string> spriteFiles{ "images/enemies/basic1/basic1.png", "images/enemies/basic2/basic2.png", "images/enemies/basic3/basic3.png", "images/enemies/basic4/basic4.png", "images/enemies/boss/boss.png"};
	vector<glm::ivec2> enemySizes{ glm::ivec2(23, 24), glm::ivec2(30, 28), glm::ivec2(27, 33), glm::ivec2(16, 16), glm::ivec2(113, 207) };
	vector<glm::vec2> spriteSheetSize{ glm::vec2(0.25f, 0.5f), glm::vec2(0.2f, 0.5f), glm::vec2(0.25f, 0.5f), glm::vec2(0.2f, 0.25f), glm::vec2(0.2f, 1.f) };

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

	//Jump
	void jumpEnemy();
	bool startJumping();

};

