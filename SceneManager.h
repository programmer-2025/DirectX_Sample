#pragma once
#include "SceneBase.h"

namespace SceneManager {

	void InitManager();
	SceneBase* GetCurrentScene();
	void DrawScene();
	void InitScene();
	void UpdateScene();
	void ReleaseScene();

}