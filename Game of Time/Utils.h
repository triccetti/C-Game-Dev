#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include <string>
#include "AssetManager.h"

class CollisionComponent;
class Utils {
public:
	static bool isMouseOver(SDL_Rect * rect);

	static bool collision(const SDL_Rect& recA, const SDL_Rect& recB);
	static bool collision(const CollisionComponent& colA, const CollisionComponent& colB);

	static void saveTexturePng(SDL_Renderer* renderer, SDL_Texture* texture, const char* file_name);
	static void save_texture(SDL_Renderer *ren, SDL_Texture *tex, const char *filename);
};
