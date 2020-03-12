#include "CollisionMap.h"

//void CollisionMap::addEntity(int x, int y, Entity* ent)
//{
//	gridMap[x * mapSize.x + y] = ent;
//}
//
//Entity* CollisionMap::getEntity(int x, int y, bool& success) {
//	if (gridMap[x * mapSize.x + y] == new Entity())
//	{
//		success = false;
//		return NULL;
//	}
//	else {
//		success = true;
//		return gridMap[x * mapSize.x + y];
//	}
//}
//
//CollisionMap::CollisionMap(glm::ivec2 mapSize) {
//	this->mapSize = mapSize;
//	gridMap = new Entity*[mapSize.x * mapSize.y];
//	for (int i = 0; i < mapSize.x; i++)
//	{
//		for (int j = 0; j < mapSize.y; j++)
//		{
//			gridMap[j * mapSize.x + i] = new Entity();
//		}
//	}
//}