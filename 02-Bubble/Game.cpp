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
	
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	//glClearColor(0,0,0,0);
	
	mainMenu.init();
	levelSelection.init();
	helpMenu.init();
}

void Game::changeActiveScene(int sceneId)
{
	switch (sceneId)
	{
	case 0:
		inMenu = true;
		menuScreen = 0;
		/*if (scene != NULL)
			delete scene;*/
		break;
	case -1:
		inMenu = true;
		menuScreen = 1;
		break;
	case -2:
		inMenu = true;
		menuScreen = 2;
		break;

	default:
		inMenu = false;
		menuScreen = 0;
		scene = new Scene();
		const string level = "levels/level0" + to_string(sceneId) + ".txt";
		scene->init(level);
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
	//scene->render();
	//mainMenu.render();

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







