#pragma once
#include "SDL.h"

class Utils {
public:

	static bool isMouseOver(SDL_Rect * rect);
	static void save_texture(SDL_Renderer *ren, SDL_Texture *tex, const char *filename);
};
