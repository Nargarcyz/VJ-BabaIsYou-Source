#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include <set>



#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 14
#define INIT_PLAYER_Y_TILES 15

Scene::Scene()
{
	map = NULL;

}

Scene::Scene(int levelId)
{
	map = NULL;
	this->levelId = levelId;
	
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
		else// if (type == "Baba" || type == "Flag")
		{
			string name;
			glm::vec2 pos;
			stringstream userString;
			int type;
			userString.str(line);
			userString >> name >> pos.x >> pos.y;
			OutputDebugStringA(("\n" + name).c_str());
			OutputDebugStringA(("\n" + to_string(pos.x)).c_str());
			OutputDebugStringA(("\n" + to_string(pos.y)).c_str());
			if (name == "Baba")
			{
				Player* pl = new Player(Baba_p);
				pl->init(glm::ivec2(0, 0), texProgram, glm::ivec2(24, 24));
				pl->setPosition(pos, map->getTileSize());
				pl->setCollision(false);
				//map->addEntity(pos.x, pos.y, (Entity*)pl);
				possessables.push_back(pl);
			}
			else if (name == "Flag")
			{
				Player* pl = new Player(Flag_p);
				pl->init(glm::ivec2(0, 0), texProgram, glm::ivec2(24, 24));
				pl->setPosition(pos, map->getTileSize());
				pl->setCollision(false);
				//map->addEntity(pos.x,pos.y, (Entity*)pl);
				possessables.push_back(pl);
			}
			
		}
		std::getline(fin, line);
	}




}
void Scene::applyRule(Objects obj, Properties prop)
{
	for (int i = 0; i < possessables.size(); i++)
	{
		if (possessables[i]->getPlayerType() == obj)
		{
			switch (prop)
			{
			case You:
				//changePossession((PlayerType)obj);
				possessables[i]->setPossessed(true);
				noPossession = false;
				map->removeEntity(possessables[i]->getGridPos());

				break;
			case Stop:
				possessables[i]->setCollision(true);
				map->addEntity(possessables[i]->getGridPos().x, possessables[i]->getGridPos().y, possessables[i]);
				break;
			case Win:
				if (possessables[i]->isPossessed())
				{
					levelCompleted();
				}
				possessables[i]->setWin(true);
				map->addEntity(possessables[i]->getGridPos().x, possessables[i]->getGridPos().y, possessables[i]);
				break;
			case Push:
				possessables[i]->setPushable(true);
				map->addEntity(possessables[i]->getGridPos().x, possessables[i]->getGridPos().y, possessables[i]);
			default:
				break;
			}
		}
	}
}


void Scene::changePossession(PlayerType newPlayer) {
	for (int i = 0; i < possessables.size(); i++)
	{
		if (newPlayer == NoPl)
		{
			possessables[i]->setPossessed(false);

		}
		else if (possessables[i]->getPlayerType() == newPlayer)
		{
			OutputDebugStringA("\nFound an entity to possess");
			possessables[i]->setPossessed(true);
			map->removeEntity(possessables[i]->getGridPos());
		}
		else {
			possessables[i]->setPossessed(false);
			map->addEntity(possessables[i]->getGridPos().x, possessables[i]->getGridPos().y, possessables[i]);
		}
	}
}

