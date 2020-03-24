#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 15
#define INIT_PLAYER_Y_TILES 15


Scene::Scene()
{
	map = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
}



bool Scene::createInstances(const string& levelFile)
{
	string line, type;
	ifstream fin;
	stringstream sstream;

	fin.open(levelFile.c_str());
	if (!fin.is_open())
		return false;
	int iLine = 0;
	while (iLine != 29 && std::getline(fin,line))
	{
		++iLine;
	}
	if (iLine != 29)
		return false;
	std::getline(fin, line);


	while (line.compare(0, 3, "END") != 0)
	{
		sstream.str(line);
		sstream >> type;
		
		if (type == "Object" || type == "Relation" || type == "Property")
		{
			string name;
			glm::vec2 pos;
			stringstream movableString;
			movableString.str(line);
			movableString >> type >> name >> pos.x >> pos.y;
			OutputDebugStringA(("\n" + movableString.str()).c_str());
			OutputDebugStringA(("\n" + type).c_str());
			OutputDebugStringA(("\n" + name).c_str());
			OutputDebugStringA(("\n" + to_string(pos.x)).c_str());
			OutputDebugStringA(("\n" + to_string(pos.y)).c_str());

			const char** it;
			int iType, index;
			if (type == "Object")
			{
				iType = 0;
				const char** it = std::find(std::begin(ObjectStrings), std::end(ObjectStrings), name);
				if (it != std::end(ObjectStrings))
				{
					index = std::distance(std::begin(ObjectStrings), it);
				}
				OutputDebugStringA(("\n" + type + " " +  ObjectStrings[index] + " at " + to_string(pos.x) + " " + to_string(pos.y)).c_str());
					
			}
			else if (type == "Relation")
			{
				iType = 1;
				const char** it = std::find(std::begin(RelationStrings), std::end(RelationStrings), name);
				if (it != std::end(RelationStrings))
				{
					index = std::distance(std::begin(RelationStrings), it);
				}
				OutputDebugStringA(("\n" + type + " " + RelationStrings[index] + " at " + to_string(pos.x) + " " + to_string(pos.y)).c_str());
			}
			else if (type == "Property")
			{
				iType = 2;
				const char** it = std::find(std::begin(PropertyStrings), std::end(PropertyStrings), name);
				if (it != std::end(PropertyStrings))
				{
					index = std::distance(std::begin(PropertyStrings),it);
				}
				OutputDebugStringA(("\n" + type + " " + PropertyStrings[index] + " at " + to_string(pos.x) + " " + to_string(pos.y)).c_str());
			}

			Movable* mov = new Movable((Words)iType, index);
			mov->init(glm::ivec2(0, 0), texProgram, glm::ivec2(24, 24));
			mov->setPosition(pos, map->getTileSize());
			map->addEntity(pos.x, pos.y, (Entity*)mov);
			movables.push_back(mov);
		}
		else if (type == "Baba" || type == "Flag")
		{
			string name;
			glm::vec2 pos;
			stringstream userString;
			int type;
			userString >> name >> pos.x >> pos.y;
			if (name == "Baba")
			{
				Player* pl = new Player(Baba_p);
				pl->init(glm::ivec2(0, 0), texProgram, glm::ivec2(24, 24));
				pl->setPosition(pos, map->getTileSize());
				map->addEntity(pos.x, pos.y, (Entity*)pl);
				possessables.push_back(pl);
			}
			else if (name == "Flag")
			{
				Player* pl = new Player(Flag_p);
				pl->init(glm::ivec2(0, 0), texProgram, glm::ivec2(24, 24));
				pl->setPosition(pos, map->getTileSize());
				map->addEntity(pos.x,pos.y, (Entity*)pl);
				possessables.push_back(pl);
			}
			
		}
		std::getline(fin, line);
	}
}void checkRules() {

}

void Scene::changePossession(PlayerType newPlayer) {
	switch (possessed)
	{
	case Baba_p:
		break;
	case Wall_p:
		break;
	case Flag_p:
		break;
	default:
		break;
	}
}

