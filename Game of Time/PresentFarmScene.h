#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>

#include "Game.h"
#include "Scenes.h"
#include "Components.h"

class PresentFarmScene : public Scene {
private:
	std::string playerID;
	std::string eyesID;

	Entity * player;
	CollisionComponent * cc;
public:

	PresentFarmScene(std::string playerName, std::string eyes) : playerID(playerName), eyesID(eyes) {
		printf("Welcome to the game %s\n", playerName.c_str());
	};

	~PresentFarmScene() {}

	void init() override {
		initialized = true;
		//initMap("terrain", "Assets\\map.mmap", 4);
		Game::assets->LoadTileMap("Assets\\map.tmx", this, &Game::viewPort);

		player = &manager->createEntity<PresentFarmScene>();
		player->addComponent<TransformComponent>(50.0f, 50.0f, 32, 16, 4);

		SpriteComponent * skin = &player->addComponent<SpriteComponent>(playerID);
		SpriteComponent * eye = &player->addComponent<SpriteComponent>(eyesID);

		eye->addAnimation("idle", Animation(0, 1, 500, false, "front-blink"));
		eye->addAnimation("front-blink", Animation(0, 4, 300, false, "idle"));
		eye->addAnimation("right-blink", Animation(4, 4, 250));
		eye->addAnimation("none", Animation(8, 4, 250));
		eye->addAnimation("left-blink", Animation(12, 4, 250));

		skin->addAnimation("front-walk", Animation(0, 4, 200));
		skin->addAnimation("right-walk", Animation(4, 4, 200));
		skin->addAnimation("back-walk", Animation(8, 4, 200));
		skin->addAnimation("left-walk", Animation(12, 4, 200));
		skin->addAnimation("front-idle", Animation(0, 1, 200));
		skin->addAnimation("right-idle", Animation(4, 1, 200));
		skin->addAnimation("back-idle", Animation(8, 1, 200));
		skin->addAnimation("left-idle", Animation(12, 1, 200));

		eye->playAnim("idle");
		skin->playAnim("front-idle");

		player->addComponent<ControllerComponent>(skin, eye);
		player->addComponent<CollisionComponent>("player");
		cc = &player->addComponent<CollisionComponent>("player-feet");

		player->addGroup(Game::groupPlayers);
	}

	void update() override {
		SDL_Rect playerCol = cc->collider;
		Vector2D playerPos = player->getComponent<TransformComponent>().position;

		manager->refresh();
		manager->updateEntities<PresentFarmScene>();

		auto& colliders = manager->getGroup(Game::groupColliders);

		for (auto& c : colliders) {
			SDL_Rect cCol = c->getComponent<CollisionComponent>().collider;
			if (Utils::collision(cCol, playerCol)) {
				player->getComponent<TransformComponent>().position = playerPos;
			}
		}

		Game::camera.x = static_cast<int>(player->getComponent<TransformComponent>().position.x - (Game::camera.w / 2));
		Game::camera.y = static_cast<int>(player->getComponent<TransformComponent>().position.y - (Game::camera.h / 2));
	}

	void render() override {
		manager->renderEntities<PresentFarmScene>();
	}

	void unload() override {
		manager->destroyEntities<PresentFarmScene>();
		initialized = false;
	}

	// TODO: scale needs to be some constant in properties file.
	void addTile(int srcX, int srcY, int xpos, int ypos, int tileSize, int tilesetGid, int tileGid) override {
		Entity * tile = &manager->createEntity<PresentFarmScene>();
		std::string terrainType = Game::assets->GetTerrain(tileGid);
		tile->addGroup(Game::groupMap);
		tile->addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, 4, tilesetGid);
		if (terrainType == "cliff") {
			tile->addComponent<TransformComponent>(static_cast<float>(xpos * 4), static_cast<float>(ypos * 4), tileSize, 4);
			tile->addComponent<CollisionComponent>(terrainType);
			tile->addGroup(Game::groupColliders);
		} else if (terrainType == "dirt") {
			// you can plant stuff on this!
		} else if (terrainType == "left-edge") {
			tile->addComponent<TransformComponent>(static_cast<float>(xpos * 4), static_cast<float>(ypos * 4), tileSize, 1, 4);
			tile->addComponent<CollisionComponent>(terrainType);
			tile->addGroup(Game::groupColliders);
		} else if (terrainType == "right-edge") {
			tile->addComponent<TransformComponent>(static_cast<float>((xpos * 4) + (tileSize * 4) - 4), static_cast<float>(ypos * 4), tileSize, 1, 4);
			tile->addComponent<CollisionComponent>(terrainType);
			tile->addGroup(Game::groupColliders);
		}
	}
};
