#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};

//string Player::getInfo()
//{
//	string info = "Player Type: " + to_string(type) + "\n -isStop = " + std::to_string(isStop) + "\n - bJumping = " + std::to_string(bJumping) + "\n - bMoving = " + std::to_string(bMoving);
//	return info;
//}



//void Player::changePlayerType(PlayerType type)
//{
//	this->type = type;
//	switch (type)
//	{
//	case Baba:
//		spriteFile = "images/bub.png";
//		spriteTCoords = glm::vec2(.25f, .25f);
//		break;
//	case Wall:
//		spriteFile = "images/Wall.png";
//		spriteTCoords = glm::vec2(1, 1);
//		break;
//	default:
//		break;
//	}
//
//}
//
//
//PlayerType Player::getPlayerType()
//{
//	return type;
//}

Player::Player()
{
	this->type = Baba_p;
}

Player::Player(PlayerType type)
{
	this->type = type;
	entityType = User;
	switch (type)
	{
	case Baba_p:
		spriteFile = "images/bub.png";
		spriteTCoords = glm::vec2(.25f, .25f);
		break;
	case Wall_p:
		spriteFile = "images/Wall.png";
		spriteTCoords = glm::vec2(1, 1);
		break;
	default:
		break;
	}
}
