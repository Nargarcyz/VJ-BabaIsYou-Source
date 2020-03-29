#include "HelpScreen.h"
#include <iostream>
#include <cmath>
#include "Game.h"
#include <glm/gtc/matrix_transform.hpp>

struct LevelEntity {
	int number;
	glm::vec2 position;
};
HelpScreen::HelpScreen()
{
}

HelpScreen::~HelpScreen()
{
}

void HelpScreen::initShaders()
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

void HelpScreen::init() {
	initShaders();

	projection = glm::ortho(0.f, float(glutGet(GLUT_WINDOW_WIDTH) - 1), float(glutGet(GLUT_WINDOW_HEIGHT) - 1), 0.f);
	currentTime = 0.0f;
	clickedTime = 0.0f;

	if (!backText.init("fonts/OpenSans-Regular.ttf"))
		cout << "Could not load font!!!" << endl;
	if (!helpText.init("fonts/OpenSans-Regular.ttf"))
		cout << "Could not load font!!!" << endl;


	lines.push_back("This is a clone of the popular game Baba Is You.");
	lines.push_back("");
	lines.push_back("Controls:");
	lines.push_back("   Up, down, left, right = Move player");
	lines.push_back("");
	lines.push_back("How to play:");
	lines.push_back("   You must get the player to the goal of each level.");
	lines.push_back("   In order to do so, you may need to alter the attributes");
	lines.push_back("   of some of the objects in the level by pushing them.");
	lines.push_back("");
	lines.push_back("");
	lines.push_back("");
	lines.push_back("");
	lines.push_back("Credits:");
	lines.push_back("   Oscar Garcia Toledo, Email: oscar.garcia.toledo@est.fib.upc.edu");

}

void HelpScreen::update(int deltaTime)
{
	currentTime += deltaTime;
	if (Game::instance().getKey(8)) {
		if (currentTime - clickedTime > 200)
		{
			Game::instance().changeActiveScene(0);
			clickedTime = currentTime;
		}
	}
}

void HelpScreen::render()
{
	glm::mat4 modelview;


	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	/*modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);*/

	float scaleFactor = (glm::sin(currentTime / 1000) / 5 + 2) * 10;
	backText.render("[Backspace] <- Go Back to Main Menu", glm::vec2(0, scaleFactor), scaleFactor, glm::vec4(1, 1, 1, 1));

	

	glm::vec2 prevLinePos = glm::vec2(10, glutGet(GLUT_WINDOW_HEIGHT) / 4);
	for (int i = 0; i < lines.size(); i++)
	{
		helpText.render(lines[i], prevLinePos, 20, glm::vec4(1, 1, 1, 1));
		prevLinePos.y += 20;
	}

}

