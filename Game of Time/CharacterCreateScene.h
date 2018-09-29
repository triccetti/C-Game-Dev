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
		playerSkin->addComponent<SpriteComponent>("skin" + std::to_string(skin));

		nextSkin = &manager->createEntity<CharacterCreateScene>();
		nextSkin->addComponent<UIComponent>(195, 140, "->", "vt323", WHITE, true);
		nextSkin->getComponent<UIComponent>().setOnClick([&]() {
			printf("Skin selector\n");
			skin++;
			if (skin >= 6) skin = 1;
			playerSkin->getComponent<SpriteComponent>().setTex("skin" + std::to_string(skin));
		});
		prevSkin = &manager->createEntity<CharacterCreateScene>();
		prevSkin->addComponent<UIComponent>(65, 140, "<-", "vt323", WHITE, true);
		prevSkin->getComponent<UIComponent>().setOnClick([&]() {
			skin--;
			if (skin <= 0) skin = 5;
			playerSkin->getComponent<SpriteComponent>().setTex("skin" + std::to_string(skin));
		});

		playerEyes = &manager->createEntity<CharacterCreateScene>();
		playerEyes->addComponent<TransformComponent>(100.0f, 50.0f, 32, 16, 5);
		playerEyes->addComponent<SpriteComponent>("eyes" + std::to_string(eye));

		nextEyes = &manager->createEntity<CharacterCreateScene>();
		nextEyes->addComponent<UIComponent>(195, 180, "->", "vt323", WHITE, true);
		nextEyes->getComponent<UIComponent>().setOnClick([&]() {
			eye++;
			if (eye >= 6) eye = 1;
			playerEyes->getComponent<SpriteComponent>().setTex("eyes" + std::to_string(eye));
		});
		prevEyes = &manager->createEntity<CharacterCreateScene>();
		prevEyes->addComponent<UIComponent>(65, 180, "<-", "vt323", WHITE, true);
		prevEyes->getComponent<UIComponent>().setOnClick([&]() {
			eye--;
			if (eye <= 0) eye = 5;
			playerEyes->getComponent<SpriteComponent>().setTex("eyes" + std::to_string(eye));
		});

		name = &manager->createEntity<CharacterCreateScene>();
		name->addComponent<TextBoxComponent>(50, 550, "vt323", WHITE);
		name->getComponent<TextBoxComponent>().setOnClick([&]() {
			name->getComponent<TextBoxComponent>().isFocused = true;
		});

		create = &manager->createEntity<CharacterCreateScene>();
		create->addComponent<UIComponent>(600, 550, "CREATE", "vt323", WHITE, true);
		create->getComponent<UIComponent>().setOnClick([&]() {
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

	/*void handleEvent(SDL_Event event) override {
		printf("Handling events in create a caracter\n");
		manager->handleEntities<CharacterCreateScene>(event);
	}*/

	SDL_Color WHITE = { 255, 255, 255, 255 };
private:
	Entity* create;

	Entity* name;

	int skin = 1;
	Entity* playerSkin;
	Entity* nextSkin;
	Entity* prevSkin;

	int eye = 1;
	Entity* playerEyes;
	Entity* nextEyes;
	Entity* prevEyes;

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
		SDL_QueryTexture(Game::assets->GetTexture("skin" + std::to_string(skin)), NULL, NULL, &dest.w, &dest.h);

		dest.x = 0;
		dest.y = 0;

		SDL_SetRenderTarget(Game::renderer, target_texture);
		SDL_RenderCopy(Game::renderer, Game::assets->GetTexture("skin" + std::to_string(skin)), NULL, NULL);
		SDL_RenderCopy(Game::renderer, Game::assets->GetTexture("eyes" + std::to_string(eye)), NULL, NULL);

		SDL_SetRenderTarget(Game::renderer, NULL);

		// might not do this
		Utils::save_texture(Game::renderer, target_texture, "character.png");
		return target_texture;
	}
};
