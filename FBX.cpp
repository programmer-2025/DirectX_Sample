#include "FBX.h"
#include "fbxsdk.h"
#include "DirectX3D.h"
#include "BootScene.h"
#include "Texture.h"
#include <vector>
#include "ImGUI/imgui.h"

using namespace fbxsdk;
using namespace DirectX;

FBX::FBX()
	: BaseObject("FBX", true) {
}

FBX::~FBX() {
}

HRESULT FBX::Load(const std::string fName) {
	FbxManager* fbxManager = FbxManager::Create();
	FbxImporter* fbxImporter = FbxImporter::Create(fbxManager, "imp");
	fbxImporter->Initialize(fName.c_str(), -1, fbxManager->GetIOSettings());
	FbxScene* fbxScene = FbxScene::Create(fbxManager, "fbxscene");
	fbxImporter->Import(fbxScene);
	fbxImporter->Destroy();

	rootNode = fbxScene->GetRootNode();
	
	auto a = rootNode->GetChildCount();

	for (int i = 0; i < rootNode->GetChildCount(); i++)
	{
		FbxNode* child = rootNode->GetChild(i);

		auto ii = child->GetName();

		auto uu = child->GetMesh();
	}

	node = rootNode->GetChild(0); //結合済み前提
	mesh = node->GetMesh();

	vertexCount_ = mesh->GetControlPointsCount();
	polygonCount_ = mesh->GetPolygonCount();
	materialCount_ = node->GetMaterialCount();

	InitVertex(mesh);
	InitIndex(mesh);
	InitConstantBuffer();

	return S_OK;
}

void FBX::Init()
{
}

void FBX::InitVertex(FbxMesh* mesh) {
	VERTEX* vertices = new VERTEX[vertexCount_]; //頂点
	for (DWORD poly = 0; poly < polygonCount_; poly++) {
		for (int vertex = 0; vertex < 3; vertex++) {
			int index = mesh->GetPolygonVertex(poly, vertex);
			FbxVector4 pos = mesh->GetControlPointAt(index);
			vertices[index].position = DirectX::XMVectorSet((float)pos[0], (float)pos[1], (float)pos[2], 0.0f);

		}
	}

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX) * vertexCount_;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = vertices;

	HRESULT hr = DirectX3D::d3d11Device_->CreateBuffer(&bd, &data, &pVertexBuffer_);
}

void FBX::InitIndex(FbxMesh* mesh) {
	int* index = new int[polygonCount_ * 3];
	int count = 0;

	for (DWORD poly = 0; poly < polygonCount_; poly++) {
		for (DWORD vertex = 0; vertex < 3; vertex++) {
			index[count] = mesh->GetPolygonVertex(poly, vertex);
			count++;
		}
	}

	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = sizeof(int) * polygonCount_ * 3;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = index;

	HRESULT hr = DirectX3D::d3d11Device_->CreateBuffer(&bd, &data, &pIndexBuffer_);
}

void FBX::InitConstantBuffer() {
	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.ByteWidth = sizeof(ConstantBuffer);
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	HRESULT hr = DirectX3D::d3d11Device_->CreateBuffer(&constantBufferDesc, nullptr, &pConstantBuffer_);
}

void FBX::Update() {
	static float angle = 0.0f;
	static float leftX = 0.0f, leftY = 0.0f, z = 0.0f;
	ConstantBuffer cb = {};
	DirectX::XMMATRIX world = XMMatrixScaling(0.5, 0.5, 0.5) * (XMMatrixRotationZ(angle)) * XMMatrixTranslation(leftX, leftY, z);
	DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(
		DirectX::XMVectorSet(0.0f, 0.0f, -5.0f, 1.0f), //EyeLocation（※カメラの位置）
		DirectX::XMVectorSet(0.0f, 2.0f, 0.0f, 1.0f), //FoucusPostion（※注視点）
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f)  //カメラの上方向
	);
	DirectX::XMMATRIX projection = DirectX::XMMatrixPerspectiveFovLH(XMConvertToRadians(90.0f), 1280.0f / 720.0f, 0.1f, 100.0f);

	cb.worldViewProj = XMMatrixTranspose(world * view * projection);

	ImGui::Begin("FBX");
	ImGui::SliderFloat("X", &leftX, -1.0f, 1.0f);
	ImGui::SliderFloat("Y", &leftY, -1.0f, 1.0f);
	ImGui::SliderFloat("Z", &z, -1.0f, 1.0f);
	ImGui::SliderFloat("angle", &angle, -1.0f, 1.0f);
	ImGui::End();

	DirectX3D::d3d11Context_->UpdateSubresource(pConstantBuffer_, 0, nullptr, &cb, 0, 0);
}

void FBX::Draw() {
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	ID3D11ShaderResourceView* srv[1] = { nullptr };
	ID3D11SamplerState* sampler[1] = { nullptr };

	DirectX3D::d3d11Context_->PSSetShaderResources(0, 1, srv);
	DirectX3D::d3d11Context_->PSSetSamplers(0, 1, sampler);

	DirectX3D::d3d11Context_->VSSetShader(DirectX3D::vertexShader, nullptr, 0);
	DirectX3D::d3d11Context_->PSSetShader(DirectX3D::pixelShader, nullptr, 0);
	DirectX3D::d3d11Context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DirectX3D::d3d11Context_->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);
	DirectX3D::d3d11Context_->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);
	DirectX3D::d3d11Context_->VSSetConstantBuffers(0, 1, &pConstantBuffer_);
	D3D11_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FrontCounterClockwise = FALSE;
	ID3D11RasterizerState* rasterizerState = nullptr;
	DirectX3D::d3d11Device_->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
	DirectX3D::d3d11Context_->RSSetState(rasterizerState);
	DirectX3D::d3d11Context_->DrawIndexed(polygonCount_ * 3, 0, 0);
	DirectX3D::d3d11Context_->RSSetState(nullptr);


}

void FBX::Release()
{
}
