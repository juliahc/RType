#ifndef _FORCE_INCLUDE
#define _FORCE_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Entity.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.



class Force : public Entity
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime, const glm::ivec2& posPlayer);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

	glm::ivec2 getPosition();

private:
	glm::ivec2 tileMapDispl, posForce;
	Sprite* sprite;
	TileMap* map;
	Texture spritesheet;
	int animation = 0;
	bool active = true;
};


#endif // _PLAYER_INCLUDE