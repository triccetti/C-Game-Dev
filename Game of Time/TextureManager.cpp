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

/*
SDL_Color TextureManager::GetPixel(SDL_Texture* texture, int x, int y) {
	const Uint8 &bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	Uint32 result = 0;

	switch (bpp)
	{
	case 1:
		result = *p;

	case 2:
		result = *(Uint16 *)p;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			result = (p[0] << 16 | p[1] << 8 | p[2]);
		else
			result = (p[0] | p[1] << 8 | p[2] << 16);

	case 4:
		result = *(Uint32 *)p;

	default:
		result = 0;       /* shouldn't happen, but avoids warnings
	}

	SDL_Color theKey;
	SDL_GetRGB(result, surface->format, &theKey.r, &theKey.g, &theKey.b);

	return theKey;
}*/