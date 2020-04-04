#include "Entity.h"
#include "Game.h"
#include <irrKlang.h>
string Entity::getInfo()
{
	string info = "Entity:\n -isStop = " + std::to_string(isStop) + "\n - bJumping = " + std::to_string(bJumping) + "\n - bMoving = " + std::to_string(bMoving);
	return info;
}

void Entity::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const glm::ivec2& spriteSize)
{
	test = false;
	fMoveFraction = 0.f;
	moveDestination = glm::ivec2(NULL, NULL);

	int e = Baba;
	isUser = false;
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
bool Entity::stops()
{
	return isStop;
}

bool Entity::isMoving()
{
	return bMoving;
}

void Entity::setCollision(bool collision)
{
	isStop = collision;
}

void Entity::setStop(bool stop)
{
	isStop = stop;
}

glm::ivec2 Entity::getGridPos()
{
	return gridPos;
}

void Entity::setGridPos(glm::ivec2 pos)
{
	gridPos = pos;
}


//void Entity::move(glm::ivec2 increment)
//{
//	bMoving = true;
//	moveDestination = posPlayer + increment * spriteSize;
//}

void Entity::setMoving(bool mov) {
	bMoving = mov;
}

bool Entity::isPossessed()
{
	return isUser;
}

void Entity::setPossessed(bool pos)
{
	isUser = pos;
}

bool Entity::isPushable()
{
	return isPush;
}

void Entity::setPushable(bool push)
{
	isPush = push;
}

void Entity::move(glm::ivec2 newGridPos)
{
	bMoving = true;
	moveDestination = posPlayer + newGridPos * spriteSize;
}

void Entity::update(int deltaTime)
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
			gridPos = posPlayer/spriteSize;
		}
	}

	/*if (bMoving) {
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
	}*/







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

void Entity::render()
{
	sprite->render();

}

EntityType Entity::getEntityType()
{
	return entityType;
}

void Entity::setEntityType(EntityType type)
{
	entityType = type;
}



//void Entity::setTileMap(TileMap *tileMap)
//{
//	map = tileMap;
//	//gridPos = glm::ivec2((posPlayer.x) / map->getTileSize(), (posPlayer.y) / map->getTileSize());
//}

void Entity::setPosition(const glm::vec2& pos, int tileSize)
{
	posPlayer = pos * glm::vec2(tileSize, tileSize);
	gridPos.x = posPlayer.x / 24;
	gridPos.y = posPlayer.y / 24;


	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::vec2 Entity::getPosition()
{
	return posPlayer;
}