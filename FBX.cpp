#include "FBX.h"
#include "fbxsdk.h"
#include "DirectX3D.h"

FBX::FBX()
	: BaseObject("FBX", true) {
}

FBX::~FBX()
{
}

HRESULT FBX::Load(const std::string fName) {
	FbxManager* fbxManager = FbxManager::Create();
	FbxImporter* fbxImporter = FbxImporter::Create(fbxManager, "imp");
	fbxImporter->Initialize(fName.c_str(), -1, fbxManager->GetIOSettings());
	FbxScene* fbxScene = FbxScene::Create(fbxManager, "fbxscene");
	fbxImporter->Import(fbxScene);
	fbxImporter->Destroy();

	FbxNode* rootNode = fbxScene->GetRootNode();
	FbxNode* node = rootNode->GetChild(0); //結合済み前提
	FbxMesh* mesh = node->GetMesh();

	vertexCount_ = mesh->GetControlPointsCount();
	polygonCount_ = mesh->GetPolygonCount();

	InitVertex(mesh);
	InitIndex(mesh);
	InitConstantBuffer();
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
}

void FBX::InitConstantBuffer() {
	constantbuffer.worldViewProj = XMMatrixTranspose(wvp); // 行列を縦横を入れ替える

	DirectX3D::d3d11Context_->UpdateSubresource(constantBuffer_, 0, nullptr, &constantbuffer, 0, 0);
}

void FBX::Update()
{
}

void FBX::Draw()
{
}

void FBX::Release()
{
}
