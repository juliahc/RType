#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"
#include "Entity.h"
#include "Shot.h"
#include "Quad.h"
#include "TexturedQuad.h"
#include "Sprite.h"
#include <vector>
#include <map>
#include <set>


// Scene contains all the entities of our game.
// It is responsible for updating and render them.

class Scene
{

public:

	Scene();
	~Scene();

	void init();
	
	void updateGame(int deltaTime);
	void updateMenu(int deltaTime);
	void updateInstructions(int deltaTime);
	void updateCredits(int deltaTime);

	void renderGame();
	void renderMenu();
	void renderInstructions();
	void renderCredits();

	void addPlayerShot();
	void addShot(string& spriteFolder, const glm::ivec2& velocity, glm::ivec2& pos, const glm::ivec2& size, const glm::vec2& sizeInSpriteSheet, const int& damage);
	
	bool inScreen(const glm::ivec2& pos, const glm::ivec2& size);

private:
	void initShaders();
	void initEnemies();
	void createEnemies();
	TileMap *map;
	Player *player;
	std::set<Shot*> shots;
	ShaderProgram simpleProgram, texProgram;
	float currentTime;
	glm::mat4 projection;

	int screenMovement = 0;
	int screenExtraPosition = 0;
	int enemyGenerator = 0;

	/*menu*/
	int menuState = 1; // 1=play/resume 2=instructions 3=credits
	Texture menuTexs[5];
	TexturedQuad* menuTexQuad[5];
	std::map<int, Enemy> enemies;
	vector<Enemy*> activeEnemies;
};


#endif // _SCENE_INCLUDE

