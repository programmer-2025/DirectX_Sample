#pragma once
#include <Windows.h>
#include "BaseObject.h"
#include <DirectXMath.h>
#include <d3d11.h>
#include "fbxsdk.h"
#include "Texture.h"
#pragma comment(lib, "LibFbxSDK-MD.lib")
#pragma comment(lib, "LibXml2-MD.lib")
#pragma comment(lib, "zlib-MD.lib")

struct MATERIAL {
	Texture* texture;
};

/// <summary>
/// FBXの3Dモデルを表示するクラス
/// </summary>
class FBX : public BaseObject {
private:
	ID3D11Buffer* pVertexBuffer_; //頂点バッファ
	ID3D11Buffer** pIndexBuffer_; //インデックスバッファ（※数は分からないため、ポインタの配列）
	ID3D11Buffer* pConstantBuffer_; //コンスタントバッファ

	FbxManager* fbxManager_;
	FbxImporter* fbxImporter_;
	int* index_;
	int materialCount_;
	int vertexCount_;
	int polygonCount_;
	int indexCount_;
	MATERIAL* materials_; //マテリアルの配列データ（※数は分からないため、ポインタ）
	Vertex* vertices_; //頂点の配列データ（※数は分からないため、ポインタ）
public:
	FBX();
	~FBX();

	HRESULT Load(const std::string name);
	ID3D11Buffer* GetVertexBuffer() const { return pVertexBuffer_; }
	ID3D11Buffer** GetIndexBuffer() const { return pIndexBuffer_; }
	ID3D11Buffer* GetConstantBuffer() const { return pConstantBuffer_; }

	void Init() override;
	void InitVertex(fbxsdk::FbxMesh* mesh);
	void InitIndex(fbxsdk::FbxMesh* mesh);
	void InitConstantBuffer();
	void InitMaterial(fbxsdk::FbxNode* node);
	void Update() override;
	void Draw() override;
	void Release() override;
};

