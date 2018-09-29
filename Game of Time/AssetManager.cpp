#include "AssetManager.h"
#include "ECS.h"

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
Returns the font of the given id.
*/
TTF_Font* AssetManager::GetFont(std::string id) {
	if (!fonts[id]) {
		printf("No font %s in the AssetManager\n", id.c_str());
		exit(1);
	}
	return fonts[id];
}