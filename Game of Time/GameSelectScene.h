#pragma once
#include "Scenes.h"
#include "Components.h"
#include "CharacterCreateScene.h"
#include "MainMenuScene.h"

class GameSelectScene : public Scene {
public:
	GameSelectScene() = default;
	~GameSelectScene() {}

	void init() override {
		printf("Game select menu scene init\n");
		// for each file in saves file, read and create button.

		save = &manager->createEntity<GameSelectScene>();
		newSave = &manager->createEntity<GameSelectScene>();
		back = &manager->createEntity<GameSelectScene>(); 

		back->addComponent<UILabel>(50, 50, "<- Back", "vt323", WHITE);
		back->addComponent<UIButton>([&]() {
			printf("Go to main menu\n");
			//manager->changeScene<MainMenuScene>();
			manager->prevScene();
		});
		
		save->addComponent<UILabel>(Game::camera.w / 2, Game::camera.h / 2 + 3 * FONT_SIZE, "save 1", "vt323", WHITE);
		save->addComponent<UIButton>([&]() {
			printf("Selected save 1. \n");
		});

		newSave->addComponent<UILabel>(Game::camera.w / 2, Game::camera.h / 2 + 2 * FONT_SIZE, "New Game", "vt323", WHITE);
		newSave->addComponent<UIButton>([&]() {
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
	Entity* back;
};
