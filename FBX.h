#pragma once
#include <Windows.h>
#include "BaseObject.h"
#include <DirectXMath.h>
#include <d3d11.h>
#include "fbxsdk.h"
#pragma comment(lib, "LibFbxSDK-MD.lib")
#pragma comment(lib, "LibXml2-MD.lib")
#pragma comment(lib, "zlib-MD.lib")

class FBX : public BaseObject {
public:
	struct VERTEX { //ちょ
		DirectX::XMVECTOR position;
	};

	ID3D11Buffer* pVertexBuffer_;
	ID3D11Buffer* pIndexBuffer_; 
	ID3D11Buffer* pConstantBuffer_; //コンスタントバッファ
	int vertexCount_;	//頂点数
	int polygonCount_;	//ポリゴン数
	int materialCount_;
	FbxNode* rootNode = nullptr;
	FbxNode* node = nullptr; //結合済み前提
	FbxMesh* mesh = nullptr;
	int indexCount_ = 0;

	FBX();
	~FBX();
	HRESULT Load(const std::string name);
	void Init() override;
	void InitVertex(fbxsdk::FbxMesh* mesh);
	void InitIndex(fbxsdk::FbxMesh* mesh);
	void InitConstantBuffer();
	void Update() override;
	void Draw() override;
	void Release() override;
};

