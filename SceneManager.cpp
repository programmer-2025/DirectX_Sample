#include "SceneManager.h"
#include "SceneBase.h"
#include "BootScene.h"
#include <vector>
#include "ObjectManager.h"

namespace {
	std::vector<SceneBase*> sceneList;
	SceneBase* currentScene = nullptr;
}

namespace SceneManager {

	void InitManager() {
		sceneList.clear();
		auto bootScene = new BootScene();
		sceneList.push_back(bootScene);
		ChangeScene("BootScene");
	}

	SceneBase* GetCurrentScene() {
		return currentScene;
	}

	void ChangeScene(std::string name) {
		for (auto& scene : sceneList) {
			if (scene->GetName() == name) {
				currentScene = scene;
				currentScene->Init();
				ObjectManager::InitManager();
				break;
			}
		}
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