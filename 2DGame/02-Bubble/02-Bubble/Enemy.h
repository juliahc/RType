#pragma once
#include <vector>
#include <utility>
#include "Entity.h"

class Enemy :
    public Entity
{
public:
	void init(const glm::ivec2& tileMapPos, Enemies enemy, int rotationAnimations, ShaderProgram& shaderProgram);
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
	vector<string> spriteFiles{ "images/enemicDeProba.png", "", "", "" };
	vector<pair<int, int>> enemySizes{ make_pair(23,24), make_pair(0,0), make_pair(0,0), make_pair(0,0) };
	//If the enemy have more than one animation assigned for the same animation movements
	int rotationAnims;
	int lastRotationAnim = 0;
	int timeLastRotationAnim = -1;
};

