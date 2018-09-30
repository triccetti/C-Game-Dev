#pragma once 
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>

class GameManager;

class Map {
public:
	Map(GameManager * man, std::string texId, int mScale);
	~Map();

	void LoadMap(std::string path);
	void AddTile(int srcX, int srcY, int xpos, int ypos);
	int getHeight() { return mapHeight; }
	int getWidth() { return mapWidth; }

private:
	GameManager * manager;

	std::string textureId;
	int mapScale;
	int tileSize;
	int scaledSize;
	int mapHeight;
	int mapWidth;
};