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


//Deprecated. Used to extract entity data from individual lines in the level file from line 30 onwards.
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
				//OutputDebugStringA(("\n" + type + " " +  ObjectStrings[index] + " at " + to_string(pos.x) + " " + to_string(pos.y)).c_str());
					
			}
			else if (type == "Relation")
			{
				iType = 1;
				const char** it = std::find(std::begin(RelationStrings), std::end(RelationStrings), name);
				if (it != std::end(RelationStrings))
				{
					index = std::distance(std::begin(RelationStrings), it);
				}
				//OutputDebugStringA(("\n" + type + " " + RelationStrings[index] + " at " + to_string(pos.x) + " " + to_string(pos.y)).c_str());
			}
			else if (type == "Property")
			{
				iType = 2;
				const char** it = std::find(std::begin(PropertyStrings), std::end(PropertyStrings), name);
				if (it != std::end(PropertyStrings))
				{
					index = std::distance(std::begin(PropertyStrings),it);
				}
				//OutputDebugStringA(("\n" + type + " " + PropertyStrings[index] + " at " + to_string(pos.x) + " " + to_string(pos.y)).c_str());
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
			/*OutputDebugStringA(("\n" + name).c_str());
			OutputDebugStringA(("\n" + to_string(pos.x)).c_str());
			OutputDebugStringA(("\n" + to_string(pos.y)).c_str());*/
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

//Gets entity locations directly from the tilemap and creates the instances as required
void Scene::extractEntities(){
	int* tilemap = map->extractMap();
	glm::vec2 mapSize = map->getMapSize();
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			int tile = tilemap[j * (int)mapSize.x + i];
			switch (tile)
			{
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:{
				Player* ent = new Player(static_cast<PlayerType>(tile - 1));
				ent->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(24, 24));
				ent->setPosition(glm::vec2(i, j), map->getTileSize());
				map->addEntity(i, j, (Entity*)ent);
				if (static_cast<PlayerType>(tile - 1) == Baba_p)
				{
					possessables.insert(possessables.begin(), ent);
				}else
					possessables.push_back(ent);
				break;
			}
			case 7:
			case 8:
			case 9:
				continue;
				
			default:
				if (tile != 0 && tile != 1 )//&& tile != 7 && tile != 18 && tile != 19)
				{
	
					int iType, index;
					switch (tile/5)
					{
					case 2:
						iType = Property;
						break;
					case 3:
						iType = Object;
						break;
					case 4:
						iType = Relation;
						break;
					default:
						break;
					}
					index = (tile % 5) + 1;
					Movable* mov = new Movable((Words)iType, index);
					mov->init(glm::ivec2(0, 0), texProgram, glm::ivec2(24, 24));
					mov->setPosition(glm::vec2(i,j), map->getTileSize());
					map->addEntity(i, j, (Entity*)mov);
					movables.push_back(mov);
				}
				
				break;
			}
			
		}
	}
}

//Given an object and a property, applies the "Is" relation
void Scene::applyRule(Objects obj, Properties prop)
{
	for (int i = 0; i < possessables.size(); i++)
	{
		if (possessables[i]->getPlayerType() == obj)
		{
			switch (prop)
			{
			case You:
				possessables[i]->setPossessed(true);
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
				break;
			case Sink:
				possessables[i]->setSink(true);
				map->addEntity(possessables[i]->getGridPos().x, possessables[i]->getGridPos().y, possessables[i]);
				break;
			default:
				break;
			}
		}
	}
}


