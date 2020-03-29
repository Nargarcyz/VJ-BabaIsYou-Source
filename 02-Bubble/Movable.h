#pragma once
#include "Entity.h"
enum Words {Object = 0, Relation = 1, Property = 2};
enum Objects{Baba = 1, Flag = 2, Wall = 3};
static const char* ObjectStrings[] = { "", "BabaB", "Flag", "Wall" };
enum Relations{Is = 1, And = 2};
static const char* RelationStrings[] = { "", "Is", "And"};
enum Properties {You = 1, Stop = 2, Win = 3};
static const char* PropertyStrings[] = { "", "You", "Stop","Win"};

class Movable :
	public Entity
{
public:
	Movable(Words word, int typeIndex);
	Words getWordType();
	int getTypeIndex();
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const glm::ivec2& spriteSize);
	

private:
	Words wordType;
	int movableTypeIndex;

};

