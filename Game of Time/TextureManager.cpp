#include "TextureManager.h"

/*
	Loads the given texture from the file path given.
	Returns the SDL_Texture loaded.
*/
SDL_Texture* TextureManager::LoadTexture(const char* path) {
	SDL_Surface* tempSurface = IMG_Load(path);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	return tex;
}

/*
	Displays the given texture to the Game's renderer.
	Flips the display if flip is true.
*/
void TextureManager::RenderTexture(SDL_Texture* texture, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip) {
	SDL_RenderCopyEx(Game::renderer, texture, &src, &dest, NULL, NULL, flip);
}