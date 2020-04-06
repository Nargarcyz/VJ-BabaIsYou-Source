#pragma once
#include "Scene.h"
#include "TexturedQuad.h"
#include "Text.h"
class LevelSelection
{
public:
	LevelSelection();
	~LevelSelection();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();
	void lerpSize(float& textSize, float baseSize, float targetSize, float& fraction, int deltaTime);
	ShaderProgram texProgram;
	glm::mat4 projection;
	Text backText;
	int levels = 5;
	int selectedLevel = 0;
	float currentTime;
	float clickedTime;
	
};



