#pragma once
#include "Components.h"
#include "TextureManager.h"

class CollisionComponent : public Component {
public:

	SDL_Rect collider;
	std::string tag;

	TransformComponent* transform;

	CollisionComponent(std::string t) {
		tag = t;
	}

	CollisionComponent(std::string t, int xpos, int ypos, int size) {
		tag = t;
		collider.x = xpos;
		collider.y = ypos;
		collider.h = collider.w = size;
	}

	void init() override {
		if (!entity->hasComponent<TransformComponent>()) {
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();
	}

	void update() override {
		if (tag == "player" || tag == "cliff" || tag == "left-edge" || tag == "right-edge") {
			collider.x = static_cast<int>(transform->position.x - Game::camera.x);
			collider.y = static_cast<int>(transform->position.y - Game::camera.y);
		}
		if (tag == "player-feet") {
			collider.x = static_cast<int>(transform->position.x - Game::camera.x);
			collider.w = transform->width * transform->scale;
			collider.h = (transform->height * transform->scale) / 4;
			collider.y = static_cast<int>((transform->position.y + (collider.h * 3)) - Game::camera.y);
		} else {
			collider.w = transform->width * transform->scale;
			collider.h = transform->height * transform->scale;
		}
	}

	void render() override {
		SDL_SetRenderDrawColor(Game::renderer, 255, 0, 0, 255);
		SDL_RenderDrawRect(Game::renderer, &collider);
		SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 255);
	}
};