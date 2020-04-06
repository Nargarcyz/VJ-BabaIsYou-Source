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
	Scene(int levelId);
	~Scene();

	void switchPos(int key);
	void init(const string levelFile);
	bool createInstances(const string& levelFile);
	void applyRule(Objects obj, Properties prop);
	//void applyRule(vector<Objects>* objs, vector<Relations*> rel, vector<Properties*> prop);
	void applyRule(Objects obj, Relations rel, Properties prop);
	void checkRules();
	void changePossession(PlayerType newPlayer);
	void update(int deltaTime);
	void render();
	bool push(Entity* entity, glm::ivec2& direction);

	void levelCompleted();
	void renderLevelCompletedText();

private:
	void initShaders();
	vector<Movable*> getMovableLine(glm::vec2 startPos, glm::vec2 direction);

private:
	TileMap* map;
	Player* baba;
	Player* yaga;
	
	PlayerType possessed;
	
	vector<Player*> possessables;
	
	vector<Player*> walls;

	vector<Movable*> movables;
	ShaderProgram texProgram;
	float currentTime, clickedTime, completedTime;
	glm::mat4 projection;
	bool completed = false;
	bool noPossession = true;
	bool needToRecheckRules = false;
	int levelId;

	Text levelCompletedText;

};




#endif // _SCENE_INCLUDE

