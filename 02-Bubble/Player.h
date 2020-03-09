#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"

// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, const glm::ivec2 &spriteSize);
	void changeSprite(string file, glm::vec2 coords);
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);


	bool test;
	
	
private:
	bool bJumping;
	bool bMoving;
	string spriteFile = "images/bub.png";
	glm::vec2 spriteTCoords = glm::vec2(.25f, .25f);
	float fMoveSpeed;
	float fMoveFraction;
	glm::ivec2 moveDestination;
	glm::ivec2 tileMapDispl, posPlayer;
	glm::ivec2 spriteSize;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

};


#endif // _PLAYER_INCLUDE


