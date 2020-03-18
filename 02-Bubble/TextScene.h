#pragma once
#include "Scene.h"
#include "TexturedQuad.h"
#include "Text.h"



class TextScene
{
public:
	TextScene();
	~TextScene();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();
	void lerpSize(float& textSize, float baseSize, float targetSize, float& fraction, int deltaTime);
	ShaderProgram texProgram;
	glm::mat4 projection;
	Text titleText;
	Text levelSelectText;
	Text helpText;
	Text quitText;

	int selection = 0;

	float defaultTextSize = 32;
	float selectedTextSize = 40;

	float titleTextSize;
	float levelSelectTextSize;
	float helpTextSize;
	float quitTextSize;

	float titleTextFraction;
	float levelSelectTextFraction;
	float helpTextFraction;
	float quitTextFraction;

	float growSpeed;
	float currentTime;
	float clickedTime;
};

