#include "ECS.h"

void Entity::addGroup(Group mGroup) {
	groupBitset[mGroup] = true;
	manager.AddToGroup(this, mGroup);
}

/*
	Updates the current scene.
*/
void GameManager::update() {
	if (currentSceneID < maxScenes) {
		auto& currentScene(sceneArray[currentSceneID]);
		if (!currentScene->isInit()) {
			currentScene->init();
		}
		currentScene->update();
	}
}

/*
	Renders the current scene.
*/
void GameManager::render() {
	if (currentSceneID < maxScenes) {
		auto& currentScene(sceneArray[currentSceneID]);
		currentScene->render();
	}
}