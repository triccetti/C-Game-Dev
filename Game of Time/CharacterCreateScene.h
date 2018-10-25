#pragma once
#include "Components.h"
#include "Scenes.h"
#include "PresentFarmScene.h"
#include "Game.h"

class CharacterCreateScene : public Scene {
public:
	SDL_Color WHITE = { 255,255,255,255 };
	SDL_Color RED = { 255,0,0,255 };

	CharacterCreateScene() = default;
	~CharacterCreateScene() {}

	void init() override {
		initialized = true;
		Entity * back = &manager->createEntity<CharacterCreateScene>();
		back->addComponent<UILabel>(40, 40, "<- Back", "vt323", WHITE);
		back->addComponent<UIButton>([&]() {
			printf("Go to main menu\n");
			//manager->changeScene<MainMenuScene>();
			manager->prevScene();
		});

		name = &manager->createEntity<CharacterCreateScene>();
		name->addComponent<UILabel>(100, 100, "Name", "vt323", WHITE);
		name->addComponent<TextBoxComponent>(150, 100, 16, "vt323", WHITE, RED);

		playerSkin = &manager->createEntity<CharacterCreateScene>();
		playerSkin->addComponent<TransformComponent>(350.0f, 125.0f, 32, 16, 10);
		playerSkin->addComponent<SpriteComponent>("skin1");
		playerSkin->addComponent<UILabel>(175, 225, "Skin Color", "vt323", WHITE);
		// TODO: 10 is a magic number add it to properties
		playerSkin->addComponent<UISelector>("skin", "vt323", 10, 450, 225, 300, 225);

		playerEyes = &manager->createEntity<CharacterCreateScene>();
		playerEyes->addComponent<TransformComponent>(350.0f, 125.0f, 32, 16, 10);
		playerEyes->addComponent<SpriteComponent>("eyes1");
		playerEyes->addComponent<UILabel>(175, 175, "Eyes", "vt323", WHITE);
		playerEyes->addComponent<UISelector>("eyes", "vt323", 5, 450, 175, 300, 175);

		create = &manager->createEntity<CharacterCreateScene>();
		create->addComponent<UILabel>(600, 550, "CREATE", "vt323", WHITE);
		create->addComponent<UIButton>([&]() {
			if (name->getComponent<TextBoxComponent>().getText().length() < 3) {
				name->getComponent<TextBoxComponent>().displayError("Name must be 3 characters");
			} else {
				// get all the currently selected stuff and go!
				std::string playerName = createPlayerTexture();
				int eyeIndex = playerEyes->getComponent<UISelector>().getSelectedIndex();

				manager->addScene<PresentFarmScene>(playerName, "eyes" + std::to_string(eyeIndex));
				manager->changeScene<PresentFarmScene>();
			}
		});

		back->addGroup(Game::groupUI);
		name->addGroup(Game::groupUI);
		playerSkin->addGroup(Game::groupUI);
		playerEyes->addGroup(Game::groupUI);
		create->addGroup(Game::groupUI);
	}

	void update() override {
		manager->updateEntities<CharacterCreateScene>();
	}

	void render() override {
		manager->renderEntities<CharacterCreateScene>();
	}

	void unload() override {
		manager->destroyEntities<CharacterCreateScene>();
	}

private:
	Entity* create;

	Entity* name;

	Entity* playerSkin;
	Entity* playerEyes;

	Entity* accessories;
	Entity* top;
	Entity* bottom;
	Entity* shoes;

	Entity* accessorySelector;
	Entity* topSelector;
	Entity* bottomSelector;
	Entity* shoeSelector;

	std::string createPlayerTexture() {
		SDL_Rect dest;
		int skinIndex = playerSkin->getComponent<UISelector>().getSelectedIndex();
		SDL_QueryTexture(Game::assets->GetTexture("skin" + std::to_string(skinIndex)), NULL, NULL, &dest.w, &dest.h);

		SDL_Texture*  target_texture = SDL_CreateTexture(Game::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, dest.w, dest.h);

		dest.x = 0;
		dest.y = 0;

		SDL_SetRenderTarget(Game::renderer, target_texture);
		SDL_RenderCopy(Game::renderer, Game::assets->GetTexture("skin" + std::to_string(skinIndex)), NULL, NULL);

		//int eyeIndex = playerEyes->getComponent<UISelector>().getSelectedIndex();
		//SDL_RenderCopy(Game::renderer, Game::assets->GetTexture("eyes" + std::to_string(eyeIndex)), NULL, NULL);

		SDL_SetRenderTarget(Game::renderer, NULL);

		// might not do this
		std::string playerName = name->getComponent<TextBoxComponent>().getText();
		std::string fileName("Assets\\SavedAssets\\" + playerName + ".png");
		Utils::saveTexturePng(Game::renderer, target_texture, fileName.c_str());
		Game::assets->AddTexture(playerName, fileName.c_str());
		return playerName;
	}
};
