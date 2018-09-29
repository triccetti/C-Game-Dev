#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include <string>
#include <map>
#include <set>
#include <functional>
#include "Vector2D.h"

class AssetManager;

class Game {
public:
	Game();
	~Game();

	void init(const char* title, int width, int height, bool fullscreen);
	void handleEvents();
	void update();
	void render();
	void clean();

	bool isRunning() { return running; }

	static SDL_Renderer * renderer;

	static AssetManager * assets;

	static SDL_Event event;
	static bool running;
	static SDL_Rect camera;
	static Vector2D clickPos;

private:
	int windowHeight;
	int windowWidth;
	SDL_Window *window;
};
