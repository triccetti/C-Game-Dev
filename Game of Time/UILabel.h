#pragma once

#include "ECS.h"
#include "AssetManager.h"
#include "Game.h"
#include "Utils.h"
#include "Components.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class UILabel : public Component {
private:
	//SpriteComponent * sprite;

	//typedef void(*eventFunction)();

	SDL_Rect position;
	std::string labelText;
	std::string labelFont;
	SDL_Texture* labelTexture;
	SDL_Color textColor;

public:

	UILabel(int xpos, int ypos, std::string text, std::string font, SDL_Color& color) :
		labelText(text), labelFont(font), textColor(color) {
		position.x = xpos;
		position.y = ypos;
		SetLabelText(labelText, labelFont);
	}

	~UILabel() {}

	void render() override {
		SDL_RenderCopy(Game::renderer, labelTexture, nullptr, &position);
	}

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
