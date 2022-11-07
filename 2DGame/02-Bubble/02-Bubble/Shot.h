#ifndef _SHOT_INCLUDE
#define _SHOT_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Entity.h"



class Shot : public Entity
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const string& spriteFolder, const glm::ivec2& shotVelocity, const glm::ivec2& size, const glm::vec2& sizeInSpriteSheet, const int& damage, const int upgrade, int type);
	void update(int deltaTime, glm::ivec2 forcePos, glm::ivec2 forceSize, ShaderProgram& shaderProgram);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

	glm::ivec2 getPosition();
	glm::ivec2 getSize();
	int getDamage();
	int getCategory();

private:
	glm::ivec2 tileMapDispl, velocity, posShot, sizeShot;
	int jumpAngle, startY;
	int shotDamage;
	int state = 0;
	bool upgrade1 = false, upgrade2 = false, initState;
	Sprite* sprite;
	TileMap* map;
	Texture spritesheet;
	ShaderProgram shaderProgramShot;
	int count;

	/*
	BOSS Helpers
	*/
	int category = 0;
	bool bossShot = false, egg = false, beam = false;
	//Elesctic shot helper
	bool firstElectric = false;
	bool electrics = false;
	int pause = 0;
	int speedDelay = 0;

	//Egg shot
	glm::ivec2 finalEggPosition;
	glm::ivec2 calcVelocity();
	int eggStartY; // 0 => top, 1 => bottom
	bool horizontalMovement = false, right = true, up = false;
};


#endif