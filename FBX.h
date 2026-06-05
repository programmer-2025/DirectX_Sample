#pragma once
#include <Windows.h>
#include "BaseObject.h"
#include <DirectXMath.h>
#include <d3d11.h>
#pragma comment(lib, "LibFbxSDK-MT.lib")
#pragma comment(lib, "LibXml2-MT.lib")
#pragma comment(lib, "zlib-MT.lib")

class FBX : public BaseObject {
public:
	struct CONSTANT_BUFFER
	{
		DirectX::XMMATRIX	matWVP;
		DirectX::XMMATRIX	matNormal;
	};

	struct VERTEX { //ちょ
		DirectX::XMVECTOR position;
	};

	ID3D11Buffer* pVertexBuffer_;
	ID3D11Buffer* pIndexBuffer_; 
	ID3D11Buffer* pConstantBuffer_; //コンスタントバッファ
	int vertexCount_;	//頂点数
	int polygonCount_;	//ポリゴン数

	FBX();
	~FBX();
	HRESULT Load(const std::string name);
	void Init() override;
	void InitVertex(FbxMesh* mesh);
	void InitIndex(FbxMesh* mesh);
	void InitConstantBuffer();
	void Update() override;
	void Draw() override;
	void Release() override;
};

