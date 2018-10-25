#pragma once
#include "Scenes.h"
#include "Components.h"
#include "GameSelectScene.h"

class MainMenuScene : public Scene {
public:
	SDL_Color WHITE = { 255,255,255,255 };

	MainMenuScene() = default;
	~MainMenuScene() {}

	void init() override {
		initialized = true;
		printf("Main menu scene init\n");
		playGame = &manager->createEntity<MainMenuScene>();
		exitGame = &manager->createEntity<MainMenuScene>();

		playGame->addComponent<UILabel>(Game::camera.w / 2, Game::camera.h / 2 - 46, "Play", "vt323", WHITE);
		playGame->addComponent<UIButton>([&]() {
			printf("Changing to game select\n");
			manager->changeScene<GameSelectScene>();
		});

		exitGame->addComponent<UILabel>(Game::camera.w / 2, Game::camera.h / 2, "Exit", "vt323", WHITE);
		exitGame->addComponent<UIButton>([&]() {
			exit(0);
		});

		playGame->addGroup(Game::groupUI);
		exitGame->addGroup(Game::groupUI);
	}

	void update() override {
		manager->updateEntities<MainMenuScene>();
	}

	void render() override {
		manager->renderEntities<MainMenuScene>();
	}

	void unload() override {
		manager->destroyEntities<MainMenuScene>();
		initialized = false;
	}

private:
	Entity* playGame;
	Entity* exitGame;
};