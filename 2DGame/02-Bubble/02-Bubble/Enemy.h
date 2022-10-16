#pragma once
#include <vector>
#include <utility>
#include "Entity.h"

class Enemy :
    public Entity
{
public:
	void init(const glm::ivec2& tileMapPos, Enemies enemy, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	bool checkCollisionRight();
	bool checkCollisionLeft();
	bool checkCollisionUp();
	bool checkCollisionDown();

private:
	bool right = false, bJumping = false, up = true, attacking = false;
	glm::ivec2 tileMapDispl, posEnemy;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	Enemies myType;
	vector<string> spriteFiles{ "images/enemies/basic1.png", "images/enemies/basic2.png", "", "" };
	vector<glm::ivec2> enemySizes{ glm::ivec2(23, 24), glm::ivec2(30, 28), glm::ivec2(0, 0), glm::ivec2(0, 0) };
	vector<glm::vec2> spriteSheetSize{ glm::vec2(0.25f, 0.5f), glm::vec2(0.2f, 0.5f), glm::vec2(0., 0.), glm::vec2(0., 0.) };
	int lastRotationAnim = 0;
	int timeLastRotationAnim = -1;
};

