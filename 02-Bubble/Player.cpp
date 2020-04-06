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
	STAND_RIGHT = 0, STAND_UP = 5, STAND_LEFT = 10, STAND_DOWN = 15
};

Player::Player()
{
	this->type = Baba_p;
}

Player::Player(PlayerType type)
{
	this->type = type;
	entityType = (EntityType)0;
	switch (type)
	{
	case Baba_p:
		spriteFile = "images/baba.png";
		spriteTCoords = glm::vec2(.25f, .25f);
		break;
	case Wall_p:
		spriteFile = "images/wall.png";
		break;
	case Flag_p:
		spriteFile = "images/flag.png";
		break;
	case Rock_p:
		spriteFile = "images/rock.png";
		break;
	case Water_p:
		spriteFile = "images/water.png";
		break;
	default:
		break;
	}
}

PlayerType Player::getPlayerType()
{
	return type;
}


void Player::move(glm::ivec2 newGridPos)
{
	bMoving = true;
	if (type==Baba_p)
	{
		animStep = (animStep + 1) % 5;

		if (newGridPos.x != 0 && newGridPos.x < 0)
		{
			OutputDebugStringA("LEFT");
			sprite->changeAnimation(STAND_LEFT + animStep);
		}
		else if (newGridPos.x != 0 && newGridPos.x > 0)
		{
			OutputDebugStringA("RIGHT");
			sprite->changeAnimation(STAND_RIGHT + animStep);
		}
		else if (newGridPos.y != 0 && newGridPos.y > 0)
		{
			OutputDebugStringA("DOWN");
			sprite->changeAnimation(STAND_DOWN + animStep);
		}
		else if (newGridPos.y != 0 && newGridPos.y < 0)
		{
			OutputDebugStringA("UP");
			sprite->changeAnimation(STAND_UP + animStep);
		}
	}
	


	Game::instance().soundEngine->play2D(Game::instance().moveSound,false,false,false);
	moveDestination = posPlayer + newGridPos * spriteSize;

	

}

bool Player::canWin()
{
	return isWin;
}

void Player::setWin(bool win)
{
	isWin = win;
}


void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const glm::ivec2& spriteSize)
{
	test = false;
	fMoveFraction = 0.f;
	moveDestination = glm::ivec2(NULL, NULL);

	this->spriteSize = spriteSize;

	entityType = (EntityType)0;
	isUser = true;

	switch (type)
	{
	case Baba_p: {

		spritesheet.loadFromFile(spriteFile, TEXTURE_PIXEL_FORMAT_RGBA);
		sprite = Sprite::createSprite(this->spriteSize, glm::vec2(.05, 0.33), &spritesheet, &shaderProgram);
		int animWidth = 20;
		int animHeight = 3;


		sprite->setNumberAnimations(animWidth);

		for (int i = 0; i < animWidth; i++)
		{
			sprite->setAnimationSpeed(i, 8);
			for (int j = 0; j < animHeight; j++)
			{
				sprite->addKeyframe(i, glm::vec2(i / (float)animWidth, j / (float)animHeight));
			}
		}
		sprite->changeAnimation(0);

		tileMapDispl = tileMapPos;
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

		break;
	}
	default:
		spritesheet.loadFromFile(spriteFile, TEXTURE_PIXEL_FORMAT_RGBA);
		sprite = Sprite::createSprite(this->spriteSize, glm::vec2(1, 1 / 3.), &spritesheet, &shaderProgram);
		sprite->setNumberAnimations(1);
		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, glm::vec2(0., 0.));
		sprite->addKeyframe(0, glm::vec2(0, 1 / 3.));
		sprite->addKeyframe(0, glm::vec2(0, 2 / 3.));
		sprite->changeAnimation(0);

		tileMapDispl = tileMapPos;
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

		break;
	}

}