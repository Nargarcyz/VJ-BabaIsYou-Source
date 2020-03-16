#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE



#include "Texture.h"
#include "ShaderProgram.h"
#include "Player.h"
#include "Movable.h"
#include <typeinfo>
#include <GL/glew.h>
#include <GL/glut.h>



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

	Entity* getEntity(glm::ivec2 testPosition, bool& success, bool& outOfBounds);

	//Entity* getEntity(int x, int y, bool& success);


	int getTileSize() const { return tileSize; }
	void getWallLocations(vector<glm::ivec2>& wallLocs);
	bool collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size) const;
	bool collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size) const;
	bool collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const;
	bool collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const;

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
	Entity** gridMap;


};


#endif // _TILE_MAP_INCLUDE


