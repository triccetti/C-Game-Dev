#pragma once
#include <algorithm>
#include <array>
#include <bitset>
#include <deque>
#include <functional>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <map>
#include <memory>
#include <stack>
#include <sstream>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <utility>
#include <vector>
#include "SDL.h"

class Component;
class Entity;
class Scene;
class GameManager;

using ComponentID = std::size_t;
using SceneID = std::size_t;
using Group = std::size_t;

inline ComponentID getNewComponentTypeID() {
	static ComponentID lastID = 0u;
	return lastID++;
}

inline SceneID getNewSceneTypeID() {
	static SceneID lastID = 0u;
	return lastID++;
}

template <typename T> inline ComponentID getComponentTypeID() noexcept {
	static_assert (std::is_base_of<Component, T>::value, "");
	static ComponentID typeID = getNewComponentTypeID();
	return typeID;
}

template <typename T> inline SceneID getSceneTypeID() noexcept {
	static_assert (std::is_base_of<Scene, T>::value, "");
	static SceneID typeID = getNewSceneTypeID();
	return typeID;
}

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxScenes = 32;
constexpr std::size_t maxGroups = 32;

using GroupBitset = std::bitset<maxGroups>;
using ComponentBitSet = std::bitset<maxComponents>;
using SceneBitSet = std::bitset<maxScenes>;
using ComponentArray = std::array<Component*, maxComponents>;
using SceneArray = std::array<Scene*, maxScenes>;

enum class ComponentType { SPRITE, TILE, TRANSFORM, CONTROLLER, UI_BUTTON };

class Component {
public:
	virtual ~Component() {}
	virtual void init() {}
	virtual void update() {}
	virtual void render() {}

	bool isClickable() {
		return clickable;
	}

	Entity * entity;

private:
	bool clickable = false;
};

class Entity {
public:
	Entity(GameManager& man) : manager(man) {}

	void update() {
		for (auto& c : components) c->update();
	}

	/*
		Renders all components of the entity.
	*/
	void render() {
		for (auto& c : components) c->render();
	}

	/*
		Returns is the entity is active or not.
	*/
	bool isActive() { return active; }

	/*
		Sets the entity to be destroyed.
	*/
	void destroy() { active = false; }

	/* Returns true or false if the entity has the component. */
	template <typename T> bool hasComponent() const {
		return componentBitset[getComponentTypeID<T>()];
	}

	/*
		Adds a component of type T
	*/
	template <typename T, typename... TArgs> T& addComponent(TArgs&&... mArgs) {
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->entity = this;
		std::unique_ptr<Component>uPtr{ c };
		components.emplace_back(std::move(uPtr));

		componentArray[getComponentTypeID<T>()] = c;
		componentBitset[getComponentTypeID<T>()] = true;

		c->init();
		return *c;
	}

