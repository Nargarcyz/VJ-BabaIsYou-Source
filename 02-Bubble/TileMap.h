#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE



#include "Texture.h"
#include "ShaderProgram.h"
#include "Player.h"
#include "Movable.h"
#include <typeinfo>
#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap* createTileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program);

	TileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program);
	~TileMap();

	void render() const;
	void free();

	void addEntity(int x, int y, Entity* ent);

	void moveEntity(glm::ivec2 src, glm::ivec2 dest);

	void removeEntity(glm::ivec2 pos);

	Entity* getEntity(glm::ivec2 testPosition, bool& success, bool& outOfBounds);

	//Entity* getEntity(int x, int y, bool& success);


	int getTileSize() const { return tileSize; }
	glm::vec2 getMapSize();
	void getEntityLocations(vector<glm::ivec2>& wallLocs,int tilemapId);

	int* extractMap();


private:
	bool loadLevel(const string& levelFile);
	void prepareArrays(const glm::vec2& minCoords, ShaderProgram& program);

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int* map;
	glm::ivec2 gridMapSize;
	Entity** gridMap;


};


#endif // _TILE_MAP_INCLUDE


