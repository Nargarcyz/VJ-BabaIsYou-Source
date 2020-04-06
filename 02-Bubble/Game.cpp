#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	bPlay = true;
	inMenu = true;
	soundEngine = irrklang::createIrrKlangDevice();
	if (!soundEngine)
	{
		exit(333);
	}
	aspectRatio = glutGet(GLUT_WINDOW_WIDTH) / glutGet(GLUT_WINDOW_HEIGHT);
	menuMusic = soundEngine->addSoundSourceFromFile("sounds/mainTheme.ogg");
	menuMusic->setDefaultVolume(0.5f);

	levelMusic = soundEngine->addSoundSourceFromFile("sounds/levelMusic.ogg");
	levelMusic->setDefaultVolume(0.5f);

	deadMusic = soundEngine->addSoundSourceFromFile("sounds/deadMusic.ogg");
	deadMusic->setDefaultVolume(0.5f);

	winMusic = soundEngine->addSoundSourceFromFile("sounds/winMusic.ogg");
	winMusic->setDefaultVolume(0.5f);

	sinkSound = soundEngine->addSoundSourceFromFile("sounds/sinkSound.ogg");
	sinkSound->setDefaultVolume(0.5f);

	

	completedLevels = vector<bool>(5, 0);
	unlockedLevels = vector<bool>(5, 1);
	unlockedLevels[0] = 1;

	glClearColor(3.1f / 255, 3.1f / 255, 3.1f / 255, 0);

	
	mainMenu.init();
	levelSelection.init();
	helpMenu.init();
}

void Game::levelCompletedEvent(int levelId) {
	completedLevels[levelId - 1] = 1;
	if (levelId <= levels)
	{
		unlockedLevels[levelId] = 1;
	}
}

bool Game::isLevelCompleted(int levelId)
{
	return completedLevels[levelId - 1];
}

bool Game::isLevelUnlocked(int levelId)
{
	return unlockedLevels[levelId-1];
}

void Game::changeActiveScene(int sceneId)
{
	if (sceneId >0)
	{
		if (scene)
		{
			delete scene;
		}
	}
	switch (sceneId)
	{
	case 0:
		inMenu = true;
		menuScreen = 0;

		if (backgroundMusic->getSoundSource() != menuMusic)
		{
			soundEngine->stopAllSounds();
			backgroundMusic = soundEngine->play2D(menuMusic, true, false, true);
		}

		break;
	case -1:
		inMenu = true;
		menuScreen = 1;
		if (backgroundMusic->getSoundSource() != menuMusic)
		{
			soundEngine->stopAllSounds();
			backgroundMusic = soundEngine->play2D(menuMusic, true, false, true);
		}
		break;
	case -2:
		inMenu = true;
		menuScreen = 2;
		if (backgroundMusic->getSoundSource() != menuMusic)
		{
			soundEngine->stopAllSounds();
			backgroundMusic = soundEngine->play2D(menuMusic, true, false, true);
		}
		break;

	default:
		inMenu = false;
		menuScreen = 0;
		
		scene = new Scene(sceneId);
		const string level = "levels/level0" + to_string(sceneId) + ".txt";
		scene->init(level);

		if (backgroundMusic->getSoundSource() != levelMusic)
		{
			soundEngine->stopAllSounds();
			backgroundMusic = soundEngine->play2D(levelMusic, true, false, true);
		}

		break;
	}
}



bool Game::update(int deltaTime)
{
	
	
	if (inMenu)
	{
		switch (menuScreen)
		{
		case 0:
			mainMenu.update(deltaTime);
			break;
		case 1:
			levelSelection.update(deltaTime);
			break;
		case 2:
			helpMenu.update(deltaTime);
			break;
		default:
			break;
		}
		
	}
	else {
		scene->update(deltaTime);
	}
	
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	aspectRatio = (float)glutGet(GLUT_WINDOW_WIDTH) / (float)glutGet(GLUT_WINDOW_HEIGHT);
	if (inMenu)
	{
		switch (menuScreen)
		{
		case 0:
			mainMenu.render();
			break;
		case 1:
			levelSelection.render();
			break;
		case 2:
			helpMenu.render();
			break;
		default:
			break;
		}

	}
	else {
		scene->render();
	}
}

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}


bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}







