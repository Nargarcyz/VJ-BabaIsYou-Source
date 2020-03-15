#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE



#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"




// Scene contains all the entities of our game.
// It is responsible for updating and render them.

enum class Possessable { Baba = 0, Wall = 1 };

class Scene
{

public:
	Scene();
	~Scene();

	void switchPos(int key);
	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:
	TileMap* map;
	Player* baba;
	Player* yaga;
	Possessable possessed;
	vector<Player*> walls;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

};




#endif // _SCENE_INCLUDE

