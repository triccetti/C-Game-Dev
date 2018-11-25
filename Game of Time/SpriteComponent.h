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
	TransformComponent * transform;

	SDL_Texture * texture;
	SDL_Rect srcRect, destRect;
	int index;

	bool animated = false;
	int textureHeight;
	int textureWidth;
	int totalFrames = -1;

	int speed = 120;
	int frames = 0;
	int delay = 0;
	int elapsedTime = 0;

	std::string currAnimName = "";

public:
	int animationIndex = 0;
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
		Animation idle = Animation(0, 1, speed, 0);
		animations.emplace("idle", idle);
		anims = { "idle" };
		index = 0;
		playAnim("idle");
	}

	~SpriteComponent() {}

	void setTex(std::string id) {
		texture = Game::assets->GetTexture(id);

		SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight);
	}

	void init() override {
		transform = &entity->getComponent<TransformComponent>();

		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;
	}

	void update() override {
		if (animated) {
			totalFrames = (textureWidth / transform->width);
			int currTime = static_cast<int>(SDL_GetTicks());
			// TODO figure out how to see if animation is over
			int col = (static_cast<int>((currTime / speed) % frames) + animationIndex) % totalFrames;
			srcRect.x = col * transform->width;
		}

		int row = animationIndex / totalFrames;
		srcRect.y = row * transform->height;

		destRect.x = static_cast<int>(transform->position.x - Game::camera.x);
		destRect.y = static_cast<int>(transform->position.y - Game::camera.y);
		destRect.w = transform->width * transform->scale;
		destRect.h = transform->height * transform->scale;
	}

	void render() override {
		if (texture)
			TextureManager::RenderTexture(texture, srcRect, destRect, spriteFlip);
	}

	void playAnim(std::string animName) {
		std::map<std::string, Animation>::iterator it = animations.find(animName);
		Animation anim;
		if (it != animations.end() && animName != currAnimName) {
			currAnimName = animName;
			animated = true;
			frames = animations[animName].frames;
			animationIndex = animations[animName].index;
			speed = animations[animName].speed;
			delay = animations[animName].delay;

			auto it = std::find(anims.begin(), anims.end(), animName);
			index = it - anims.begin();
		}
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
		std::cout << "frames: " << animations[anims[index]].frames
			<< "\nindex: " << animations[anims[index]].index
			<< "\nspeed: " << animations[anims[index]].speed << std::endl;
	}
};