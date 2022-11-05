#ifndef _FORCE_INCLUDE
#define _FORCE_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Entity.h"
#include <queue>


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.

enum forceState { INACTIVE, INIT, ACTIVE, PUSHED_LEFT, PUSHED_RIGHT };
enum forceAttached { NOATTACHED, FRONT, BOTTOM };

class Force : public Entity
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime, const glm::ivec2& posPlayer, const int screenPosX);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

	glm::ivec2 getPosition();
	glm::ivec2 getSize();
	int getWidth();
	bool isActive();
	bool isAttached();
	void setActive(bool newActive);
	void setAttached(string newAttached);
	void setType(int newType);
	void addShot(int& nbShots, glm::ivec2 & posShot, glm::ivec2* velocities, bool& front);

private:

	void nextPosition(const int screenPosX);
	void nextPositionPushed(const int screenPosX);

	glm::ivec2 tileMapDispl, posForce, sizeForces[3], sizeForce, vel;
	Sprite* sprites[4];
	TileMap* map;
	Texture spritesheets[4];
	int animation = 0;
	bool active;

	int type; // 0=upgrade0 1=upgrade1 2=upgrade2 3=upgrade1Bottom
	forceState state;

	forceAttached attached;

	int rightLine, leftLine;
	queue<glm::ivec2> lastPlayerPos;
};


#endif // _PLAYER_INCLUDE