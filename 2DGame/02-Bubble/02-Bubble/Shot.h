#ifndef _SHOT_INCLUDE
#define _SHOT_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Entity.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.



class Shot : public Entity
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const string& spriteFolder, const glm::ivec2& shotVelocity, const glm::ivec2& size, const glm::vec2& sizeInSpriteSheet, const int& damage);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

	glm::ivec2 getPosition();
	glm::ivec2 getSize();

private:
	glm::ivec2 tileMapDispl, velocity, posShot, sizeShot;
	int jumpAngle, startY;
	int shotDamage;
	Sprite* sprite;
	TileMap* map;
	Texture spritesheet;

};


#endif // _PLAYER_INCLUDE