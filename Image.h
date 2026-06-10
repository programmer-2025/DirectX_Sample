#pragma once
#include "DirectX3D.h"
#include <string>
#include "BaseObject.h"

namespace {
	ID3D11Buffer* vertexBuffer_;
}


/// <summary>
/// 画像を表示するクラス
/// </summary>
class Image : public BaseObject {
private:
	UINT width_, height_;
	std::string path_;

public:
	ID3D11SamplerState* samplerState_;
	ID3D11ShaderResourceView* shaderResourceView_;
	ID3D11Buffer* constantBuffer_;
	Vertex vertices_[6];
	DirectX::XMMATRIX matrix_;

	Image(std::string path, float leftX, float leftY);
	~Image() {};
	void Init() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	DirectX::XMMATRIX GetMatrix() const { return matrix_; }
	void SetMatrix(DirectX::XMMATRIX matrix) { this->matrix_ = matrix; }
};
