#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include "Scene.h"


#define SCREEN_WIDTH 640 //384
#define SCREEN_HEIGHT 480 //256

enum keyState { IDLE, PRESS, REPEAT, RELEASE };

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

private:
	bool bPlay;                       // Continue to play game?
	Scene scene;                      // Scene to render
	keyState keys[256], specialKeys[256]; // Store key states so that 
	enum GameState {MENU, GAME, INSTRUCTIONS, CREDITS}; // we can have access at any time
	GameState state;                        
	
};


#endif // _GAME_INCLUDE


