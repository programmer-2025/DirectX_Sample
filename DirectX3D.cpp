#include "DirectX3D.h"
#include "framework.h"
#include <assert.h>
#include <d3dcompiler.h>

int DirectX3D::initializeDevice(HWND hwnd) {
    DXGI_SWAP_CHAIN_DESC desc = {};
    desc.BufferDesc.Width = Screen::WIDTH;
    desc.BufferDesc.Height = Screen::HEIGHT;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BufferDesc.RefreshRate.Numerator = 60;
    desc.BufferDesc.RefreshRate.Denominator = 1;

    desc.Windowed = TRUE;
    desc.OutputWindow = hwnd;
    desc.BufferCount = 1;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;

    D3D_FEATURE_LEVEL level = {};
    HRESULT result = {};
    result = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &desc,
        &swapChain_,
        &d3d11Device_,
        &level,
        &d3d11Context_
    );

    if (FAILED(result)) {
        return -1;
    }

    result = swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&texture2D_);

    if (FAILED(result)) {
        return -1;
    }

    result = d3d11Device_->CreateRenderTargetView(texture2D_, NULL, &renderTargetView_);

    if (FAILED(result)) {
        return -1;
    }

    D3D11_VIEWPORT vp = {};
    vp.Width = Screen::WIDTH;
    vp.Height = Screen::HEIGHT;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;

    d3d11Context_->RSSetViewports(1, &vp);

    return 0;
}

void DirectX3D::initShader() {
	ID3DBlob* vsBlob = nullptr;
	ID3DBlob* psBlob = nullptr;
	HRESULT result = {};

	result = D3DCompileFromFile(
		L"TestPixelShader.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"ps_5_0",
		D3DCOMPILE_ENABLE_STRICTNESS,
		0,
		&psBlob,
		NULL
	);

	result = D3DCompileFromFile(
		L"TestVertexShader.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"vs_5_0",
		D3DCOMPILE_ENABLE_STRICTNESS,
		0,
		&vsBlob,
		NULL
	);

	result = DirectX3D::d3d11Device_->CreateVertexShader(
		vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(),
		NULL,
		&vertexShader);

	result = DirectX3D::d3d11Device_->CreatePixelShader(
		psBlob->GetBufferPointer(), psBlob->GetBufferSize(),
		NULL,
		&pixelShader);

	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,                          D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, UINT(3 * sizeof(float)),    D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	result = DirectX3D::d3d11Device_->CreateInputLayout(
		layout,
		ARRAYSIZE(layout),
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		&inputLayout
	);
	assert(SUCCEEDED(result));

	struct Vertex {
		float x, y, z;
		float r, g, b, a;
	};

	Vertex vertices[3] = {
		{  0.0f,  0.5f, 0.0f, 1, 0, 0, 1 }, // 上：赤
		{  0.5f, -0.5f, 0.0f, 0, 1, 0, 1 }, // 右下：緑
		{ -0.5f, -0.5f, 0.0f, 0, 0, 1, 1 }, // 左下：青
	};

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(vertices);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices;

	result = d3d11Device_->CreateBuffer(&bd, &initData, &vertexBuffer);
}

void DirectX3D::release() {
	d3d11Device_->Release();
	d3d11Context_->Release();
    swapChain_->Release();
    renderTargetView_->Release();
    texture2D_->Release();
}
