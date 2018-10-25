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
	SDL_Rect errorPos;

	std::string inputText;
	std::string labelFont;
	std::string errorText;
	SDL_Texture* labelTexture;
	SDL_Texture* errorTexture;
	SDL_Color textColor;
	SDL_Color errorColor;

	unsigned int len = 0;
	bool isFocused = false;
	bool error = false;

public:

	TextBoxComponent(int xpos, int ypos, int textLen, std::string font,
		SDL_Color& color, SDL_Color& eColor) :
		labelFont(font), textColor(color), errorColor(eColor), len(textLen) {
		textBox.x = xpos;
		textBox.y = ypos;
		textBox.h = 25;
		textBox.w = 150;
		errorPos.x = xpos;
		errorPos.y = ypos - 25;

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
				error = false; 
			} else {
				isFocused = false;
			}
		}

		if (Game::event.type == SDL_KEYDOWN) {
			if (Game::event.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0) {
				inputText = inputText.substr(0, inputText.length() - 1);
				error = false;
			}
		} else if (Game::event.type == SDL_TEXTINPUT) {
			if (!((Game::event.text.text[0] == 'c' || Game::event.text.text[0] == 'C')
				&& (Game::event.text.text[0] == 'v' || Game::event.text.text[0] == 'V')
				&& SDL_GetModState() & KMOD_CTRL)) {
				if (inputText.length() <= len) {
					inputText += Game::event.text.text;
					error = false;
				} else {
					displayError("Input too long");
				}
			}
		}

		SetLabelText(inputText, labelFont);
	}

	void render() override {
		// Set render color to blue ( rect will be rendered in this color )
		if (isFocused && !error) {
			SDL_SetRenderDrawColor(Game::renderer, 115, 115, 115, 255);
		} else if (error) {
			SDL_SetRenderDrawColor(Game::renderer, errorColor.r, errorColor.g, errorColor.b, errorColor.a);
		} else {
			SDL_SetRenderDrawColor(Game::renderer, textColor.r, textColor.g, textColor.b, textColor.a);
		}

		// Render rect
		SDL_RenderDrawRect(Game::renderer, &textBox);
		SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 255);
		SDL_RenderCopy(Game::renderer, labelTexture, nullptr, &textBox);

		if (error) { 
			SDL_RenderCopy(Game::renderer, errorTexture, nullptr, &errorPos); 
		}
	}

	SDL_Rect * getPosition() {
		return &position;
	}

	std::string getText() {
		return inputText;
	}

	void setTextColor(SDL_Color &color) {
		textColor = color;
		SetLabelText(inputText, labelFont);
	}

	void displayError(std::string eText) {
		errorText = eText;
		error = true;
		setErrorText(errorText, labelFont);
	}

	void SetLabelText(std::string text, std::string font) {
		TTF_Font * f = Game::assets->GetFont(font);
		if (!f) {
			printf("ERROR: %s\n", TTF_GetError());
			return;
		}

		// appends white space to the end of the string.
		std::string display = text;
		int l = (len - text.length());
		for (int i = 0; i <= l; i++) {
			display += " ";
		}

		SDL_Surface* surf = TTF_RenderText_Blended(f, display.c_str(), textColor);
		labelTexture = SDL_CreateTextureFromSurface(Game::renderer, surf);
		SDL_FreeSurface(surf);

		SDL_QueryTexture(labelTexture, nullptr, nullptr, &position.w, &position.h);
	}

	void setErrorText(std::string text, std::string font) {
		TTF_Font * f = Game::assets->GetFont(font);
		if (!f) {
			printf("ERROR: %s\n", TTF_GetError());
			return;
		}
		SDL_Surface* surf = TTF_RenderText_Blended(f, text.c_str(), errorColor);
		errorTexture = SDL_CreateTextureFromSurface(Game::renderer, surf);
		SDL_FreeSurface(surf);

		SDL_QueryTexture(errorTexture, nullptr, nullptr, &errorPos.w, &errorPos.h);
	}
};
