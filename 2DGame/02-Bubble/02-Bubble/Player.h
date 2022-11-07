#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Entity.h"
#include "Force.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player: public Entity
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, bool animation);
	void update(int deltaTime, int screenPosX, Force* force);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	
	void collision();
	glm::ivec2 getPosition();
	glm::ivec2 getSize();

	void setShotCharge(int charge);
	int getShotCharge();
	bool inInitAnimation();
	bool died();
	bool boomFinished();

private:
	int count, initAnimationId;
	bool bJumping;
	bool initAnimation, right;
	bool boom = false;
	bool boomFinish = false;
	bool charging = false;
	bool invulnerable = false;
	int boomAnimation = -1;
	int chargeAnimation;
	int shotCharge = 1;
	int chargeAnimationChangeTime;
	glm::ivec2 tileMapDispl, posPlayer, sizePlayer;
	int jumpAngle, startY;
	Texture spritesheet, spritesheetFire, spritesheetBoom, spritesheetCharge;
	Sprite	*sprite, * spriteFire, *spriteBoom, *spriteCharge;
	TileMap *map;
};


#endif // _PLAYER_INCLUDE


