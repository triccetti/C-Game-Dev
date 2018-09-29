#pragma once
#include <map>
#include <string>
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS.h"
#include "SDL_ttf.h"

class GameManager;
class AssetManager {
public:
	AssetManager(GameManager * man);
	~AssetManager();

	void AddTexture(std::string id, const char* path);
	void AddFont(std::string id, std::string path, int fontSize);
	SDL_Texture* GetTexture(std::string id);
	TTF_Font* GetFont(std::string id);

private:
	GameManager * manager;

	std::map<std::string, SDL_Texture*> textures;
	std::map<std::string, TTF_Font*> fonts;
};
