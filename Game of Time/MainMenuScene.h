#pragma once
#include "Components.h"
#include "Scenes.h"
#include "GameSelectScene.h"

class MainMenuScene : public Scene {
public:
	MainMenuScene() = default;
	~MainMenuScene() {}

	void init() override {
		printf("Main menu scene init\n");
		playGame = &manager->createEntity<MainMenuScene>();
		exitGame = &manager->createEntity<MainMenuScene>();

		playGame->addComponent<UIComponent>(Game::camera.w / 2, Game::camera.h / 2 - 46, "Play", "vt323", WHITE, false);
		playGame->getComponent<UIComponent>().setOnClick([&]() {
			manager->changeScene<GameSelectScene>();
		});

		exitGame->addComponent<UIComponent>(Game::camera.w / 2, Game::camera.h / 2, "Exit", "vt323", WHITE, false);
		exitGame->getComponent<UIComponent>().setOnClick([&]() {
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