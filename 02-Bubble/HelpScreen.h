#pragma once
#include "Scene.h"
#include "TexturedQuad.h"
#include "Text.h"
class HelpScreen
{
public:
	HelpScreen();
	~HelpScreen();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();
	void lerpSize(float& textSize, float baseSize, float targetSize, float& fraction, int deltaTime);
	ShaderProgram texProgram;
	glm::mat4 projection;
	Text backText;
	Text helpText;
	vector<string> lines;
	float currentTime;
	float clickedTime;
};