void Scene::init()
{
	initShaders();


	map = TileMap::createTileMap("levels/level02.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	createInstances("levels/level02.txt");
	changePossession(Baba_p);


	baba = new Player(Baba_p);
	baba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(24, 24));
	baba->setPosition(glm::vec2(INIT_PLAYER_X_TILES, INIT_PLAYER_Y_TILES), map->getTileSize());
	map->addEntity(INIT_PLAYER_X_TILES, INIT_PLAYER_Y_TILES, (Entity*)baba);
	

	bool success, outOfBounds = false;
	string msg;
	Player* e = (Player*)map->getEntity(glm::ivec2(INIT_PLAYER_X_TILES, INIT_PLAYER_Y_TILES), success, outOfBounds);
	/*if (success)
	{
		msg = "Player Type Added: " + to_string(e->getPlayerType()) + "\n";
		OutputDebugStringA(msg.c_str());
	}*/

	/*Movable* mov = new Movable(Words::Relation,Relations::Is);
	mov->init(glm::ivec2(0, 0), texProgram, glm::ivec2(24, 24));
	mov->setPosition(glm::vec2(10,10), map->getTileSize());
	map->addEntity(10,10, (Entity*)mov);
	movables.push_back(mov);

	mov = new Movable(Words::Object, Objects::Baba);
	mov->init(glm::ivec2(0, 0), texProgram, glm::ivec2(24, 24));
	mov->setPosition(glm::vec2(9, 10), map->getTileSize());
	map->addEntity(9, 10, (Entity*)mov);
	movables.push_back(mov);

	mov = new Movable(Words::Property, Properties::You);
	mov->init(glm::ivec2(0, 0), texProgram, glm::ivec2(24, 24));
	mov->setPosition(glm::vec2(11, 10), map->getTileSize());
	map->addEntity(11, 10, (Entity*)mov);
	movables.push_back(mov);*/

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
		Player* wall = new Player(Wall_p);
		//wall->changePlayerType(PlayerType::Wall);
		wall->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(24, 24));
		wall->setPosition(glm::vec2(wallLocs[i].x, (wallLocs[i].y)), map->getTileSize());
		//wall->setTileMap(map);
		map->addEntity(wallLocs[i].x, wallLocs[i].y, (Entity*)wall);
		wall->setCollision(true);

		/*msg = "Wall Added With Type: " + to_string(wall->getPlayerType()) + "\n";
		OutputDebugStringA(msg.c_str());

		bool success = false;
		Player* e = (Player*)map->getEntity(wallLocs[i].x, wallLocs[i].y, success);
		if (success)
		{
			msg = "Player Type Added: " + to_string(e->getPlayerType()) + "\t" + "Collisionable: " + to_string(e->hasCollision()) + "\n";
			OutputDebugStringA(msg.c_str());
			
		}*/

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

	checkRules();

	if (Game::instance().getSpecialKey(1))
	{
		possessed = PlayerType::Baba_p;
	}
	else if (Game::instance().getSpecialKey(2))
	{
		possessed = PlayerType::Wall_p;
	}


	glm::ivec2 movementDirection = glm::ivec2(0, 0);
	bool move = false;
	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
		movementDirection = glm::ivec2(-1, 0);
		move = true;
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
		movementDirection = glm::ivec2(1, 0);
		move = true;
	}else if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
		movementDirection = glm::ivec2(0, -1);
		move = true;
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
		movementDirection = glm::ivec2(0, 1);
		move = true;
	}


	glm::ivec2 testPosition = movementDirection;
	if (move)
	{
		switch (possessed)
		{

		case PlayerType::Baba_p:
			//baba->update(deltaTime);
			if (!baba->isMoving())
			{
				OutputDebugStringA("\Baba grid pos:");
				OutputDebugStringA(to_string(baba->getGridPos().x).c_str());
				OutputDebugStringA(",");
				OutputDebugStringA(to_string(baba->getGridPos().y).c_str());
				OutputDebugStringA("\n");
				testPosition = movementDirection + baba->getGridPos();
				bool success, outOfBounds;
				Entity* e;
				e = map->getEntity(testPosition, success, outOfBounds);
				if (outOfBounds)
				{
					break;
				}else if (success && e != NULL)
				{
					OutputDebugStringA("\nCollision\n");
					//OutputDebugStringA(to_string(e->getEntityType()).c_str());
					if (e->getEntityType() == MoveBlock || e->getEntityType() == User)
					{
						OutputDebugStringA("\n\tPUSHING\n");
						push(e, movementDirection);
					}

				}
				else {
					baba->move(movementDirection);
					map->moveEntity(baba->getGridPos(), testPosition);
					//baba->setGridPos(testPosition);
				}
			}

			break;
		case PlayerType::Wall_p: {
			int ind;
			for (int i = 0; i < walls.size(); i++)
			{
				if (glm::min(movementDirection.x,movementDirection.y)==-1)
				{
					ind = i;
				}
				else {
					ind = walls.size() - 1 - i;
				}
				//walls[i].update(deltaTime);

				if (!(walls[ind]->isMoving()))
				{
					testPosition = movementDirection + walls[ind]->getGridPos();
					bool success, outOfBounds;
					Entity* e;
					e = map->getEntity(testPosition, success, outOfBounds);

					

					if (success)
					{
						//OutputDebugStringA(e->getInfo().c_str());

					}
					else {
						walls[ind]->move(movementDirection);
						map->moveEntity(walls[ind]->getGridPos(), testPosition);
						//walls[ind]->setGridPos(testPosition);
					}
				}
			}
			break;
		}
		default:
			break;
		}
	}


	baba->update(deltaTime);

	for (int i = 0; i < walls.size(); i++)
	{
		walls[i]->update(deltaTime);
	}

	for (int i = 0; i < movables.size(); i++)
	{
		movables[i]->update(deltaTime);
	}

	
	//yaga->update(deltaTime);


	
	
	
}

