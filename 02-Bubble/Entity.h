#ifndef _ENTITY_INCLUDE
#define _ENTITY_INCLUDE

#include <string>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using std::string;


class Entity
{
public:

	string getInfo();

	bool isStop = false;
	float fMoveSpeed = 0.01f;
	glm::ivec2 gridPos;
	bool bJumping = false;
	bool bMoving = false;

	

	


};

#endif

