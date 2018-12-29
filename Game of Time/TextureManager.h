#pragma once

#include "Game.h"

class TextureManager {
public:
	static SDL_Texture* LoadTexture(const char* path);
	static void RenderTexture(SDL_Texture* texthre, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip);
	static SDL_Color GetPixel(SDL_Texture* texture, int x, int y);
};
