#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "Entity.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.

enum PlayerType {Baba = 0, Wall = 1};


class Player: protected Entity
{
public:
	string getInfo();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, const glm::ivec2 &spriteSize);
	void changePlayerType(PlayerType type);
	void update(int deltaTime);
	void render();
	
	void movePlayer(glm::ivec2 increment);

	bool hasCollision();
	bool isMoving();

	void setCollision(bool collision);
	glm::ivec2 getGridPos();
	void setGridPos(glm::ivec2 pos);

	//void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos, int tileSize);
	glm::vec2 getPosition();
	PlayerType getPlayerType();


	bool test;
	
	
private:

	string spriteFile = "images/bub.png";
	glm::vec2 spriteTCoords = glm::vec2(.25f, .25f);
	float fMoveFraction;
	glm::ivec2 moveDestination;
	glm::ivec2 tileMapDispl, posPlayer;
	glm::ivec2 spriteSize;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite* sprite;
	//TileMap* map;
	PlayerType type;

};


#endif // _PLAYER_INCLUDE


