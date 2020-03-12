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

string Player::getInfo()
{
	string info = "Player Type: " + to_string(type)+ "\n -isStop = " + std::to_string(isStop) + "\n - bJumping = " + std::to_string(bJumping) + "\n - bMoving = " + std::to_string(bMoving);
	return info;
}

void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, const glm::ivec2& spriteSize)
{
	test = false;
	fMoveFraction = 0.f;
	moveDestination = glm::ivec2(NULL, NULL);
	
	this->spriteSize = spriteSize;
	//spritesheet.loadFromFile("images/bub.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.loadFromFile(spriteFile, TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(this->spriteSize, spriteTCoords, &spritesheet, &shaderProgram);
	/*sprite->setNumberAnimations(4);
	
		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));
		
		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.25f, 0.f));
		
		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.25f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.25f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.5f));
		
	sprite->changeAnimation(0);*/
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}
bool Player::hasCollision()
{
	return isStop;
}

bool Player::isMoving()
{
	return bMoving;
}

void Player::setCollision(bool collision)
{
	isStop = collision;
}

glm::ivec2 Player::getGridPos()
{
	return gridPos;
}

void Player::setGridPos(glm::ivec2 pos)
{
	gridPos = pos;
}

void Player::changePlayerType(PlayerType type)
{
	this->type = type;
	switch (type)
	{
	case Baba:
		spriteFile = "images/bub.png";
		spriteTCoords = glm::vec2(.25f, .25f);
		break;
	case Wall:
		spriteFile = "images/Wall.png";
		spriteTCoords = glm::vec2(1, 1);
		break;
	default:
		break;
	}
	
}

void Player::update(int deltaTime)
{
	//glm::ivec2 spriteSize = glm::ivec2(32, 32);
	sprite->update(deltaTime);

	/*string msg = "\nTileSize"+to_string(map->getTileSize())+"Pos X"+to_string(posPlayer.x) + "Pos Y" + to_string(posPlayer.y)+"\nGridPos X" + to_string(gridPos.x) + "GridPos Y" + to_string(gridPos.y) + "\n";
	OutputDebugStringA(msg.c_str());*/
	/*msg = "Pos X" + to_string((posPlayer.x)/map->getTileSize()) + "Pos Y" + to_string((posPlayer.y) / map->getTileSize()) + "\n";
	OutputDebugStringA(msg.c_str());*/
	if (bMoving) {
		if (fMoveFraction < 1)
		{
			fMoveFraction += deltaTime * fMoveSpeed;

			posPlayer = glm::mix(posPlayer, moveDestination, fMoveFraction);
			sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
		}
		else {
			fMoveFraction = 0.f;
			moveDestination = glm::ivec2(NULL, NULL);
			bMoving = false;
		}
	}
	//else
	//{
	//	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	//	{
	//		moveDestination = posPlayer;
	//		moveDestination.x -= spriteSize.x;//map->getTileSize();
	//		gridPos.x -= 1;
	//		sprite->changeAnimation(STAND_LEFT);
	//		bMoving = true;
	//	}
	//	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	//	{
	//		moveDestination = posPlayer;
	//		moveDestination.x += spriteSize.x;//map->getTileSize();
	//		gridPos.x += 1;
	//		sprite->changeAnimation(STAND_RIGHT);
	//		bMoving = true;
	//	}
	//	else if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
	//		moveDestination = posPlayer;
	//		moveDestination.y -= spriteSize.x;//map->getTileSize();
	//		gridPos.y -= 1;
	//		sprite->changeAnimation(STAND_RIGHT);
	//		bMoving = true;
	//	}
	//	else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
	//		moveDestination = posPlayer;
	//		moveDestination.y += spriteSize.x;//map->getTileSize();
	//		gridPos.y += 1;
	//		sprite->changeAnimation(STAND_RIGHT);
	//		bMoving = true;
	//	}
	//	else if (Game::instance().getKey(32))
	//	{
	//		test = !test;
	//	}
	//	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	//}

}

void Player::render()
{
	sprite->render();
}

void Player::movePlayer(glm::ivec2 increment)
{
	bMoving = true;
	moveDestination = posPlayer + increment*spriteSize;
}

//void Player::setTileMap(TileMap *tileMap)
//{
//	map = tileMap;
//	//gridPos = glm::ivec2((posPlayer.x) / map->getTileSize(), (posPlayer.y) / map->getTileSize());
//}

void Player::setPosition(const glm::vec2 &pos, int tileSize)
{
	posPlayer = pos * glm::vec2(tileSize, tileSize);
	gridPos.x = posPlayer.x / 24;
	gridPos.y = posPlayer.y / 24;


	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::vec2 Player::getPosition()
{
	return posPlayer;
}

PlayerType Player::getPlayerType()
{
	return type;
}




