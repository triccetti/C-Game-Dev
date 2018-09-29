#include "Game.h"

#include "TextureManager.h"
#include "Map.h"
#include "ECS.h"
#include "AssetManager.h"
#include "Scenes.h"

int windowHeight;
int windowWidth;
int mapHeight;
int mapWidth;

GameManager manager;

SDL_Renderer * Game::renderer = nullptr;
AssetManager* Game::assets = new AssetManager(&manager);

SDL_Event Game::event;
bool Game::running = false;

SDL_Rect Game::camera = { 0, 0, 800, 640 };
Vector2D Game::clickPos;

Game::Game() {}

Game::~Game() {}

void Game::init(const char* title, int width, int height, bool fullscreen) {
	int flags = 0;

	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}
	printf("Creating window... ");
	windowHeight = height;
	windowWidth = width;

	if (SDL_Init(SDL_INIT_VIDEO) == 0) {
		window = SDL_CreateWindow(title,
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			windowWidth, windowHeight, flags);

		camera.h = windowHeight;
		camera.w = windowWidth;

		if (window == NULL) {
			printf("Window could not be created: %s\n", SDL_GetError());
			exit(1);
		}

		printf("	Window created successfully.\n");
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);
		}

		running = true;
	}

	if (TTF_Init() < 0) {
		printf("ERROR: %s\n", TTF_GetError());
	}

	printf("Loading textures... ");
	assets->AddTexture("terrain", "Assets/map_ts.png");
	assets->AddTexture("player", "Assets/character.png");

	for (int i = 1; i < 6; i++) {
		std::string skinTexPath = "Assets/skin" + std::to_string(i) + ".png";
		assets->AddTexture("skin" + std::to_string(i), skinTexPath.c_str());
	}

	for (int i = 1; i < 6; i++) {
		std::string skinTexPath = "Assets/eyes" + std::to_string(i) + ".png";
		assets->AddTexture("eyes" + std::to_string(i), skinTexPath.c_str());
	}

	assets->AddFont("vt323", "Assets/VT323-Regular.ttf", Scene::FONT_SIZE);
	printf("	Textures loaded.\n");

	printf("Loading scenes... ");
	manager.addScene<MainMenuScene>();
	manager.addScene<GameSelectScene>();
	manager.addScene<CharacterCreateScene>();
	printf("	Scenes loaded.\n");

	// Sets the start scene
	manager.changeScene<MainMenuScene>();

	SDL_StartTextInput();
}

void Game::handleEvents() {
	//SDL_PollEvent(&event);
	while (SDL_PollEvent(&event) != 0) {
		printf("event: %d\n", event.type);
		switch (event.type) {
		case SDL_QUIT:
			running = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			int x, y;
			SDL_GetMouseState(&x, &y);
			clickPos = Vector2D(static_cast<float>(x), static_cast<float>(y));
			break;
		case SDL_KEYDOWN:
			//printf("Type: %d\n", event.type);
			break;
		default:
			break;
		}
	}
}

void Game::update() {
	manager.refresh();
	manager.update();

	if (camera.x < 0)
		camera.x = 0;
	if (camera.y < 0)
		camera.y = 0;
	if (camera.x > camera.w / 2)
		camera.x = camera.w / 2;
	if (camera.y > camera.h / 2)
		camera.y = camera.h / 2;
}

void Game::render() {
	SDL_RenderClear(renderer);
	manager.render();
	SDL_RenderPresent(renderer);
}

void Game::clean() {
	SDL_StopTextInput();
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}