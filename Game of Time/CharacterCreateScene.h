#pragma once
#include "Components.h"
#include "Scenes.h"

class CharacterCreateScene : public Scene {
public:
	CharacterCreateScene() = default;
	~CharacterCreateScene() {}

	void init() override {
		playerSkin = &manager->createEntity<CharacterCreateScene>();
		playerSkin->addComponent<TransformComponent>(100.0f, 50.0f, 32, 16, 5);
		playerSkin->addComponent<SpriteComponent>("skin1");
		playerSkin->addComponent<UISelector>("skin", "vt323", WHITE, 6, 195, 140, 65, 140);

		playerEyes = &manager->createEntity<CharacterCreateScene>();
		playerEyes->addComponent<TransformComponent>(100.0f, 50.0f, 32, 16, 5);
		playerEyes->addComponent<SpriteComponent>("eyes1");
		playerEyes->addComponent<UISelector>("eyes", "vt323", WHITE, 6, 195, 180, 65, 180);

		name = &manager->createEntity<CharacterCreateScene>();
		name->addComponent<UILabel>(50, 550, "Name", "vt323", WHITE);
		name->addComponent<TextBoxComponent>(100, 550, "vt323", WHITE);

		create = &manager->createEntity<CharacterCreateScene>();
		create->addComponent<UILabel>(600, 550, "CREATE", "vt323", WHITE);
		create->addComponent<UIButton>([&]() {
			SDL_Texture* playerTexture = createPlayerTexture();
			// get all the currently selected stuff and go!
		});
	}

	void update() override {
		manager->updateEntities<CharacterCreateScene>();
	}

	void render() override {
		manager->renderEntities<CharacterCreateScene>();
	}

	SDL_Color WHITE = { 255, 255, 255, 255 };
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

	SDL_Texture* createPlayerTexture() {
		SDL_Texture*  target_texture = SDL_CreateTexture(Game::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 64, 32);

		SDL_Rect dest;
		int skinIndex = playerSkin->getComponent<UISelector>().getSelectedIndex();
		SDL_QueryTexture(Game::assets->GetTexture("skin" + std::to_string(skinIndex)), NULL, NULL, &dest.w, &dest.h);

		dest.x = 0;
		dest.y = 0;

		SDL_SetRenderTarget(Game::renderer, target_texture);
		SDL_RenderCopy(Game::renderer, Game::assets->GetTexture("skin" + std::to_string(skinIndex)), NULL, NULL);

		int eyeIndex = playerEyes->getComponent<UISelector>().getSelectedIndex();
		SDL_RenderCopy(Game::renderer, Game::assets->GetTexture("eyes" + std::to_string(eyeIndex)), NULL, NULL);

		SDL_SetRenderTarget(Game::renderer, NULL);

		// might not do this
		Utils::save_texture(Game::renderer, target_texture, "character.png");
		return target_texture;
	}
};
