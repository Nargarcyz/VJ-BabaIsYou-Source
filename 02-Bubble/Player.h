#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "Entity.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.

enum PlayerType { Baba = 0, Wall = 1 };


class Player : public Entity
{
public:
	string getInfo();
	void changePlayerType(PlayerType type);
	PlayerType getPlayerType();


	bool test;


private:

	/*string spriteFile = "images/bub.png";
	glm::vec2 spriteTCoords = glm::vec2(.25f, .25f);
	float fMoveFraction;
	glm::ivec2 moveDestination;
	glm::ivec2 tileMapDispl, posPlayer;
	glm::ivec2 spriteSize;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite* sprite;*/
	//TileMap* map;
	PlayerType type;

};


#endif // _PLAYER_INCLUDE


