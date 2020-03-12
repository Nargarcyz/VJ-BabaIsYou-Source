#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
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
	baba->changePlayerType(PlayerType::Baba);
	baba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(24,24));
	baba->setPosition(glm::vec2(INIT_PLAYER_X_TILES, INIT_PLAYER_Y_TILES), map->getTileSize());
	map->addEntity(INIT_PLAYER_X_TILES, INIT_PLAYER_Y_TILES, (Entity*)baba);

	bool success = false;
	string msg;
	Player* e = (Player*)map->getEntity(INIT_PLAYER_X_TILES, INIT_PLAYER_Y_TILES, success);
	if (success)
	{
		msg = "Player Type Added: " + to_string(e->getPlayerType()) + "\n";
		OutputDebugStringA(msg.c_str());
	}

	

	//baba->setTileMap(map);

	/*yaga = new Player();
	yaga->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(24, 24));
	yaga->setPosition(glm::vec2((INIT_PLAYER_X_TILES-1) * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	yaga->setTileMap(map);*/
	
	/*for (int i = 0; i < 10; i++)
	{
		Player* p = new Player();
		p->changePlayerType(PlayerType::Wall);
		p->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(24, 24));
		p->setPosition(glm::vec2((INIT_PLAYER_X_TILES - i), INIT_PLAYER_Y_TILES), map->getTileSize());
		map->addEntity()
		walls.push_back(*p);
	}*/


	vector<glm::ivec2> wallLocs;
	map->getWallLocations(wallLocs);
	for (int i = 0; i < wallLocs.size(); i++)
	{
		Player* wall = new Player();
		wall->changePlayerType(PlayerType::Wall);
		wall->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(24, 24));
		wall->setPosition(glm::vec2(wallLocs[i].x, (wallLocs[i].y)), map->getTileSize());
		//wall->setTileMap(map);
		map->addEntity(wallLocs[i].x, wallLocs[i].y, (Entity*)wall);
		wall->setCollision(true);

		msg = "Wall Added With Type: " + to_string(wall->getPlayerType()) + "\n";
		OutputDebugStringA(msg.c_str());

		bool success = false;
		Player* e = (Player*)map->getEntity(wallLocs[i].x, wallLocs[i].y, success);
		if (success)
		{
			msg = "Player Type Added: " + to_string(e->getPlayerType()) + "\t" + "Collisionable: " + to_string(e->hasCollision()) + "\n";
			OutputDebugStringA(msg.c_str());
			
		}

		walls.push_back(wall);

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
	/*switch (possessed)
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
	*/
	
	glm::ivec2 movementDirection = glm::ivec2(0,0);
	bool move = false;
	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
		movementDirection = glm::ivec2(-1, 0);
		move = true;
		OutputDebugStringA("LEFT");
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
		movementDirection = glm::ivec2(1, 0);
		move = true;
		OutputDebugStringA("RIGHT");
	}

	//...

	glm::ivec2 testPosition = movementDirection;
	if (move)
	{
		switch (possessed)
		{

		case Possessable::Baba:
			//baba->update(deltaTime);
			if (!baba->isMoving())
			{
				testPosition += baba->getGridPos();
				bool success;
				Entity* e;
				e = map->getEntity(testPosition.x, testPosition.y, success);
				if (success)
				{
					OutputDebugStringA("\nCollision\n");

				}
				else {
					baba->movePlayer(movementDirection);
					map->moveEntity(baba->getGridPos(), testPosition);
					baba->setGridPos(testPosition);
				}
			}
			
			break;
		case Possessable::Wall:
			for (int i = 0; i < walls.size(); i++)
			{
				//walls[i].update(deltaTime);
				
				if (!(walls[i]->isMoving()))
				{
					testPosition += walls[i]->getGridPos();
					bool success;
					Entity* e;
					e = map->getEntity(testPosition.x, testPosition.y, success);
					if (success)
					{
						//OutputDebugStringA(e->getInfo().c_str());

					}
					else {
						walls[i]->movePlayer(movementDirection);
						map->moveEntity(walls[i]->getGridPos(), testPosition);
						walls[i]->setGridPos(testPosition);
					}
				}
			}
			break;
		default:
			break;
		}
	}
	switch (possessed)
	{
	case Possessable::Baba:
		baba->update(deltaTime);
		break;
	case Possessable::Wall:
		for (int i = 0; i < walls.size(); i++)
		{
			walls[i]->update(deltaTime);
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
	modelview = glm::translate(modelview, glm::vec3(0, 0, 0));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	//map->render();
	//yaga->render();
	//interactable->render();


	for (int i = 0; i < walls.size(); i++)
	{
		/*glBegin(GL_POINTS); 
		glm::vec2 pos = walls[i]->getPosition();
		glColor3f(255, 0, 0);
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				glVertex2f(pos.x+i, pos.y+j);
			}
		}
		glEnd();*/
		walls[i]->render();
		//glBegin(GL_POINTS); //set opengl to point drawing mode
		//for(int x=0; x<128; x+=2)
		//	for(int y=0; y<128; y+=2)
		//		for(int z=0; z<128; z+=2){    
		//			glColor3f((float)x/128.0f,(float)y/128.0f,(float)z/128.0f);
		//			//slightly different colour for each dot
		//			glVertex3f(x,y,z);        
		//			//draw the point
		//		}
		//glEnd();//end point drawing mode
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



