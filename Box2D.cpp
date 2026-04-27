#include "Box2D.h"

using namespace DirectX3D;

Box2D::Box2D(float x, float y, float w, float h)
	: BaseObject("Box2D", true) {

	Vertex vertices_[6] = {
		// 三角形①
		{ x,  y, 0.0f, 1,0,0,1 }, //左上
		{  x+w,  y, 0.0f, 0,1,0,1 }, //右上
		{  x+w, y+h, 0.0f, 1,1,0,1 }, //右下

		// 三角形②
		{ x,  y, 0.0f, 1,0,0,1 }, //左上
		{  x+w, y+h, 0.0f, 1,1,0,1 }, //右下
		{ x, y+h, 0.0f, 0,0,1,1 }, //左下
	};

	//Vertex vertices_[6] = {
	//	// 三角形①
	//	{ -0.5f,  0.5f, 0.5f, 1,0,0,1 }, //左上
	//	{  0.5f,  0.5f, 0.0f, 0,1,0,1 }, //右上
	//	{  0.5f, -0.5f, 0.0f, 1,1,0,1 }, //右下

	//	// 三角形②
	//	{ -0.5f,  0.5f, 0.0f, 1,0,0,1 }, //左上
	//	{  0.5f, -0.5f, 0.0f, 1,1,0,1 }, //右下
	//	{ -0.5f, -0.5f, 0.0f, 0,0,1,1 }, //左下
	//};

	memcpy_s(vertices, sizeof(vertices), vertices_, sizeof(vertices_));
}

Box2D::~Box2D() {
}

void Box2D::Init() {
	static HRESULT result = {};
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(vertices);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices;

	result = d3d11Device_->CreateBuffer(&bd, &initData, &vertexBuffer);
}

void Box2D::Update(){
}

void Box2D::Draw() {
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	DirectX3D::d3d11Context_->IASetInputLayout(inputLayout);
	DirectX3D::d3d11Context_->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	DirectX3D::d3d11Context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DirectX3D::d3d11Context_->VSSetShader(vertexShader, nullptr, 0);
	DirectX3D::d3d11Context_->PSSetShader(pixelShader, nullptr, 0);

	DirectX3D::d3d11Context_->Draw(6, 0);
}

void Box2D::Release()
{
}
