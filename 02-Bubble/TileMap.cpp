#include "TileMap.h"


using namespace std;


TileMap* TileMap::createTileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	TileMap* map = new TileMap(levelFile, minCoords, program);

	return map;
}


TileMap::TileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
}

TileMap::~TileMap()
{
	if (map != NULL) {
		delete map;
	}
	if (gridMap != NULL) {
		delete gridMap;
	}

}




void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

void TileMap::addEntity(int x, int y, Entity* ent)
{
	gridMap[y * gridMapSize.x + x] = ent;
}
void TileMap::moveEntity(glm::ivec2 src, glm::ivec2 dest) {
	gridMap[dest.x + gridMapSize.x * dest.y] = gridMap[src.x + gridMapSize.x * src.y];
	gridMap[src.x + gridMapSize.x * src.y] = NULL;
}
void TileMap::removeEntity(glm::ivec2 src) {
	gridMap[src.x + gridMapSize.x * src.y] = NULL;
}

Entity* TileMap::getEntity(glm::ivec2 testPosition, bool& success, bool& outOfBounds) {
	//string s = typeid(gridMap[x * mapSize.x + y]).name();
	int x = testPosition.x;
	int y = testPosition.y;
	string s = "\nChecking X: " + to_string(x) + "Y: " + to_string(y);
	OutputDebugStringA(s.c_str());
	/*s = "\nMax X: " + to_string(gridMapSize.x) + " Max Y : " + to_string(gridMapSize.y);
	OutputDebugStringA(s.c_str());*/

	//if (x<0 || x>mapSize.x || y<0 || y>mapSize.y)
	outOfBounds = false;
	//if (x<0 || (x>(gridMapSize.x -(gridMapSize.x % tileSize)) / tileSize) || y<0 || (y > (gridMapSize.y-tileSize) / tileSize))
	/*if (x < 0)
	{
		OutputDebugStringA("\nOut left");
	}
	else if (x > (gridMapSize.x - 1))
	{
		OutputDebugStringA("\nOut right");
	}
	else if (y < 0)
	{
		OutputDebugStringA("\nOut up");
	}
	else if (y > (gridMapSize.y - 1))
	{
		OutputDebugStringA("\nOut down");
	}*/
	if (x < 0 || (x > (gridMapSize.x-1)) || y < 0 || (y > (gridMapSize.y-1)))
	{
		success = true;
		outOfBounds = true;
		//OutputDebugStringA("\nOut of bounds");
		return NULL;
	}else if (gridMap[x + gridMapSize.x * y] == NULL)
	{
		//OutputDebugStringA("\nNo entity detected");
		success = false;
		return NULL;
	} else if (gridMap[x + gridMapSize.x * y] != NULL) {
		/*OutputDebugStringA("\nEntity detected at");
		OutputDebugStringA(to_string(x).c_str());
		OutputDebugStringA(",");
		OutputDebugStringA(to_string(y).c_str());
		OutputDebugStringA("\n");
		OutputDebugStringA("\nEntity data: ");
		OutputDebugStringA(to_string(gridMap[y * gridMapSize.x + x]->getGridPos().x).c_str());
		OutputDebugStringA(",");
		OutputDebugStringA(to_string(gridMap[y * gridMapSize.x + x]->getGridPos().y).c_str());
		OutputDebugStringA("\n");*/
		success = true;
		return gridMap[x + gridMapSize.x * y];
	}
}

