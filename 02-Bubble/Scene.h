#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE



#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Text.h"




// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void switchPos(int key);
	void init();
	bool createInstances(const string& levelFile);
	void changePossession(PlayerType newPlayer);
	void update(int deltaTime);
	void render();
	bool push(Entity* entity, glm::ivec2& direction);

private:
	void initShaders();

private:
	TileMap* map;
	Player* baba;
	Player* yaga;
	
	PlayerType possessed;
	
	vector<Player*> possessables;
	
	vector<Player*> walls;

	vector<Movable*> movables;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	Text text;
	bool completed;

};




#endif // _SCENE_INCLUDE

