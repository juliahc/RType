#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"
#include "Entity.h"
#include "Quad.h"
#include "TexturedQuad.h"
#include <vector>

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

private:
	void initShaders();
	void createEnemies();
	TileMap *map;
	Player *player;
	Enemy *basicEnemy;
	ShaderProgram simpleProgram, texProgram;
	float currentTime;
	glm::mat4 projection;

	int enemyGenerator = 0;

	/*menu*/
	int menuState = 1; // 1=play/resume 2=instructions 3=credits
	Texture menuTexs[5];
	TexturedQuad* menuTexQuad[5];
	vector<Enemy*> activeEnemies;
};


#endif // _SCENE_INCLUDE