//TODO: Fix increment not updating on next movement

bool Scene::push(Entity* entity, glm::ivec2& direction)
{	
	bool success, outOfBounds;
	Entity* e = map->getEntity(entity->getGridPos()+ direction, success, outOfBounds);
	OutputDebugStringA("\nEntity Position: ");
	OutputDebugStringA(to_string(entity->getGridPos().x).c_str());
	OutputDebugStringA(" ");
	OutputDebugStringA(to_string(entity->getGridPos().y).c_str());

	OutputDebugStringA("\nSuccess: ");
	OutputDebugStringA(to_string(success).c_str());

	OutputDebugStringA(" OutOfBounds: ");
	OutputDebugStringA(to_string(outOfBounds).c_str());
	OutputDebugStringA("\n");
	string s = "Movement Direction X: " + to_string(direction.x) + "Y: " + to_string(direction.y) + "\n";
	OutputDebugStringA(s.c_str());
	if (success && !outOfBounds)
	{
		OutputDebugStringA("Collision and may move");
		if (push(e, direction)) {
			entity->move(direction);
			map->moveEntity(entity->getGridPos(), entity->getGridPos() + direction);
			//entity->setGridPos(entity->getGridPos() + direction);
		}
	}
	else if (success && outOfBounds) {
		OutputDebugStringA("Collision and out of bounds");
		return false;
	}
	else if (!success){
		OutputDebugStringA("No collision, moves right away\n");
		entity->move(direction);
		map->moveEntity(entity->getGridPos(), entity->getGridPos()+direction);
		//entity->setGridPos(entity->getGridPos() + direction);
		string s = "Moves to X: " + to_string(entity->getGridPos().x) + "Y: " + to_string(entity->getGridPos().y) + "\n";
		OutputDebugStringA(s.c_str());
	}

	return false;
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
	//yaga->render();
	//interactable->render();

	map->render();

	for (int i = 0; i < walls.size(); i++)
	{
		walls[i]->render();
		
	}
	for (int i = 0; i < movables.size(); i++)
	{
		movables[i]->render();
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



