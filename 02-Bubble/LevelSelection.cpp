#include "LevelSelection.h"
#include <iostream>
#include <cmath>
#include "Game.h"
#include <glm/gtc/matrix_transform.hpp>


LevelSelection::LevelSelection()
{
}

LevelSelection::~LevelSelection()
{
}

void LevelSelection::initShaders()
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

void LevelSelection::init() {
	initShaders();

	//projection = glm::ortho(0.f, float(glutGet(GLUT_WINDOW_WIDTH) - 1), float(glutGet(GLUT_WINDOW_HEIGHT) - 1), 0.f);
	currentTime = 0.0f;
	clickedTime = 0.0f;

	if (!backText.init("fonts/OpenSans-Regular.ttf"))
		cout << "Could not load font!!!" << endl;

	


}

void LevelSelection::update(int deltaTime)
{
	currentTime += deltaTime;
	if (Game::instance().getKey(8)) {
		if (currentTime - clickedTime > 200)
		{
			Game::instance().changeActiveScene(0);
			clickedTime = currentTime;
		}
	}else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
		if (currentTime - clickedTime > 100)
		{
			selectedLevel--;
			clickedTime = currentTime;
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
		if (currentTime - clickedTime > 100)
		{
			selectedLevel++;
			clickedTime = currentTime;
		}
	}
	else if (Game::instance().getKey(13)) {
		if (currentTime - clickedTime > 100)
		{
			if (Game::instance().isLevelUnlocked(selectedLevel+1))
			{
				Game::instance().changeActiveScene(selectedLevel + 1);
				clickedTime = currentTime;
			}
		}
	}
	if (clickedTime == currentTime) Game::instance().soundEngine->play2D("sounds/menuSound.ogg");
	selectedLevel = (selectedLevel>=0) ? selectedLevel % levels : levels-1;
}

void drawSquare(double x1, double y1, double sidelength)
{
	double halfside = sidelength / 2;

	/*glColor3d(1, 1, 1);
	glBegin(GL_POLYGON);

	glVertex2d(x1 + halfside, y1 + halfside);
	glVertex2d(x1 + halfside, y1 - halfside);
	glVertex2d(x1 - halfside, y1 - halfside);
	glVertex2d(x1 - halfside, y1 + halfside);

	glEnd();*/

	glPointSize(5.0f);
	glColor3f(1, 1, 1);
	glBegin(GL_POINTS);
	glVertex3f(x1, y1, 0.f);
	glEnd();
}

void LevelSelection::render()
{
	glm::mat4 modelview;
	glClearColor(21 / 255., 24 / 255., 31 / 255., 0);
	texProgram.use();
	
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	
	
	glm::vec2 lvlPos = glm::vec2(100, 100);
	int offset = 20;
	//drawSquare((lvlPos.x + 10) + offset * selectedLevel, (lvlPos.y-10) + offset * selectedLevel, 20);

	for (int i = 0; i < levels; i++)
	{	
		if (!Game::instance().isLevelUnlocked(i+1))
		{
			backText.render(to_string(i + 1), lvlPos + glm::vec2(offset * i, 0), 20, glm::vec4(.3, .3, .3, 1));
		}
		else {
			if (!Game::instance().isLevelCompleted(i+1))
			{
				backText.render(to_string(i+1), lvlPos + glm::vec2(offset * i,0), 20, glm::vec4(1, 1, 1, 1));

			}
			else {
				backText.render(to_string(i + 1), lvlPos + glm::vec2(offset * i, 0), 20, glm::vec4(0, 1, 0, 1));
			}
		}
		if (i == selectedLevel)
		{
			drawSquare(1.25/2,-0.25, 2.5);

		}
	}


	float scaleFactor = (glm::sin(currentTime / 1000)/5 + 2) * 10;
	backText.render("[Backspace] <- Go Back to Main Menu", glm::vec2(0, scaleFactor), scaleFactor, glm::vec4(1, 1, 1, 1));

	

}