// For each movable object in the scene, gets the adjacent movable entities to the left and directly downwards, and if they
// are parseable lines, rules get created and applied.
void Scene::checkRules() {
	for (int i = 0; i < possessables.size(); i++) {
		map->removeEntity(possessables[i]->getGridPos());
	}
	for (int i = 0; i < movables.size(); i++)
	{
		map->addEntity(movables[i]->getGridPos().x, movables[i]->getGridPos().y, movables[i]);
	}
	for (int i = 0; i < possessables.size(); i++)
	{
		((Player*)possessables[i])->setStop(false);
		((Player*)possessables[i])->setWin(false);
		((Player*)possessables[i])->setPushable(false);
		((Player*)possessables[i])->setSink(false);
		possessables[i]->setPossessed(false);
	}

	set< pair<Objects, Properties> > rulesToProcess;
	Relations rel;
	
	OutputDebugStringA("\t---RECHECKING RULES ---\n");
	
	for (int i = 0; i < movables.size(); i++)
	{
		if (movables[i]->getWordType() == Object)
		{
			//Checking to the left of the entity
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

			//Checking down the entity
			OutputDebugStringA("\nChecking down");
			line = getMovableLine(movables[i]->getGridPos(), glm::vec2(0, 1));
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
					OutputDebugStringA(("\n" + (string)ObjectStrings[line[object]->getTypeIndex()] + " will have attributes:").c_str());
					for (int property = separator + 1; property < line.size(); property = property + 2)
					{
						OutputDebugStringA(("," + (string)PropertyStrings[line[property]->getTypeIndex()]).c_str());
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
}

// Returns a valid rule line from a start position in a certain direction. Can return empty in case no valid rule is found.
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
					OutputDebugStringA("\nProperty at right position");
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
	//createInstances(levelFile);
	extractEntities();

	// Reverses the possessables vector in an attempt to ensure that baba gets rendered on top of anything else
	std::reverse(possessables.begin(), possessables.end());
	checkRules();
	
	currentTime = 0.0f;
	clickedTime = currentTime;
}





void Scene::update(int deltaTime)
{
	currentTime += deltaTime;

	bool moving = false;
	bool move = false;
	noPossession = true;

	// UPDATE PHASE: Updates certain information based on the new game state, such as:
	// - Checking if there is any possessable or movable entity moving, in order to halt updates that may break gameplay
	// - Checking if there is any possessed entity in case of a rule change or an gameplay event, while also erasing the entities that need to be destroyed.
	for (int i = 0; i < possessables.size(); i++)
	{
		if (possessables[i]->isMoving())
		{
			moving = true;
			break;
		}
		if (possessables[i]->isPossessed())
		{
			noPossession = false;
		}
	}

	for (int i = 0; i < movables.size(); i++)
	{
		if (movables[i]->isMoving())
		{
			moving = true;
			break;
		}
	}

	// Destroys entities that have been marked for destruction due to ingame events
	for (int i = 0; i < possessables.size(); i++)
	{
		if (possessables[i]->isDestroyed)
		{
			map->removeEntity(possessables[i]->getGridPos());
			possessables.erase(possessables.begin() + i);
		}
		else
			possessables[i]->update(deltaTime);
	}


	// Removes entities that overlap in the same tile location
	for (int i = 0; i < possessables.size(); i++)
	{
		for (int j = 0; j < possessables.size(); j++)
		{
			if (i != j && possessables[i]->getPlayerType() == possessables[j]->getPlayerType() && possessables[i]->getGridPos() == possessables[j]->getGridPos())
			{
				map->removeEntity(possessables[i]->getGridPos());
				possessables.erase(possessables.begin() + j);
			}
		}
	}

	// Also destroys movable entities that have been marked for destruction
	for (int i = 0; i < movables.size(); i++)
	{
		if (movables[i]->isDestroyed)
		{
			map->removeEntity(possessables[i]->getGridPos());
			movables.erase(movables.begin() + i);
		}
		else
			movables[i]->update(deltaTime);
	}

	// If any entity is moving, don't process further to ensure stability
	if (moving) return;

	// LEVEL FAILED: This conditional ensures that the level reaches a failed state, by disabling user interaction of the level
	// and playing the fail music
	if (noPossession)
	{

		if (Game::instance().backgroundMusic->getSoundSource() != Game::instance().deadMusic)
		{
			Game::instance().soundEngine->stopAllSoundsOfSoundSource(Game::instance().levelMusic);
			Game::instance().backgroundMusic = Game::instance().soundEngine->play2D(Game::instance().deadMusic, true, false, true);
		}

		if (Game::instance().getKey(8)) {
			Game::instance().changeActiveScene(-1);
		}
		return;
	}

	// LEVEL COMPLETED: This conditional ensures that the level reaches a win state, by disabling user interaction of the level
	// and playing the win music, as well as unlocking the next level
	if (!moving && completed)
	{
		if (Game::instance().backgroundMusic->getSoundSource() != Game::instance().winMusic)
		{
			Game::instance().soundEngine->stopAllSounds();
			Game::instance().backgroundMusic = Game::instance().soundEngine->play2D(Game::instance().winMusic, false, false, true);
		}

		if (currentTime - completedTime > 3000)
		{
			Game::instance().changeActiveScene(-1);
		}

		if (Game::instance().getKey(13)) {
			Game::instance().changeActiveScene(-1);
		}
	

		return;
	}

	// GAMEPLAY UPDATE: This section processes user input and changes entity states as required
	glm::ivec2 movementDirection = glm::ivec2(0, 0);
	if (!moving)
	{
		
		
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

		if (needToRecheckRules)
		{
			checkRules();
		}
	}
	
	// Return to level select key
	if (Game::instance().getKey(8)) {
		if (currentTime - clickedTime > 100) 
			Game::instance().changeActiveScene(-1);
	}


	// ACTUAL GAMEPLAY UPDATE: If an user input was recieved, process it
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

			// If the possessable is possessed and not moving, it can be updated
			if (possessables[ind]->isPossessed())
			{
				if (!(possessables[ind]->isMoving()))
				{

					// We check for an entity presence in the grid position
					testPosition = movementDirection + possessables[ind]->getGridPos();
					bool success, outOfBounds;
					Entity* e;
					e = map->getEntity(testPosition, success, outOfBounds);

					// If the position is out of bounds, ignore the input for this possessable
					if (outOfBounds)
					{
						continue;
					}
					// If the position is not out of bounds and a non null entity was found, proceed
					else if (success && e != NULL)
					{
						if (e->getEntityType() == MoveBlock || e->getEntityType() == User)
						{
							push(possessables[ind], movementDirection);
						}

					}
					// If nothing was found, just move the possessable
					else {
						possessables[ind]->move(movementDirection);
						map->moveEntity(possessables[ind]->getGridPos(), testPosition);
					}

				}
			}
			
		}


		vector<Entity*> allEntities(possessables.begin(), possessables.end());
		allEntities.insert(allEntities.end(), movables.begin(), movables.end());

		for (int i = 0; i < allEntities.size(); i++)
		{
			if (!allEntities[i]->isMoving())
			{
				for (int j = 0; j < allEntities.size(); j++)
				{
					if (!allEntities[j]->isMoving() && allEntities[i]->moveDestination == allEntities[j]->getGridPos())
					{
						allEntities[i]->setMoving(false);
						allEntities[i]->moveDestination = glm::vec2(NULL, NULL);
					}
				}
			}

		}
	}


	


	

	
	


	
	
	
}

