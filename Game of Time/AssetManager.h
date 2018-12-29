#pragma once
#include <map>
#include <string>
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS.h"
#include "SDL_ttf.h"

#include "tmxlite/Map.hpp"
#include "tmxlite/Layer.hpp"
#include "tmxlite/TileLayer.hpp"

class AssetManager {
public:
	AssetManager(GameManager * man);
	~AssetManager();

	//void AddColor(std::string id, int r, int g, int b, int a);
	void AddTexture(std::string id, const char* path);
	void AddTileset(int gid, const char* path);
	void AddTerrain(int gid, std::string name);
	void AddFont(std::string id, std::string path, int fontSize);

	void LoadTileMap(std::string path, Scene* parent, SDL_Rect * mapSize);

	SDL_Texture* GetTexture(std::string id);
	std::string GetTerrain(int gid);
	SDL_Texture* GetTileset(int gid);
	TTF_Font* GetFont(std::string id);

private:
	GameManager * manager;

	std::map<std::string, SDL_Texture*> textures;
	std::map<int, SDL_Texture*> tilesets;
	std::map<int, std::string> terrains;
	std::map<std::string, TTF_Font*> fonts;
};
