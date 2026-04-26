#include "BootScene.h"
#include "SceneBase.h"
#include <Windows.h>
#include "DirectX3D.h"

struct Vertex {
	float x, y, z;
	float r, g, b, a;
};

using namespace DirectX3D;

BootScene::BootScene()
    : SceneBase("BootScene") {

}

BootScene::~BootScene() {
}

void BootScene::Update() {
    counter_++;
}

void BootScene::Draw() {
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	DirectX3D::d3d11Context_->IASetInputLayout(inputLayout);
	DirectX3D::d3d11Context_->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	DirectX3D::d3d11Context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DirectX3D::d3d11Context_->VSSetShader(vertexShader, nullptr, 0);
	DirectX3D::d3d11Context_->PSSetShader(pixelShader, nullptr, 0);

	DirectX3D::d3d11Context_->Draw(6, 0);
}

void BootScene::Init() {
    counter_ = 0;
}

void BootScene::Release() {
}