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
		if (Utils::isMouseOver(&position)) {
			isHover = true;
			if (isTextButton) {
				SDL_Color gray = { 115, 115, 115, 255 };
				label->setTextColor(gray);
			}
			SDL_Event e;
			SDL_PollEvent(&e);
			if(e.type == SDL_MOUSEBUTTONDOWN) { 
				printf("clicked!\n");
				onClick();
			}
		} else {
			isHover = false;
			if (isTextButton) {
				SDL_Color white = { 255,255,255, 255 };
				label->setTextColor(white);
			} 
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
