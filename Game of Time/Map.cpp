#include "Map.h"
#include "Game.h"
#include "ECS.h"
#include "Components.h"

Map::Map(std::string texId, int mScale) {
	mapScale = mScale;
	textureId = texId;
}

Map::~Map() {}

void Map::LoadMap(std::string path) {
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

			AddTile(srcX * tileSize, srcY * tileSize, x * scaledSize, y * scaledSize);

			mapFile.get(c);
		}
		mapFile.ignore();
	}

	mapFile.close();
}

void Map::AddTile(int srcX, int srcY, int xpos, int ypos) {
	//shared_ptr<Entity> tile = Game::entityManager->CreateEntity();
	//tile->addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, scaledSize, textureId);
}