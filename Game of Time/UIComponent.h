#pragma once

#include "ECS.h"
#include "AssetManager.h"
#include "Game.h"
#include "Utils.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class UIComponent : public Component {
private:
	SpriteComponent * sprite;

	typedef void(*eventFunction)();
	bool isCharacterCreate;

	SDL_Rect position;
	std::string labelText;
	std::string labelFont;
	SDL_Color textColor;
	SDL_Texture* labelTexture;

public:

	UIComponent(int xpos, int ypos, std::string text, std::string font, SDL_Color& color, bool characterCreate) :
		labelText(text), labelFont(font), textColor(color) {
		position.x = xpos;
		position.y = ypos;
		SetLabelText(labelText, labelFont);
		isCharacterCreate = characterCreate;
	}

	~UIComponent() {}

	void init() override {
		if (isCharacterCreate) {
			sprite = &entity->getComponent<SpriteComponent>();
		}
	}

	void update() override {
		if (Utils::isMouseOver(&position)) {
			SDL_Color gray = { 115, 115, 115, 255 };
			UIComponent::setTextColor(gray);

			SDL_Event e;
			SDL_PollEvent(&e);
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				onClick();
			}
		} else {
			SDL_Color white = { 255, 255, 255, 255 };
			UIComponent::setTextColor(white);
		}
	}

	void render() override {
		SDL_RenderCopy(Game::renderer, labelTexture, nullptr, &position);
	}

	/*void handleEvent(SDL_Event event) override {
		switch (event.type) {
		case SDL_MOUSEBUTTONDOWN:

			if (Utils::isMouseOver(&position)) {
				onClick();
			}

			break;
		default:
			printf("CODE: %d\n", event.type);
			break;
		}
	}*/

	SDL_Rect * getPosition() {
		return &position;
	}

	void setTextColor(SDL_Color &color) {
		textColor = color;
		SetLabelText(labelText, labelFont);
	}

	void SetLabelText(std::string text, std::string font) {
		TTF_Font * f = Game::assets->GetFont(font);
		if (!f) {
			printf("ERROR: %s\n", TTF_GetError());
			return;
		}
		SDL_Surface* surf = TTF_RenderText_Blended(f, text.c_str(), textColor);
		labelTexture = SDL_CreateTextureFromSurface(Game::renderer, surf);
		SDL_FreeSurface(surf);

		SDL_QueryTexture(labelTexture, nullptr, nullptr, &position.w, &position.h);
	}
};
