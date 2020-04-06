#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "Entity.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.

enum PlayerType { NoPl = 0,  Baba_p = 1, Flag_p = 2, Wall_p = 3 , Rock_p = 4, Water_p = 5};


class Player : public Entity
{
public:
	Player();
	Player(PlayerType type);
	//string getInfo();
	//void changePlayerType(PlayerType type);
	PlayerType getPlayerType();

	void move(glm::ivec2 newGridPos);
	bool canWin();
	void setWin(bool win);
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const glm::ivec2& spriteSize);


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
	int animStep = 0;
	bool isWin = false;
	bool isStop = false;
};


#endif // _PLAYER_INCLUDE


