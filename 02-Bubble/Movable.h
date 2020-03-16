#pragma once
#include "Entity.h"
enum Words {Object = 0, Relation = 1, Property = 2};
enum Objects{Baba = 1, Flag = 2, Wall = 3};
static const char* ObjectStrings[] = { "", "Baba", "Flag", "Wall" };
enum Relations{Is = 1, And = 2};
static const char* RelationStrings[] = { "", "Is", "And"};

class Movable :
	public Entity
{
public:
	Movable(Words word, int typeIndex);
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const glm::ivec2& spriteSize);
	

private:
	Words wordType;
	int movableTypeIndex;

};

