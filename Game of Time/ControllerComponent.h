#pragma once

#include "Game.h"
#include "ECS.h"
#include "Components.h"

class ControllerComponent : public Component {
public:
	//std::shared_ptr<TransformComponent> transform;
	//std::shared_ptr<SpriteComponent> sprite;
	TransformComponent * transform;
	SpriteComponent * sprite;

	ControllerComponent() {
	}

	void init() override {
		printf("Controller init\n");
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override {
		printf("Controller update\n");
		if (Game::event.type == SDL_KEYDOWN) {
			switch (Game::event.key.keysym.sym) {
			case SDLK_w:
				transform->velocity.y = -1;
				sprite->playAnim("walk");
				break;
			case SDLK_a:
				transform->velocity.x = -1;
				sprite->playAnim("walk");
				break;
			case SDLK_d:
				transform->velocity.x = 1;
				sprite->playAnim("walk");
				break;
			case SDLK_s:
				transform->velocity.y = 1;
				sprite->playAnim("walk");
				break;
			default:
				break;
			}
		}

		if (Game::event.type == SDL_KEYUP) {
			switch (Game::event.key.keysym.sym) {
			case SDLK_w:
				transform->velocity.y = 0;
				sprite->playAnim("idle");
				break;
			case SDLK_a:
				transform->velocity.x = 0;
				sprite->playAnim("idle");
				break;
			case SDLK_d:
				transform->velocity.x = 0;
				sprite->playAnim("idle");
				break;
			case SDLK_s:
				transform->velocity.y = 0;
				sprite->playAnim("idle");
				break;
			case SDLK_ESCAPE:
				Game::running = false;
			default:
				break;
			}
		}
	}
};