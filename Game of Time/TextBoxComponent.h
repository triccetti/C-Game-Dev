#pragma once

#include "ECS.h"
#include "AssetManager.h"
#include "Game.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class TextBoxComponent : public Component {
private:

	typedef void(*eventFunction)();

	SDL_Rect position;
	std::string inputText;
	std::string labelFont;
	SDL_Color textColor;
	SDL_Texture* labelTexture;

public:
	bool isFocused = false;

	TextBoxComponent(int xpos, int ypos, std::string font, SDL_Color& color) : labelFont(font), textColor(color) {
		position.x = xpos;
		position.y = ypos;
		SetLabelText(inputText, labelFont);
	}

	~TextBoxComponent() {
		SDL_StopTextInput();
	}

	void init() override {
		inputText = "";
	}

	void update() override {
		int x, y;
		SDL_GetMouseState(&x, &y);

		SDL_Rect * pos = TextBoxComponent::getPosition();
		//if (x <= pos->x + pos->w && x >= pos->x
		//	&& y <= pos->y + pos->h && y >= pos->y) {
		//	SDL_Color gray = { 115, 115, 115, 255 };
		//	TextBoxComponent::setTextColor(gray);

		SDL_Event e;
		SDL_WaitEvent(&e);
		//if (e.type == SDL_MOUSEBUTTONDOWN) {
		//	entity->onClick();
		//}
		if (e.type == SDL_KEYDOWN) {
			if (e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0) {
				inputText = inputText.substr(0, inputText.length() - 1);
			} else if (e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL) {
				SDL_SetClipboardText(inputText.c_str());
			} else if (e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) {
				inputText = SDL_GetClipboardText();
			}
		} else if (e.type == SDL_TEXTINPUT) {
			if (!((e.text.text[0] == 'c' || e.text.text[0] == 'C')
				&& (e.text.text[0] == 'v' || e.text.text[0] == 'V')
				&& SDL_GetModState() & KMOD_CTRL)) {
				inputText += e.text.text;
			}
		}
		SetLabelText(inputText, labelFont);
		//} else {
		//	SDL_Color g = { 0, 255, 255, 255 };
		//	TextBoxComponent::setTextColor(g);
		//	isFocused = false;
		//}
	}

	void render() override {
		SDL_RenderCopy(Game::renderer, labelTexture, nullptr, &position);
	}

	SDL_Rect * getPosition() {
		return &position;
	}

	void setTextColor(SDL_Color &color) {
		textColor = color;
		SetLabelText(inputText, labelFont);
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
