#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 13
#define INIT_PLAYER_Y_TILES 13


Scene::Scene()
{
	map = NULL;
	baba = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(baba != NULL)
		delete baba;
}


void Scene::init()
{
	initShaders();
	
	map = TileMap::createTileMap("levels/level02.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	baba = new Player();
	baba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(24,24));
	baba->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	baba->setTileMap(map);

	/*yaga = new Player();
	yaga->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(24, 24));
	yaga->setPosition(glm::vec2((INIT_PLAYER_X_TILES-1) * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	yaga->setTileMap(map);*/
	
	//for (int i = 0; i < 10; i++)
	//{
	//	Player* p = new Player();
	//	p->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(24, 24));
	//	p->setPosition(glm::vec2((INIT_PLAYER_X_TILES - i) * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	//	p->setTileMap(map);
	//	walls.push_back(*p);
	//}
	vector<glm::ivec2> wallLocs;
	map->getWallLocations(wallLocs);
	for (int i = 0; i < wallLocs.size(); i++)
	{
		Player* wall = new Player();
		wall->changeSprite("images/wall.png", glm::vec2(1, 1));
		wall->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(24, 24));
		wall->setPosition(glm::vec2(wallLocs[i].x * map->getTileSize(), (wallLocs[i].y) * map->getTileSize()));
		wall->setTileMap(map);
		walls.push_back(*wall);

	}
	//possessed = baba;

	/*interactable = new Interactable();
	interactable->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(24, 24));
	interactable->setPosition(glm::vec2(3 * map->getTileSize(), 13 * map->getTileSize()));
	interactable->setTileMap(map);*/

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	//possessed->update(deltaTime);


	if (Game::instance().getSpecialKey(1))
	{
		possessed = Possessable::Baba;
	}
	else if (Game::instance().getSpecialKey(2))
	{
		possessed = Possessable::Wall;
	}

	switch (possessed)
	{
	case Possessable::Baba:
		baba->update(deltaTime);
		break;
	case Possessable::Wall:
		for (int i = 0; i < walls.size(); i++)
		{
			walls[i].update(deltaTime);
		}
		break;
	default:
		break;
	}
	
	//yaga->update(deltaTime);


	
	
	
}



void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	//yaga->render();
	//interactable->render();


	for (int i = 0; i < walls.size(); i++)
	{
		walls[i].render();
	}
	baba->render();
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



