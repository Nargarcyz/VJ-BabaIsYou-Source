#pragma once
#include "Entity.h"
class CollisionMap
{
public:

	/*void addEntity(int x, int y, Entity* ent);

	Entity* getEntity(int x, int y, bool& success);

	CollisionMap(glm::ivec2 mapSize);*/

private:
	glm::vec2 mapSize;
	Entity** gridMap;
};

