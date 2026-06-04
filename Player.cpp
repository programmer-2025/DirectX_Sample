#include "Player.h"
#include "Texture.h"
#include "Input.h"
#include "ImGUI/imgui.h"

namespace {
	Texture* texture = nullptr;
}

Player::Player()
	: BaseObject("Player", true) {
}

void Player::Init() {
	texture = new Texture("texture.png", -0.5f, -0.5f);
	texture->Init();
}

void Player::Update() {
	if (texture == nullptr) return;

	if (Input::IsPushKey(DIK_E)) {
		texture->leftY_ += 0.05f;
		texture->angle += 0.05f;
	}
	else if (Input::IsPushKey(DIK_D)) {
		texture->leftY_ -= 0.05f;
		texture->angle -= 0.05f;
	}
}

void Player::Draw(){

	ImGui::Begin("Player");
	ImGui::SliderFloat("X", &texture->leftX_, -1.0, 1.0);
	ImGui::SliderFloat("Y", &texture->leftY_, -1.0, 1.0);
	ImGui::End();
}

void Player::Release()
{
}
