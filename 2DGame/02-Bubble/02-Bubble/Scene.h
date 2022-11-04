#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "force.h"
#include "Enemy.h"
#include "Entity.h"
#include "Shot.h"
#include "Quad.h"
#include "TexturedQuad.h"
#include "Sprite.h"
#include <vector>
#include <map>
#include <set>

enum menuTypeEnum {INITIAL, PLAYING};
// Scene contains all the entities of our game.
// It is responsible for updating and render them.

class Scene
{

public:

	Scene();
	~Scene();

	void init();
	void restartGame();

	void updateGame(int deltaTime);
	void updateGameBackground(int deltaTime);
	void updateGameEnemies(int deltaTime);
	void updateGamePlayer(int deltaTime);
	void updateGameForce(int deltaTime);
	void updateGameShots(int deltaTime);

	void updateMenu(int deltaTime);
	void updateInstructions(int deltaTime);
	void updateCredits(int deltaTime);
	void updateTransition(int deltaTime);
	void updateGameOver(int deltaTime);

	void renderGame();
	void renderMenu();
	void renderInstructions();
	void renderCredits();
	void renderTransition();
	void renderGameover();

	void checkCollisions();

	void addPlayerShot();
	void addShot(string& spriteFolder, const glm::ivec2& velocity, glm::ivec2& pos, const glm::ivec2& size, const glm::vec2& sizeInSpriteSheet, const int& damage, bool fromPlayer);
	
	bool inScreen(const glm::ivec2& pos, const glm::ivec2& size);
	bool isCollision(const glm::ivec2& pos1, const glm::ivec2& size1, const glm::ivec2& pos2, const glm::ivec2& size2);

private:
	void initShaders();
	void initEnemies();
	void createEnemies();
	TileMap *map;
	Player *player;
	Force* force;
	std::set<Shot*> playerShots, enemyShots;
	ShaderProgram texProgram, texProgramGame, simpleProgram;
	float currentTime;
	glm::mat4 projection, gameProjection;

	int screenMovement = 0;
	int screenExtraPosition = 0;
	int enemyGenerator = 0;
	std::map<int, Enemy> enemies;
	set<Enemy*> activeEnemies;
	set<Enemy*> boomEnemies;
	TexturedQuad *heart, *gameBackground, *gameOver;
	Texture heartTex, gameBackTex, gameOverTex;
	Quad* quad;

	/*menu*/
	int menuState = 1; // 1=play/resume 2=instructions 3=credits
	Texture menuTexs[2];
	TexturedQuad* menuTexQuad[10];
	TexturedQuad* menuBackground;
	menuTypeEnum menuType = INITIAL;

	int lifes = 3;
	int gameOverCount = 0;

	int transitionCount = 0;
};


#endif // _SCENE_INCLUDE

