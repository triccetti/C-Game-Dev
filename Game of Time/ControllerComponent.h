#pragma once

#include "Game.h"
#include "ECS.h"
#include "Components.h"

class ControllerComponent : public Component {
public:
	//std::shared_ptr<TransformComponent> transform;
	//std::shared_ptr<SpriteComponent> sprite;
	TransformComponent * transform;
	SpriteComponent * skinSprite;
	SpriteComponent * eyesSprite;

	ControllerComponent(SpriteComponent * skin, SpriteComponent * eye) {
		skinSprite = skin;
		eyesSprite = eye;
	}

	void init() override {
		transform = &entity->getComponent<TransformComponent>();
	}

	void update() override {
		if (Game::event.type == SDL_KEYDOWN) {
			printf("KEY PRESSED %d\n", Game::event.key.keysym.sym);
			switch (Game::event.key.keysym.sym) {
			case SDLK_w:
				transform->velocity.y = -1;
				skinSprite->playAnim("back-walk");
				eyesSprite->playAnim("none");
				break;
			case SDLK_a:
				transform->velocity.x = -1;
				skinSprite->playAnim("left-walk");
				eyesSprite->playAnim("left-blink");
				break;
			case SDLK_d:
				transform->velocity.x = 1;
				skinSprite->playAnim("right-walk");
				eyesSprite->playAnim("right-blink");
				break;
			case SDLK_s:
				transform->velocity.y = 1;
				skinSprite->playAnim("front-walk");
				eyesSprite->playAnim("front-blink");
				break;
			default:
				break;
			}
		} else if (Game::event.type == SDL_KEYUP) {
			printf("KEY UP %d\n", Game::event.key.keysym.sym);
			switch (Game::event.key.keysym.sym) {
			case SDLK_w:
				transform->velocity.y = 0;
				skinSprite->playAnim("back-idle");
				eyesSprite->playAnim("none");
				break;
			case SDLK_a:
				transform->velocity.x = 0;
				skinSprite->playAnim("left-idle");
				eyesSprite->playAnim("left-blink");
				break;
			case SDLK_d:
				transform->velocity.x = 0;
				skinSprite->playAnim("right-idle");
				eyesSprite->playAnim("right-blink");
				break;
			case SDLK_s:
				transform->velocity.y = 0;
				skinSprite->playAnim("front-idle");
				eyesSprite->playAnim("front-blink");
				break;
			case SDLK_ESCAPE:
				Game::running = false;
			default:
				break;
			}
		}
	}
};