	/*
		Returns a pointer to the component of the type T.
		TODO: add checks if doesnt contain component.
	*/
	template<typename T> T& getComponent() {
		auto ptr(componentArray[getComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}

	void setSceneID(SceneID sceneID) {
		scene = sceneID;
	}

	SceneID getSceneID() {
		return scene;
	}

	bool hasGroup(Group mGroup) {
		return groupBitset[mGroup];
	}

	void addGroup(Group mGroup);
	void delGroup(Group mGroup) {
		groupBitset[mGroup] = false;
	}

	void setPlayer(bool p) { player = p; }

	bool isPlayer() { return player; }

private:
	GameManager& manager;
	bool active = true;
	bool player = false;

	std::vector<std::unique_ptr<Component>> components;

	// The scene the entity belongs too.
	SceneID scene;

	GroupBitset groupBitset;
	ComponentArray componentArray;
	ComponentBitSet componentBitset;
};

class Scene {
public:

	const static int FONT_SIZE = 24;

	virtual void init() {}
	virtual void update() {}
	virtual void render() {}

	//virtual void load() {}
	virtual void unload() {}
	virtual ~Scene() {}
	// virtual void  transitionIn();
	// virtual void  transitionOut();

	bool isInit() { return initialized; }

	virtual void addTile(int srcX, int srcY, int xpos, int ypos, int tileSize, int tilesetGid, int tileGid) {}

	GameManager * manager;
	bool initialized = false;

private:
};

class GameManager {
public:

	void update();
	void render();

	/*
	Creates an entity and returns a pointer to it.
	*/
	template <typename T> Entity& createEntity() {
		Entity *e = new Entity(*this);

		e->setSceneID(getSceneTypeID<T>());
		std::unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(std::move(uPtr));
		return *e;
	}

	/* Updates all entities in the given scene T. */
	template <typename T> void updateEntities() {
		auto& tiles(getGroup(Game::groupMap));
		auto& players(getGroup(Game::groupPlayers));
		auto& colliders(getGroup(Game::groupColliders));
		auto& projectiles(getGroup(Game::groupProjectiles));
		auto& ui(getGroup(Game::groupUI));

		SceneID currId = getSceneTypeID<T>();
		for (auto& t : tiles) {
			if (t->getSceneID() == currId) {
				t->update();
			}
		}

		for (auto& c : colliders) {
			if (c->getSceneID() == currId) {
				c->update();
			}
		}

		for (auto& p : players) {
			if (p->getSceneID() == currId) {
				p->update();
			}
		}

		for (auto& p : projectiles) {
			if (p->getSceneID() == currId) {
				p->update();
			}
		}

		for (auto& u : ui) {
			if (u->getSceneID() == currId) {
				u->update();
			}
		}
	}

	/* Renders all entities in the given scene T. */
	template <typename T> void renderEntities() {
		auto& tiles(getGroup(Game::groupMap));
		auto& players(getGroup(Game::groupPlayers));
		auto& colliders(getGroup(Game::groupColliders));
		auto& projectiles(getGroup(Game::groupProjectiles));
		auto& ui(getGroup(Game::groupUI));

		for (auto& t : tiles) {
			if (t->getSceneID() == getSceneTypeID<T>()) {
				t->render();
			}
		}

		for (auto& p : players) {
			if (p->getSceneID() == getSceneTypeID<T>()) {
				p->render();
			}
		}

		for (auto& p : projectiles) {
			if (p->getSceneID() == getSceneTypeID<T>()) {
				p->render();
			}
		}

		for (auto& u : ui) {
			if (u->getSceneID() == getSceneTypeID<T>()) {
				u->render();
			}
		}
	}

	/* Destroys all entities in the given scene T. */
	template <typename T> void destroyEntities() {
		for (auto& e : entities) {
			if (e->getSceneID() == getSceneTypeID<T>()) {
				e->destroy();
			}
		}
	}

	void refresh() {
		entities.erase(std::remove_if(std::begin(entities), std::end(entities),
			[](const std::unique_ptr<Entity> &mEntity) {
			return !mEntity->isActive();
		}),
			std::end(entities));
	}

	/*
		Returns true if the Game has scene of type T otherwise returns false.
	*/
	template <typename T> bool hasScene() const {
		return sceneBitset[getSceneTypeID<T>()];
	}

	/*
		Adds a scene of type T to the Game manager and returns it.
	*/
	template <typename T, typename... TArgs> T& addScene(TArgs&&... mArgs) {
		T* s(new T(std::forward<TArgs>(mArgs)...));
		s->manager = this;
		std::unique_ptr<Scene>uPtr{ s };
		scenes.emplace_back(std::move(uPtr));

		sceneArray[getSceneTypeID<T>()] = s;
		sceneBitset[getSceneTypeID<T>()] = true;

		return *s;
	}

	/*
		Returns the scene of type T in the Game manager.
	*/
	template<typename T> T& getScene() const {
		auto ptr(sceneArray[getSceneTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}

	/*
		Updates the current scene to a scene of type T.
	*/
	template<typename T> void changeScene() {
		if (currentSceneID < maxScenes) {
			auto& currentScene(sceneArray[currentSceneID]);
			currentScene->unload();
		}

		currentSceneID = getSceneTypeID<T>();

		auto& newScene(sceneArray[currentSceneID]);

		sceneStack.push(currentSceneID);
	}

	/*
		Returns true or false if the current scene is equal to the type of scene passed in.
	*/
	template<typename T> bool isCurrentScene() {
		return getSceneTypeID<T>() == currentSceneID;
	}

	void prevScene() {
		if (!sceneStack.empty()) {
			currentSceneID = sceneStack.top();
			sceneStack.pop();
		}
	}

	void AddToGroup(Entity* mEntity, Group mGroup) {
		groupedEntities[mGroup].emplace_back(mEntity);
	}

	std::vector<Entity*>& getGroup(Group mGroup) {
		return groupedEntities[mGroup];
	}

private:
	std::vector<std::unique_ptr<Scene>> scenes;
	std::vector<std::unique_ptr<Entity>> entities;
	std::array<std::vector<Entity*>, maxGroups> groupedEntities;

	std::stack<SceneID> sceneStack;

	SceneArray sceneArray;
	SceneBitSet sceneBitset;
	SceneID currentSceneID = maxScenes;
};