void Scene::checkRules() {
	for (int i = 0; i < possessables.size(); i++) {
		//map->addEntity(possessables[i]->getGridPos().x, possessables[i]->getGridPos().y, possessables[i]);
		map->removeEntity(possessables[i]->getGridPos());
	}
	for (int i = 0; i < possessables.size(); i++)
	{
		((Player*)possessables[i])->setStop(false);
		((Player*)possessables[i])->setWin(false);
		((Player*)possessables[i])->setPushable(false);
		possessables[i]->setPossessed(false);
		noPossession = true;
		//changePossession(NoPl);
	}

	set< pair<Objects, Properties> > rulesToProcess;
	Relations rel;
	
	OutputDebugStringA("\t---RECHECKING RULES ---\n");
	
	for (int i = 0; i < movables.size(); i++)
	{
		if (movables[i]->getWordType() == Object)
		{
			OutputDebugStringA(("\nFound a movable object at " +to_string(movables[i]->getGridPos().x) + "," + to_string(movables[i]->getGridPos().y)).c_str());
			vector<Movable*> line = getMovableLine(movables[i]->getGridPos(), glm::vec2(1, 0));
			if (!line.empty())
			{

				string lineInfo = "";
				int separator = 0;
				for (int i = 0; i < line.size(); i++)
				{
					switch (line[i]->getWordType())
					{
					case Object:
						lineInfo += " " + (string)ObjectStrings[line[i]->getTypeIndex()];
						break;
					case Relation:
						if (line[i]->getTypeIndex() == Relations::Is)
						{
							separator = i;
						}
						lineInfo += " " + (string)RelationStrings[line[i]->getTypeIndex()];
						break;
					case Property:
						lineInfo += " " + (string)PropertyStrings[line[i]->getTypeIndex()];
						break;
					default:
						break;
					}
				}
				OutputDebugStringA("\nFound valid line:");
				OutputDebugStringA(("\n" + lineInfo).c_str());


				OutputDebugStringA("\n--Parsing objects:");
				for (int object = 0; object < separator; object = object + 2)
				{
					OutputDebugStringA(("\n"+(string)ObjectStrings[line[object]->getTypeIndex()] + " will have attributes:").c_str());
					for (int property = separator+1; property < line.size(); property = property + 2)
					{
						OutputDebugStringA((","+(string)PropertyStrings[line[property]->getTypeIndex()]).c_str());
						OutputDebugStringA(("\n Inserting " + to_string((Objects)object) + "," + to_string((Properties)property)).c_str());
						rulesToProcess.insert(make_pair((Objects)line[object]->getTypeIndex(), (Properties)line[property]->getTypeIndex()));
					}
				}



			}
		}
	}


	OutputDebugStringA("\n|-----RULE PARSING-----|");
	set< pair <Objects, Properties> >::iterator it = rulesToProcess.begin();
	while (it != rulesToProcess.end())
	{
		OutputDebugStringA("\n|-----Rule:");
		OutputDebugStringA(("\n|-----"+(string)ObjectStrings[(*it).first]+"is" + (string)PropertyStrings[(*it).second]).c_str());
		applyRule((*it).first, (*it).second);
		it++;
	}

	needToRecheckRules = false;

	/*for (int i = 0; i < movables.size(); i++)
	{
		if (movables[i]->getWordType() == Object)
		{

			while (true)
			{

			}





			glm::vec2 position = movables[i]->getGridPos();
			bool success = false;
			bool outOfBounds = false;
			Entity* ent = map->getEntity(position + glm::vec2(1, 0), success, outOfBounds);
			
			if (success && !outOfBounds && ent->getEntityType() == MoveBlock && ((Movable*)ent)->getWordType() == Relation)
			{
				success = false;
				outOfBounds = false;
				ent = map->getEntity(position + glm::vec2(2, 0), success, outOfBounds);
				if (success && !outOfBounds && ent->getEntityType() == MoveBlock && ((Movable*)ent)->getWordType() == Property)
				{

				}
			}
		}
	}*/

	/*for (int i = 0; i < toProcess.size(); i++)
	{
		applyRule(toProcess[i].first, rel, toProcess[i].second);
	}*/
}

vector<Movable*> Scene::getMovableLine(glm::vec2 startPos, glm::vec2 direction)
{
	
	vector<Movable*> mov;
	vector<Movable*> partial = vector<Movable*>();
	glm::vec2 pos = startPos ;
	bool success = false;
	bool outOfBounds = false;
	bool valid = false;
	bool onObjects = true;
	bool hasAnd = false;
	int i = 0;
	Entity* ent = map->getEntity(pos, success, outOfBounds);
	OutputDebugStringA(("\n--Success: " + to_string(success) + " OutOfBounds: " + to_string(outOfBounds)).c_str());
	OutputDebugStringA(("\n\tOBTAINING LINE: START - " + to_string(startPos.x) + "," + to_string(startPos.y)).c_str());
	while (success && !outOfBounds)
	//while (!outOfBounds)
	{
		
		OutputDebugStringA(("\n--Success Checking at " + to_string(pos.x) + "," + to_string(pos.y)).c_str());
		OutputDebugStringA(("\n--Obtained " + to_string(ent->getGridPos().x) + "," + to_string(ent->getGridPos().y)).c_str());
		OutputDebugStringA(("\n--Entity type: " + to_string(ent->getEntityType())).c_str());
		if (ent->getEntityType() != MoveBlock)
		{
			OutputDebugStringA("\n----stopping");
			break;
		} else 
		
		if (onObjects)
		{
			if (i % 2 == 1) //If position is odd
			{
				if (((Movable*)ent)->getWordType() != Relation)
					{
						//If something that is not a Relation is in an odd position,
						//the phrase is invalid
						valid = false;
						break;
					}
					else if (((Movable*)ent)->getTypeIndex() == Relations::Is)
					{
						//If at the first relation position there is not an and
						//there won't be an and in the objects section
						onObjects = false;
						mov.push_back((Movable*)ent);
					}
					else if (((Movable*)ent)->getTypeIndex() == Relations::And)
					{
						//If at the first relation there is an and, keep parsing ands until
						//an if is found
						mov.push_back((Movable*)ent);
					}
					
				
			}
			else {
				if (((Movable*)ent)->getWordType() != Object)
				{
					//return vector<Movable*>();
					valid = false;
					break;
				}
				else
				{
					mov.push_back((Movable*)ent);
				}
			}
		}
		else {
			OutputDebugStringA("\nAfter Objects");
			if (i % 2 == 1)
			{
				if (((Movable*)ent)->getWordType() != Relation)
				{
					if (valid)
					{
						break;
					}
					//return vector<Movable*>();
					valid = false;
					break;
				}
				else if (((Movable*)ent)->getTypeIndex() == Relations::And)
				{
					valid = false;
					mov.push_back((Movable*)ent);
				}
			}
			else {
				if (((Movable*)ent)->getWordType() != Property)
				{
					//return vector<Movable*>();
					valid = false;
					break;
				}
				else
				{
					OutputDebugStringA("\Property at right position");
					valid = true;
					mov.push_back((Movable*)ent);
					partial = mov;
				}
			}
		}
		OutputDebugStringA("\nNew IT");
		pos = pos + direction;
		i++;
		ent = map->getEntity(pos, success, outOfBounds);
		OutputDebugStringA(("\n--Success: " + to_string(success) + " OutOfBounds: " + to_string(outOfBounds)).c_str());
	}
	OutputDebugStringA("\nENDED");

	/*if (valid)
	{
		OutputDebugStringA("\nVALID LINE");
		return partial;
	}
	else {
		OutputDebugStringA("\nNOT A VALID LINE");
		return vector<Movable*>();
	}*/
	return partial;
	//return vector<Movable*>();
}


