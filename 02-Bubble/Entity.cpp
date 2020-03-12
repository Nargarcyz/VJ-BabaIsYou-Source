#include "Entity.h"

string Entity::getInfo()
{
	string info = "Entity:\n -isStop = " + std::to_string(isStop) + "\n - bJumping = " + std::to_string(bJumping) + "\n - bMoving = " + std::to_string(bMoving);
	return info;
}
