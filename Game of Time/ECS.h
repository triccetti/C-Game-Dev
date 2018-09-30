#pragma once
#include <map>
#include <string>
#include <typeindex>
#include <vector>
#include <iostream>
#include <typeinfo>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>
#include <utility>
#include <stack>
#include <deque>
#include <functional>
#include "SDL.h"

class Component;
class Entity;
class Scene;
class GameManager;

using ComponentID = std::size_t;
using SceneID = std::size_t;

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
	//virtual void handleEvent(SDL_Event event) {}

	void setOnClick(std::function<void()> func);
	void setOnKeyPressed(std::function<void()> func);

	bool isClickable() {
		return clickable;
	}

	//std::function<void()> onClick;
	//std::function<void()> onKeyPressed;

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
		Handles events for all components of the entity.

	void handleEvent(SDL_Event event) {
		for (auto& c : components) {
			if (c->isClickable()) {
				c->handleEvent(event);
			}
		}
	}*/

	/*
		Returns is the entity is active or not.
	*/
	bool isActive() { return active; }

	/*
		Returns if the entity is dead or not.
	*/
	bool isDead() { return dead; }

	/*
		Sets the entities active value to the given value.
	*/
	void setActive(bool act) {
		active = act;
	}

	/*
		Sets the entity to be destroyed.
	*/
	void destroy() { dead = true; }

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

	template<typename T> void setSceneID() {
		scene = getSceneTypeID<T>();
	}

	SceneID getSceneID() {
		return scene;
	}

private:
	GameManager& manager;
	bool active = true;
	bool dead = false;

	std::vector<std::unique_ptr<Component>> components;

	// The scene the entity belongs too.
	SceneID scene;

	ComponentArray componentArray;
	ComponentBitSet componentBitset;
};

class Scene {
public:

	const static int FONT_SIZE = 24;

	virtual void init() {}
	virtual void update() {}
	virtual void render() {}
	//virtual void handleEvent(SDL_Event event) {}

	//virtual void load() {}
	//virtual void unload() {}
	virtual ~Scene() {}
	// virtual void  transitionIn();
	// virtual void  transitionOut();

	GameManager * manager;
private:
	//Map * map;
};

class GameManager {
public:

	// Updates the current scene.
	void update();
	void render();
	//void handleEvents(SDL_Event event);

	/*
	Creates an entity and returns a pointer to it.
	*/
	template <typename T> Entity& createEntity() {
		Entity *e = new Entity(*this);
		e->setSceneID<T>();
		std::unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(std::move(uPtr));
		return *e;
	}

	/* Updates all entities in the given scene T. */
	template <typename T> void updateEntities() {
		for (auto& e : entities) {
			if (e->getSceneID() == getSceneTypeID<T>()) {
				e->update();
			}
		}
	}

	/* Updates all entities in the given scene T. */
	template <typename T> void renderEntities() {
		for (auto& e : entities) {
			if (e->getSceneID() == getSceneTypeID<T>()) {
				e->render();
			}
		}
	}

	/* Handles events for any entities in the given scene T.
	template <typename T> void handleEntities(SDL_Event event) {
		for (auto& e : entities) {
			if (e->getSceneID() == getSceneTypeID<T>()) {
				e->handleEvent(event);
			}
		}
	}*/

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

		s->init();
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
		currentSceneID = getSceneTypeID<T>();
		sceneStack.push(currentSceneID);
	}

	void prevScene() {
		printf("%d", sceneStack.empty());
		if (!sceneStack.empty()) {
			currentSceneID = sceneStack.top();
			sceneStack.pop();
		}
	}

private:
	std::vector<std::unique_ptr<Scene>> scenes;
	std::vector<std::unique_ptr<Entity>> entities;
	std::stack<SceneID> sceneStack;

	SceneArray sceneArray;
	SceneBitSet sceneBitset;
	SceneID currentSceneID;
};