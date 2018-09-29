#pragma once

#include "SDL.h"
#include "Components.h"
#include "TextureManager.h"
#include "Animation.h"
#include "AssetManager.h"

#include <iostream>
#include <map>
#include <vector>
#include <string>

class SpriteComponent : public Component {
private:
	//std::shared_ptr<TransformComponent> transform;
	TransformComponent * transform;

	SDL_Texture * texture;
	SDL_Rect srcRect, destRect;

	bool animated = false;
	int frames = 1;
	int speed = 100;
	int index = -1;

public:
	int animIndex = 0;
	std::map<std::string, Animation> animations;
	std::vector<std::string> anims;

	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	SpriteComponent(std::string id) {
		setTex(id);
		animated = false;
	}

	SpriteComponent(std::string id, bool isAnimated) {
		animated = isAnimated;
		setTex(id);
		Animation idle = Animation(0, 1, speed);
		animations.emplace("idle", idle);
		anims = { "idle" };
		index = 0;
		std::cout << "current anim " << index << std::endl;
		playAnim("idle");
	}

	~SpriteComponent() {}

	void setTex(std::string id) {
		texture = Game::assets->GetTexture(id);
	}

	void init() override {
		transform = &entity->getComponent<TransformComponent>();

		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;
	}

	void update() override {
		if (animated) {
			srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
		}

		srcRect.y = animIndex * transform->height;

		destRect.x = static_cast<int>(transform->position.x - Game::camera.x);
		destRect.y = static_cast<int>(transform->position.y - Game::camera.y);
		destRect.w = transform->width * transform->scale;
		destRect.h = transform->height * transform->scale;
	}

	void render() override {
		TextureManager::RenderTexture(texture, srcRect, destRect, spriteFlip);
	}

	void playAnim(std::string animName) {
		frames = animations[animName].frames;
		animIndex = animations[animName].index;
		speed = animations[animName].speed;

		auto it = std::find(anims.begin(), anims.end(), animName);
		index = it - anims.begin();
	}

	void addAnimation(std::string animName, Animation anim) {
		if (anims.size() == -1) {
			anims = { animName };
			index = 0;
		}

		animations.emplace(animName, anim);
		anims.push_back(animName);
	}

	void nextAnimation() {
		index++;
		if (index == anims.size()) {
			index = 0;
		}
		playAnim(anims[index]);
		std::cout << index << std::endl;
		std::cout << "frames: " << animations[anims[index]].frames
			<< "\nindex: " << animations[anims[index]].index
			<< "\nspeed: " << animations[anims[index]].speed << std::endl;

		std::cout << srcRect.x << " " << srcRect.y << " " << srcRect.h << " " << srcRect.w << std::endl;
	}
};