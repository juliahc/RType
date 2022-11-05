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

	//Init
	void init();

	//Updates
	void updateMenu(int deltaTime);
	void updateGame(int deltaTime);
	void updateInstructions(int deltaTime);
	void updateCredits(int deltaTime);
	void updateTransition(int deltaTime);
	void updateGameOver(int deltaTime);

	//Renders
	void renderMenu();
	void renderGame();
	void renderInstructions();
	void renderCredits();
	void renderTransition();
	void renderGameover();

private:

	/* FUNCTIONS */

	//Subinits
	void initMenu();
	void initGame();
	void initInstructions();
	void initCredits();
	void initTransition();
	void initGameover();

	void initShaders();
	void initEnemies();
	
	//Subupdates
	void updateGameBackground(int deltaTime);
	void updateGameEnemies(int deltaTime);
	void updateGamePlayer(int deltaTime);
	void updateGameForce(int deltaTime);
	void updateGameShots(int deltaTime);

	//Subupdates functions
	void createEnemies();
	void restartGame();
	void addPlayerShot();
	void addForceShot();
	void addShot(string& spriteFolder, const glm::ivec2& velocity, glm::ivec2& pos, const glm::ivec2& size, const glm::vec2& sizeInSpriteSheet, const int& damage, bool fromPlayer, int upgrade);

	//Collisions
	void checkCollisions();
	bool isCollision(const glm::ivec2& pos1, const glm::ivec2& size1, const glm::ivec2& pos2, const glm::ivec2& size2);
	bool inScreen(const glm::ivec2& pos, const glm::ivec2& size);
	

	/* ATTRIBUTES */
	
	ShaderProgram texProgram, texProgramGame, simpleProgram;
	glm::mat4 projection;


	/* MENU ATTRIBUTES */
	
	int menuState = 1; // 1=play/resume 2=instructions 3=credits
	Texture menuTexs[2];
	TexturedQuad* menuTexQuad[10];
	TexturedQuad* menuBackground;
	menuTypeEnum menuType = INITIAL;


	/* GAME ATTRIBUTES */
	int count;
	float currentTime;
	TileMap *map;
	glm::mat4 gameProjection;
	TexturedQuad *gameBackground, *heart, *upgradeTokens[3];
	Texture gameBackTex, heartTex, upgradeTokensTex;

	//Background
	int screenMovement = 0;
	int screenExtraPosition = 0;

	//Player
	Player *player;
	Force* force;

	//Enemies
	int enemyGenerator = 0;
	std::map<int, Enemy> enemies;
	set<Enemy*> activeEnemies;
	set<Enemy*> boomEnemies;

	//Shots
	std::set<Shot*> playerShots, enemyShots;
	int lastUpgrade2Shot = 0;

	//Lifes (hearts)
	int lifes = 3;

	//tokens
	bool showTokens[3];
	glm::vec3 tokenPositions[3];


	/* INSTRUCTIONS ATTRIBUTES */


	/* CREDITS ATTRIBUTES */


	/* TRANSITION ATTRIBUTES */
	
	Quad* quad;
	int transitionCount = 0;


	/* GAMEOVER ATTRIBUTES */
	
	TexturedQuad* gameOver;
	Texture gameOverTex;
	int gameOverCount = 0;
};


#endif // _SCENE_INCLUDE

