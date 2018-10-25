#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>

#include "ECS.h"
#include "Components.h"

class Map {
public:
	Map(GameManager * man, std::string texId, int mScale);
	~Map();

	//void LoadMap(std::string path);
	template <typename T>
	void LoadMap(std::string path) {
		char c;
		std::ifstream mapFile;
		mapFile.open(path);

		int height, width;

		std::string line;
		getline(mapFile, line);

		std::stringstream ss(line);
		ss >> tileSize >> height >> width;
		scaledSize = tileSize * mapScale;

		mapHeight = tileSize * height;
		mapWidth = tileSize * width;

		getline(mapFile, line);
		int srcX, srcY;
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				mapFile.get(c);
				srcY = atoi(&c);
				mapFile.get(c);
				srcX = atoi(&c);

				AddTile<T>(srcX * tileSize, srcY * tileSize, x * scaledSize, y * scaledSize);

				mapFile.get(c);
			}
			mapFile.ignore();
		}

		mapFile.close();
	}

	//void AddTile(int srcX, int srcY, int xpos, int ypos);
	template <typename T>
	void AddTile(int srcX, int srcY, int xpos, int ypos) {
		printf("added tile\n");
		Entity * tile = &manager->createEntity<T>();
		tile->addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, scaledSize, textureId);
	}

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