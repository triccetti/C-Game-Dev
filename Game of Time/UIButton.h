#pragma once

#include "ECS.h"
#include "AssetManager.h"
#include "Game.h"
#include "Utils.h"
#include "Components.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class UIButton : public Component {
private:
	SDL_Rect position;
	SDL_Rect srcRect, destRect;

	UILabel* label;

	bool isTextButton = false;
	bool isHover = false;

	SDL_Texture * buttonTexture;
	SDL_Texture * hoverTexture;

public:
	SDL_Color WHITE = { 255,255,255,255 };
	SDL_Color GRAY = { 115,115,115,255 };

	std::function<void()> onClick;

	UIButton() {};

	UIButton(std::function<void()> func) {
		isTextButton = true;
		onClick = func;
	}

	UIButton(std::function<void()> func, int srcX, int srcY, int xpos, int ypos, int tileSize, int scale, std::string buttonImg, std::string buttonImgHover) {
		position.x = xpos;
		position.y = ypos;

		srcRect.x = srcX;
		srcRect.y = srcY;
		srcRect.w = srcRect.h = tileSize;
		destRect.w = destRect.h = scale;

		buttonTexture = Game::assets->GetTexture(buttonImg);
		hoverTexture = Game::assets->GetTexture(buttonImgHover);
	}

	~UIButton() {
		SDL_DestroyTexture(buttonTexture);
		SDL_DestroyTexture(hoverTexture);
	}

	void init() override {
		if (isTextButton) {
			label = &entity->getComponent<UILabel>();
			position = *label->getPosition();
		}
	}

	void update() override {
		if (Game::event.type == SDL_MOUSEBUTTONDOWN && Utils::isMouseOver(&position)) {
			isHover = true;
			printf("clicked!\n");
			onClick();
		} else if (Utils::isMouseOver(&position)) {
			isHover = true;
			label->setTextColor(GRAY);
		} else {
			isHover = false;
			label->setTextColor(WHITE);
		}
	}

	void render() override {
		if (!isTextButton) {
			if (isHover) {
				TextureManager::RenderTexture(hoverTexture, srcRect, destRect, SDL_FLIP_NONE);
			} else {
				TextureManager::RenderTexture(buttonTexture, srcRect, destRect, SDL_FLIP_NONE);
			}
		}
	}
};
