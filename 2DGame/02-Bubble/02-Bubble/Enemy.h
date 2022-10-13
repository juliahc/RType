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

private:
	bool right = false, bJumping = false, up = true;
	glm::ivec2 tileMapDispl, posEnemy;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	vector<string> spriteFiles{ "images/enemies/basic1.png", "images/enemies/basic2.png", "", "" };
	vector<pair<int, int>> enemySizes{ make_pair(23,24), make_pair(24,26), make_pair(0,0), make_pair(0,0) };
	int lastRotationAnim = 0;
	int timeLastRotationAnim = -1;
};

