#pragma once
#include "Scenes.h" 
#include "Components.h"
#include "GameSelectScene.h"

class MainMenuScene : public Scene {
public:
	MainMenuScene() = default;
	~MainMenuScene() {}

	void init() override {
		printf("Main menu scene init\n");
		playGame = &manager->createEntity<MainMenuScene>();
		exitGame = &manager->createEntity<MainMenuScene>();

		playGame->addComponent<UILabel>(Game::camera.w / 2, Game::camera.h / 2 - 46, "Play", "vt323", WHITE);
		playGame->addComponent<UIButton>([&]() {
			manager->changeScene<GameSelectScene>();
		});

		exitGame->addComponent<UILabel>(Game::camera.w / 2, Game::camera.h / 2, "Exit", "vt323", WHITE);
		exitGame->addComponent<UIButton>([&]() {
			exit(0);
		});
	}

	void update() override {
		manager->updateEntities<MainMenuScene>();
	}

	void render() override {
		manager->renderEntities<MainMenuScene>();
	}

	/* void handleEvent(SDL_Event event) override {
		manager->handleEntities<MainMenuScene>(event);
	}*/

	SDL_Color WHITE = { 255, 255, 255, 255 };

private:
	Entity* playGame;
	Entity* exitGame;
};