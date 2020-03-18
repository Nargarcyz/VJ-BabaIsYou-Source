#include "TextScene.h"
#include <iostream>
#include <cmath>
#include "Game.h"
#include <glm/gtc/matrix_transform.hpp>
TextScene::TextScene()
{
}

TextScene::~TextScene()
{
}

void TextScene::init() {
	initShaders();

	projection = glm::ortho(0.f, float(glutGet(GLUT_WINDOW_WIDTH) - 1), float(glutGet(GLUT_WINDOW_HEIGHT) - 1), 0.f);
	currentTime = 0.0f;
	clickedTime = 0.0f;

	//Text titleText;
	if (!titleText.init("fonts/OpenSans-Regular.ttf"))
		cout << "Could not load font!!!" << endl;
	if (!levelSelectText.init("fonts/OpenSans-Regular.ttf"))
		cout << "Could not load font!!!" << endl;
	if (!helpText.init("fonts/OpenSans-Regular.ttf"))
		cout << "Could not load font!!!" << endl;
	if (!quitText.init("fonts/OpenSans-Regular.ttf"))
		cout << "Could not load font!!!" << endl;
	
	titleTextSize = defaultTextSize;
	levelSelectTextSize = defaultTextSize;
	helpTextSize = defaultTextSize;
	quitTextSize = defaultTextSize;

	titleTextFraction = 0;
	levelSelectTextFraction = 0;
	helpTextFraction = 0;
	quitTextFraction = 0;

	growSpeed = 0.001;

	
	
}

void TextScene::update(int deltaTime) {
	currentTime += deltaTime;


	if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
		if (currentTime-clickedTime > 200)
		{
			selection -= 1;
			clickedTime = currentTime;
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
		if (currentTime - clickedTime > 200)
		{
			selection += 1;
			clickedTime = currentTime;
		}
	}
	if (selection > 2) selection = 2;
	if (selection < 0) selection = 0;


	switch (selection)
	{
	case 0:

		lerpSize(levelSelectTextSize, defaultTextSize, selectedTextSize, levelSelectTextFraction, deltaTime);


		lerpSize(titleTextSize,selectedTextSize, defaultTextSize, titleTextFraction, deltaTime);
		lerpSize(helpTextSize, selectedTextSize, defaultTextSize, helpTextFraction, deltaTime);
		lerpSize(quitTextSize, selectedTextSize, defaultTextSize, quitTextFraction, deltaTime);


		break;
	case 1:
		//lerpSize(titleTextSize, selectedTextSize, titleTextFraction, deltaTime);
		lerpSize(helpTextSize, defaultTextSize, selectedTextSize, helpTextFraction, deltaTime);

		lerpSize(titleTextSize, selectedTextSize, defaultTextSize, titleTextFraction, deltaTime);
		lerpSize(levelSelectTextSize, selectedTextSize, defaultTextSize, levelSelectTextFraction, deltaTime);
		lerpSize(quitTextSize, selectedTextSize, defaultTextSize, quitTextFraction, deltaTime);


		break;
	case 2:
		//lerpSize(titleTextSize, selectedTextSize, titleTextFraction, deltaTime);
		lerpSize(quitTextSize, defaultTextSize, selectedTextSize, quitTextFraction, deltaTime);

		lerpSize(titleTextSize, selectedTextSize, defaultTextSize, titleTextFraction, deltaTime);
		lerpSize(levelSelectTextSize, selectedTextSize, defaultTextSize, levelSelectTextFraction, deltaTime);
		lerpSize(helpTextSize, selectedTextSize, defaultTextSize, helpTextFraction, deltaTime);
		break;
	default:
		break;
	}



	OutputDebugStringA(to_string(selection).c_str());
	//OutputDebugStringA(to_string(currentTime).c_str());
	OutputDebugStringA("\n");

}

void TextScene::lerpSize(float& textSize, float baseSize, float targetSize, float& fraction, int deltaTime){
	
	//float f = (float)textSize / (float)targetSize;
	//float f = (float)abs(textSize - baseSize) / (float)abs(targetSize - baseSize);
	//OutputDebugStringA("textSize : ");
	//OutputDebugStringA(to_string((float)abs(textSize)).c_str());
	//OutputDebugStringA("\n");
	//OutputDebugStringA("baseSize: ");
	//OutputDebugStringA(to_string((float)abs(baseSize)).c_str());
	//OutputDebugStringA("\n");
	//OutputDebugStringA("textSize - baseSize: ");
	//OutputDebugStringA(to_string((float)abs(textSize - baseSize)).c_str());
	//OutputDebugStringA("\n");
	//OutputDebugStringA("targetSize - baseSize: ");
	//OutputDebugStringA(to_string((float)abs(targetSize - baseSize)).c_str());
	//OutputDebugStringA("\n");


	OutputDebugStringA("Fraction before: ");
	OutputDebugStringA(to_string(fraction).c_str());
	OutputDebugStringA("\n");

	//if (f < 1)
	//{
	//	f += deltaTime * growSpeed*f;
	//	OutputDebugStringA("Fraction after: ");
	//	OutputDebugStringA(to_string(f).c_str());
	//	OutputDebugStringA("\n");
	//	textSize = glm::mix(textSize, targetSize, f);
	//	if (textSize > targetSize)
	//	{
	//		textSize = targetSize;
	//	}
	//	else if (textSize < baseSize)
	//	{
	//		textSize = baseSize;
	//	}
	//	//textSize = 40;
	//}

	OutputDebugStringA("textSize : ");
	OutputDebugStringA(to_string((float)abs(textSize)).c_str());
	OutputDebugStringA("\n");
	
	if (targetSize>baseSize && fraction < 1)
	{
		fraction += deltaTime * growSpeed;
		textSize = glm::mix(textSize, targetSize, fraction);
	}
	else if (targetSize < baseSize&& fraction > 0)
	{
		fraction -= deltaTime * growSpeed;
		textSize = glm::mix(textSize, targetSize, fraction);
	}
	/*else {
		fraction = 0.f;
	}*/
}

void TextScene::render()
{
	glm::mat4 modelview;


	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	/*modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);*/
	int separation = 10;
	glm::vec2 prevPos = glm::vec2(0, glutGet(GLUT_WINDOW_HEIGHT) / 4);
	titleText.render("BABA IS YOU CLONE", prevPos, titleTextSize, glm::vec4(1, 1, 1, 1));


	prevPos.y += titleTextSize + separation;
	levelSelectText.render("Select Level", prevPos, levelSelectTextSize, glm::vec4(1, 1, 1, 1));
	prevPos.y += levelSelectTextSize + separation;
	helpText.render("Help", prevPos, helpTextSize, glm::vec4(1, 1, 1, 1));
	prevPos.y += helpTextSize + separation;
	quitText.render("Quit Game", prevPos, quitTextSize, glm::vec4(1, 1, 1, 1));
}

void TextScene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}