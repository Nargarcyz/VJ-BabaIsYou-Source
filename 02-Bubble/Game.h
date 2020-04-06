#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include "TextScene.h"
#include "LevelSelection.h"
#include "HelpScreen.h"
#include <irrKlang.h>


#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480


// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

public:
	Game() {}
	
	
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();

	void levelCompletedEvent(int levelId);


	bool isLevelCompleted(int levelId);
	bool isLevelUnlocked(int levelId);
	
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
	
	bool getKey(int key) const;
	bool getSpecialKey(int key) const;
	void playSound(const char* path);

	void changeActiveScene(int sceneId);
	

	irrklang::ISoundEngine* soundEngine;
	irrklang::ISound* moveSound;
	irrklang::ISound* backgroundMusic;
	
	irrklang::ISoundSource* menuMusic;
	irrklang::ISoundSource* levelMusic;
	irrklang::ISoundSource* deadMusic;
	irrklang::ISoundSource* winMusic;
	irrklang::ISoundSource* sinkSound;
	float aspectRatio;
	
private:
	bool bPlay;                       // Continue to play game?
	Scene* scene;                      // Scene to render
	Scene levelChooser;
	Scene* activeScene;
	bool inMenu, inSelectionScreen, inHelpScreen;
	int menuScreen = 0;
	TextScene mainMenu;
	LevelSelection levelSelection;
	HelpScreen helpMenu;
	bool keys[256], specialKeys[256]; // Store key states so that 
	                                  // we can have access at any time
	
	vector<bool> completedLevels;
	vector<bool> unlockedLevels;
	int levels = 5;

};


#endif // _GAME_INCLUDE


