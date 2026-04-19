#include "DirectX3D.h"
#include "framework.h"

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

void DirectX3D::release() {
	d3d11Device_->Release();
	d3d11Context_->Release();
    swapChain_->Release();
    renderTargetView_->Release();
    texture2D_->Release();
}
