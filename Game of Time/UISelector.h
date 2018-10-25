#pragma once

#include "ECS.h"
#include "AssetManager.h"
#include "Game.h"
#include "Utils.h"
#include "Components.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class UISelector : public Component {
private:
	SDL_Rect nextPosition, prevPosition;

	std::string nextText, prevText;
	std::string labelFont;
	SDL_Texture* nextTexture;
	SDL_Texture* prevTexture;

	std::string selectorTexture;

	bool isHover = false;
	int index = 1;
	int max = 1;
	SpriteComponent * sprite;

public:
	SDL_Color WHITE = { 255,255,255,255 };
	SDL_Color GRAY = { 115,115,115,255 };

	std::function<void()> onClick;

	UISelector(std::string texture, std::string font,
		int maxTextures, int nextXpos, int nextYpos, int prevXpos, int prevYpos)
		: labelFont(font), selectorTexture(texture), max(maxTextures) {
		nextPosition.x = nextXpos;
		nextPosition.y = nextYpos;
		prevPosition.x = prevXpos;
		prevPosition.y = prevYpos;
		nextText = "->";
		prevText = "<-";
	};

	~UISelector() {
	}

	void init() override {
		sprite = &entity->getComponent<SpriteComponent>();
		setNextColor(WHITE);
		setPrevColor(WHITE);
	}

	void update() override {
		if (Game::event.type == SDL_MOUSEBUTTONDOWN) {
			if (Utils::isMouseOver(&nextPosition)) {
				index++;
				if (index > max) index = 1;
				sprite->setTex(selectorTexture + std::to_string(index));
			} else if (Utils::isMouseOver(&prevPosition)) {
				index--;
				if (index <= 0) index = max;
				sprite->setTex(selectorTexture + std::to_string(index));
			}
		}
		if (Utils::isMouseOver(&nextPosition)) {
			setNextColor(GRAY);
		} else {
			setNextColor(WHITE);
		}

		if (Utils::isMouseOver(&prevPosition)) {
			setPrevColor(GRAY);
		} else {
			setPrevColor(WHITE);
		}
	}

	void render() override {
		SDL_RenderCopy(Game::renderer, nextTexture, nullptr, &nextPosition);
		SDL_RenderCopy(Game::renderer, prevTexture, nullptr, &prevPosition);
	}

	int getSelectedIndex() {
		return index;
	}

	void setNextColor(SDL_Color &color) {
		TTF_Font * f = Game::assets->GetFont(labelFont);
		if (!f) {
			printf("ERROR: %s\n", TTF_GetError());
			return;
		}
		SDL_Surface* surf = TTF_RenderText_Blended(f, nextText.c_str(), color);
		nextTexture = SDL_CreateTextureFromSurface(Game::renderer, surf);
		SDL_FreeSurface(surf);

		SDL_QueryTexture(nextTexture, nullptr, nullptr, &nextPosition.w, &nextPosition.h);
	}

	void setPrevColor(SDL_Color &color) {
		TTF_Font * f = Game::assets->GetFont(labelFont);
		if (!f) {
			printf("ERROR: %s\n", TTF_GetError());
			return;
		}
		SDL_Surface* surf3 = TTF_RenderText_Blended(f, prevText.c_str(), color);
		prevTexture = SDL_CreateTextureFromSurface(Game::renderer, surf3);
		SDL_FreeSurface(surf3);

		SDL_QueryTexture(prevTexture, nullptr, nullptr, &prevPosition.w, &prevPosition.h);
	}
};
