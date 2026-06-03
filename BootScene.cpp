#include "BootScene.h"
#include "SceneBase.h"
#include <Windows.h>
#include "DirectX3D.h"
#include "Box2D.h"
#include "Texture.h"

using namespace DirectX3D;

BootScene::BootScene()
    : SceneBase("BootScene") {
    new Box2D(-0.5f, 0.5f, 1.0f, -1.0f);
    new Box2D(-0.7f, 0.5f, 0.1f, -1.0f);
	new Texture("texture.png", -0.5f, -0.5f);
}

BootScene::~BootScene() {
}

void BootScene::Update() {
    counter_++;
}

void BootScene::Draw() {
}

void BootScene::Init() {
    counter_ = 0;
}

void BootScene::Release() {
}