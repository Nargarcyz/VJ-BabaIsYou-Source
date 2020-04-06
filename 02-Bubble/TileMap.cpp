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

// Adds an entity to the collision map in a given position
void TileMap::addEntity(int x, int y, Entity* ent)
{
	gridMap[y * gridMapSize.x + x] = ent;
}
// Moves an entity from a source to a destination position in the collision grid
void TileMap::moveEntity(glm::ivec2 src, glm::ivec2 dest) {
	gridMap[dest.x + gridMapSize.x * dest.y] = gridMap[src.x + gridMapSize.x * src.y];
	gridMap[src.x + gridMapSize.x * src.y] = NULL;
}

// Removes an entity from the collision map from a source position
void TileMap::removeEntity(glm::ivec2 src) {
	if (gridMap[src.x + gridMapSize.x * src.y] != NULL && gridMap[src.x + gridMapSize.x * src.y]->getEntityType() != MoveBlock)
	{
		gridMap[src.x + gridMapSize.x * src.y] = NULL;
	}
}
// Scans a position for an entity in the collision grid. Checks for success in detection of anything,
// and specifies if it is out of bounds, or if it is an entity
Entity* TileMap::getEntity(glm::ivec2 testPosition, bool& success, bool& outOfBounds) {

	int x = testPosition.x;
	int y = testPosition.y;
	string s = "\nChecking X: " + to_string(x) + "Y: " + to_string(y);
	OutputDebugStringA(s.c_str());

	outOfBounds = false;
	if (x < 0 || (x > (gridMapSize.x-1)) || y < 0 || (y > (gridMapSize.y-1)))
	{
		success = true;
		outOfBounds = true;
		return NULL;
	}else if (gridMap[x + gridMapSize.x * y] == NULL)
	{
		success = false;
		return NULL;
	} else if (gridMap[x + gridMapSize.x * y] != NULL) {
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
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{

			string number = "";
			fin.get(tile);
			while (tile != *",")
			{
				number += tile;
				fin.get(tile);
			}
			
			if (std::stoi(number) == ' ' || std::stoi(number) == 0)
				map[j * mapSize.x + i] = 0;
			else
				map[j * mapSize.x + i] = std::stoi(number);
		}
	}

	// Creates a collision map
	gridMapSize = glm::ivec2(floor(glutGet(GLUT_WINDOW_WIDTH) / tileSize), floor(glutGet(GLUT_WINDOW_HEIGHT) / tileSize));
	gridMap = new Entity * [gridMapSize.x * gridMapSize.y]{ NULL };

	fin.close();

	return true;
}

void TileMap::render() const
{
	glClearColor(0.141176470588235, 0.172549019607843, 0.278431372549020, 0.0);

	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
	glDisable(GL_TEXTURE_2D);
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
			//if (tile != 0)//&& tile != 1 && tile != 4)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				if (tile == 0 || tile == 1)
				{
					texCoordTile[0] = glm::vec2(float((tile) % 3) / tilesheetSize.x, float((tile ) / 3) / tilesheetSize.y);
				}else
					texCoordTile[0] = glm::vec2(float(0 % 2) / tilesheetSize.x, float(0 / 2) / tilesheetSize.y);

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



glm::vec2 TileMap::getMapSize()
{
	return mapSize;
}

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

int* TileMap::extractMap() {
	return map;
}































