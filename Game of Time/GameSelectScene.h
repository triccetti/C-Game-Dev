#pragma once
#include "Components.h"
#include "Scenes.h"
#include "CharacterCreateScene.h"

class GameSelectScene : public Scene {
public:
	GameSelectScene() = default;
	~GameSelectScene() {}

	void init() override {
		printf("Game select menu scene init\n");
		// for each file in saves file, read and create button.

		save = &manager->createEntity<GameSelectScene>();
		newSave = &manager->createEntity<GameSelectScene>();

		save->addComponent<UIComponent>(Game::camera.w / 2, Game::camera.h / 2 + 3 * FONT_SIZE, "save 1", "vt323", WHITE, false);
		save->getComponent<UIComponent>().setOnClick([&]() {
			printf("Selected save 1. \n");
		});

		newSave->addComponent<UIComponent>(Game::camera.w / 2, Game::camera.h / 2 + 2 * FONT_SIZE, "New Game", "vt323", WHITE, false);
		newSave->getComponent<UIComponent>().setOnClick([&]() {
			manager->changeScene<CharacterCreateScene>();
		});
	}

	void update() override {
		manager->updateEntities<GameSelectScene>();
	}

	void render() override {
		manager->renderEntities<GameSelectScene>();
	}

	/*void handleEvent(SDL_Event event) override {
		manager->handleEntities<GameSelectScene>(event);
	}*/

	SDL_Color WHITE = { 255, 255, 255, 255 };
private:
	Entity* save;
	Entity* newSave;
};
