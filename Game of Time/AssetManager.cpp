#include "AssetManager.h"

#include <iostream>
#include <algorithm>
#include "Components.h"

//#include "TMXLoader/TMXLoader.h"

AssetManager::AssetManager(GameManager* man) : manager(man) {}

AssetManager::~AssetManager() {}

/*
	Adds the texture of the given path to the AssetManager with the given id.
*/
void AssetManager::AddTexture(std::string id, const char* path) {
	if (textures[id] != NULL) {
		printf("Error: texture %s has already been loaded.\n", id.c_str());
	}
	textures[id] = TextureManager::LoadTexture(path);
}

/*
	Adds the texture of the given path to the AssetManager with the given id.
*/
void AssetManager::AddTerrain(int gid, std::string name) {
	if (!terrains[gid].empty()) {
		printf("Error: tileset %d has already been loaded.\n", gid);
	}
	terrains[gid] = name;
}

/*
	Adds the terrain of the given path to the AssetManager with the given id.
*/
void AssetManager::AddTileset(int gid, const char* path) {
	if (tilesets[gid] != NULL) {
		printf("Error: tileset %d has already been loaded.\n", gid);
	}
	tilesets[gid] = TextureManager::LoadTexture(path);
}

/*
	Adds the font from the given path with a given font size to the
	AssetManager with the given id.
*/
void AssetManager::AddFont(std::string id, std::string path, int fontSize) {
	if (fonts[id] != NULL) {
		printf("Error loading font %s. Error: font %s has already been loaded.\n", id.c_str(), id.c_str());
		exit(1);
	}

	TTF_Font* font;
	font = TTF_OpenFont(path.c_str(), fontSize);
	if (!font) {
		printf("Error loading font %s. Error: %s\n", id.c_str(), TTF_GetError());
		exit(1);
	}
	fonts[id] = font;
}

/*
	Returns the texture of the given id.
*/
SDL_Texture* AssetManager::GetTexture(std::string id) {
	if (!textures[id]) {
		printf("No texture %s in the AssetManager\n", id.c_str());
		exit(1);
	}
	return textures[id];
}

/*
	Returns the texture of the given id.
*/
SDL_Texture* AssetManager::GetTileset(int gid) {
	if (!tilesets[gid]) {
		printf("No tileset with gid %d in the AssetManager\n", gid);
		exit(1);
	}
	return tilesets[gid];
}

/*
Returns the font of the given id.
*/
TTF_Font* AssetManager::GetFont(std::string id) {
	if (!fonts[id]) {
		printf("No font %s in the AssetManager\n", id.c_str());
		exit(1);
	}
	return fonts[id];
}

/*
Returns the terrain type of the given id. Can return an empty terrain type.
*/
std::string AssetManager::GetTerrain(int gid) {
	return terrains[gid];
}

// REF: https://github.com/connorkuehl/sdl2-tiled-example/blob/master/src/level.cpp
// REF: https://bitbucket.org/martingrant/tmxloader
void AssetManager::LoadTileMap(std::string path, Scene* parent, SDL_Rect &mapSize) {
	tmx::Map map;

	if (map.load(path)) {
		// Get map dimensions
		auto mapDimensions = map.getTileCount();
		int mapHeight = mapDimensions.y;
		int mapWidth = mapDimensions.x;
		printf("map h: %d w: %d\n", mapHeight, mapWidth);

		// Get tile dimensions
		auto tileDimensions = map.getTileSize();
		int tileHeight = tileDimensions.y;
		int tileWidth = tileDimensions.x;
		printf("tile h: %d w: %d\n", tileHeight, tileWidth);

		mapSize.h = tileHeight * mapHeight;
		mapSize.w = tileWidth * mapWidth;

		// Add maps tilesets to textures
		auto& mapTilesets = map.getTilesets();
		for (auto& tileset : mapTilesets) {
			auto tiles = tileset.getTiles();
			auto& terrains = tileset.getTerrainTypes();
			for (auto t : tiles) {
				std::string terrainType = terrains[t.terrainIndices[0]].name;
				AddTerrain(t.ID, terrainType);
			}
			printf("Adding...\n%s\n", tileset.getName().c_str());
			printf("%s\n", tileset.getImagePath().c_str());
			AddTileset(tileset.getFirstGID(), tileset.getImagePath().c_str());
		}

		auto& layers = map.getLayers();

		// Process layer in map.
		for (auto& layer : layers) {
			// Object layer
			if (layer->getType() != tmx::Layer::Type::Tile) {
				printf("This layer is an object layer???\n");
			} else { // Tile layer
				auto* tileLayer = dynamic_cast<const tmx::TileLayer*>(layer.get());

				// Grab all of this layer's tiles.
				auto& tiles = tileLayer->getTiles();
				for (auto y = 0; y < mapHeight; ++y) {
					for (auto x = 0; x < mapWidth; ++x) {
						int index = x + (y * mapWidth);

						// get the tile
						auto curGID = tiles[index].ID;
						auto terra = tiles[index];
						if (curGID != 0) {
							auto tilesetGID = -1;
							for (auto& ts : tilesets) {
								if (ts.first <= (int)curGID) {
									tilesetGID = ts.first;
									break;
								}
							}

							if (tilesetGID == -1) continue;

							curGID -= tilesetGID;

							auto tsWidth = 0;
							auto tsHeight = 0;
							SDL_QueryTexture(tilesets[tilesetGID], NULL, NULL, &tsWidth, &tsHeight);

							auto srcX = (curGID % (tsWidth / tileWidth)) * tileWidth;
							auto srcY = (curGID / (tsWidth / tileHeight)) * tileHeight;
							auto xpos = x * tileWidth;
							auto ypos = y * tileHeight;

							//(SDL_Texture* tset, int x, int y, int tx, int ty, int w, int h)
							parent->addTile(srcX, srcY, xpos, ypos, tileHeight, tilesetGID, curGID);
						}
					}
				}
			}
		}
	} else {
		printf("Error loading map %s in the AssetManager\n", path.c_str());
		exit(1);
	}
}