#pragma once
#include "BaseObject.h"
#include <d3d11.h>
#include "DirectX3D.h"

class Box2D : public BaseObject{
private:
	ID3D11Buffer* vertexBuffer = nullptr;
	Vertex vertices[6] = {};
protected:
public:
	Box2D(float x, float y, float w, float h);
	~Box2D();

	void Init() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};