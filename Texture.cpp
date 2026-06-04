#include "Texture.h"
#include "BaseObject.h"
#include <wincodec.h>
#include <assert.h>
#include <vector>
#include "Input.h"
#include <directxmath.h>
#include "BootScene.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace DirectX3D;

Texture::Texture(const std::string path, const float leftX, const float leftY)
	: BaseObject("Texture", true) {
	path_ = path;
	leftX_ = leftX;
	leftY_ = leftY;

	// スクリーン座標 → 画像の座標
	vertices_[0] = { leftX, leftY + 1.0f, 0.0f, 0,1,0,1, 0, 0 }; // スクリーン座標: 左下 → 画像の座標: 左上 
	vertices_[1] = { leftX, leftY, 0.0f, 1,0,0,1, 0, 1 }; // 左上 → 左下
	vertices_[2] = { leftX + 1.0f, leftY, 0.0f, 1,1,0,1, 1, 1 }; // 右上 → 右下

	vertices_[3] = { leftX + 1.0f, leftY, 0.0f, 1,0,0,1, 1, 1 }; // 右上 → 右下
	vertices_[4] = { leftX, leftY + 1.0f, 0.0f, 1,1,0,1, 0, 0 }; // 左下 → 左上
	vertices_[5] = { leftX + 1.0f, leftY + 1.0f, 0.0f, 0,0,1,1, 1, 0 }; // 右下 → 右上
}

void Texture::Init() {
	static HRESULT result = { 0 };
	IWICImagingFactory* pFactory = nullptr;
	IWICBitmapDecoder* pDecoder = nullptr;
	IWICBitmapFrameDecode* pFrame = nullptr;
	IWICFormatConverter* pConverter = nullptr;
	std::wstring wPath(path_.begin(), path_.end());

	result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(result));
	result = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFactory));
	assert(SUCCEEDED(result));
	result = pFactory->CreateDecoderFromFilename(
		wPath.c_str(),
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
	);
	assert(SUCCEEDED(result));
	result = pDecoder->GetFrame(0, &pFrame);
	assert(SUCCEEDED(result));

	result = pFactory->CreateFormatConverter(&pConverter);

	pConverter->Initialize(
		pFrame,
		GUID_WICPixelFormat32bppRGBA,
		WICBitmapDitherTypeNone,
		nullptr,
		1.0f,
		WICBitmapPaletteTypeMedianCut
	);
	pConverter->GetSize(&width_, &height_);

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = width_;
	desc.Height = height_;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MipLevels = 1;
	
	std::vector<BYTE> imageData;
	size_t rowBytes = static_cast<size_t>(width_) * 4;
	size_t imageSize = rowBytes * static_cast<size_t>(height_);
	imageData.resize(imageSize);
	result = pConverter->CopyPixels(nullptr, static_cast<UINT>(rowBytes), static_cast<UINT>(imageSize), imageData.data());

	D3D11_SUBRESOURCE_DATA textureData = {};
	textureData.pSysMem = imageData.data();
	textureData.SysMemPitch = static_cast<UINT>(rowBytes);
	DirectX3D::d3d11Device_->CreateTexture2D(
		&desc,
		&textureData,
		&DirectX3D::texture2D_
	);

	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
	viewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipLevels = 1;
	DirectX3D::d3d11Device_->CreateShaderResourceView(DirectX3D::texture2D_, &viewDesc, &shaderResourceView_);

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	DirectX3D::d3d11Device_->CreateSamplerState(&samplerDesc, &samplerState_);

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(vertices_);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA bufferData = {};
	bufferData.pSysMem = vertices_;

	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.ByteWidth = sizeof(ConstantBuffer);
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	DirectX3D::d3d11Device_->CreateBuffer(&constantBufferDesc, nullptr, &constantBuffer_);

	DirectX3D::d3d11Device_->CreateBuffer(&bufferDesc, &bufferData, &vertexBuffer_);

}

void Texture::Update() {
	auto world =
		XMMatrixRotationZ(angle) *
		XMMatrixTranslation(leftX_, leftY_, 0.0f);

	auto proj = XMMatrixOrthographicOffCenterLH(
		-1.0f, 1.0f, 
		-1.0f, 1.0f, 
		0.0f, 1.0f  
	);

	auto wvp = world * proj;

	ConstantBuffer constantbuffer = {};
	constantbuffer.worldViewProj = XMMatrixTranspose(wvp); // 行列を縦横を入れ替える

	DirectX3D::d3d11Context_->UpdateSubresource(constantBuffer_, 0, nullptr, &constantbuffer, 0, 0);
}

void Texture::Draw() {
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	DirectX3D::d3d11Context_->IASetInputLayout(DirectX3D::inputLayout);
	DirectX3D::d3d11Context_->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);
	DirectX3D::d3d11Context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DirectX3D::d3d11Context_->VSSetShader(DirectX3D::vertexShader, nullptr, 0);
	DirectX3D::d3d11Context_->PSSetShader(DirectX3D::pixelShader, nullptr, 0);
	DirectX3D::d3d11Context_->PSSetShaderResources(0, 1, &shaderResourceView_);
	DirectX3D::d3d11Context_->PSSetSamplers(0, 1, &samplerState_);
	DirectX3D::d3d11Context_->VSSetConstantBuffers(0, 1, &constantBuffer_);

	D3D11_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FrontCounterClockwise = FALSE;
	ID3D11RasterizerState* rasterizerState = nullptr;
	DirectX3D::d3d11Device_->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
	DirectX3D::d3d11Context_->RSSetState(rasterizerState);

	DirectX3D::d3d11Context_->Draw(6, 0);

	DirectX3D::d3d11Context_->RSSetState(nullptr);
}

void Texture::Release() {
}