void Scene::init(const string levelFile)
{
	initShaders();
	if (!levelCompletedText.init("fonts/OpenSans-Regular.ttf"))
		cout << "Could not load font!!!" << endl;

	map = TileMap::createTileMap(levelFile, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	createInstances(levelFile);
	
	

	/*baba = new Player(Baba_p);
	baba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(24, 24));
	baba->setPosition(glm::vec2(INIT_PLAYER_X_TILES, INIT_PLAYER_Y_TILES), map->getTileSize());
	map->addEntity(INIT_PLAYER_X_TILES, INIT_PLAYER_Y_TILES, (Entity*)baba);*/
	

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
	map->getEntityLocations(wallLocs,1);
	for (int i = 0; i < wallLocs.size(); i++)
	{
		Player* wall = new Player(Wall_p);
		//wall->changePlayerType(PlayerType::Wall);
		wall->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(24, 24));
		wall->setPosition(glm::vec2(wallLocs[i].x, (wallLocs[i].y)), map->getTileSize());
		//wall->setTileMap(map);
		map->addEntity(wallLocs[i].x, wallLocs[i].y, (Entity*)wall);
		wall->setCollision(true);
		
		//OutputDebugStringA(("\n\t" + to_string(wall->getEntityType() == User)).c_str());


		//walls.push_back(wall);
		possessables.push_back(wall);

	}

	vector<glm::ivec2> rockLocs;
	map->getEntityLocations(rockLocs, 3);
	for (int i = 0; i < rockLocs.size(); i++)
	{
		Player* rock = new Player(Rock_p);
		//wall->changePlayerType(PlayerType::Wall);
		rock->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(24, 24));
		rock->setPosition(glm::vec2(rockLocs[i].x, (rockLocs[i].y)), map->getTileSize());
		//wall->setTileMap(map);
		map->addEntity(rockLocs[i].x, rockLocs[i].y, (Entity*)rock);
		rock->setCollision(true);

		//OutputDebugStringA(("\n\t" + to_string(wall->getEntityType() == User)).c_str());


		//walls.push_back(wall);
		possessables.push_back(rock);

	}




	//possessed = baba;

	/*interactable = new Interactable();
	interactable->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(24, 24));
	interactable->setPosition(glm::vec2(3 * map->getTileSize(), 13 * map->getTileSize()));
	interactable->setTileMap(map);*/

	//changePossession(Baba_p);
	std::reverse(possessables.begin(), possessables.end());
	checkRules();
	//projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	
	currentTime = 0.0f;
	clickedTime = currentTime;
}





