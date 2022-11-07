#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include "Scene.h"

#define SCREEN_WIDTH 384 
#define SCREEN_HEIGHT 256

enum keyState { IDLE, PRESS, REPEAT, RELEASE };
enum GameState { MENU, GAME, INSTRUCTIONS, CREDITS, TRANSITION, GAMEOVER , READY, THEEND, NONE };

// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

private:
	Game() {}
	
public:
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);
	
	keyState getKey(int key) const;
	keyState getSpecialKey(int key) const;

	void renderState(GameState state);
	void setState(GameState newState);
	GameState getPreviousState();
	GameState getNextState();

private:
	void processInput();

	bool bPlay;                       // Continue to play game?
	Scene scene;                      // Scene to render
	bool keysBool[256], specialKeysBool[256];
	int keysInt[256];
	keyState keys[256], specialKeys[256]; // Store key states so that 
	GameState state, previousState, nextState;
	int nbFramesRepeat = 8;
};


#endif // _GAME_INCLUDE


