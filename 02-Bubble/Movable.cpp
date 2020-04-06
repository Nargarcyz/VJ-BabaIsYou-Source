#include "Movable.h"

Movable::Movable(Words word, int typeIndex)
{
	entityType = MoveBlock;
	wordType = word;
	movableTypeIndex = typeIndex;
	isUser = false;
}

Words Movable::getWordType()
{
	return wordType;
}

int Movable::getTypeIndex()
{
	return movableTypeIndex;
}

void Movable::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const glm::ivec2& spriteSize)
{
	fMoveFraction = 0.f;
	moveDestination = glm::ivec2(NULL, NULL);
	
	this->spriteSize = spriteSize;
	string file = "images/";
	switch (wordType)
	{
	case Object:
		file = file + ObjectStrings[movableTypeIndex] + ".png";
		break;
	case Relation:
		file = file + RelationStrings[movableTypeIndex] + ".png";
		break;
	case Property:
		file = file + PropertyStrings[movableTypeIndex] + ".png";
		break;
	default:
		break;
	}
	spritesheet.loadFromFile(file, TEXTURE_PIXEL_FORMAT_RGBA);

	sprite = Sprite::createSprite(this->spriteSize, glm::vec2(1, 1 / 3.), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 8);
	sprite->addKeyframe(0, glm::vec2(0., 0.));
	sprite->addKeyframe(0, glm::vec2(0, 1 / 3.));
	sprite->addKeyframe(0, glm::vec2(0, 2 / 3.));
	sprite->changeAnimation(0);

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}
