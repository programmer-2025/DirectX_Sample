#pragma once
#include <Windows.h>
#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

namespace DirectX3D {
	inline ID3D11Device* d3d11Device_ = nullptr;
	inline ID3D11DeviceContext* d3d11Context_ = nullptr;
	inline IDXGISwapChain* swapChain_ = nullptr;
	inline ID3D11RenderTargetView* renderTargetView_ = nullptr;
	inline ID3D11Texture2D* texture2D_ = nullptr;

	//シェーダー
	inline ID3D11VertexShader* vertexShader = nullptr;
	inline ID3D11PixelShader* pixelShader = nullptr;
	inline ID3D11InputLayout* inputLayout = nullptr;
	inline ID3D11Buffer* vertexBuffer = nullptr;
	
	void initShader();
	int initializeDevice(HWND hwnd);
	void release();
}