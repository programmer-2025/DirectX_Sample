#pragma once
#include "SceneBase.h"

namespace SceneManager {

	void InitManager();
	SceneBase* GetCurrentScene();
	void ChangeScene(std::string name);
	void DrawScene();
	void InitScene();
	void UpdateScene();
	void ReleaseScene();

}