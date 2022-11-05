#ifndef _SHOT_INCLUDE
#define _SHOT_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Entity.h"



class Shot : public Entity
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const string& spriteFolder, const glm::ivec2& shotVelocity, const glm::ivec2& size, const glm::vec2& sizeInSpriteSheet, const int& damage, const int upgrade);
	void update(int deltaTime, glm::ivec2 forcePos, glm::ivec2 forceSize, ShaderProgram& shaderProgram);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

	glm::ivec2 getPosition();
	glm::ivec2 getSize();
	int getDamage();

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
};


#endif