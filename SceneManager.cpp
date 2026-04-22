#include "SceneManager.h"
#include "SceneBase.h"
#include "BootScene.h"
#include <vector>

namespace {
	std::vector<SceneBase*> sceneList;
	SceneBase* currentScene = nullptr;
}

namespace SceneManager {

	void InitManager() {
		sceneList.clear();
		auto bootScene = new BootScene();
		sceneList.push_back(bootScene);
		currentScene = bootScene;
	}

	SceneBase* GetCurrentScene() {
		return currentScene;
	}

	void DrawScene() {
		if (currentScene == nullptr) return;
		currentScene->Draw();
	}

	void InitScene() {
		if (currentScene == nullptr) return;
		currentScene->Init();
	}

	void UpdateScene() {
		if (currentScene == nullptr) return;
		currentScene->Update();
	}

	void ReleaseScene() {
		if (currentScene == nullptr) return;
		currentScene->Release();
	}

}