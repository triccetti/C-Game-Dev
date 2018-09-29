#pragma once

#include "ECS.h"
#include "Vector2D.h"
#include "Game.h"
#include "TextureManager.h"
 
class TileComponent : public Component {
public:
	SDL_Texture * texture;
	SDL_Rect srcRect, destRect;
	Vector2D position;

	TileComponent() {
	}

	~TileComponent() {
		SDL_DestroyTexture(texture);
	}

	TileComponent(int srcX, int srcY, int xpos, int ypos, int tileSize, int scale, std::string id) {
		texture = Game::assets->GetTexture(id);
		srcRect.x = srcX;
		srcRect.y = srcY;
		srcRect.w = srcRect.h = tileSize;
		position.x = static_cast<float>(xpos);
		position.y = static_cast<float>(ypos);
		destRect.w = destRect.h = scale;
	}

	void update() override {
		printf("Tile update\n");
		destRect.x = static_cast<int>(position.x - Game::camera.x);
		destRect.y = static_cast<int>(position.y - Game::camera.y);
	}

	void render() override {
		printf("Tile render\n");
		TextureManager::RenderTexture(texture, srcRect, destRect, SDL_FLIP_NONE);
	}
};