// Function that, given an entity and a direction, tries to push it, checking for collisions, on that direction.
bool Scene::push(Entity* entity, glm::ivec2& direction)
{	
	bool success, outOfBounds;
	Entity* e = map->getEntity(entity->getGridPos()+ direction, success, outOfBounds);
	bool moveThis = false;
	// If the collision check was successful
	if (success && !outOfBounds)
	{
		// If the entity obtained is a possessable entity
		if (e->getEntityType() == User)
		{
			OutputDebugStringA("\nUser");
			if (((Player*)e)->canWin() && entity->isPossessed())
			{
				moveThis = true;
				levelCompleted();
			}			
			else if (e->stops())
			{
				return false;
			}
			else if (e->canSink())
			{
				OutputDebugStringA("\nCollided sinks");
				entity->isDestroyed = true;
				e->isDestroyed = true;
				Game::instance().soundEngine->play2D(Game::instance().sinkSound, false, false, false);
				return false;
			}
			else if (entity->canSink())
			{
				OutputDebugStringA("\Collider sinks");
				entity->isDestroyed = true;
				e->isDestroyed = true;
				Game::instance().soundEngine->play2D(Game::instance().sinkSound, false, false, false);
				return false;
			}
			else if (e->isPushable() && push(e, direction))
			{
				moveThis = true;
			}

		}
		// If the entity obtained is a movable block
		else if (e->getEntityType() == MoveBlock)
		{
			if (e->canSink())
			{
				entity->isDestroyed = true;
				return false;
			}
			else if (push(e, direction))
			{
				moveThis = true;
				needToRecheckRules = true;
			}
		}
	}
	// If out of bounds, dont move
	else if (success && outOfBounds) {
		return false;
	}
	// If nothing was detected, move
	else if (!success){
		moveThis = true;
	}

	// Movement processing
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

// This function handles the event of completing a level
void Scene::levelCompleted()
{
	completed = true;
	completedTime = currentTime;
	Game::instance().levelCompletedEvent(this->levelId);
}

// This function renders the completed level text
void Scene::renderLevelCompletedText() {
	string st = "Level Complete!";
	levelCompletedText.render(st, glm::vec2((glutGet(GLUT_WINDOW_WIDTH) / 2)  - 200, glutGet(GLUT_WINDOW_HEIGHT) / 2), 50, glm::vec4(1, 1, 1, 1));
}


void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	projection = glm::ortho(0.f, float(glutGet(GLUT_WINDOW_WIDTH)), float(glutGet(GLUT_WINDOW_HEIGHT)), 0.f);
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	modelview = glm::translate(modelview, glm::vec3(abs(glutGet(GLUT_WINDOW_WIDTH)/2)-SCREEN_WIDTH/2, abs(glutGet(GLUT_WINDOW_HEIGHT)/2)-SCREEN_HEIGHT/2,0));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	map->render();

	for (int i = 0; i < possessables.size(); i++)
	{
		possessables[i]->render();
	}

	for (int i = 0; i < movables.size(); i++)
	{
		movables[i]->render();
	}

	modelview = glm::mat4(1.0f);
	if (completed)
	{
			renderLevelCompletedText();
	}

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





