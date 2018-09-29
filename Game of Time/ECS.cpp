#include "ECS.h"

void Component::setOnClick(std::function<void()> func) {
	clickable = true;
	onClick = func;
}

void Component::setOnKeyPressed(std::function<void()> func) {
	onKeyPressed = func;
}

/*
	Updates the current scene.
*/
void GameManager::update() {
	auto& currentScene(sceneArray[currentSceneID]);
	currentScene->update();
}

/*
	Renders the current scene.
*/
void GameManager::render() {
	auto& currentScene(sceneArray[currentSceneID]);
	currentScene->render();
}

/*
Handles mouse for scene.
*/
/*
void GameManager::handleEvents(SDL_Event event) {
	auto& currentScene(sceneArray[currentSceneID]);
	currentScene->handleEvent(event);
}*/