bool TileMap::loadLevel(const string& levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;

	fin.open(levelFile.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	if (line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);

	map = new int[mapSize.x * mapSize.y];
	//gridMap = new Entity*[mapSize.x * mapSize.y];
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{

			fin.get(tile);
			if (tile == ' ')
				map[j * mapSize.x + i] = 0;
			else
				map[j * mapSize.x + i] = tile - int('0');
		}
		fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}


	gridMapSize = glm::ivec2(floor(glutGet(GLUT_WINDOW_WIDTH) / tileSize), floor(glutGet(GLUT_WINDOW_HEIGHT) / tileSize));
	//gridMapSize = glm::ivec2(21,20);
	//gridMap = new Entity * [floor(glutGet(GLUT_WINDOW_WIDTH) / tileSize) * floor(glutGet(GLUT_WINDOW_HEIGHT) / tileSize)]{ NULL };
	gridMap = new Entity * [gridMapSize.x * gridMapSize.y]{ NULL };


	OutputDebugStringA("\TILEMAP SIZE");
	OutputDebugStringA(to_string(gridMapSize.x).c_str());
	OutputDebugStringA(",");
	OutputDebugStringA(to_string(gridMapSize.y).c_str());
	/*for (int i = 0; i < gridMapSize.x * gridMapSize.y; i++)
	{
		gridMap[i] = NULL;
	}*/
	/*gridMap = new Entity * [mapSize.x * mapSize.y]{ NULL };*/
	/*for (int j = 0; j < glutGet(GLUT_WINDOW_HEIGHT) / tileSize; j++)
	{
		for (int i = 0; i < glutGet(GLUT_WINDOW_WIDTH) / tileSize; i++)
		{
			gridMap[j * mapSize.x + i] = NULL;
		}
	}*/
	/*for (int i = 0; i < sizeof(gridMap); i++)
	{
		gridMap[i] = NULL;
	}*/
	fin.close();

	return true;
}

void TileMap::render() const
{
	glClearColor(0.03, 0.03, 0.03, 0);
	//glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
	glDisable(GL_TEXTURE_2D);

	/*for (int i = 0; i < gridMapSize.x; i++)
	{
		for (int j = 0; j < gridMapSize.y; j++) {
			if (gridMap[j * gridMapSize.x + i] == NULL)
			{
				glPointSize(5.0f);
				glColor3f(255, 0, 0);
				glBegin(GL_POINTS);
				glVertex3f(i * tileSize + tileSize / 2, j * tileSize + tileSize / 2, 0.f);
				glEnd();
			}

		}
	}*/

	/*for (int i = 0; i < floor(glutGet(GLUT_WINDOW_WIDTH) / tileSize) * floor(glutGet(GLUT_WINDOW_HEIGHT) / tileSize); i++)
	{
		if (gridMap[i] != NULL)
		{
			glClearColor(1.0, 1.0, 1.0, 0.0);
			glColor3f(1, 0, 0);
			glPointSize(5.0f);
			glBegin(GL_POINTS);
			glVertex3f(gridMap[i]->getGridPos().x*tileSize, gridMap[i]->getGridPos().y* tileSize, 0.f);

			if (gridMap[i]->getEntityType() == EntityType::User && ((Player*)gridMap[i])->getPlayerType() == PlayerType::Baba_p)
			{
				OutputDebugStringA("\nEntity detected at");
				OutputDebugStringA(to_string(gridMap[i]->getGridPos().x).c_str());
				OutputDebugStringA(",");
				OutputDebugStringA(to_string(gridMap[i]->getGridPos().y).c_str());
				OutputDebugStringA("\n");
			}

			glEnd();
		}
	}*/


}

void TileMap::prepareArrays(const glm::vec2& minCoords, ShaderProgram& program)
{
	int tile, nTiles = 0;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;

	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if (tile != 0 && tile != 1 && tile != 3)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float((tile - 1) % 2) / tilesheetSize.x, float((tile - 1) / 2) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

void TileMap::getEntityLocations(vector<glm::ivec2>& wallLocs, int tilemapId)
{
	for (int j = 0; j < mapSize.y; j++)
	{
	for (int i = 0; i < mapSize.x; i++)
		{
			if (map[j * mapSize.x + i] == tilemapId) {
				wallLocs.push_back(glm::ivec2(i, j));
			}
		}
	}
}































