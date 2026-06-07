#pragma once
#include "BaseObject.h"
#include <Windows.h>
#include <string>
#include <d3d11.h>
#include "DirectX3D.h"
#include <DirectXMath.h>

namespace {
	ID3D11Buffer* vertexBuffer_;
}

class Texture : public BaseObject {
private:
	UINT width_, height_;
	std::string path_;
	
	ID3D11SamplerState* samplerState_;
	ID3D11ShaderResourceView* shaderResourceView_;
	ID3D11Buffer* constantBuffer_;
	Vertex vertices_[6];
	DirectX::XMMATRIX matrix_;
public:
	float leftX_, leftY_;
	float angle;
	Texture(std::string path, float leftX, float leftY);
	~Texture() {};
	void Init() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	DirectX::XMMATRIX GetMatrix() const { return matrix_; }
	void SetMatrix(DirectX::XMMATRIX matrix) { this->matrix_ = matrix; }
};

