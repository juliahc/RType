#ifndef _FORCE_INCLUDE
#define _FORCE_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Entity.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.

enum forceState { INACTIVE, INIT, LEFT, RIGHT, UP, DOWN };
enum forceType { UPGRADE_0, UPGRADE_1, UPGRADE_2 };

class Force : public Entity
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime, const glm::ivec2& posPlayer, const int screenExtraPosition);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

	glm::ivec2 getPosition();
	bool isActive();
	void setActive(bool newActive);

private:
	glm::ivec2 tileMapDispl, posForce;
	Sprite* sprite;
	TileMap* map;
	Texture spritesheet;
	int animation = 0;
	bool active;

	forceType type;
	forceState state;

	bool frontAttached;
	bool bottomAttached;

};


#endif // _PLAYER_INCLUDE