void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	//possessed->update(deltaTime);

	bool moving = false;
	bool move = false;
	for (int i = 0; i < possessables.size(); i++)
	{
		if (possessables[i]->isMoving())
		{
			moving = true;
			break;
		}
	}

	if (noPossession)
	{

		if (Game::instance().backgroundMusic->getSoundSource() != Game::instance().deadMusic)
		{
			Game::instance().soundEngine->stopAllSounds();
			Game::instance().backgroundMusic = Game::instance().soundEngine->play2D(Game::instance().deadMusic, true, false, true);
		}

		if (Game::instance().getKey(13)) {
			Game::instance().changeActiveScene(-1);
		}

		for (int i = 0; i < possessables.size(); i++)
		{
			possessables[i]->update(deltaTime);
		}

		for (int i = 0; i < movables.size(); i++)
		{
			movables[i]->update(deltaTime);
		}
		return;
	}

	if (!moving && completed)
	{
		if (Game::instance().backgroundMusic->getSoundSource() != Game::instance().winMusic)
		{
			Game::instance().soundEngine->stopAllSounds();
			Game::instance().backgroundMusic = Game::instance().soundEngine->play2D(Game::instance().winMusic, false, false, true);
		}
		OutputDebugStringA("COMPLETED");
		if (Game::instance().getKey(13)) {
			Game::instance().changeActiveScene(-1);
		}
		

		for (int i = 0; i < possessables.size(); i++)
		{
			possessables[i]->update(deltaTime);
		}

		for (int i = 0; i < movables.size(); i++)
		{
			movables[i]->update(deltaTime);
		}

		return;
	}
	


	if (Game::instance().getSpecialKey(1))
	{
		changePossession(PlayerType::Baba_p);
	}
	else if (Game::instance().getSpecialKey(2))
	{
		changePossession(PlayerType::Wall_p);
	}
	else if (Game::instance().getSpecialKey(3))
	{
		checkRules();
	}
	
	glm::ivec2 movementDirection = glm::ivec2(0, 0);
	if (!moving)
	{
		OutputDebugStringA("\nCAN MOVE");
		if (needToRecheckRules)
		{
			checkRules();
		}
		
		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
			if (currentTime - clickedTime > 100) {
				movementDirection = glm::ivec2(-1, 0);
				move = true;
				clickedTime = currentTime;
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
			if (currentTime - clickedTime > 100) {
				movementDirection = glm::ivec2(1, 0);
				move = true;
				clickedTime = currentTime;
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
			if (currentTime - clickedTime > 100) {
				movementDirection = glm::ivec2(0, -1);
				move = true;
				clickedTime = currentTime;
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
			if (currentTime - clickedTime > 100) {
				movementDirection = glm::ivec2(0, 1);
				move = true;
				clickedTime = currentTime;
			}
		}
	}
	
	if (Game::instance().getKey(8)) {
		Game::instance().changeActiveScene(0);
	}



	glm::ivec2 testPosition = movementDirection;
	if (move)
	{
		int ind;
		for (int i = 0; i < possessables.size(); i++)
		{
			
				if (glm::min(movementDirection.x, movementDirection.y) == -1)
				{
					ind = i;
				}
				else {
					ind = possessables.size() - 1 - i;
				}
				//walls[i].update(deltaTime);

			//if (possessables[ind]->getPlayerType() == possessed)
			if (possessables[ind]->isPossessed())
			{
				OutputDebugStringA("\nPos");
				if (!(possessables[ind]->isMoving()))
				{
					testPosition = movementDirection + possessables[ind]->getGridPos();
					bool success, outOfBounds;
					Entity* e;
					e = map->getEntity(testPosition, success, outOfBounds);

					//TODO REWORK PUSH


					if (outOfBounds)
					{
						continue;
					}
					else if (success && e != NULL)
					{
						OutputDebugStringA("\nCollision\n");
						//OutputDebugStringA(to_string(e->getEntityType()).c_str());
						if (e->getEntityType() == MoveBlock || e->getEntityType() == User)
						{
							OutputDebugStringA("\n\tPUSHING\n");
							//push(e, movementDirection);
							push(possessables[ind], movementDirection);
						}

					}
					else {
						possessables[ind]->move(movementDirection);
						map->moveEntity(possessables[ind]->getGridPos(), testPosition);
						//baba->setGridPos(testPosition);
					}

				}
			}
			
		}

		vector<Entity*> allEntities(possessables.begin(), possessables.end());
		allEntities.insert(allEntities.end(), movables.begin(), movables.end());

		for (int i = 0; i < allEntities.size(); i++)
		{
			if (allEntities[i]->isMoving())
			{
				for (int j = 0; j < allEntities.size(); j++)
				{
					if (!allEntities[j]->isMoving() && allEntities[i]->moveDestination == allEntities[j]->getGridPos())
					{
						OutputDebugStringA("\nSAME SPOT STOPPPPPPPPPPPPPPPPPPPPP");
						allEntities[i]->setMoving(false);
						allEntities[i]->moveDestination = glm::vec2(NULL, NULL);
					}
				}
			}

		}
	}


	


	for (int i = 0; i < possessables.size(); i++)
	{
		possessables[i]->update(deltaTime);
	}


	//Remove entities in the same place
	for (int i = 0; i < possessables.size(); i++)
	{
		for (int j = 0; j < possessables.size(); j++)
		{
			if (i != j && possessables[i]->getPlayerType() == possessables[j]->getPlayerType() && possessables[i]->getGridPos() == possessables[j]->getGridPos())
			{
				possessables.erase(possessables.begin() + j);
			}
		}
	}


	for (int i = 0; i < movables.size(); i++)
	{
		movables[i]->update(deltaTime);
	}

	
	


	
	
	
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
	bool moveThis = false;
	if (success && !outOfBounds)
	{
		OutputDebugStringA("Collision and may move");
		//string st = (" Entity type: ") + ("User" ? e->getEntityType() == User : "Movable");
		string st = to_string(e->isUser);
		OutputDebugStringA(("\n\t"+st).c_str());
		if (e->getEntityType() == User)
		{
			OutputDebugStringA("\nPushing a possessable");
			if (((Player*)e)->canWin() && entity->isPossessed())
			{
				moveThis = true;
				levelCompleted();
			}else if (e->stops())// && entity->getEntityType() == User && ((Player*)entity)->getPlayerType() == possessed)
			{
				OutputDebugStringA("\nCant friggin push man, it stops");
				return false;
			}
			else if (e->isPushable() && push(e, direction))
			{
				OutputDebugStringA("\nThas a pushable");
				moveThis = true;
			}
			/*else {
				moveThis = true;
			}*/
			else if (push(e, direction))
			{
				OutputDebugStringA("\nPush it to the fucking limit");
				moveThis = true;
			}
		}
		else if (e->getEntityType() == MoveBlock)
		{
			if (push(e, direction))
			{
				moveThis = true;
				needToRecheckRules = true;
			}
		}
		else if (e->getEntityType() == User)
		{

		}
	}
	else if (success && outOfBounds) {
		OutputDebugStringA("Collision and out of bounds");
		return false;
	}
	else if (!success){
		OutputDebugStringA("No collision, moves right away\n");
		moveThis = true;
		//entity->move(direction);
		//map->moveEntity(entity->getGridPos(), entity->getGridPos()+direction);
		////entity->setGridPos(entity->getGridPos() + direction);
		//string s = "Moves to X: " + to_string(entity->getGridPos().x) + "Y: " + to_string(entity->getGridPos().y) + "\n";
		//OutputDebugStringA(s.c_str());
		//return true;
	}

	if (moveThis)
	{
		switch (entity->getEntityType()) {
		case User:
			((Player*)entity)->move(direction);
			map->moveEntity(entity->getGridPos(), entity->getGridPos() + direction);
			break;
		case MoveBlock:
			((Movable*)entity)->move(direction);
			map->moveEntity(entity->getGridPos(), entity->getGridPos() + direction);
			break;
		default:
			break;
		}
		return true;

	}

	return false;
}

void Scene::levelCompleted()
{
	completed = true;
	Game::instance().levelCompletedEvent(this->levelId);
}

void Scene::renderLevelCompletedText() {
	levelCompletedText.render("Level Complete!", glm::vec2(glutGet(GLUT_WINDOW_HEIGHT /2), glutGet( GLUT_WINDOW_WIDTH / 2)), 20, glm::vec4(1, 1, 1, 1));
}


void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	projection = glm::ortho(0.f, float(glutGet(GLUT_WINDOW_WIDTH)), float(glutGet(GLUT_WINDOW_HEIGHT)), 0.f);
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	modelview = glm::translate(modelview, glm::vec3(abs(glutGet(GLUT_WINDOW_WIDTH)-SCREEN_WIDTH)/2, abs(glutGet(GLUT_WINDOW_HEIGHT) - SCREEN_HEIGHT)/2,0));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	//yaga->render();
	//interactable->render();

	map->render();

	for (int i = 0; i < possessables.size(); i++)
	{
		possessables[i]->render();
	}

	for (int i = 0; i < movables.size(); i++)
	{
		movables[i]->render();
	}

	if (completed)
	{
			renderLevelCompletedText();
	}

	/*for (int i = 0; i < walls.size(); i++)
	{
		walls[i]->render();
		
	}
	for (int i = 0; i < movables.size(); i++)
	{
		movables[i]->render();
	}
	baba->render();*/

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





