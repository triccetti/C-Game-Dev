#pragma once

#include "ECS.h"
#include "AssetManager.h"
#include "Game.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class TextBoxComponent : public Component {
private:
	std::function<void()> onClick;

	SDL_Rect position;
	SDL_Rect textBox;

	std::string inputText;
	std::string labelFont;
	SDL_Texture* labelTexture;
	SDL_Color textColor;

public:
	bool isFocused = false;

	TextBoxComponent(int xpos, int ypos, std::string font, SDL_Color& color) : labelFont(font), textColor(color) {
		textBox.x = xpos;
		textBox.y = ypos;
		textBox.h = 25;
		textBox.w = 150;
		inputText = "";

		onClick = [&]() {
			printf("FOCUSED!\n");
		};

		SetLabelText(inputText, labelFont);
	}

	~TextBoxComponent() {
	}

	void update() override {
		if (Game::event.type == SDL_MOUSEBUTTONDOWN) {
			if (Utils::isMouseOver(&textBox)) {
				isFocused = true;
			} else {
				isFocused = false;
			}
		}

		if (Game::event.type == SDL_KEYDOWN) {
			if (Game::event.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0) {
				inputText = inputText.substr(0, inputText.length() - 1);
				printf("%s\n", inputText.c_str());
			}
		} else if (Game::event.type == SDL_TEXTINPUT) {
			if (!((Game::event.text.text[0] == 'c' || Game::event.text.text[0] == 'C')
				&& (Game::event.text.text[0] == 'v' || Game::event.text.text[0] == 'V')
				&& SDL_GetModState() & KMOD_CTRL)) {
				inputText += Game::event.text.text;
				printf("%s\n", inputText.c_str());
			}
		}
		SetLabelText(inputText, labelFont);
	}

	void render() override {
		// Set render color to blue ( rect will be rendered in this color )
		if (isFocused) {
			SDL_SetRenderDrawColor(Game::renderer, 115, 115, 115, 255);
		} else {
			SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 255);
		}
		// Render rect
		SDL_RenderDrawRect(Game::renderer, &textBox);
		SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 255);
		SDL_RenderCopy(Game::renderer, labelTexture, nullptr, &textBox);
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
