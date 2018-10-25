#pragma once

#include "Scenes.h"
#include "Components.h"
#include "PresentFarmScene.h"
#include "CharacterCreateScene.h"

class GameSelectScene : public Scene {
public:
	SDL_Color WHITE = { 255,255,255,255 };

	GameSelectScene() = default;
	~GameSelectScene() {}

	void init() override {
		initialized = true;
		printf("Game select menu scene init\n");
		// for each file in saves file, read and create button.

		save = &manager->createEntity<GameSelectScene>();
		newSave = &manager->createEntity<GameSelectScene>();
		back = &manager->createEntity<GameSelectScene>();

		back->addComponent<UILabel>(50, 50, "<- Back", "vt323", WHITE);
		back->addComponent<UIButton>([&]() {
			//printf("Go to main menu\n");
			//manager->changeScene<MainMenuScene>();
			manager->prevScene();
		});

		save->addComponent<UILabel>(Game::camera.w / 2, Game::camera.h / 2 + 3 * FONT_SIZE, "save 1", "vt323", WHITE);
		save->addComponent<UIButton>([&]() {
			manager->addScene<PresentFarmScene>("skin5", "eyes1");
			manager->changeScene<PresentFarmScene>();
		});

		newSave->addComponent<UILabel>(Game::camera.w / 2, Game::camera.h / 2 + 2 * FONT_SIZE, "New Game", "vt323", WHITE);
		newSave->addComponent<UIButton>([&]() {
			manager->changeScene<CharacterCreateScene>();
		});

		save->addGroup(Game::groupUI);
		newSave->addGroup(Game::groupUI);
		back->addGroup(Game::groupUI);
	}

	void update() override {
		manager->updateEntities<GameSelectScene>();
	}

	void render() override {
		manager->renderEntities<GameSelectScene>();
	}

private:
	Entity* save;
	Entity* newSave;
	Entity* back;
};
