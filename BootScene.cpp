#include "BootScene.h"
#include "SceneBase.h"
#include <Windows.h>
#include "DirectX3D.h"
#include "Box2D.h"
#include "Texture.h"
#include "Player.h"
#include "FBX.h"
#include "ImGUI/imgui.h"
#include "Camera.h"
#include "FontText.h"

using namespace DirectX3D;
using namespace DirectX;

BootScene::BootScene()
    : SceneBase("BootScene") {
    //new Box2D(-0.5f, 0.5f, 1.0f, -1.0f);
    //new Box2D(-0.7f, 0.5f, 0.1f, -1.0f);
    //new Player();
    auto fbx = (new FBX());
    fbx->Load("Oden.fbx");
    assert(DirectX3D::swapChain_);
    //new FontText(L"aaa");
}

BootScene::~BootScene() {
}

void BootScene::Update() {
    counter_++;
}

void BootScene::Draw() {
    Camera* currentCamera = CameraManager::getCurentCamera();
    DirectX::XMFLOAT3 cameraPos = currentCamera->getCameraPostion();
    DirectX::XMFLOAT3 targetPos = currentCamera->getFoucsPostion();

    ImGui::Begin("BootScene");
    ImGui::SliderFloat("cameraPosX", &cameraPos.x, -20.0f, 20.0f);
	ImGui::SliderFloat("cameraPosY", &cameraPos.y, -20.0f, 20.0f);
	ImGui::SliderFloat("cameraPosZ", &cameraPos.z, -20.0f, 20.0f);
	ImGui::SliderFloat("targetPosX", &targetPos.x, -20.0f, 20.0f);
	ImGui::SliderFloat("targetPosY", &targetPos.y, -20.0f, 20.0f);
	ImGui::SliderFloat("targetPosZ", &targetPos.z, -20.0f, 20.0f);
    ImGui::End();

	currentCamera->setCameraPostion(cameraPos);
	currentCamera->setFoucsPostion(targetPos);
	CameraManager::setCurentCamera(currentCamera);
}

void BootScene::Init() {
    counter_ = 0;
}

void BootScene::Release() {
}